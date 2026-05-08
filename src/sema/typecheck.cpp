#include "typecheck.h"

#include <variant>

#include "../util.h"

namespace alvo::sema::typecheck {
    using namespace resolve;

    static const ast::Type TYPE_INVALID = ast::Type { ast::Invalid {} };

    static const ast::Type TYPE_BOOL = ast::Type { ast::Type::Bool {} };

    static const ast::Type TYPE_INT = ast::Type { ast::Type::Int {} };

    static const ast::Type TYPE_BYTE = ast::Type { ast::Type::Byte {} };

    static const ast::Type TYPE_CHAR = ast::Type { ast::Type::Char {} };

    static const ast::Type TYPE_FLOAT = ast::Type { ast::Type::Float {} };

    static bool type_is_invalid(const ast::Type& t) {
        return std::holds_alternative<ast::Invalid>(t.val);
    }

    Typechecker::Typechecker(
        resolve::NameIndex& name_index, mem::Arena& arena) :
        m_index(&name_index),
        m_arena(&arena),
        m_node_ctx(arena),
        m_diag_emitter(),
        m_scope_stack(),
        m_generic_scope_stack() { }

    void Typechecker::set_diag_sink(diag::DiagSink& sink) {
        m_diag_emitter.set_sink(sink);
    }

    void Typechecker::typecheck() {
        for (auto decl : m_index->decls) {
            typecheck_decl(decl.element);
        }
    }

    void Typechecker::typecheck_decl(Decl& decl) {
        std::visit(util::overload { [&](Decl::Func& func) {
            typecheck_decl_func(func);
        } },
            decl.val);
    }

    void Typechecker::typecheck_decl_func(Decl::Func& func) {
        m_generic_scope_stack.push();
        put_generic_params(func.generic_params);

        typecheck_ast_func(func.func);

        m_generic_scope_stack.pop();
    }

    ast::Type Typechecker::typecheck_ast_func(
        ast::Func& func, std::optional<ast::Type> expected_type) {
        // TODO: handle expected type
    }

    std::optional<ast::Type> Typechecker::typecheck_ast_block(
        ast::Block& block, ast::Type expected_return_type) {
        if (block.is_invalid)
            return std::nullopt;

        std::optional<ast::Type> returned;
        for (auto& stmt : block.stmts) {
            auto stmt_returned = typecheck_ast_stmt(stmt, expected_return_type);
            if (stmt_returned) {
                returned = stmt_returned;
            }
        }
    }

    std::optional<ast::Type> Typechecker::typecheck_ast_stmt(
        ast::Stmt& stmt, ast::Type expected_return_type) {
        std::optional<ast::Type> returned;
        std::visit(
            util::overload {
                [&](ast::Invalid&) { },
                [&](ast::Expr& expr) { typecheck_ast_expr(expr); },
                [&](ast::Stmt::Let& let) { typecheck_ast_stmt_let(let); },
                [&](ast::Stmt::If& if_) {
                    returned = typecheck_ast_stmt_if(if_, expected_return_type);
                },
                [&](ast::Stmt::Switch& switch_) {
                    returned = typecheck_ast_stmt_switch(
                        switch_, expected_return_type);
                },
                [&](ast::Stmt::Loop& loop) {
                    typecheck_ast_stmt_loop(loop, expected_return_type);
                },
                [&](ast::Stmt::For& for_) {
                    typecheck_ast_stmt_for(for_, expected_return_type);
                },
                [&](ast::Stmt::While& while_) {
                    typecheck_ast_stmt_while(while_, expected_return_type);
                },
                [&](ast::Stmt::Return& return_) {
                    if (return_.is_invalid)
                        return;
                    if (return_.expr) {
                        returned = typecheck_ast_expr(
                            *return_.expr, expected_return_type);
                    }
                    returned = ast::Type(ast::Type::Unit());
                },
                [&](ast::Stmt::Defer& defer) {
                    typecheck_ast_stmt_defer(defer);
                },
                [&](ast::Block& block) {
                    returned = typecheck_ast_block(block, expected_return_type);
                },
                [&](ast::Stmt::Continue&) { },
                [&](ast::Stmt::Break&) { },
            },
            stmt.val);
        return returned;
    }

