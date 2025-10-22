#pragma once

#include <functional>
#include <variant>

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

        using Val = std::variant<None, UnexpectedCharacter,
            NonPrintableCharacterInCharacterLiteral,
            NonPrintableCharacterInStringLiteral, UnterminatedString,
            InvalidIntegerPrefix, NoDigitsAfterIntegerPrefix,
            BytePostfixInFloatingPointLiteral, NegativeByteLiteral,
            UnexpectedCharacterInNumberLiteral, UnexpectedToken>;

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

    class DiagEmitter {
    public:
        DiagEmitter(DiagHandler handler);

        void emit(Diag diag);

    private:
        DiagHandler m_handler;
    };

}
