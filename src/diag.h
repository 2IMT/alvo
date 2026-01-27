#pragma once

#include <functional>
#include <variant>
#include <optional>
#include <ostream>

#include "tok.h"

namespace alvo::diag {

    struct Warn {
        struct None { };

        using Val = std::variant<None>;

        Val val;

        Warn();

        Warn(const Val& val);
    };

    struct Err {

        struct None { };

        // From lexer
        struct UnexpectedCharacter { };

        struct NonPrintableCharacterInCharacterLiteral { };

        struct NonPrintableCharacterInStringLiteral { };

        struct UnterminatedString { };

        struct InvalidIntegerPrefix { };

        struct NoDigitsAfterIntegerPrefix { };

        struct BytePostfixInFloatingPointLiteral { };

        struct NegativeByteLiteral { };

        struct UnexpectedCharacterInNumberLiteral { };

        // From parser
        struct UnexpectedToken {
            tok::Tok tok;
        };

        // From name resolution
        struct DuplicatePackageNames { };

        struct DuplicateModuleNames { };

        struct DuplicateMemberFunctionNames { };

        struct DuplicateInterfaceFunctionNames { };

        struct DuplicateDeclNames { };

        struct DuplicateImportNames { };

        struct ImportAndDeclNameCollision { };

        struct PathNotFound { };

        struct ParentNotFound { };

        struct LocalRedefinition { };

        struct GenericRedefinition { };

        struct RootImported { };

        struct GlobUsedOnDecl { };

        struct AccessUsedOnNonPath { };

        struct AccessIsNotAPathSegment { };

        struct GenericParamsAreNotAllowed { };

        struct RootIsNotAllowed { };

        using Val = std::variant<None, UnexpectedCharacter,
            NonPrintableCharacterInCharacterLiteral,
            NonPrintableCharacterInStringLiteral, UnterminatedString,
            InvalidIntegerPrefix, NoDigitsAfterIntegerPrefix,
            BytePostfixInFloatingPointLiteral, NegativeByteLiteral,
            UnexpectedCharacterInNumberLiteral, UnexpectedToken,
            DuplicatePackageNames, DuplicateModuleNames,
            DuplicateMemberFunctionNames, DuplicateInterfaceFunctionNames,
            DuplicateDeclNames, DuplicateImportNames,
            ImportAndDeclNameCollision, PathNotFound, ParentNotFound,
            LocalRedefinition, GenericRedefinition, RootImported,
            GlobUsedOnDecl, AccessUsedOnNonPath, AccessIsNotAPathSegment,
            GenericParamsAreNotAllowed, RootIsNotAllowed>;

        Val val;

        Err();

        Err(const Val& val);
    };

    struct Diag {
        using Val = std::variant<Warn, Err>;

        Val val;
        tok::Pos pos;
    };

    using DiagHandler = std::function<void(const Diag&)>;

    class DiagSink {
    public:
        virtual void warn(
            std::string_view file, tok::Pos pos, const Warn& warn) = 0;

        virtual void err(
            std::string_view file, tok::Pos pos, const Err& err) = 0;
    };

    class OstreamSink : public DiagSink {
    public:
        OstreamSink(std::ostream& os);

        void warn(
            std::string_view filename, tok::Pos pos, const Warn& warn) override;

        void err(
            std::string_view filename, tok::Pos pos, const Err& err) override;

    private:
        std::ostream* m_os;
        bool m_err;
    };

    class DiagEmitter {
    public:
        DiagEmitter();

        void set_sink(DiagSink& sink);

        void warn(std::string_view filename, tok::Pos pos, const Warn& warn);

        void err(std::string_view filename, tok::Pos pos, const Err& err);

    private:
        DiagSink* m_sink;
    };

}