    void Typechecker::typecheck_ast_stmt_let(ast::Stmt::Let& let) {
        if (let.is_invalid)
            return;

        ast::Type expr_type = typecheck_ast_expr(let.expr, let.type);

        if (let.type) {
            if (expr_type != *let.type) {
                err_expected_type(*let.type);
                let.is_invalid = true;
                return;
            }
        }

        let.type = expr_type;
    }

    std::optional<ast::Type> Typechecker::typecheck_ast_stmt_if(
        ast::Stmt::If& if_, ast::Type expected_return_type) {
        if (if_.is_invalid)
            return std::nullopt;
        auto expr_type = typecheck_ast_expr(if_.expr, TYPE_BOOL);
        if (expr_type != TYPE_BOOL) {
            return std::nullopt;
        }
        auto main_ret = typecheck_ast_block(if_.main, expected_return_type);
        bool main_returned = main_ret.has_value();

        bool all_elifs_returned = true;

        for (auto& elif : if_.elifs) {
            auto elif_expr_type = typecheck_ast_expr(elif.expr, TYPE_BOOL);
            if (elif_expr_type != TYPE_BOOL) {
                continue;
            }
            auto elif_ret =
                typecheck_ast_block(elif.block, expected_return_type);
            if (!elif_ret)
                all_elifs_returned = false;
        }

        bool else_returned = false;

        if (if_.else_) {
            auto else_ret =
                typecheck_ast_block(*if_.else_, expected_return_type);
            else_returned = else_ret.has_value();
        }

        if (main_returned && all_elifs_returned && else_returned) {
            return *main_ret;
        } else {
            return std::nullopt;
        }
    }

    std::optional<ast::Type> Typechecker::typecheck_ast_stmt_switch(
        ast::Stmt::Switch& switch_, ast::Type expected_return_type) {
        if (switch_.is_invalid) {
            return std::nullopt;
        }

        auto expr_type = typecheck_ast_expr(switch_.expr);

        bool expr_type_is_invalid = false;
        bool bad_expr_type = false;
        bool expr_type_is_enum = false;
        UserDefinedType::Enum* expr_type_enum = nullptr;
        std::visit(
            util::overload {
                [&](ast::Invalid&) { expr_type_is_invalid = true; },
                [&](ast::Type::Unit&) { bad_expr_type = true; },
                [&](ast::Type::String&) { bad_expr_type = true; },
                [&](ast::Type::Char&) { },
                [&](ast::Type::Int&) { },
                [&](ast::Type::Byte&) { },
                [&](ast::Type::Float&) { bad_expr_type = true; },
                [&](ast::Type::Bool&) { },
                [&](ast::Type::Array&) { bad_expr_type = true; },
                [&](ast::Type::Tup&) { bad_expr_type = true; },
                [&](ast::Type::Func&) { bad_expr_type = true; },
                [&](ast::Type::Name&) { ALVO_UNREACHABLE(); },
                [&](ast::Type::LocalGeneric&) { bad_expr_type = true; },
                [&](ast::Type::ResolvedUserDefinedType& e) {
                    ALVO_ASSERT(m_index->user_defined_types.has_id(e.id));
                    auto& ud = m_index->user_defined_types.get_by_id(e.id);
                    if (auto enum_ =
                            std::get_if<resolve::UserDefinedType::Enum>(
                                &ud.val)) {
                        expr_type_is_enum = true;
                        expr_type_enum = enum_;
                    } else {
                        bad_expr_type = true;
                    }
                },
            },
            expr_type.val);

        if (expr_type_is_invalid) {
            return std::nullopt;
        }

        if (bad_expr_type) {
            err(diag::Err::BadSwitchExprType {});
            return std::nullopt;
        }

        for (auto& case_ : switch_.cases) {
            if (case_.is_invalid)
                return std::nullopt;
            if (case_.expr) {
                auto case_type = typecheck_ast_expr(*case_.expr, expr_type);
                if (type_is_invalid(case_type))
                    continue;
                if (case_type != expr_type) {
                    err_expected_type(expr_type);
                    continue;
                }
            }
            typecheck_ast_block(case_.block, expected_return_type);
        }

        return std::nullopt;
    }

