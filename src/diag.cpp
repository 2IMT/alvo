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
                [this](const Err::BadSwitchExprType&) {
                    fmt::print(*m_os, "bad type of switch expression");
                } },
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
