#pragma once

#include <functional>
#include <variant>
#include <ostream>

#include "ast.h"
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

        // From name resolver
        struct DuplicateGenericParams { };

        struct DuplicateGenericBounds { };

        struct DuplicateFunctionName { };

        struct DuplicateStructMemberName { };

        struct DuplicateEnumMemberName { };

        struct DuplicateInterfaceFunctionName { };

        struct NotAnInterface { };

        struct DuplicateFuncParamName { };

        struct MemberAccessOnGenericWithNoBounds { };

        struct NoMemberFound { };

        struct UndeclaredType {
            std::string_view name;

            UndeclaredType(std::string_view name) :
                name(name) { }
        };

        struct InvalidBounds { };

        struct NotAFunction { };

        struct AmbiguousReference { };

        struct TypeRedefinition {
            std::string_view name;
        };

        struct VariableRedefinition {
            std::string_view name;
        };

        struct NonStructInStructLiteral { };

        struct NoSuchFieldInStruct {
            std::string_view field;
        };

        struct DuplicateStructFieldInitialization {
            std::string_view field;
        };

        struct IncompleteStructInitialization { };

        // From typechecker

        struct BadSwitchExprType { };

        struct CantInferTypeOfNullLiteral { };

        struct CantInferTypeOfArrayLiteral { };

        struct Expected {
            std::string_view expected;
        };

        struct ExpectedType {
            ast::Type type;
        };

        struct CantBeNull { };

        struct TupleLiteralArgumentCountMismatch { };

        struct BadUnaryExpressionType { };

        using Val = std::variant<None, UnexpectedCharacter,
            NonPrintableCharacterInCharacterLiteral,
            NonPrintableCharacterInStringLiteral, UnterminatedString,
            InvalidIntegerPrefix, NoDigitsAfterIntegerPrefix,
            BytePostfixInFloatingPointLiteral, NegativeByteLiteral,
            UnexpectedCharacterInNumberLiteral, UnexpectedToken,
            DuplicateGenericParams, DuplicateGenericBounds,
            DuplicateFunctionName, DuplicateStructMemberName,
            DuplicateEnumMemberName, DuplicateInterfaceFunctionName,
            NotAnInterface, DuplicateFuncParamName,
            MemberAccessOnGenericWithNoBounds, NoMemberFound, UndeclaredType,
            InvalidBounds, NotAFunction, AmbiguousReference, TypeRedefinition,
            VariableRedefinition, NonStructInStructLiteral, NoSuchFieldInStruct,
            DuplicateStructFieldInitialization, IncompleteStructInitialization,
            BadSwitchExprType, CantInferTypeOfNullLiteral,
            CantInferTypeOfArrayLiteral, Expected, ExpectedType, CantBeNull,
            TupleLiteralArgumentCountMismatch, BadUnaryExpressionType>;

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