    void Typechecker::typecheck_ast_stmt_loop(
        ast::Stmt::Loop& loop, ast::Type expected_return_type) {
        if (loop.is_invalid)
            return;
        typecheck_ast_block(loop.block, expected_return_type);
    }

    void Typechecker::typecheck_ast_stmt_for(
        ast::Stmt::For& for_, ast::Type expected_return_type) {
        if (for_.is_invalid)
            return;
        m_scope_stack.push();

        auto iterable_type = typecheck_ast_expr(for_.expr);
        ast::Type element_type = TYPE_INVALID;
        if (auto array = std::get_if<ast::Type::Array>(&iterable_type.val)) {
            if (array->is_invalid)
                return;
            element_type = *array->type;
        } else {
            err_expected("array");
            return;
        }

        m_scope_stack.put(for_.name, element_type);

        typecheck_ast_block(for_.block, expected_return_type);

        m_scope_stack.pop();
    }

    void Typechecker::typecheck_ast_stmt_while(
        ast::Stmt::While& while_, ast::Type expected_return_type) {
        if (while_.is_invalid)
            return;

        auto expr_type = typecheck_ast_expr(while_.expr, TYPE_BOOL);
        if (!type_is_invalid(expr_type)) {
            if (expr_type != TYPE_BOOL) {
                err_expected_type(TYPE_BOOL);
            }
        }

        typecheck_ast_block(while_.block, expected_return_type);
    }

    void Typechecker::typecheck_ast_stmt_defer(
        [[maybe_unused]] ast::Stmt::Defer& defer) {
        ALVO_NOT_IMPLEMENTED();
    }

    ast::Type Typechecker::typecheck_ast_expr(
        ast::Expr& expr, std::optional<ast::Type> expected_type) {
        return std::visit(
            util::overload {
                [&](ast::Invalid&) { return TYPE_INVALID; },
                [&](ast::Expr::Literal& lit) {
                    return typecheck_ast_expr_literal(lit, expected_type);
                },
                [&](ast::Expr::Unop& unop) {
                    return typecheck_ast_expr_unop(unop, expected_type);
                },
                [&](ast::Expr::Binop& binop) {
                    return typecheck_ast_expr_binop(binop, expected_type);
                },
                [&](ast::Expr::Index& index) {
                    return typecheck_ast_expr_index(index, expected_type);
                },
                [&](ast::Expr::Call& call) {
                    return typecheck_ast_expr_call(call, expected_type);
                },
                [&](ast::Expr::Cast& cast) {
                    return typecheck_ast_expr_cast(cast, expected_type);
                },
                [&](ast::Expr::TryCast& try_cast) {
                    return typecheck_ast_expr_try_cast(try_cast, expected_type);
                },
                [&](ast::Expr::Builtin& builtin) {
                    return typecheck_ast_expr_builtin(builtin, expected_type);
                },
                [&](ast::Expr::Name&) {
                    ALVO_UNREACHABLE();
                    return TYPE_INVALID;
                },
                [&](ast::Expr::TypeMemberAccess&) {
                    ALVO_UNREACHABLE();
                    return TYPE_INVALID;
                },
                [&](ast::Expr::MemberAccess& member_access) {
                    return typecheck_ast_expr_member_access(
                        member_access, expected_type);
                },
                [&](ast::Expr::LocalVar& local_var) {
                    return typecheck_ast_expr_local_var(
                        local_var, expected_type);
                },
                [&](ast::Expr::ResolvedDecl& resolved_decl) {
                    return typecheck_ast_expr_resolved_decl(
                        resolved_decl, expected_type);
                },
                [&](ast::Expr::ResolvedTypeMemberAccess&
                        resolved_type_member_access) {
                    return typecheck_ast_expr_resolved_type_member_access(
                        resolved_type_member_access, expected_type);
                },
                [&](ast::Expr::ResolvedGenericMemberAccess&
                        resolved_generic_member_access) {
                    return typecheck_ast_expr_resolved_generic_member_access(
                        resolved_generic_member_access, expected_type);
                },
            },
            expr.val);
    }

