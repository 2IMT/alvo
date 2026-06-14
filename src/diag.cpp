#include "diag.h"

#include <fmt/format.h>
#include <fmt/ostream.h>

#include "util.h"

namespace alvo::diag {

    Warn::Warn() :
        val(None {}) { }

    Warn::Warn(const Val& val) :
        val(val) { }

    Err::Err() :
        val(None {}) { }

    Err::Err(const Val& val) :
        val(val) { };

    OstreamSink::OstreamSink(std::ostream& os) :
        m_os(&os) { }

    void OstreamSink::warn(
        std::string_view filename, tok::Pos pos, const Warn& warn) {
        fmt::print(*m_os, "{}:{}: warning: ", filename, pos);
        std::visit(util::overload { [this](const Warn::None&) {
            fmt::print(*m_os, "none");
        } },
            warn.val);
        fmt::print("\n");
    }

    void OstreamSink::err(
        std::string_view filename, tok::Pos pos, const Err& err) {
        fmt::print(*m_os, "{}:{}: error: ", filename, pos);
        std::visit(
            util::overload {
                [this](const Err::None&) { fmt::print(*m_os, "none"); },
                [this](const Err::UnexpectedCharacter&) {
                    fmt::print(*m_os, "unexpected character");
                },
                [this](const Err::NonPrintableCharacterInCharacterLiteral&) {
                    fmt::print(*m_os, "unexpected character in character "
                                      "literal");
                },
                [this](const Err::NonPrintableCharacterInStringLiteral&) {
                    fmt::print(*m_os, "non-printable character in string "
                                      "literal");
                },
                [this](const Err::UnterminatedString&) {
                    fmt::print(*m_os, "unterminated string literal");
                },
                [this](const Err::InvalidIntegerPrefix&) {
                    fmt::print(*m_os, "invalid integer prefix");
                },
                [this](const Err::NoDigitsAfterIntegerPrefix&) {
                    fmt::print(*m_os, "no digits after integer prefix");
                },
                [this](const Err::BytePostfixInFloatingPointLiteral&) {
                    fmt::print(*m_os, "byte postfix in floating point literal");
                },
                [this](const Err::NegativeByteLiteral&) {
                    fmt::print(*m_os, "negative byte literal");
                },
                [this](const Err::UnexpectedCharacterInNumberLiteral&) {
                    fmt::print(*m_os, "unexpected character in number literal");
                },
                [this](const Err::UnexpectedToken& v) {
                    fmt::print(*m_os, "unexpected token `{}`", v.tok.value);
                },
                [this](const Err::DuplicateGenericParams&) {
                    fmt::print(*m_os, "duplicate generic params");
                },
                [this](const Err::DuplicateGenericBounds&) {
                    fmt::print(*m_os, "duplicate generic bounds");
                },
                [this](const Err::DuplicateFunctionName&) {
                    fmt::print(*m_os, "duplicate function name");
                },
                [this](const Err::DuplicateStructMemberName&) {
                    fmt::print(*m_os, "duplicate struct member name");
                },
                [this](const Err::DuplicateEnumMemberName&) {
                    fmt::print(*m_os, "duplicate enum member name");
                },
                [this](const Err::DuplicateInterfaceFunctionName&) {
                    fmt::print(*m_os, "duplicate interface function name");
                },
                [this](const Err::NotAnInterface&) {
                    fmt::print(*m_os, "not an interface");
                },
                [this](const Err::DuplicateFuncParamName&) {
                    fmt::print(*m_os, "duplicate function parameter name");
                },
                [this](const Err::MemberAccessOnGenericWithNoBounds&) {
                    fmt::print(
                        *m_os, "member access on generic with no bounds");
                },
                [this](const Err::NoMemberFound&) {
                    fmt::print(*m_os, "no member found");
                },
                [this](const Err::UndeclaredType& t) {
                    fmt::print(*m_os, "undeclared type `{}`", t.name);
                },
                [this](const Err::InvalidBounds&) {
                    fmt::print(*m_os, "invalid bounds");
                },
                [this](const Err::NotAFunction&) {
                    fmt::print(*m_os, "not a function");
                },
                [this](const Err::AmbiguousReference&) {
                    fmt::print(*m_os, "ambiguous reference");
                },
                [this](const Err::TypeRedefinition& r) {
                    fmt::print(*m_os, "redefinition of type `{}`", r.name);
                },
                [this](const Err::VariableRedefinition& r) {
                    fmt::print(*m_os, "redefinition of variable `{}`", r.name);
                },
                [this](const Err::NonStructInStructLiteral&) {
                    fmt::print(*m_os, "non-struct in struct literal");
                },
                [this](const Err::NoSuchFieldInStruct&) {
                    fmt::print(*m_os, "no such field in struct");
                },
                [this](const Err::DuplicateStructFieldInitialization&) {
                    fmt::print(*m_os, "duplicate struct field initialization");
                },
                [this](const Err::IncompleteStructInitialization&) {
                    fmt::print(*m_os, "incomplete struct initialization");
                },
                [this](const Err::UndeclaredIdentifier& u) {
                    fmt::print(*m_os, "undeclared identifier `{}`", u.name);
                },
                [this](const Err::BadSwitchExprType&) {
                    fmt::print(*m_os, "bad type of switch expression");
                },
                [this](const Err::CantInferTypeOfNullLiteral&) {
                    fmt::print(*m_os, "can't infer type of null literal");
                },
                [this](const Err::CantInferTypeOfArrayLiteral&) {
                    fmt::print(*m_os, "can't infer type of array literal");
                },
                [this](const Err::Expected& expected) {
                    fmt::print(*m_os, "expected `{}`", expected.expected);
                },
                [this](const Err::ExpectedType&) {
                    fmt::print(*m_os, "expected type `{}`", "type");
                },
                [this](const Err::CantBeNull&) {
                    fmt::print(*m_os, "expr can't be null");
                },
                [this](const Err::TupleLiteralArgumentCountMismatch&) {
                    fmt::print(*m_os, "tuple literal argument count mismatch");
                },
                [this](const Err::BadUnaryExpressionType&) {
                    fmt::print(*m_os, "bad unary expression type");
                },
                [this](const Err::AttemptedToIndexNonArray&) {
                    fmt::print(*m_os, "attempted to index non array");
                },
                [this](const Err::NonIntegerIndex&) {
                    fmt::print(*m_os, "non integer index");
                },
                [this](const Err::IncompatibleTypesInBinaryExpression&) {
                    fmt::print(
                        *m_os, "incompatible types in binary expression");
                },
                [this](const Err::ExpressionNotAssignable&) {
                    fmt::print(*m_os, "expression not assignable");
                },
                [this](const Err::BadTypeForBinaryExpression&) {
                    fmt::print(*m_os, "bad type for binary expression");
                },
                [this](const Err::AttemptedToCallNonFunction&) {
                    fmt::print(*m_os, "attempted to call non-function");
                },
                [this](const Err::IncorrectNumberOfArgumentsForFunctionCall&) {
                    fmt::print(*m_os,
                        "incorrect number of argument for function call");
                },
                [this](const Err::BadCastType&) {
                    fmt::print(*m_os, "bad cast type");
                },
                [this](const Err::BadCastExprType&) {
                    fmt::print(*m_os, "bad cast expression type");
                },
                [this](const Err::BadTypeMemberAccessType&) {
                    fmt::print(*m_os, "bad type member access type");
                },
                [this](const Err::NotAllPathsReturn&) {
                    fmt::print(*m_os, "not all paths return a value");
                },
                [this](const Err::UnexpectedReturnType&) {
                    fmt::print(*m_os, "unexpected return type");
                },
                [this](const Err::MemberAccessOnNonStruct&) {
                    fmt::print(*m_os, "member access on non-struct");
                },
            },
            err.val);
        fmt::print("\n");
    }

    DiagEmitter::DiagEmitter() :
        m_sink(nullptr) { }

    void DiagEmitter::set_sink(DiagSink& sink) { m_sink = &sink; }

    void DiagEmitter::warn(
        std::string_view filename, tok::Pos pos, const Warn& warn) {
        if (m_sink) {
            m_sink->warn(filename, pos, warn);
        }
    }

    void DiagEmitter::err(
        std::string_view filename, tok::Pos pos, const Err& err) {
        if (m_sink) {
            m_sink->err(filename, pos, err);
        }
    }

}
