#include "eval.h"

#include "util.h"

#include <fmt/base.h>

namespace alvo::eval {
    static const Value VALUE_DEFAULT = Value(Value::Int { .value = -1234 });

    Interpreter::Interpreter(sema::resolve::NameIndex& index) :
        m_index(&index),
        m_stack() { }

    PlaceValue Interpreter::eval_expr(const ast::Expr& expr) {
        return std::visit(
            util::overload {
                [&](const ast::Invalid&) {
                    ALVO_UNREACHABLE();
                    return PlaceValue::value(VALUE_DEFAULT);
                },
                [&](const ast::Expr::Literal& literal) {
                    return PlaceValue::value(eval_literal(literal));
                },
                [&](const ast::Expr::Unop& unop) {
                    return PlaceValue::value(eval_unop(unop));
                },
                [&](const ast::Expr::Binop& binop) {
                    return eval_binop(binop);
                },
                [&](const ast::Expr::Index& index) {
                    return eval_index(index);
                },
                [&](const ast::Expr::Call& call) {
                    return PlaceValue::value(eval_call(call));
                },
                [&](const ast::Expr::Cast& cast) {
                    return PlaceValue::value(eval_cast(cast));
                },
                [&](const ast::Expr::TryCast&) {
                    ALVO_NOT_IMPLEMENTED();
                    return PlaceValue::value(VALUE_DEFAULT);
                },
                [&](const ast::Expr::Builtin&) {
                    ALVO_NOT_IMPLEMENTED();
                    return PlaceValue::value(VALUE_DEFAULT);
                },
                [&](const ast::Expr::Name&) {
                    ALVO_UNREACHABLE();
                    return PlaceValue::value(VALUE_DEFAULT);
                },
                [&](const ast::Expr::TypeMemberAccess&) {
                    ALVO_UNREACHABLE();
                    return PlaceValue::value(VALUE_DEFAULT);
                },
                [&](const ast::Expr::MemberAccess&) {
                    ALVO_UNREACHABLE();
                    return PlaceValue::value(VALUE_DEFAULT);
                },
                [&](const ast::Expr::LocalVar& local_var) {
                    ALVO_ASSERT(m_stack.has_id(local_var.id));
                    auto var = m_stack.get_by_id(local_var.id);
                    return PlaceValue::place(&var.element);
                },
                [&](const ast::Expr::FuncArg& func_arg) {
                    ALVO_ASSERT(m_arg_stack.has(func_arg.name));
                    auto arg = m_arg_stack.get(func_arg.name);
                    return PlaceValue::place(&arg.element);
                },
                [&](const ast::Expr::ResolvedDecl& decl) {
                    return PlaceValue::value(Value::function(decl.decl_id));
                },
                [&](const ast::Expr::ResolvedTypeMemberAccess&) {
                    ALVO_NOT_IMPLEMENTED();
                    return PlaceValue::value(VALUE_DEFAULT);
                },
                [&](const ast::Expr::ResolvedMemberAccess&
                        resolved_member_access) {
                    return eval_resolved_member_access(resolved_member_access);
                },
            },
            expr.val);
    }