    ast::Type Typechecker::typecheck_ast_expr_literal(
        ast::Expr::Literal& lit, std::optional<ast::Type> expected_type) {
        return std::visit(
            util::overload {
                [&](ast::Invalid&) { return TYPE_INVALID; },
                [&](ast::Expr::Literal::Unit&) {
                    return ast::Type(ast::Type::Unit {});
                },
                [&](ast::Expr::Literal::Null&) {
                    if (!expected_type) {
                        err(diag::Err::CantInferTypeOfNullLiteral {});
                        return TYPE_INVALID;
                    }

                    auto udtype =
                        std::get_if<ast::Type::ResolvedUserDefinedType>(
                            &expected_type->val);
                    if (!udtype) {
                        err(diag::Err::CantBeNull {});
                        return TYPE_INVALID;
                    }
                    ALVO_ASSERT(m_index->user_defined_types.has_id(udtype->id));
                    auto& type =
                        m_index->user_defined_types.get_by_id(udtype->id);
                    if (!std::holds_alternative<UserDefinedType::Struct>(
                            type.val) &&
                        !std::holds_alternative<UserDefinedType::Interface>(
                            type.val)) {
                        err(diag::Err::CantBeNull {});
                        return TYPE_INVALID;
                    }

                    return *expected_type;
                },
                [&](ast::Expr::Literal::String&) {
                    return ast::Type(ast::Type::String {});
                },
                [&](ast::Expr::Literal::Character&) {
                    return ast::Type(ast::Type::Char {});
                },
                [&](ast::Expr::Literal::Integer&) {
                    return ast::Type(ast::Type::Int {});
                },
                [&](ast::Expr::Literal::Byte&) {
                    return ast::Type(ast::Type::Byte {});
                },
                [&](ast::Expr::Literal::Floating&) {
                    return ast::Type(ast::Type::Float {});
                },
                [&](ast::Expr::Literal::Boolean&) {
                    return ast::Type(ast::Type::Bool {});
                },
                [&](ast::Expr::Literal::Array& array) {
                    return typecheck_ast_expr_literal_array(
                        array, expected_type);
                },
                [&](ast::Expr::Literal::Tup& tup) {
                    return typecheck_ast_expr_literal_tup(tup, expected_type);
                },
                [&](ast::Expr::Literal::Struct&) {
                    ALVO_UNREACHABLE();
                    return TYPE_INVALID;
                },
                [&](ast::Expr::Literal::ResolvedStruct& resolved_struct) {
                    return typecheck_ast_expr_literal_resolved_struct(
                        resolved_struct, expected_type);
                },
                [&](ast::util::Ptr<ast::Func>& func) {
                    return typecheck_ast_func(*func, expected_type);
                },
            },
            lit.val);
    }

