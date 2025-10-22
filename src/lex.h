#pragma once

#include <string_view>
#include <functional>

#include <fmt/format.h>

#include "utf8.h"
#include "diag.h"
#include "tok.h"

namespace alvo::lex {

    using TokHandler = std::function<void(const tok::Tok&)>;

    class TokEmitter {
    public:
        TokEmitter(TokHandler handler);

        void emit(const tok::Tok& tok);

    private:
        TokHandler m_handler;
    };

    class Lexer {
    public:
        Lexer(std::string_view src);

        void set_diag_emitter(diag::DiagEmitter& diag_emitter);

        void set_tok_emitter(TokEmitter& tok_emitter);

        tok::Tok next();

        tok::Tok peek() const;

    private:
        tok::Tok get_token();

        tok::Tok lex_string();

        tok::Tok lex_num(bool negative);

        tok::Tok lex_word();

        tok::Tok lex_sym();

        void advance();

        void recover();

        tok::Tok create_err_and_emit(diag::Err err);

        tok::Tok create_tok(tok::TokKind kind) const;

        tok::Tok create_tok(tok::TokKind kind, std::string_view view) const;

        std::string_view get_curr_value() const;

        std::string_view m_src;
        utf8::Utf8Iter m_src_iter;
        tok::Pos m_pos;
        tok::Pos m_pos_next;
        tok::Pos m_pos_begin;
        tok::Tok m_curr;
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