    Value Interpreter::eval_literal(const ast::Expr::Literal& literal) {
        return std::visit(
            util::overload {
                [&](const ast::Invalid&) {
                    ALVO_UNREACHABLE();
                    return VALUE_DEFAULT;
                },
                [&](const ast::Expr::Literal::Unit&) { return Value::unit(); },
                [&](const ast::Expr::Literal::Null&) { return Value::null(); },
                [&](const ast::Expr::Literal::String& string) {
                    return Value::string(string.value);
                },
                [&](const ast::Expr::Literal::Character& character) {
                    return Value::character(character.value);
                },
                [&](const ast::Expr::Literal::Integer& integer) {
                    return Value::integer(integer.value);
                },
                [&](const ast::Expr::Literal::Byte& byte) {
                    return Value::byte(byte.value);
                },
                [&](const ast::Expr::Literal::Floating& floating) {
                    return Value::floating(floating.value);
                },
                [&](const ast::Expr::Literal::Boolean& boolean) {
                    return Value::boolean(boolean.value);
                },
                [&](const ast::Expr::Literal::Array& array) {
                    std::vector<Value> values;
                    std::visit(
                        util::overload {
                            [&](const ast::Invalid&) { ALVO_UNREACHABLE(); },
                            [&](const ast::Expr::Literal::Array::Regular&
                                    regular) {
                                values.reserve(regular.elements.size());
                                for (const auto& element : regular.elements) {
                                    values.push_back(eval_expr(element));
                                }
                            },
                            [&](const ast::Expr::Literal::Array::
                                    DefaultNTimes&) { ALVO_NOT_IMPLEMENTED(); },
                            [&](const ast::Expr::Literal::Array::ExprNTimes&) {
                                ALVO_NOT_IMPLEMENTED();
                            },
                        },
                        array.val);

                    return Value::array(values);
                },
                [&](const ast::Expr::Literal::Tup& tup) {
                    std::vector<Value> values;
                    for (const auto& expr : tup.exprs) {
                        values.push_back(eval_expr(expr));
                    }

                    return Value::tup(values);
                },
                [&](const ast::Expr::Literal::Struct&) {
                    ALVO_UNREACHABLE();
                    return VALUE_DEFAULT;
                },
                [&](const ast::Expr::Literal::ResolvedStruct& resolved_struct) {
                    std::vector<Value> fields;
                    for (const auto& field : resolved_struct.fields) {
                        fields.resize(field.member_id.id + 1);
                        fields.insert(fields.begin() + field.member_id.id,
                            eval_expr(*field.expr));
                    }
                    return Value::structure(fields);
                },
                [&](const ast::util::Ptr<ast::Func>&) {
                    ALVO_NOT_IMPLEMENTED();
                    return VALUE_DEFAULT;
                },
            },
            literal.val);
    }

    Value Interpreter::eval_unop(const ast::Expr::Unop& unop) {
        Value val = eval_expr(*unop.expr);

        switch (unop.op) {
        case ast::Expr::Unop::Op::Invalid:
            return VALUE_DEFAULT;
        case ast::Expr::Unop::Op::Plus:
            return val.unary_plus();
        case ast::Expr::Unop::Op::Minus:
            return val.unary_minus();
        case ast::Expr::Unop::Op::Not:
            return val.not_();
        case ast::Expr::Unop::Op::BinaryNot:
            return val.binary_not();
        }
    }

    PlaceValue Interpreter::eval_binop(const ast::Expr::Binop& binop) {
        PlaceValue lpval = eval_expr(*binop.lhs);
        Value lval = lpval;
        Value rval = eval_expr(*binop.rhs);

        switch (binop.op) {
        case ast::Expr::Binop::Op::Invalid:
            ALVO_UNREACHABLE();
            return PlaceValue::value(VALUE_DEFAULT);
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
        case ast::Expr::Binop::Op::ShiftRightAssign:
            if (auto lp = std::get_if<Value*>(&lpval.val)) {
                switch (binop.op) {
                case ast::Expr::Binop::Op::Assign:
                    **lp = rval;
                    break;
                case ast::Expr::Binop::Op::PlusAssign:
                    **lp = (**lp).plus(rval);
                    break;
                case ast::Expr::Binop::Op::MinusAssign:
                    **lp = (**lp).minus(rval);
                    break;
                case ast::Expr::Binop::Op::MultiplyAssign:
                    **lp = (**lp).multiply(rval);
                    break;
                case ast::Expr::Binop::Op::DivideAssign:
                    **lp = (**lp).divide(rval);
                    break;
                case ast::Expr::Binop::Op::BinaryAndAssign:
                    **lp = (**lp).binary_and(rval);
                    break;
                case ast::Expr::Binop::Op::BinaryOrAssign:
                    **lp = (**lp).binary_or(rval);
                    break;
                case ast::Expr::Binop::Op::BinaryXorAssign:
                    **lp = (**lp).binary_xor(rval);
                    break;
                case ast::Expr::Binop::Op::ModAssign:
                    **lp = (**lp).mod(rval);
                    break;
                case ast::Expr::Binop::Op::ShiftLeftAssign:
                    **lp = (**lp).shift_left(rval);
                    break;
                case ast::Expr::Binop::Op::ShiftRightAssign:
                    **lp = (**lp).shift_right(rval);
                    break;
                default:
                    ALVO_UNREACHABLE();
                    break;
                }
                return PlaceValue::place(*lp);
            } else {
                ALVO_UNREACHABLE();
                return PlaceValue::value(VALUE_DEFAULT);
            }
        case ast::Expr::Binop::Op::Or:
            return PlaceValue::value(lval.or_(rval));
        case ast::Expr::Binop::Op::And:
            return PlaceValue::value(lval.and_(rval));
        case ast::Expr::Binop::Op::BinaryOr:
            return PlaceValue::value(lval.binary_or(rval));
        case ast::Expr::Binop::Op::BinaryXor:
            return PlaceValue::value(lval.binary_xor(rval));
        case ast::Expr::Binop::Op::BinaryAnd:
            return PlaceValue::value(lval.binary_and(rval));
        case ast::Expr::Binop::Op::Equal:
            return PlaceValue::value(lval.equal(rval));
        case ast::Expr::Binop::Op::NotEqual:
            return PlaceValue::value(lval.not_equal(rval));
        case ast::Expr::Binop::Op::Less:
            return PlaceValue::value(lval.less(rval));
        case ast::Expr::Binop::Op::LessEqual:
            return PlaceValue::value(lval.less_equal(rval));
        case ast::Expr::Binop::Op::Greater:
            return PlaceValue::value(lval.greater(rval));
        case ast::Expr::Binop::Op::GreaterEqual:
            return PlaceValue::value(lval.greater_equal(rval));
        case ast::Expr::Binop::Op::ShiftLeft:
            return PlaceValue::value(lval.shift_left(rval));
        case ast::Expr::Binop::Op::ShiftRight:
            return PlaceValue::value(lval.shift_right(rval));
        case ast::Expr::Binop::Op::Plus:
            return PlaceValue::value(lval.plus(rval));
        case ast::Expr::Binop::Op::Minus:
            return PlaceValue::value(lval.minus(rval));
        case ast::Expr::Binop::Op::Multiply:
            return PlaceValue::value(lval.multiply(rval));
        case ast::Expr::Binop::Op::Divide:
            return PlaceValue::value(lval.divide(rval));
        case ast::Expr::Binop::Op::Mod:
            return PlaceValue::value(lval.mod(rval));
        }
    }