    ast::Type Typechecker::typecheck_ast_expr_literal_array(
        ast::Expr::Literal::Array& array,
        std::optional<ast::Type> expected_type) {
        std::optional<ast::Type> contained_type = std::nullopt;
        if (expected_type) {
            if (type_is_invalid(*expected_type))
                return TYPE_INVALID;

            if (auto arr = std::get_if<ast::Type::Array>(&expected_type->val)) {
                if (arr->is_invalid)
                    return TYPE_INVALID;
                contained_type = *arr->type;
            } else {
                err_expected("array literal");
                return TYPE_INVALID;
            }
        }
        return std::visit(
            util::overload {
                [&](ast::Invalid&) { return TYPE_INVALID; },
                [&](ast::Expr::Literal::Array::Regular& regular) {
                    if (regular.elements.empty()) {
                        if (!expected_type) {
                            err(diag::Err::CantInferTypeOfArrayLiteral {});
                            return TYPE_INVALID;
                        }

                        return *expected_type;
                    }

                    std::optional<ast::Type> inner_type = contained_type;
                    for (auto& elem : regular.elements) {
                        auto expr_type = typecheck_ast_expr(elem, inner_type);
                        if (inner_type) {
                            if (expr_type != *inner_type) {
                                err_expected_type(*inner_type);
                                return TYPE_INVALID;
                            }
                        }
                        inner_type = expr_type;
                    }

                    // inner_type is guaranteed to have value here
                    ALVO_ASSERT(inner_type.has_value());

                    return ast::Type(ast::Type::Array(
                        false, m_node_ctx.make_node<ast::Type>(*inner_type)));
                },
                [&](ast::Expr::Literal::Array::DefaultNTimes& default_n_times) {
                    if (!expected_type) {
                        err(diag::Err::CantInferTypeOfArrayLiteral {});
                    }

                    auto times_type =
                        typecheck_ast_expr(*default_n_times.times);
                    if (!std::holds_alternative<ast::Type::Int>(
                            times_type.val)) {
                        err_expected("int");
                        return TYPE_INVALID;
                    }

                    return *expected_type;
                },
                [&](ast::Expr::Literal::Array::ExprNTimes& expr_n_times) {
                    auto expr_type = typecheck_ast_expr(*expr_n_times.expr);
                    if (contained_type) {
                        if (expr_type != *contained_type) {
                            err_expected_type(*contained_type);
                            return TYPE_INVALID;
                        }
                    }

                    auto times_type = typecheck_ast_expr(*expr_n_times.times);
                    if (!std::holds_alternative<ast::Type::Int>(
                            times_type.val)) {
                        err_expected("int");
                        return TYPE_INVALID;
                    }

                    return ast::Type(ast::Type::Array(
                        false, m_node_ctx.make_node<ast::Type>(expr_type)));
                },
            },
            array.val);
    }

    ast::Type Typechecker::typecheck_ast_expr_literal_tup(
        ast::Expr::Literal::Tup& tup, std::optional<ast::Type> expected_type) {
        if (tup.is_invalid)
            return TYPE_INVALID;

        if (expected_type) {
            if (auto expected_tup =
                    std::get_if<ast::Type::Tup>(&expected_type->val)) {
                if (expected_tup->is_invalid)
                    return TYPE_INVALID;
                if (expected_tup->types.size() != tup.exprs.size()) {
                    err_expected_type(*expected_type);
                }

                auto expected_element_type = expected_tup->types.begin();
                auto expr = tup.exprs.begin();
                while (expected_element_type != expected_tup->types.end() &&
                       expr != tup.exprs.end()) {
                    auto actual_element_type =
                        typecheck_ast_expr(*expr, *expected_element_type);
                    if (type_is_invalid(actual_element_type)) {
                        return TYPE_INVALID;
                    }
                    if (*expected_element_type != actual_element_type) {
                        err_expected_type(*expected_element_type);
                        return TYPE_INVALID;
                    }

                    expected_element_type++;
                    expr++;
                }

                return *expected_type;

            } else {
                err_expected_type(*expected_type);
                return TYPE_INVALID;
            }
        }

        // no expected type given

        ast::util::List<ast::Type> tuple_types;
        for (auto& expr : tup.exprs) {
            auto type = typecheck_ast_expr(expr);
            if (type_is_invalid(type))
                return TYPE_INVALID;
            tuple_types.push_back(*m_arena, type);
        }

        return ast::Type(ast::Type::Tup(false, tuple_types));
    }

