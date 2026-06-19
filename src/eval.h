#pragma once

#include "sema/resolve.h"
#include "ast.h"
#include "util.h"

#include <cstdint>
#include <vector>
#include <variant>
#include <charconv>

namespace alvo::eval {

    struct Value;

    struct StringValue {
        std::string value;
    };

    struct ArrayValue {
        std::vector<Value> elements;
    };

    struct TupValue {
        std::vector<Value> elements;
    };

    struct StructValue {
        std::vector<Value> fields;
    };

    struct Value {
        struct Unit { };

        struct Null { };

        struct String {
            StringValue* value;
        };

        struct Char {
            char value;
        };

        struct Int {
            std::int64_t value;
        };

        struct Byte {
            std::uint8_t value;
        };

        struct Float {
            double value;
        };

        struct Bool {
            bool value;
        };

        struct Array {
            ArrayValue* value;
        };

        struct Tup {
            TupValue* value;
        };

        struct Struct {
            StructValue* value;
        };

        struct Func {
            ast::Id decl_id;
        };

        using Val = std::variant<Unit, Null, String, Char, Int, Byte, Float,
            Bool, Array, Tup, Struct, Func>;

        Val val;

        Value() :
            val(Value::Unit {}) { }

        Value(const Val& val) :
            val(val) { }

        static Value unit() { return Value(Value::Unit()); }

        static Value null() { return Value(Value::Null()); }

        static Value string(std::string_view value) {
            return Value(
                Value::String { .value = new StringValue {
                                    .value = std::string(value.begin() + 1,
                                        value.end() - 1) } });
        }

        static Value character(std::string_view value) {
            return Value(Value::Char {
                .value = value[0],
            });
        }

        static Value integer(std::string_view value) {
            int64_t res;
            auto result = std::from_chars(
                value.data(), value.data() + value.length(), res);
            ALVO_ASSERT(result.ec == std::errc {});
            return Value(Value::Int { .value = res });
        }

        static Value integer(std::int64_t value) {
            return Value(Value::Int { .value = value });
        }

        static Value byte(std::string_view value) {
            uint8_t res;
            auto result = std::from_chars(
                value.data(), value.data() + value.length() - 1, res);
            ALVO_ASSERT(result.ec == std::errc {});
            return Value(Value::Byte { .value = res });
        }

        static Value byte(std::uint8_t value) {
            return Value(Value::Byte { .value = value });
        }

        static Value floating(std::string_view value) {
            double res;
            auto result = std::from_chars(
                value.data(), value.data() + value.length(), res);
            ALVO_ASSERT(result.ec == std::errc {});
            return Value(Value::Float { .value = res });
        }

        static Value floating(double value) {
            return Value(Value::Float { .value = value });
        }

        static Value boolean(std::string_view value) {
            if (value == "true") {
                return Value(Value::Bool { .value = true });
            } else {
                return Value(Value::Bool { .value = false });
            }
        }

        static Value boolean(bool value) {
            return Value(Value::Bool { .value = value });
        }

        static Value array(const std::vector<Value>& values) {
            return Value(Value::Array {
                .value = new ArrayValue { .elements = values } });
        }

        static Value tup(const std::vector<Value>& values) {
            return Value(
                Value::Tup { .value = new TupValue { .elements = values } });
        }

        static Value structure(const std::vector<Value>& values) {
            return Value(Value::Struct {
                .value = new StructValue { .fields = values } });
        }

        static Value function(ast::Id decl_id) {
            return Value(Value::Func {
                .decl_id = decl_id,
            });
        }

        Value unary_plus() {
            return std::visit(
                util::overload {
                    [&](Value::Int val) { return Value::integer(+val.value); },
                    [&](Value::Float val) {
                        return Value::floating(+val.value);
                    },
                    [&](Value::Byte val) { return Value::byte(+val.value); },
                    [&](auto) {
                        ALVO_UNREACHABLE();
                        return Value::integer(0);
                    } },
                val);
        }