    PlaceValue Interpreter::eval_index(const ast::Expr::Index& index) {
        PlaceValue indexed_value = eval_expr(*index.expr);
        Value index_value = eval_expr(*index.index);
        if (!std::holds_alternative<Value::Int>(index_value.val)) {
            ALVO_UNREACHABLE();
        }
        std::int64_t index_int = std::get<Value::Int>(index_value.val).value;

        return std::visit(
            util::overload {
                [&](Value* value) {
                    if (auto array = std::get_if<Value::Array>(&value->val)) {
                        return PlaceValue::place(
                            &array->value->elements[index_int]);
                    } else {
                        ALVO_UNREACHABLE();
                        return PlaceValue::value(VALUE_DEFAULT);
                    }
                },
                [&](const Value& value) {
                    if (auto array = std::get_if<Value::Array>(&value.val)) {
                        return PlaceValue::value(
                            array->value->elements[index_int]);
                    } else {
                        ALVO_UNREACHABLE();
                        return PlaceValue::value(VALUE_DEFAULT);
                    }
                },
            },
            indexed_value.val);
    }

    Value Interpreter::eval_call(const ast::Expr::Call& call) {
        Value expr_val = eval_expr(*call.expr);
        if (auto func = std::get_if<Value::Func>(&expr_val.val)) {
            std::vector<Value> args;
            args.reserve(call.args.size());
            for (const auto& arg : call.args) {
                Value arg_value = eval_expr(arg);
                args.push_back(arg_value);
            }
            return call_function(func->decl_id, args);
        } else {
            ALVO_UNREACHABLE();
            return VALUE_DEFAULT;
        }
    }

    Value Interpreter::eval_cast(const ast::Expr::Cast& cast) {
        Value value = eval_expr(*cast.expr);
        return std::visit(
            util::overload {
                [&](const ast::Type::Int&) {
                    return std::visit(
                        util::overload { [&](Value::Int&) { return value; },
                            [&](Value::Byte& val) {
                                return Value::byte(
                                    static_cast<std::uint8_t>(val.value));
                            },
                            [&](Value::Float& val) {
                                return Value::floating(
                                    static_cast<float>(val.value));
                            },
                            [&](auto) {
                                ALVO_UNREACHABLE();
                                return VALUE_DEFAULT;
                            } },
                        value.val);
                },
                [&](const ast::Type::Byte&) {
                    return std::visit(
                        util::overload { [&](Value::Int& val) {
                                            return Value::integer(
                                                static_cast<std::int64_t>(
                                                    val.value));
                                        },
                            [&](Value::Byte&) { return value; },
                            [&](Value::Float& val) {
                                return Value::floating(
                                    static_cast<float>(val.value));
                            },
                            [&](auto) {
                                ALVO_UNREACHABLE();
                                return VALUE_DEFAULT;
                            } },
                        value.val);
                },
                [&](const ast::Type::Float&) {
                    return std::visit(
                        util::overload { [&](Value::Int& val) {
                                            return Value::integer(
                                                static_cast<std::int64_t>(
                                                    val.value));
                                        },
                            [&](Value::Byte& val) {
                                return Value::byte(
                                    static_cast<std::uint8_t>(val.value));
                            },
                            [&](Value::Float&) { return value; },
                            [&](auto) {
                                ALVO_UNREACHABLE();
                                return VALUE_DEFAULT;
                            } },
                        value.val);
                },
                [&](auto) {
                    ALVO_UNREACHABLE();
                    return VALUE_DEFAULT;
                },
            },
            cast.type.val);
    }