    ast::Type Typechecker::typecheck_ast_expr_literal_resolved_struct(
        ast::Expr::Literal::ResolvedStruct& struct_,
        [[maybe_unused]] std::optional<ast::Type> expected_type) {

        if (!struct_.generic_params.empty()) {
            auto new_id =
                instantiate_type(struct_.type_id, struct_.generic_params);
            if (new_id) {
                struct_.type_id = *new_id;
                struct_.generic_params.clear();
            } else {
                return TYPE_INVALID;
            }
        }

        for (auto& field : struct_.fields) {
            auto type = typecheck_ast_expr(*field.expr, field.type);
            if (type_is_invalid(type))
                continue;
            if (type != field.type) {
                err_expected_type(type);
                continue;
            }
        }

        return ast::Type(
            ast::Type::ResolvedUserDefinedType(struct_.type_id, {}));
    }

    ast::Type Typechecker::typecheck_ast_expr_unop(
        ast::Expr::Unop& unop, std::optional<ast::Type> expected_type) {
        using enum ast::Expr::Unop::Op;

        if (unop.op == Invalid)
            return TYPE_INVALID;

        if (unop.op == Not) {
            auto expr_type = typecheck_ast_expr(*unop.expr, TYPE_BOOL);
            if (expr_type != TYPE_BOOL) {
                err_expected_type(TYPE_BOOL);
                return TYPE_INVALID;
            }
            return TYPE_BOOL;
        }

        if (unop.op == Plus || unop.op == Minus) {
            auto expr_type = typecheck_ast_expr(*unop.expr);
            if (type_is_invalid(expr_type))
                return TYPE_INVALID;
            bool bad_type = false;
            std::visit(util::overload {
                           [&](ast::Invalid&) { ALVO_UNREACHABLE(); },
                           [&](ast::Type::Unit&) { bad_type = true; },
                           [&](ast::Type::String&) { bad_type = true; },
                           [&](ast::Type::Char&) { bad_type = true; },
                           [&](ast::Type::Int&) { },
                           [&](ast::Type::Byte&) { },
                           [&](ast::Type::Float&) { },
                           [&](ast::Type::Bool&) { bad_type = true; },
                           [&](ast::Type::Array&) { bad_type = true; },
                           [&](ast::Type::Tup&) { bad_type = true; },
                           [&](ast::Type::Func&) { bad_type = true; },
                           [&](ast::Type::Name&) { ALVO_UNREACHABLE(); },
                           [&](ast::Type::LocalGeneric&) { bad_type = true; },
                           [&](ast::Type::ResolvedUserDefinedType&) {
                               bad_type = true;
                           },
                       },
                expr_type.val);
            if (bad_type) {
                err(diag::Err::BadUnaryExpressionType {});
                return TYPE_INVALID;
            }
            return expr_type;
        }

        if (unop.op == BinaryNot) {
            auto expr_type = typecheck_ast_expr(*unop.expr);
            bool bad_type;
            if (std::holds_alternative<ast::Type::Int>(expr_type.val)) {
                bad_type = false;
            } else if (std::holds_alternative<ast::Type::Byte>(expr_type.val)) {
                bad_type = false;
            } else {
                bad_type = true;
            }
            if (bad_type) {
                err(diag::Err::BadUnaryExpressionType {});
                return TYPE_INVALID;
            }
            return expr_type;
        }

        ALVO_UNREACHABLE();
        return TYPE_INVALID;
    }

