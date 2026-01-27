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
                [this](const Err::DuplicatePackageNames&) {
                    fmt::print(*m_os, "duplicate package names");
                },
                [this](const Err::DuplicateModuleNames&) {
                    fmt::print(*m_os, "duplicate module names");
                },
                [this](const Err::DuplicateMemberFunctionNames&) {
                    fmt::print(*m_os, "duplicate member function names");
                },
                [this](const Err::DuplicateInterfaceFunctionNames&) {
                    fmt::print(*m_os, "duplicate interface function names");
                },
                [this](const Err::DuplicateDeclNames&) {
                    fmt::print(*m_os, "duplicate declaration names");
                },
                [this](const Err::DuplicateImportNames&) {
                    fmt::print(*m_os, "duplicate import names");
                },
                [this](const Err::ImportAndDeclNameCollision&) {
                    fmt::print(*m_os, "import and declaration name collision");
                },
                [this](const Err::PathNotFound&) {
                    fmt::print(*m_os, "path not found");
                },
                [this](const Err::ParentNotFound&) {
                    fmt::print(*m_os, "parent not found");
                },
                [this](const Err::LocalRedefinition&) {
                    fmt::print(*m_os, "local redefinition");
                },
                [this](const Err::GenericRedefinition&) {
                    fmt::print(*m_os, "generic redefinition");
                },
                [this](const Err::RootImported&) {
                    fmt::print(*m_os, "root imported");
                },
                [this](const Err::GlobUsedOnDecl&) {
                    fmt::print(*m_os, "glob used on declaration");
                },
                [this](const Err::AccessUsedOnNonPath&) {
                    fmt::print(*m_os, "access operator used on non-path");
                },
                [this](const Err::AccessIsNotAPathSegment&) {
                    fmt::print(*m_os, "access operand is not a path segment");
                },
                [this](const Err::GenericParamsAreNotAllowed&) {
                    fmt::print(*m_os, "generic params are not allowed");
                },
                [this](const Err::RootIsNotAllowed&) {
                    fmt::print(*m_os, "root is not allowed");
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