        Value unary_minus() {
            return std::visit(
                util::overload {
                    [&](Value::Int val) { return Value::integer(-val.value); },
                    [&](Value::Float val) {
                        return Value::floating(-val.value);
                    },
                    [&](Value::Byte val) { return Value::byte(-val.value); },
                    [&](auto) {
                        ALVO_UNREACHABLE();
                        return Value::integer(0);
                    } },
                val);
        }

        Value not_() {
            if (auto value = std::get_if<Value::Bool>(&val)) {
                return Value::boolean(!value->value);
            } else {
                ALVO_UNREACHABLE();
                return Value::integer(0);
            }
        }

        Value binary_not() {
            return std::visit(
                util::overload {
                    [&](Value::Int val) { return Value::integer(~val.value); },
                    [&](Value::Byte val) { return Value::byte(~val.value); },
                    [&](auto) {
                        ALVO_UNREACHABLE();
                        return Value::integer(0);
                    } },
                val);
        }

        Value plus(const Value& rhs) {
            return std::visit(
                util::overload {
                    [&](Value::Int val) {
                        if (auto rval = std::get_if<Value::Int>(&rhs.val)) {
                            return Value::integer(val.value + rval->value);
                        } else {
                            ALVO_UNREACHABLE();
                            return Value::integer(0);
                        }
                    },
                    [&](Value::Byte val) {
                        if (auto rval = std::get_if<Value::Byte>(&rhs.val)) {
                            return Value::byte(val.value + rval->value);
                        } else {
                            ALVO_UNREACHABLE();
                            return Value::integer(0);
                        }
                    },
                    [&](Value::Float val) {
                        if (auto rval = std::get_if<Value::Float>(&rhs.val)) {
                            return Value::floating(val.value + rval->value);
                        } else {
                            ALVO_UNREACHABLE();
                            return Value::integer(0);
                        }
                    },
                    [&](auto) {
                        ALVO_UNREACHABLE();
                        return Value::integer(0);
                    } },
                val);
        }

        Value minus(const Value& rhs) {
            return std::visit(
                util::overload {
                    [&](Value::Int val) {
                        if (auto rval = std::get_if<Value::Int>(&rhs.val)) {
                            return Value::integer(val.value - rval->value);
                        } else {
                            ALVO_UNREACHABLE();
                            return Value::integer(0);
                        }
                    },
                    [&](Value::Byte val) {
                        if (auto rval = std::get_if<Value::Byte>(&rhs.val)) {
                            return Value::byte(val.value - rval->value);
                        } else {
                            ALVO_UNREACHABLE();
                            return Value::integer(0);
                        }
                    },
                    [&](Value::Float val) {
                        if (auto rval = std::get_if<Value::Float>(&rhs.val)) {
                            return Value::floating(val.value - rval->value);
                        } else {
                            ALVO_UNREACHABLE();
                            return Value::integer(0);
                        }
                    },
                    [&](auto) {
                        ALVO_UNREACHABLE();
                        return Value::integer(0);
                    } },
                val);
        }

        Value multiply(const Value& rhs) {
            return std::visit(
                util::overload {
                    [&](Value::Int val) {
                        if (auto rval = std::get_if<Value::Int>(&rhs.val)) {
                            return Value::integer(val.value * rval->value);
                        } else {
                            ALVO_UNREACHABLE();
                            return Value::integer(0);
                        }
                    },
                    [&](Value::Byte val) {
                        if (auto rval = std::get_if<Value::Byte>(&rhs.val)) {
                            return Value::byte(val.value * rval->value);
                        } else {
                            ALVO_UNREACHABLE();
                            return Value::integer(0);
                        }
                    },
                    [&](Value::Float val) {
                        if (auto rval = std::get_if<Value::Float>(&rhs.val)) {
                            return Value::floating(val.value * rval->value);
                        } else {
                            ALVO_UNREACHABLE();
                            return Value::integer(0);
                        }
                    },
                    [&](auto) {
                        ALVO_UNREACHABLE();
                        return Value::integer(0);
                    } },
                val);
        }