    ast::Type Typechecker::typecheck_ast_expr_binop(
        ast::Expr::Binop& binop, std::optional<ast::Type> expected_type) {
        switch (binop.op) {
        case ast::Expr::Binop::Op::Invalid:
            break;
        case ast::Expr::Binop::Op::Assign:
            break;
        case ast::Expr::Binop::Op::PlusAssign:
            break;
        case ast::Expr::Binop::Op::MinusAssign:
            break;
        case ast::Expr::Binop::Op::MultiplyAssign:
            break;
        case ast::Expr::Binop::Op::DivideAssign:
            break;
        case ast::Expr::Binop::Op::BinaryAndAssign:
            break;
        case ast::Expr::Binop::Op::BinaryOrAssign:
            break;
        case ast::Expr::Binop::Op::BinaryXorAssign:
            break;
        case ast::Expr::Binop::Op::ModAssign:
            break;
        case ast::Expr::Binop::Op::ShiftLeftAssign:
            break;
        case ast::Expr::Binop::Op::ShiftRightAssign:
            break;
        case ast::Expr::Binop::Op::Or:
            break;
        case ast::Expr::Binop::Op::And:
            break;
        case ast::Expr::Binop::Op::BinaryOr:
            break;
        case ast::Expr::Binop::Op::BinaryXor:
            break;
        case ast::Expr::Binop::Op::BinaryAnd:
            break;
        case ast::Expr::Binop::Op::Equal:
            break;
        case ast::Expr::Binop::Op::NotEqual:
            break;
        case ast::Expr::Binop::Op::Less:
            break;
        case ast::Expr::Binop::Op::LessEqual:
            break;
        case ast::Expr::Binop::Op::Greater:
            break;
        case ast::Expr::Binop::Op::GreaterEqual:
            break;
        case ast::Expr::Binop::Op::ShiftLeft:
            break;
        case ast::Expr::Binop::Op::ShiftRight:
            break;
        case ast::Expr::Binop::Op::Plus:
            break;
        case ast::Expr::Binop::Op::Minus:
            break;
        case ast::Expr::Binop::Op::Multiply:
            break;
        case ast::Expr::Binop::Op::Divide:
            break;
        case ast::Expr::Binop::Op::Mod:
            break;
        }
    }

    ast::Type Typechecker::typecheck_ast_expr_index(
        ast::Expr::Index& index, std::optional<ast::Type> expected_type) { }

    ast::Type Typechecker::typecheck_ast_expr_call(
        ast::Expr::Call& call, std::optional<ast::Type> expected_type) { }

    ast::Type Typechecker::typecheck_ast_expr_cast(
        ast::Expr::Cast& cast, std::optional<ast::Type> expected_type) { }

    ast::Type Typechecker::typecheck_ast_expr_try_cast(
        ast::Expr::TryCast& try_cast, std::optional<ast::Type> expected_type) {
    }

    ast::Type Typechecker::typecheck_ast_expr_builtin(
        ast::Expr::Builtin& builtin, std::optional<ast::Type> expected_type) { }

    ast::Type Typechecker::typecheck_ast_expr_member_access(
        ast::Expr::MemberAccess& member_access,
        std::optional<ast::Type> expected_type) { }

    ast::Type Typechecker::typecheck_ast_expr_local_var(
        ast::Expr::LocalVar& local_var,
        std::optional<ast::Type> expected_type) { }

    ast::Type Typechecker::typecheck_ast_expr_resolved_decl(
        ast::Expr::ResolvedDecl& resolved_decl,
        std::optional<ast::Type> expected_type) { }

    ast::Type Typechecker::typecheck_ast_expr_resolved_type_member_access(
        ast::Expr::ResolvedTypeMemberAccess& resolved_decl,
        std::optional<ast::Type> expected_type) { }

    ast::Type Typechecker::typecheck_ast_expr_resolved_generic_member_access(
        ast::Expr::ResolvedGenericMemberAccess& resolved_decl,
        std::optional<ast::Type> expected_type) { }

    void Typechecker::put_generic_params(
        const resolve::GenericParams& generic_params) {
        for (auto& [name, param] : generic_params.params) {
            m_generic_scope_stack.put(name, param);
        }
    }

    void Typechecker::err(const diag::Err::Val& err) {
        m_diag_emitter.err("TODO", { 0, 0, 0 }, diag::Err(err));
    }

    void Typechecker::err_expected(std::string_view expected) {
        err(diag::Err::Expected(expected));
    }

    void Typechecker::err_expected_type(const ast::Type& expected) {
        err(diag::Err::ExpectedType(expected));
    }

    std::optional<ast::Id> Typechecker::instantiate_type(
        ast::Id id, const ast::util::List<ast::Type> generic_params) {
        ALVO_ASSERT(m_index->user_defined_types.has_id(id));
        auto& udtype = m_index->user_defined_types.get_by_id(id);
        udtype.generic_params;
    }
}
