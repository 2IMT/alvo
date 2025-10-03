#pragma once

#include <string_view>
#include <functional>

#include <fmt/format.h>

#include "utf8.h"
#include "diag.h"
#include "loc.h"

namespace alvo::lex {

    enum class TokKind {
        // Special
        Continue = -4,
        Err = -3,
        Eof = -2,
        None = -1,
        // Identifier
        Ident = 0,
        // Literals
        LitString,
        LitCharacter,
        LitInteger,
        LitByte,
        LitFloating,
        LitBoolean,
        // Types
        KwNull,
        KwString,
        KwChar,
        KwInt,
        KwByte,
        KwFloat,
        KwBool,
        KwTup,
        KwUnit,
        // Visibility and importing
        KwImport,
        KwExport,
        KwRoot,
        KwSuper,
        // Casting
        KwAs,
        KwTryAs,
        // Declarations
        KwStruct,
        KwEnum,
        KwFunc,
        KwType,
        KwConst,
        KwDefines,
        // Statements
        KwDefer,
        KwLet,
        KwIf,
        KwElif,
        KwElse,
        KwLoop,
        KwFor,
        KwWhile,
        KwSwitch,
        KwDefault,
        KwBreak,
        KwContinue,
        KwReturn,
        // Symbols
        LParen, // (
        RParen, // )
        LBrace, // {
        RBrace, // }
        LBracket, // [
        RBracket, // ]
        LAngle, // <
        LAngleEq, // <=
        LAngleLAngle,
        LAngleLAngleEq, // <<=
        RAngle, // >
        RAngleEq, // >=
        RAngleRAngle,
        RAngleRAngleEq, // >>=
        Exclam, // !
        ExclamEq, // !=
        Colon, // :
        ColonColon, // ::
        Dot, // .
        Comma, // ,
        Semicolon, // ;
        Eq, // =
        EqEq, // ==
        EqRAngle, // =>
        Plus, // +
        PlusEq, // +=
        Dash, // -
        DashEq, // -=
        DashRAngle, // ->
        Star, // *
        StarEq, // *=
        Slash, // /
        SlashEq, // /=
        Amp, // &
        AmpEq, // &=
        AmpAmp, // &&
        Pipe, // |
        PipeEq, // |=
        PipePipe, // ||
        Caret, // ^
        CaretEq, // ^=
        Tilde, // ~
        Percent, // %
        PercentEq, // %=
        Question, // ?
        // Count
        Count
    };

    struct Tok {
        loc::Loc loc;
        TokKind kind;
        std::string_view value;

        Tok() :
            loc(),
            kind(TokKind::None),
            value() { }

        Tok(loc::Loc loc, TokKind kind) :
            loc(loc),
            kind(kind),
            value() { }

        Tok(loc::Loc loc, TokKind kind, std::string_view value) :
            loc(loc),
            kind(kind),
            value(value) { }

        bool is_continue() const { return kind == TokKind::Continue; }

        bool is_err() const { return kind == TokKind::Err; }

        bool is_eof() const { return kind == TokKind::Eof; }

        bool is_none() const { return kind == TokKind::None; }
    };

    using TokHandler = std::function<void(const lex::Tok&)>;

    class TokEmitter {
    public:
        TokEmitter(TokHandler handler);

        void emit(const lex::Tok& tok);

    private:
        TokHandler m_handler;
    };

    class Lexer {
    public:
        Lexer(std::string_view src);

        void set_diag_emitter(diag::DiagEmitter& diag_emitter);

        void set_tok_emitter(TokEmitter& tok_emitter);

        Tok next();

        Tok peek() const;

    private:
        Tok get_token();

        Tok lex_string();

        Tok lex_num(bool negative);

        Tok lex_word();

        Tok lex_sym();

        void advance();

        void recover();

        Tok create_err_and_emit(diag::Err err);

        Tok create_tok(TokKind kind) const;

        Tok create_tok(TokKind kind, std::string_view view) const;

        std::string_view get_curr_value() const;

        std::string_view m_src;
        utf8::Utf8Iter m_src_iter;
        loc::Pos m_pos;
        loc::Pos m_pos_next;
        loc::Pos m_pos_begin;
        Tok m_curr;
        utf8::Codepoint m_ch;
        bool m_eof;
        diag::DiagEmitter* m_diag_emitter;
        TokEmitter* m_tok_emitter;
    };

    static_assert(std::is_trivially_copyable_v<Lexer>,
        "Lexer must remain trivially copyable for cheap state save/restore.");

    static_assert(std::is_trivially_destructible_v<Lexer>,
        "Lexer must not manage non-trivial resources.");

    static_assert(std::is_trivially_copy_assignable_v<Lexer>,
        "Lexer must remain cheap to assign (used in backtracking).");
}

template<>
struct fmt::formatter<alvo::lex::TokKind> {
    constexpr auto parse(fmt::format_parse_context& ctx) { return ctx.begin(); }