        Value divide(const Value& rhs) {
            return std::visit(
                util::overload {
                    [&](Value::Int val) {
                        if (auto rval = std::get_if<Value::Int>(&rhs.val)) {
                            return Value::integer(val.value / rval->value);
                        } else {
                            ALVO_UNREACHABLE();
                            return Value::integer(0);
                        }
                    },
                    [&](Value::Byte val) {
                        if (auto rval = std::get_if<Value::Byte>(&rhs.val)) {
                            return Value::byte(val.value / rval->value);
                        } else {
                            ALVO_UNREACHABLE();
                            return Value::integer(0);
                        }
                    },
                    [&](Value::Float val) {
                        if (auto rval = std::get_if<Value::Float>(&rhs.val)) {
                            return Value::floating(val.value / rval->value);
                        } else {
                            ALVO_UNREACHABLE();
                            return Value::integer(0);
                        }
                    },
                    [&](auto) {
                        ALVO_UNREACHABLE();
                        return Value::integer(0);
                    } },
                val);
        }

        Value mod(const Value& rhs) {
            return std::visit(
                util::overload {
                    [&](Value::Int val) {
                        if (auto rval = std::get_if<Value::Int>(&rhs.val)) {
                            return Value::integer(val.value % rval->value);
                        } else {
                            ALVO_UNREACHABLE();
                            return Value::integer(0);
                        }
                    },
                    [&](Value::Byte val) {
                        if (auto rval = std::get_if<Value::Byte>(&rhs.val)) {
                            return Value::byte(val.value % rval->value);
                        } else {
                            ALVO_UNREACHABLE();
                            return Value::integer(0);
                        }
                    },
                    [&](auto) {
                        ALVO_UNREACHABLE();
                        return Value::integer(0);
                    } },
                val);
        }

        Value and_(const Value& rhs) {
            if (auto value = std::get_if<Value::Bool>(&val)) {
                if (auto rvalue = std::get_if<Value::Bool>(&rhs.val)) {
                    return Value::boolean(value->value && rvalue->value);
                } else {
                    ALVO_UNREACHABLE();
                    return Value::integer(0);
                }
            } else {
                ALVO_UNREACHABLE();
                return Value::integer(0);
            }
        }

        Value or_(const Value& rhs) {
            if (auto value = std::get_if<Value::Bool>(&val)) {
                if (auto rvalue = std::get_if<Value::Bool>(&rhs.val)) {
                    return Value::boolean(value->value || rvalue->value);
                } else {
                    ALVO_UNREACHABLE();
                    return Value::integer(0);
                }
            } else {
                ALVO_UNREACHABLE();
                return Value::integer(0);
            }
        }

        Value binary_and(const Value& rhs) {
            return std::visit(
                util::overload {
                    [&](Value::Int val) {
                        if (auto rval = std::get_if<Value::Int>(&rhs.val)) {
                            return Value::integer(val.value & rval->value);
                        } else {
                            ALVO_UNREACHABLE();
                            return Value::integer(0);
                        }
                    },
                    [&](Value::Byte val) {
                        if (auto rval = std::get_if<Value::Byte>(&rhs.val)) {
                            return Value::byte(val.value & rval->value);
                        } else {
                            ALVO_UNREACHABLE();
                            return Value::integer(0);
                        }
                    },
                    [&](auto) {
                        ALVO_UNREACHABLE();
                        return Value::integer(0);
                    } },
                val);
        }

        Value binary_or(const Value& rhs) {
            return std::visit(
                util::overload {
                    [&](Value::Int val) {
                        if (auto rval = std::get_if<Value::Int>(&rhs.val)) {
                            return Value::integer(val.value | rval->value);
                        } else {
                            ALVO_UNREACHABLE();
                            return Value::integer(0);
                        }
                    },
                    [&](Value::Byte val) {
                        if (auto rval = std::get_if<Value::Byte>(&rhs.val)) {
                            return Value::byte(val.value | rval->value);
                        } else {
                            ALVO_UNREACHABLE();
                            return Value::integer(0);
                        }
                    },
                    [&](auto) {
                        ALVO_UNREACHABLE();
                        return Value::integer(0);
                    } },
                val);
        }