    PlaceValue Interpreter::eval_resolved_member_access(
        const ast::Expr::ResolvedMemberAccess& resolved_member_access) {
        auto val = eval_expr(*resolved_member_access.expr);
        return std::visit(
            util::overload {
                [&](Value* value) {
                    if (auto struct_ =
                            std::get_if<Value::Struct>(&value->val)) {
                        Value* res =
                            &struct_->value
                                 ->fields[resolved_member_access.member_id.id];
                        return PlaceValue::place(res);
                    } else {
                        ALVO_UNREACHABLE();
                        return PlaceValue::value(VALUE_DEFAULT);
                    }
                },
                [&](const Value& value) {
                    if (auto struct_ = std::get_if<Value::Struct>(&value.val)) {
                        Value res =
                            struct_->value
                                ->fields[resolved_member_access.member_id.id];
                        return PlaceValue::value(res);
                    } else {
                        ALVO_UNREACHABLE();
                        return PlaceValue::value(VALUE_DEFAULT);
                    }
                } },
            val.val);
    }

    Value Interpreter::call_function(
        ast::Id decl_id, const std::vector<Value>& args) {
        ALVO_ASSERT(m_index->decls.has_id(decl_id));
        auto& decl = m_index->decls.get_by_id(decl_id);
        if (auto func = std::get_if<sema::resolve::Decl::Func>(&decl.val)) {
            m_arg_stack.push();

            auto param_iter = func->func.signature.params.begin();
            auto arg_iter = args.begin();
            while (arg_iter != args.end()) {
                m_arg_stack.put(param_iter->name, *arg_iter);
                param_iter++;
                arg_iter++;
            }

            auto res = eval_block(func->func.block);

            m_arg_stack.pop();

            ALVO_ASSERT(res.is_return());
            return res.get_return();
        } else {
            ALVO_UNREACHABLE();
            return VALUE_DEFAULT;
        }
    }

    StmtResult Interpreter::eval_block(const ast::Block& block) {
        m_stack.push();

        for (const auto& stmt : block.stmts) {
            auto res = eval_stmt(stmt);
            if (!res.is_none()) {
                m_stack.pop();
                return res;
            }
        }

        m_stack.pop();
        return StmtResult::none();
    }

    StmtResult Interpreter::eval_stmt(const ast::Stmt& stmt) {
        return std::visit(
            util::overload {
                [&](const ast::Invalid&) {
                    ALVO_UNREACHABLE();
                    return StmtResult::none();
                },
                [&](const ast::Expr& expr) {
                    eval_expr(expr);
                    return StmtResult::none();
                },
                [&](const ast::Stmt::Let& let) { return eval_stmt_let(let); },
                [&](const ast::Stmt::If& if_) { return eval_stmt_if(if_); },
                [&](const ast::Stmt::Switch& switch_) {
                    return eval_stmt_switch(switch_);
                },
                [&](const ast::Stmt::Loop& loop) {
                    return eval_stmt_loop(loop);
                },
                [&](const ast::Stmt::For& for_) { return eval_stmt_for(for_); },
                [&](const ast::Stmt::While& while_) {
                    return eval_stmt_while(while_);
                },
                [&](const ast::Stmt::Return& return_) {
                    if (return_.expr) {
                        Value res = eval_expr(*return_.expr);
                        return StmtResult::return_(res);
                    } else {
                        return StmtResult::return_(Value::unit());
                    }
                },
                [&](const ast::Stmt::Defer&) {
                    ALVO_NOT_IMPLEMENTED();
                    return StmtResult::none();
                },
                [&](const ast::Block& block) { return eval_block(block); },
                [&](const ast::Stmt::Continue&) {
                    return StmtResult::continue_();
                },
                [&](const ast::Stmt::Break&) { return StmtResult::break_(); },
                [&](const ast::Stmt::Print& print) {
                    return eval_stmt_print(print);
                },
            },
            stmt.val);
    }