    auto format(const alvo::lex::TokKind& obj, fmt::format_context& ctx) const {
        const char* res = "";
        switch (obj) {
            using enum alvo::lex::TokKind;
        case Continue:
            res = "Continue";
            break;
        case Err:
            res = "Err";
            break;
        case Eof:
            res = "Eof";
            break;
        case None:
            res = "None";
            break;
        case Ident:
            res = "Ident";
            break;
        case LitString:
            res = "LitString";
            break;
        case LitCharacter:
            res = "LitCharacter";
            break;
        case LitInteger:
            res = "LitInteger";
            break;
        case LitByte:
            res = "LitByte";
            break;
        case LitFloating:
            res = "LitFloating";
            break;
        case LitBoolean:
            res = "LitBoolean";
            break;
        case KwNull:
            res = "KwNull";
            break;
        case KwString:
            res = "KwString";
            break;
        case KwChar:
            res = "KwChar";
            break;
        case KwInt:
            res = "KwInt";
            break;
        case KwByte:
            res = "KwByte";
            break;
        case KwFloat:
            res = "KwFloat";
            break;
        case KwBool:
            res = "KwBool";
            break;
        case KwTup:
            res = "KwTup";
            break;
        case KwUnit:
            res = "KwUnit";
            break;
        case KwImport:
            res = "KwImport";
            break;
        case KwExport:
            res = "KwExport";
            break;
        case KwRoot:
            res = "KwRoot";
            break;
        case KwSuper:
            res = "KwSuper";
            break;
        case KwAs:
            res = "KwAs";
            break;
        case KwTryAs:
            res = "KwTryAs";
            break;
        case KwStruct:
            res = "KwStruct";
            break;
        case KwEnum:
            res = "KwEnum";
            break;
        case KwFunc:
            res = "KwFunc";
            break;
        case KwType:
            res = "KwType";
            break;
        case KwConst:
            res = "KwConst";
            break;
        case KwDefines:
            res = "KwDefines";
            break;
        case KwDefer:
            res = "KwDefer";
            break;
        case KwLet:
            res = "KwLet";
            break;
        case KwIf:
            res = "KwIf";
            break;
        case KwElif:
            res = "KwElif";
            break;
        case KwElse:
            res = "KwElse";
            break;
        case KwLoop:
            res = "KwLoop";
            break;
        case KwFor:
            res = "KwFor";
            break;
        case KwWhile:
            res = "KwWhile";
            break;
        case KwSwitch:
            res = "KwSwitch";
            break;
        case KwDefault:
            res = "KwDefault";
            break;
        case KwBreak:
            res = "KwBreak";
            break;
        case KwContinue:
            res = "KwContinue";
            break;
        case KwReturn:
            res = "KwReturn";
            break;
        case LParen:
            res = "LParen";
            break;
        case RParen:
            res = "RParen";
            break;
        case LBrace:
            res = "LBrace";
            break;
        case RBrace:
            res = "RBrace";
            break;
        case LBracket:
            res = "LBracket";
            break;
        case RBracket:
            res = "RBracket";
            break;
        case LAngle:
            res = "LAngle";
            break;
        case LAngleEq:
            res = "LAngleEq";
            break;
        case LAngleLAngle:
            res = "LAngleLAngle";
            break;
        case LAngleLAngleEq:
            res = "LAngleLAngleEq";
            break;
        case RAngle:
            res = "RAngle";
            break;
        case RAngleEq:
            res = "RAngleEq";
            break;
        case RAngleRAngle:
            res = "RAngleRAngle";
            break;
        case RAngleRAngleEq:
            res = "RAngleRAngleEq";
            break;
        case Exclam:
            res = "Exclam";
            break;
        case ExclamEq:
            res = "ExclamEq";
            break;
        case Colon:
            res = "Colon";
            break;
        case ColonColon:
            res = "ColonColon";
            break;
        case Dot:
            res = "Dot";
            break;
        case Comma:
            res = "Comma";
            break;
        case Semicolon:
            res = "Semicolon";
            break;
        case Eq:
            res = "Eq";
            break;
        case EqEq:
            res = "EqEq";
            break;
        case EqRAngle:
            res = "EqRAngle";
            break;
        case Plus:
            res = "Plus";
            break;
        case PlusEq:
            res = "PlusEq";
            break;
        case Dash:
            res = "Dash";
            break;
        case DashEq:
            res = "DashEq";
            break;
        case DashRAngle:
            res = "DashRAngle";
            break;
        case Star:
            res = "Star";
            break;
        case StarEq:
            res = "StarEq";
            break;
        case Slash:
            res = "Slash";
            break;
        case SlashEq:
            res = "SlashEq";
            break;
        case Amp:
            res = "Amp";
            break;
        case AmpEq:
            res = "AmpEq";
            break;
        case AmpAmp:
            res = "AmpAmp";
            break;
        case Pipe:
            res = "Pipe";
            break;
        case PipeEq:
            res = "PipeEq";
            break;
        case PipePipe:
            res = "PipePipe";
            break;
        case Caret:
            res = "Caret";
            break;
        case CaretEq:
            res = "CaretEq";
            break;
        case Tilde:
            res = "Tilde";
            break;
        case Percent:
            res = "Percent";
            break;
        case PercentEq:
            res = "PercentEq";
            break;
        case Question:
            res = "Question";
            break;
        case Count:
            res = "Count";
            break;
        default:
            res = "Unknown";
            break;
        }
        return fmt::format_to(ctx.out(), "{}", res);
    }
};

template<>
struct fmt::formatter<alvo::lex::Tok> {
    constexpr auto parse(fmt::format_parse_context& ctx) { return ctx.begin(); }

    auto format(const alvo::lex::Tok& obj, fmt::format_context& ctx) const {
        return fmt::format_to(
            ctx.out(), "{}: {}(`{}`)", obj.loc, obj.kind, obj.value);
    }
};