        Value binary_xor(const Value& rhs) {
            return std::visit(
                util::overload {
                    [&](Value::Int val) {
                        if (auto rval = std::get_if<Value::Int>(&rhs.val)) {
                            return Value::integer(val.value ^ rval->value);
                        } else {
                            ALVO_UNREACHABLE();
                            return Value::integer(0);
                        }
                    },
                    [&](Value::Byte val) {
                        if (auto rval = std::get_if<Value::Byte>(&rhs.val)) {
                            return Value::byte(val.value ^ rval->value);
                        } else {
                            ALVO_UNREACHABLE();
                            return Value::integer(0);
                        }
                    },
                    [&](auto) {
                        ALVO_UNREACHABLE();
                        return Value::integer(0);
                    } },
                val);
        }

        Value shift_left(const Value& rhs) {
            if (auto rvalue = std::get_if<Value::Int>(&rhs.val)) {
                return std::visit(
                    util::overload {
                        [&](Value::Int val) {
                            return Value::integer(val.value << rvalue->value);
                        },
                        [&](Value::Byte val) {
                            return Value::byte(val.value << rvalue->value);
                        },
                        [&](auto) {
                            ALVO_UNREACHABLE();
                            return Value::integer(0);
                        },
                    },
                    val);
            } else {
                ALVO_UNREACHABLE();
                return Value::integer(0);
            }
        }

        Value shift_right(const Value& rhs) {
            if (auto rvalue = std::get_if<Value::Int>(&rhs.val)) {
                return std::visit(
                    util::overload {
                        [&](Value::Int val) {
                            return Value::integer(val.value >> rvalue->value);
                        },
                        [&](Value::Byte val) {
                            return Value::byte(val.value >> rvalue->value);
                        },
                        [&](auto) {
                            ALVO_UNREACHABLE();
                            return Value::integer(0);
                        },
                    },
                    val);
            } else {
                ALVO_UNREACHABLE();
                return Value::integer(0);
            }
        }

        Value equal(const Value& rhs) {
            bool res = std::visit(
                util::overload {
                    [&](Value::Unit) { return true; },
                    [&](Value::Null) {
                        ALVO_UNREACHABLE();
                        return false;
                    },
                    [&](Value::String val) {
                        if (auto rval = std::get_if<Value::String>(&rhs.val)) {
                            return val.value->value == rval->value->value;
                        } else {
                            ALVO_UNREACHABLE();
                            return false;
                        }
                    },
                    [&](Value::Char val) {
                        if (auto rval = std::get_if<Value::Char>(&rhs.val)) {
                            return val.value == rval->value;
                        } else {
                            ALVO_UNREACHABLE();
                            return false;
                        }
                    },
                    [&](Value::Int val) {
                        if (auto rval = std::get_if<Value::Int>(&rhs.val)) {
                            return val.value == rval->value;
                        } else {
                            ALVO_UNREACHABLE();
                            return false;
                        }
                    },
                    [&](Value::Byte val) {
                        if (auto rval = std::get_if<Value::Byte>(&rhs.val)) {
                            return val.value == rval->value;
                        } else {
                            ALVO_UNREACHABLE();
                            return false;
                        }
                    },
                    [&](Value::Float val) {
                        if (auto rval = std::get_if<Value::Float>(&rhs.val)) {
                            return val.value == rval->value;
                        } else {
                            ALVO_UNREACHABLE();
                            return false;
                        }
                    },
                    [&](Value::Bool val) {
                        if (auto rval = std::get_if<Value::Bool>(&rhs.val)) {
                            return val.value == rval->value;
                        } else {
                            ALVO_UNREACHABLE();
                            return false;
                        }
                    },
                    [&](Value::Array) {
                        ALVO_UNREACHABLE();
                        return false;
                    },
                    [&](Value::Tup) {
                        ALVO_UNREACHABLE();
                        return false;
                    },
                    [&](Value::Struct) {
                        ALVO_UNREACHABLE();
                        return false;
                    },
                    [&](Value::Func) {
                        ALVO_UNREACHABLE();
                        return false;
                    },
                },
                val);
            return Value::boolean(res);
        }

