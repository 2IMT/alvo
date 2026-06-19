#include "typecheck.h"

#include <variant>
#include <unordered_set>

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

    ast::Type Typechecker::typecheck_ast_func(ast::Func& func,
        [[maybe_unused]] std::optional<ast::Type> expected_type) {
        if (func.is_invalid) {
            return TYPE_INVALID;
        }

        if (func.signature.is_invalid) {
            return TYPE_INVALID;
        }

        m_arg_stack.push();
        for (auto& param : func.signature.params) {
            if (param.is_invalid) {
                m_arg_stack.pop();
                return TYPE_INVALID;
            }
            m_arg_stack.put(param.name, param.type);
        }

        auto block_ret = typecheck_ast_block(func.block, func.signature.ret);

        if (!block_ret) {
            err(diag::Err::NotAllPathsReturn {});
            m_arg_stack.pop();
            return TYPE_INVALID;
        }

        if (type_is_invalid(*block_ret)) {
            m_arg_stack.pop();
            return TYPE_INVALID;
        }

        if (*block_ret != func.signature.ret) {
            err(diag::Err::UnexpectedReturnType {});
            m_arg_stack.pop();
            return TYPE_INVALID;
        }

        m_arg_stack.pop();

        // TODO: Return function type
        return TYPE_INVALID;
    }

    std::optional<ast::Type> Typechecker::typecheck_ast_block(
        ast::Block& block, ast::Type expected_return_type) {
        if (block.is_invalid)
            return TYPE_INVALID;

        m_scope_stack.push();

        std::optional<ast::Type> returned;
        for (auto& stmt : block.stmts) {
            auto stmt_returned = typecheck_ast_stmt(stmt, expected_return_type);
            if (stmt_returned) {
                returned = stmt_returned;
            }
        }

        m_scope_stack.pop();

        return returned;
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
                    } else {
                        returned = ast::Type(ast::Type::Unit());
                    }
                },
                [&](ast::Stmt::Defer& defer) {
                    typecheck_ast_stmt_defer(defer);
                },
                [&](ast::Block& block) {
                    returned = typecheck_ast_block(block, expected_return_type);
                },
                [&](ast::Stmt::Continue&) { },
                [&](ast::Stmt::Break&) { },
                [&](ast::Stmt::Print& print) {
                    if (print.is_invalid) {
                        return;
                    }
                    for (auto& expr : print.exprs) {
                        typecheck_ast_expr(expr);
                    }
                },
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

        m_scope_stack.put(let.name, expr_type);
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
            expr_type.type.val);

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
                if (type_is_invalid(case_type.type))
                    continue;
                if (case_type.type != expr_type.type) {
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
        if (type_is_invalid(iterable_type.type)) {
            m_scope_stack.pop();
            return;
        }
        ast::Type element_type = TYPE_INVALID;
        if (auto array =
                std::get_if<ast::Type::Array>(&iterable_type.type.val)) {
            if (array->is_invalid) {
                m_scope_stack.pop();
                return;
            }
            element_type = *array->type;
        } else {
            err_expected("array");
            m_scope_stack.pop();
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

    Value Typechecker::typecheck_ast_expr(
        ast::Expr& expr, std::optional<ast::Type> expected_type) {
        return std::visit(
            util::overload { [&](ast::Invalid&) { return Value(TYPE_INVALID); },
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
                    return Value(TYPE_INVALID);
                },
                [&](ast::Expr::TypeMemberAccess&) {
                    ALVO_UNREACHABLE();
                    return Value(TYPE_INVALID);
                },
                [&](ast::Expr::MemberAccess& member_access) {
                    auto res = typecheck_ast_expr_member_access(
                        member_access, expected_type);
                    expr.val = res.second;
                    return res.first;
                },
                [&](ast::Expr::LocalVar& local_var) {
                    return typecheck_ast_expr_local_var(
                        local_var, expected_type);
                },
                [&](ast::Expr::FuncArg& func_arg) {
                    return typecheck_ast_expr_func_arg(func_arg, expected_type);
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
                [&](ast::Expr::ResolvedMemberAccess&) {
                    ALVO_UNREACHABLE();
                    return Value(TYPE_INVALID);
                } },
            expr.val);
    }

    Value Typechecker::typecheck_ast_expr_literal(
        ast::Expr::Literal& lit, std::optional<ast::Type> expected_type) {
        return std::visit(
            util::overload {
                [&](ast::Invalid&) { return Value(TYPE_INVALID); },
                [&](ast::Expr::Literal::Unit&) {
                    return Value(ast::Type(ast::Type::Unit {}));
                },
                [&](ast::Expr::Literal::Null&) {
                    if (!expected_type) {
                        err(diag::Err::CantInferTypeOfNullLiteral {});
                        return Value(TYPE_INVALID);
                    }

                    auto udtype =
                        std::get_if<ast::Type::ResolvedUserDefinedType>(
                            &expected_type->val);
                    if (!udtype) {
                        err(diag::Err::CantBeNull {});
                        return Value(TYPE_INVALID);
                    }
                    ALVO_ASSERT(m_index->user_defined_types.has_id(udtype->id));
                    auto& type =
                        m_index->user_defined_types.get_by_id(udtype->id);
                    if (!std::holds_alternative<UserDefinedType::Struct>(
                            type.val) &&
                        !std::holds_alternative<UserDefinedType::Interface>(
                            type.val)) {
                        err(diag::Err::CantBeNull {});
                        return Value(TYPE_INVALID);
                    }

                    return Value(*expected_type);
                },
                [&](ast::Expr::Literal::String&) {
                    return Value(ast::Type(ast::Type::String {}));
                },
                [&](ast::Expr::Literal::Character&) {
                    return Value(ast::Type(ast::Type::Char {}));
                },
                [&](ast::Expr::Literal::Integer&) {
                    return Value(ast::Type(ast::Type::Int {}));
                },
                [&](ast::Expr::Literal::Byte&) {
                    return Value(ast::Type(ast::Type::Byte {}));
                },
                [&](ast::Expr::Literal::Floating&) {
                    return Value(ast::Type(ast::Type::Float {}));
                },
                [&](ast::Expr::Literal::Boolean&) {
                    return Value(ast::Type(ast::Type::Bool {}));
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
                    return Value(TYPE_INVALID);
                },
                [&](ast::Expr::Literal::ResolvedStruct& resolved_struct) {
                    return typecheck_ast_expr_literal_resolved_struct(
                        resolved_struct, expected_type);
                },
                [&](ast::util::Ptr<ast::Func>& func) {
                    return Value(typecheck_ast_func(*func, expected_type));
                },
            },
            lit.val);
    }

    Value Typechecker::typecheck_ast_expr_literal_array(
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
                            times_type.type.val)) {
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
                            times_type.type.val)) {
                        err_expected("int");
                        return TYPE_INVALID;
                    }

                    return ast::Type(ast::Type::Array(
                        false, m_node_ctx.make_node<ast::Type>(expr_type)));
                },
            },
            array.val);
    }

    Value Typechecker::typecheck_ast_expr_literal_tup(
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

    Value Typechecker::typecheck_ast_expr_literal_resolved_struct(
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

    Value Typechecker::typecheck_ast_expr_unop(ast::Expr::Unop& unop,
        [[maybe_unused]] std::optional<ast::Type> expected_type) {
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
                expr_type.type.val);
            if (bad_type) {
                err(diag::Err::BadUnaryExpressionType {});
                return TYPE_INVALID;
            }
            return expr_type;
        }

        if (unop.op == BinaryNot) {
            auto expr_type = typecheck_ast_expr(*unop.expr);
            bool bad_type;
            if (std::holds_alternative<ast::Type::Int>(expr_type.type.val)) {
                bad_type = false;
            } else if (std::holds_alternative<ast::Type::Byte>(
                           expr_type.type.val)) {
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

    Value Typechecker::typecheck_ast_expr_binop(ast::Expr::Binop& binop,
        [[maybe_unused]] std::optional<ast::Type> expected_type) {
        auto l_type = typecheck_ast_expr(*binop.lhs);
        auto r_type = typecheck_ast_expr(*binop.rhs);

        if (type_is_invalid(l_type.type))
            return TYPE_INVALID;
        if (type_is_invalid(r_type.type))
            return TYPE_INVALID;

        if (r_type.type != l_type.type) {
            err(diag::Err::IncompatibleTypesInBinaryExpression {});
            return TYPE_INVALID;
        }

        switch (binop.op) {
        case ast::Expr::Binop::Op::Invalid:
            return Value(TYPE_INVALID);
        case ast::Expr::Binop::Op::Assign:
        case ast::Expr::Binop::Op::PlusAssign:
        case ast::Expr::Binop::Op::MinusAssign:
        case ast::Expr::Binop::Op::MultiplyAssign:
        case ast::Expr::Binop::Op::DivideAssign:
        case ast::Expr::Binop::Op::BinaryAndAssign:
        case ast::Expr::Binop::Op::BinaryOrAssign:
        case ast::Expr::Binop::Op::BinaryXorAssign:
        case ast::Expr::Binop::Op::ModAssign:
        case ast::Expr::Binop::Op::ShiftLeftAssign:
        case ast::Expr::Binop::Op::ShiftRightAssign: {
            std::unordered_set<ast::Type> allowed_types;
            switch (binop.op) {
            case ast::Expr::Binop::Op::Assign:
                allowed_types = {};
                break;

            case ast::Expr::Binop::Op::PlusAssign:
            case ast::Expr::Binop::Op::MinusAssign:
            case ast::Expr::Binop::Op::MultiplyAssign:
            case ast::Expr::Binop::Op::DivideAssign:
                allowed_types = { TYPE_INT, TYPE_BYTE, TYPE_FLOAT };
                break;

            case ast::Expr::Binop::Op::BinaryAndAssign:
            case ast::Expr::Binop::Op::BinaryOrAssign:
            case ast::Expr::Binop::Op::BinaryXorAssign:
            case ast::Expr::Binop::Op::ModAssign:
            case ast::Expr::Binop::Op::ShiftLeftAssign:
            case ast::Expr::Binop::Op::ShiftRightAssign:
                allowed_types = { TYPE_INT, TYPE_BYTE };
                break;

            default:
                ALVO_UNREACHABLE();
            }

            if (!l_type.is_assignable) {
                err(diag::Err::ExpressionNotAssignable {});
                return TYPE_INVALID;
            }

            if (!allowed_types.empty() &&
                !allowed_types.contains(l_type.type)) {
                err(diag::Err::BadTypeForBinaryExpression {});
                return TYPE_INVALID;
            }

            return Value(l_type.type);
        } break;
        case ast::Expr::Binop::Op::Or:
        case ast::Expr::Binop::Op::And:
        case ast::Expr::Binop::Op::BinaryOr:
        case ast::Expr::Binop::Op::BinaryXor:
        case ast::Expr::Binop::Op::BinaryAnd:
        case ast::Expr::Binop::Op::Equal:
        case ast::Expr::Binop::Op::NotEqual:
        case ast::Expr::Binop::Op::Less:
        case ast::Expr::Binop::Op::LessEqual:
        case ast::Expr::Binop::Op::Greater:
        case ast::Expr::Binop::Op::GreaterEqual:
        case ast::Expr::Binop::Op::ShiftLeft:
        case ast::Expr::Binop::Op::ShiftRight:
        case ast::Expr::Binop::Op::Plus:
        case ast::Expr::Binop::Op::Minus:
        case ast::Expr::Binop::Op::Multiply:
        case ast::Expr::Binop::Op::Divide:
        case ast::Expr::Binop::Op::Mod: {
            std::unordered_set<ast::Type> allowed_types;
            ast::Type res_type = l_type.type;

            switch (binop.op) {
            case ast::Expr::Binop::Op::Or:
            case ast::Expr::Binop::Op::And:
                allowed_types = { TYPE_BOOL };
                break;

            case ast::Expr::Binop::Op::BinaryOr:
            case ast::Expr::Binop::Op::BinaryXor:
            case ast::Expr::Binop::Op::BinaryAnd:
                allowed_types = { TYPE_INT, TYPE_BYTE };
                break;

            case ast::Expr::Binop::Op::Equal:
            case ast::Expr::Binop::Op::NotEqual:
                allowed_types = {};
                res_type = TYPE_BOOL;
                break;

            case ast::Expr::Binop::Op::Less:
            case ast::Expr::Binop::Op::LessEqual:
            case ast::Expr::Binop::Op::Greater:
            case ast::Expr::Binop::Op::GreaterEqual:
                allowed_types = { TYPE_INT, TYPE_BYTE, TYPE_FLOAT };
                res_type = TYPE_BOOL;
                break;

            case ast::Expr::Binop::Op::ShiftLeft:
            case ast::Expr::Binop::Op::ShiftRight:
                allowed_types = { TYPE_INT, TYPE_BYTE };
                break;

            case ast::Expr::Binop::Op::Plus:
            case ast::Expr::Binop::Op::Minus:
            case ast::Expr::Binop::Op::Multiply:
            case ast::Expr::Binop::Op::Divide:
                allowed_types = { TYPE_INT, TYPE_BYTE, TYPE_FLOAT };
                break;

            case ast::Expr::Binop::Op::Mod:
                allowed_types = { TYPE_INT, TYPE_BYTE };
                break;

            default:
                ALVO_UNREACHABLE();
            }

            if (!allowed_types.empty() &&
                !allowed_types.contains(l_type.type)) {
                err(diag::Err::BadTypeForBinaryExpression {});
                return TYPE_INVALID;
            }

            return Value(res_type);
        } break;
        }
    }

    Value Typechecker::typecheck_ast_expr_index(ast::Expr::Index& index,
        [[maybe_unused]] std::optional<ast::Type> expected_type) {
        auto expr_type = typecheck_ast_expr(*index.expr);
        auto index_type = typecheck_ast_expr(*index.index, TYPE_INT);

        if (index_type.type != TYPE_INT) {
            err(diag::Err::NonIntegerIndex {});
            return Value(TYPE_INVALID);
        }

        if (auto array = std::get_if<ast::Type::Array>(&expr_type.type.val)) {
            return Value(*array->type, true);
        } else {
            err(diag::Err::AttemptedToIndexNonArray {});
            return Value(TYPE_INVALID);
        }
    }

    Value Typechecker::typecheck_ast_expr_call(ast::Expr::Call& call,
        [[maybe_unused]] std::optional<ast::Type> expected_type) {
        auto expr_type = typecheck_ast_expr(*call.expr);
        if (type_is_invalid(expr_type.type)) {
            return TYPE_INVALID;
        }

        if (auto func = std::get_if<ast::Type::Func>(&expr_type.type.val)) {
            if (func->params.size() != call.args.size()) {
                err(diag::Err::IncorrectNumberOfArgumentsForFunctionCall {});
                return TYPE_INVALID;
            }

            auto param_iter = func->params.begin();
            auto arg_iter = call.args.begin();
            while (param_iter != func->params.end()) {
                auto& param_type = *param_iter;
                auto& arg_expr = *arg_iter;

                auto arg_type = typecheck_ast_expr(arg_expr, param_type);
                if (type_is_invalid(arg_type.type)) {
                    return TYPE_INVALID;
                }

                if (arg_type.type != param_type) {
                    err_expected_type(param_type);
                    return TYPE_INVALID;
                }

                param_iter++;
                arg_iter++;
            }

            return *func->return_type;
        } else {
            err(diag::Err::AttemptedToCallNonFunction {});
            return TYPE_INVALID;
        }
    }

    Value Typechecker::typecheck_ast_expr_cast(ast::Expr::Cast& cast,
        [[maybe_unused]] std::optional<ast::Type> expected_type) {
        if (type_is_invalid(cast.type)) {
            return TYPE_INVALID;
        }

        auto expr_type = typecheck_ast_expr(*cast.expr);
        if (type_is_invalid(expr_type.type)) {
            return TYPE_INVALID;
        }

        std::unordered_set<ast::Type> allowed_types;
        bool bad_type = false;

        std::visit(
            util::overload {
                [&](ast::Invalid&) { },
                [&](ast::Type::Unit&) { allowed_types = {}; },
                [&](ast::Type::String&) { bad_type = true; },
                [&](ast::Type::Char&) { bad_type = true; },
                [&](ast::Type::Int&) {
                    allowed_types = { TYPE_INT, TYPE_BYTE, TYPE_FLOAT };
                },
                [&](ast::Type::Byte&) {
                    allowed_types = { TYPE_INT, TYPE_BYTE, TYPE_FLOAT };
                },
                [&](ast::Type::Float&) {
                    allowed_types = { TYPE_INT, TYPE_BYTE, TYPE_FLOAT };
                },
                [&](ast::Type::Bool&) { bad_type = true; },
                [&](ast::Type::Array&) { bad_type = true; },
                [&](ast::Type::Tup&) { bad_type = true; },
                [&](ast::Type::Func&) { bad_type = true; },
                [&](ast::Type::Name&) { ALVO_UNREACHABLE(); },
                [&](ast::Type::LocalGeneric&) { bad_type = true; },
                [&](ast::Type::ResolvedUserDefinedType&) { bad_type = true; },
            },
            cast.type.val);

        if (bad_type) {
            err(diag::Err::BadCastType {});
            return TYPE_INVALID;
        }

        if (!allowed_types.empty() && !allowed_types.contains(expr_type.type)) {
            err(diag::Err::BadCastExprType {});
            return TYPE_INVALID;
        }

        return cast.type;
    }

    Value Typechecker::typecheck_ast_expr_try_cast(
        [[maybe_unused]] ast::Expr::TryCast& try_cast,
        [[maybe_unused]] std::optional<ast::Type> expected_type) {
        ALVO_NOT_IMPLEMENTED();
        return TYPE_INVALID;
    }

    Value Typechecker::typecheck_ast_expr_builtin(
        [[maybe_unused]] ast::Expr::Builtin& builtin,
        [[maybe_unused]] std::optional<ast::Type> expected_type) {
        ALVO_NOT_IMPLEMENTED();
        return TYPE_INVALID;
    }

    std::pair<Value, ast::Expr::ResolvedMemberAccess>
    Typechecker::typecheck_ast_expr_member_access(
        ast::Expr::MemberAccess& member_access,
        [[maybe_unused]] std::optional<ast::Type> expected_type) {
        static const std::pair<Value, ast::Expr::ResolvedMemberAccess>
            INVALID = { TYPE_INVALID, { nullptr, 0 } };

        auto expr_type = typecheck_ast_expr(*member_access.expr);
        if (member_access.name.is_invalid) {
            return INVALID;
        }

        if (auto udtype = std::get_if<ast::Type::ResolvedUserDefinedType>(
                &expr_type.type.val)) {
            auto& udtype_val =
                m_index->user_defined_types.get_by_id(udtype->id);
            if (auto struct_ =
                    std::get_if<UserDefinedType::Struct>(&udtype_val.val)) {
                if (!struct_->members.has(member_access.name.name)) {
                    err(diag::Err::NoMemberFound {});
                    return INVALID;
                }
                auto& member = struct_->members.get(member_access.name.name);
                auto id = struct_->members.get_id(member_access.name.name);
                if (auto field =
                        std::get_if<UserDefinedType::Struct::Member::Field>(
                            &member.val)) {

                    Value res_val = field->type;
                    if (expr_type.is_assignable) {
                        res_val.is_assignable = true;
                    }
                    return { res_val, ast::Expr::ResolvedMemberAccess(
                                          member_access.expr, id) };
                } else {
                    err(diag::Err::NoMemberFound {});
                    return INVALID;
                }
            } else {
                err(diag::Err::MemberAccessOnNonStruct {});
                return INVALID;
            }
        } else {
            err(diag::Err::MemberAccessOnNonStruct {});
            return INVALID;
        }
    }

    Value Typechecker::typecheck_ast_expr_local_var(
        ast::Expr::LocalVar& local_var,
        [[maybe_unused]] std::optional<ast::Type> expected_type) {
        auto entry = m_scope_stack.get_by_id(local_var.id);
        return Value(entry.element, true);
    }

    Value Typechecker::typecheck_ast_expr_func_arg(ast::Expr::FuncArg& func_arg,
        [[maybe_unused]] std::optional<ast::Type> expected_type) {
        auto entry = m_arg_stack.get(func_arg.name);
        return Value(entry.element, true);
    }

    Value Typechecker::typecheck_ast_expr_resolved_decl(
        ast::Expr::ResolvedDecl& resolved_decl,
        [[maybe_unused]] std::optional<ast::Type> expected_type) {
        ast::Id decl_id = resolved_decl.decl_id;
        if (!resolved_decl.generic_params.empty()) {
            auto new_id =
                instantiate_decl(decl_id, resolved_decl.generic_params);
            if (!new_id) {
                return TYPE_INVALID;
            }
            decl_id = *new_id;
        }
        ALVO_ASSERT(m_index->decls.has_id(decl_id));
        auto& decl = m_index->decls.get_by_id(decl_id);
        return std::visit(
            util::overload {
                [&](Decl::Func& func) {
                    return Value(ast::Type(decl_func_to_type_func(func.func)));
                },
            },
            decl.val);
    }

    Value Typechecker::typecheck_ast_expr_resolved_type_member_access(
        ast::Expr::ResolvedTypeMemberAccess& resolved_type_member_access,
        [[maybe_unused]] std::optional<ast::Type> expected_type) {
        bool invalid_type = false;
        bool bad_type = false;
        std::optional<ast::Type::ResolvedUserDefinedType> udtype = std::nullopt;
        std::visit(
            util::overload {
                [&](ast::Invalid&) { invalid_type = true; },
                [&](ast::Type::Unit&) { bad_type = true; },
                [&](ast::Type::String&) { bad_type = true; },
                [&](ast::Type::Char&) { bad_type = true; },
                [&](ast::Type::Int&) { bad_type = true; },
                [&](ast::Type::Byte&) { bad_type = true; },
                [&](ast::Type::Float&) { bad_type = true; },
                [&](ast::Type::Bool&) { bad_type = true; },
                [&](ast::Type::Array&) { bad_type = true; },
                [&](ast::Type::Tup&) { bad_type = true; },
                [&](ast::Type::Func&) { bad_type = true; },
                [&](ast::Type::Name&) { ALVO_UNREACHABLE(); },
                [&](ast::Type::LocalGeneric&) { bad_type = true; },
                [&](ast::Type::ResolvedUserDefinedType& resolved_udtype) {
                    udtype = resolved_udtype;
                },
            },
            resolved_type_member_access.type.val);
        ALVO_ASSERT(m_index->user_defined_types.has_id(udtype->id));

        if (invalid_type) {
            return TYPE_INVALID;
        }
        if (bad_type) {
            err(diag::Err::BadTypeMemberAccessType {});
            return TYPE_INVALID;
        }

        ALVO_ASSERT(udtype.has_value());

        ast::Id type_id = udtype->id;
        if (!udtype->generic_params.empty()) {
            auto new_id = instantiate_type(type_id, udtype->generic_params);
            if (!new_id) {
                return TYPE_INVALID;
            }
            type_id = *new_id;
        }

        ALVO_ASSERT(m_index->user_defined_types.has_id(type_id));
        auto& udtype_val = m_index->user_defined_types.get_by_id(type_id);

        std::visit(util::overload {
                       [&](UserDefinedType::Struct& struct_) { },
                       [&](UserDefinedType::Enum& enum_) { },
                       [&](UserDefinedType::Interface& interface_) { },
                   },
            udtype_val.val);

        if (!resolved_type_member_access.generic_params.empty()) { }
    }

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

    std::optional<ast::Id> Typechecker::instantiate_type(ast::Id id,
        [[maybe_unused]] const ast::util::List<ast::Type> generic_params) {
        ALVO_ASSERT(m_index->user_defined_types.has_id(id));
        [[maybe_unused]] auto& udtype =
            m_index->user_defined_types.get_by_id(id);
        ALVO_NOT_IMPLEMENTED();
        return std::nullopt;
    }

    std::optional<ast::Id> Typechecker::instantiate_member(ast::Id type_id,
        [[maybe_unused]] ast::Id member_id,
        [[maybe_unused]] const ast::util::List<ast::Type> generic_params) {
        ALVO_ASSERT(m_index->user_defined_types.has_id(type_id));
        [[maybe_unused]] auto& udtype =
            m_index->user_defined_types.get_by_id(type_id);
        ALVO_NOT_IMPLEMENTED();
        return std::nullopt;
    }

    std::optional<ast::Id> Typechecker::instantiate_named_member(
        ast::Id type_id, [[maybe_unused]] std::string_view member_name,
        [[maybe_unused]] const ast::util::List<ast::Type> generic_params) {
        ALVO_ASSERT(m_index->user_defined_types.has_id(type_id));
        [[maybe_unused]] auto& udtype =
            m_index->user_defined_types.get_by_id(type_id);
        ALVO_NOT_IMPLEMENTED();
        return std::nullopt;
    }

    std::optional<ast::Id> Typechecker::instantiate_decl(ast::Id decl_id,
        [[maybe_unused]] const ast::util::List<ast::Type> generic_params) {
        ALVO_ASSERT(m_index->decls.has_id(decl_id));
        [[maybe_unused]] auto& decl = m_index->decls.get_by_id(decl_id);
        ALVO_NOT_IMPLEMENTED();
        return std::nullopt;
    }

    ast::Type::Func Typechecker::decl_func_to_type_func(const ast::Func& func) {
        ast::util::List<ast::Type> params;
        for (const auto& param : func.signature.params) {
            params.push_back(*m_arena, param.type);
        }
        return ast::Type::Func(
            false, params, m_node_ctx.make_node<ast::Type>(func.signature.ret));
    }
}
