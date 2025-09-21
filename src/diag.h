#pragma once

#include <functional>

#include "loc.h"

namespace alvo::diag {

    enum class WarnKind {
        None,
    };

    struct Warn {
        WarnKind kind;

        Warn() :
            kind(WarnKind::None) { }

        Warn(WarnKind kind) :
            kind(kind) { }
    };

    enum class ErrKind {
        None,

        // From Lexer
        UnexpectedCharacter,
        NonPrintableCharacterInCharacterLiteral,
        NonPrintableCharacterInStringLiteral,
        UnterminatedString,
        InvalidIntegerPrefix,
        NoDigitsAfterIntegerPrefix,
        BytePostfixInFloatingPointLiteral,
        NegativeByteLiteral,
        UnexpectedCharacterInNumberLiteral,
    };

    struct Err {
        ErrKind kind;

        Err() :
            kind(ErrKind::None) { }

        Err(ErrKind kind) :
            kind(kind) { }
    };

    enum class DiagKind {
        Warn,
        Err,
    };

    struct Diag {
        DiagKind kind;
        loc::Pos pos;
        Warn warn;
        Err err;

        Diag(Warn warn, loc::Pos pos) :
            kind(DiagKind::Warn),
            pos(pos),
            warn(warn),
            err() { }

        Diag(Err err, loc::Pos pos) :
            kind(DiagKind::Err),
            pos(pos),
            warn(),
            err(err) { }
    };

    using DiagHandler = std::function<void(Diag)>;

    class DiagEmitter {
    public:
        DiagEmitter(DiagHandler handler);

        void emit(Diag diag);

    private:
        DiagHandler m_handler;
    };

}