        Value not_equal(const Value& rhs) {
            Value eq = this->equal(rhs);
            return Value::boolean(!std::get<Value::Bool>(eq.val).value);
        }

        Value less(const Value& rhs) {
            return std::visit(
                util::overload {
                    [&](Value::Int val) {
                        if (auto rval = std::get_if<Value::Int>(&rhs.val)) {
                            return Value::boolean(val.value < rval->value);
                        } else {
                            ALVO_UNREACHABLE();
                            return Value::integer(0);
                        }
                    },
                    [&](Value::Float val) {
                        if (auto rval = std::get_if<Value::Float>(&rhs.val)) {
                            return Value::boolean(val.value < rval->value);
                        } else {
                            ALVO_UNREACHABLE();
                            return Value::integer(0);
                        }
                    },
                    [&](Value::Byte val) {
                        if (auto rval = std::get_if<Value::Byte>(&rhs.val)) {
                            return Value::boolean(val.value < rval->value);
                        } else {
                            ALVO_UNREACHABLE();
                            return Value::integer(0);
                        }
                    },
                    [&](auto) {
                        ALVO_UNREACHABLE();
                        return Value::integer(0);
                    } },
                val);
        }

        Value less_equal(const Value& rhs) {
            return std::visit(
                util::overload {
                    [&](Value::Int val) {
                        if (auto rval = std::get_if<Value::Int>(&rhs.val)) {
                            return Value::boolean(val.value <= rval->value);
                        } else {
                            ALVO_UNREACHABLE();
                            return Value::integer(0);
                        }
                    },
                    [&](Value::Float val) {
                        if (auto rval = std::get_if<Value::Float>(&rhs.val)) {
                            return Value::boolean(val.value <= rval->value);
                        } else {
                            ALVO_UNREACHABLE();
                            return Value::integer(0);
                        }
                    },
                    [&](Value::Byte val) {
                        if (auto rval = std::get_if<Value::Byte>(&rhs.val)) {
                            return Value::boolean(val.value <= rval->value);
                        } else {
                            ALVO_UNREACHABLE();
                            return Value::integer(0);
                        }
                    },
                    [&](auto) {
                        ALVO_UNREACHABLE();
                        return Value::integer(0);
                    } },
                val);
        }

        Value greater(const Value& rhs) {
            return std::visit(
                util::overload {
                    [&](Value::Int val) {
                        if (auto rval = std::get_if<Value::Int>(&rhs.val)) {
                            return Value::boolean(val.value > rval->value);
                        } else {
                            ALVO_UNREACHABLE();
                            return Value::integer(0);
                        }
                    },
                    [&](Value::Float val) {
                        if (auto rval = std::get_if<Value::Float>(&rhs.val)) {
                            return Value::boolean(val.value > rval->value);
                        } else {
                            ALVO_UNREACHABLE();
                            return Value::integer(0);
                        }
                    },
                    [&](Value::Byte val) {
                        if (auto rval = std::get_if<Value::Byte>(&rhs.val)) {
                            return Value::boolean(val.value > rval->value);
                        } else {
                            ALVO_UNREACHABLE();
                            return Value::integer(0);
                        }
                    },
                    [&](auto) {
                        ALVO_UNREACHABLE();
                        return Value::integer(0);
                    } },
                val);
        }