    StmtResult Interpreter::eval_stmt_let(const ast::Stmt::Let& let) {
        Value value = eval_expr(let.expr);
        m_stack.put(let.name, value);
        return StmtResult::none();
    }

    StmtResult Interpreter::eval_stmt_if(const ast::Stmt::If& if_) {
        Value main_cond = eval_expr(if_.expr);
        if (auto main_cond_val = std::get_if<Value::Bool>(&main_cond.val)) {
            if (main_cond_val->value) {
                return eval_block(if_.main);
            }
        } else {
            ALVO_UNREACHABLE();
            return StmtResult::none();
        }

        for (const auto& elif : if_.elifs) {
            Value elif_cond = eval_expr(elif.expr);
            if (auto elif_cond_val = std::get_if<Value::Bool>(&elif_cond.val)) {
                if (elif_cond_val->value) {
                    return eval_block(elif.block);
                }
            } else {
                ALVO_UNREACHABLE();
                return StmtResult::none();
            }
        }

        if (if_.else_) {
            return eval_block(*if_.else_);
        }

        return StmtResult::none();
    }

    StmtResult Interpreter::eval_stmt_switch(
        [[maybe_unused]] const ast::Stmt::Switch& switch_) {
        ALVO_NOT_IMPLEMENTED();
        return StmtResult::none();
    }

    StmtResult Interpreter::eval_stmt_loop(const ast::Stmt::Loop& loop) {
        while (true) {
            auto res = eval_block(loop.block);
            if (res.is_break()) {
                return StmtResult::none();
            }
            if (res.is_return()) {
                return res;
            }
        }
    }

    StmtResult Interpreter::eval_stmt_for(const ast::Stmt::For& for_) {
        Value value = eval_expr(for_.expr);
        if (auto array = std::get_if<Value::Array>(&value.val)) {
            for (auto& element : array->value->elements) {
                m_stack.push();

                m_stack.put(for_.name, element);

                auto res = eval_block(for_.block);
                if (res.is_break()) {
                    m_stack.pop();
                    return StmtResult::none();
                }
                if (res.is_return()) {
                    m_stack.pop();
                    return res;
                }

                m_stack.pop();
            }
        } else {
            ALVO_UNREACHABLE();
            return StmtResult::none();
        }

        return StmtResult::none();
    }

    StmtResult Interpreter::eval_stmt_while(const ast::Stmt::While& while_) {
        while (true) {
            Value res = eval_expr(while_.expr);
            if (auto res_val = std::get_if<Value::Bool>(&res.val)) {
                if (res_val->value) {
                    auto block_res = eval_block(while_.block);
                    if (block_res.is_break()) {
                        return StmtResult::none();
                    }
                    if (block_res.is_return()) {
                        return block_res;
                    }
                } else {
                    return StmtResult::none();
                }
            } else {
                ALVO_UNREACHABLE();
                return StmtResult::none();
            }
        }
    }

    StmtResult Interpreter::eval_stmt_print(
        [[maybe_unused]] const ast::Stmt::Print& print) {
        for (const auto& expr : print.exprs) {
            Value value = eval_expr(expr);
            std::visit(
                util::overload {
                    [&](const Value::Unit&) { fmt::print("unit"); },
                    [&](const Value::Null&) { fmt::print("null"); },
                    [&](const Value::String& val) {
                        fmt::print("{}", val.value->value);
                    },
                    [&](const Value::Char& val) {
                        fmt::print("{}", val.value);
                    },
                    [&](const Value::Int& val) { fmt::print("{}", val.value); },
                    [&](const Value::Byte& val) {
                        fmt::print("{}", static_cast<std::int64_t>(val.value));
                    },
                    [&](const Value::Float& val) {
                        fmt::print("{}", val.value);
                    },
                    [&](const Value::Bool& val) {
                        if (val.value) {
                            fmt::print("true");
                        } else {
                            fmt::print("false");
                        }
                    },
                    [&](const Value::Array&) { ALVO_NOT_IMPLEMENTED(); },
                    [&](const Value::Tup&) { ALVO_NOT_IMPLEMENTED(); },
                    [&](const Value::Struct&) { ALVO_NOT_IMPLEMENTED(); },
                    [&](const Value::Func&) { ALVO_NOT_IMPLEMENTED(); },
                },
                value.val);
        }

        fmt::println("");

        return StmtResult::none();
    }

}