        Value greater_equal(const Value& rhs) {
            return std::visit(
                util::overload {
                    [&](Value::Int val) {
                        if (auto rval = std::get_if<Value::Int>(&rhs.val)) {
                            return Value::boolean(val.value >= rval->value);
                        } else {
                            ALVO_UNREACHABLE();
                            return Value::integer(0);
                        }
                    },
                    [&](Value::Float val) {
                        if (auto rval = std::get_if<Value::Float>(&rhs.val)) {
                            return Value::boolean(val.value >= rval->value);
                        } else {
                            ALVO_UNREACHABLE();
                            return Value::integer(0);
                        }
                    },
                    [&](Value::Byte val) {
                        if (auto rval = std::get_if<Value::Byte>(&rhs.val)) {
                            return Value::boolean(val.value >= rval->value);
                        } else {
                            ALVO_UNREACHABLE();
                            return Value::integer(0);
                        }
                    },
                    [&](auto) {
                        ALVO_UNREACHABLE();
                        return Value::integer(0);
                    } },
                val);
        }
    };

    struct PlaceValue {
        using Val = std::variant<Value*, Value>;
        Val val;

        PlaceValue(const Val& val) :
            val(val) { }

        static PlaceValue place(Value* value) { return PlaceValue(value); }

        static PlaceValue value(const Value& value) {
            return PlaceValue(value);
        }

        operator Value() {
            return std::visit(util::overload {
                                  [&](Value* value) { return *value; },
                                  [&](const Value& value) { return value; },
                              },
                val);
        }
    };

    struct StmtResult {
        struct None { };

        struct Continue { };

        struct Break { };

        struct Return {
            Value value;
        };

        using Val = std::variant<None, Continue, Break, Return>;

        Val val;

        bool is_none() { return std::holds_alternative<None>(val); }

        bool is_continue() { return std::holds_alternative<Continue>(val); }

        bool is_break() { return std::holds_alternative<Break>(val); }

        bool is_return() { return std::holds_alternative<Return>(val); }

        Value get_return() { return std::get<Return>(val).value; }

        static StmtResult none() { return StmtResult { .val = None {} }; }

        static StmtResult continue_() {
            return StmtResult { .val = Continue {} };
        }

        static StmtResult break_() { return StmtResult { .val = Break {} }; }

        static StmtResult return_(const Value& value) {
            return StmtResult { .val = Return { .value = value } };
        }
    };

    class Interpreter {
    public:
        Interpreter(sema::resolve::NameIndex& index);

        PlaceValue eval_expr(const ast::Expr& expr);

        Value eval_literal(const ast::Expr::Literal& literal);

        Value eval_unop(const ast::Expr::Unop& unop);

        PlaceValue eval_binop(const ast::Expr::Binop& binop);

        PlaceValue eval_index(const ast::Expr::Index& index);

        Value eval_call(const ast::Expr::Call& call);

        Value eval_cast(const ast::Expr::Cast& cast);

        PlaceValue eval_resolved_member_access(
            const ast::Expr::ResolvedMemberAccess& resolved_member_access);

        Value call_function(ast::Id decl_id, const std::vector<Value>& args);

        StmtResult eval_block(const ast::Block& block);

        StmtResult eval_stmt(const ast::Stmt& stmt);

        StmtResult eval_stmt_let(const ast::Stmt::Let& let);

        StmtResult eval_stmt_if(const ast::Stmt::If& if_);

        StmtResult eval_stmt_switch(const ast::Stmt::Switch& switch_);

        StmtResult eval_stmt_loop(const ast::Stmt::Loop& loop);

        StmtResult eval_stmt_for(const ast::Stmt::For& for_);

        StmtResult eval_stmt_while(const ast::Stmt::While& while_);

        StmtResult eval_stmt_print(const ast::Stmt::Print& print);

    private:
        sema::resolve::NameIndex* m_index;
        sema::resolve::ScopeStack<Value, true> m_stack;
        sema::resolve::ScopeStack<Value, true, true> m_arg_stack;
    };

}
