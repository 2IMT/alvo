#include "lex.h"
#include "diag.h"

#include <utility>
#include <algorithm>
#include <iterator>

namespace alvo::lex {

    static constexpr std::pair<std::string_view, TokKind> _keyword_table[] = {
        { "as", TokKind::KwAs },
        { "bool", TokKind::KwBool },
        { "break", TokKind::KwBreak },
        { "byte", TokKind::KwByte },
        { "char", TokKind::KwChar },
        { "const", TokKind::KwConst },
        { "continue", TokKind::KwContinue },
        { "default", TokKind::KwDefault },
        { "defer", TokKind::KwDefer },
        { "defines", TokKind::KwDefines },
        { "elif", TokKind::KwElif },
        { "else", TokKind::KwElse },
        { "enum", TokKind::KwEnum },
        { "export", TokKind::KwExport },
        { "float", TokKind::KwFloat },
        { "for", TokKind::KwFor },
        { "func", TokKind::KwFunc },
        { "if", TokKind::KwIf },
        { "import", TokKind::KwImport },
        { "int", TokKind::KwInt },
        { "let", TokKind::KwLet },
        { "loop", TokKind::KwLoop },
        { "null", TokKind::KwNull },
        { "return", TokKind::KwReturn },
        { "root", TokKind::KwRoot },
        { "string", TokKind::KwString },
        { "struct", TokKind::KwStruct },
        { "super", TokKind::KwSuper },
        { "switch", TokKind::KwSwitch },
        { "try_as", TokKind::KwTryAs },
        { "tup", TokKind::KwTup },
        { "type", TokKind::KwType },
        { "unit", TokKind::KwUnit },
        { "while", TokKind::KwWhile },
    };

    template<std::size_t N>
    static constexpr bool _is_keyword_table_sorted(
        const std::pair<std::string_view, TokKind> (&arr)[N]) {
        for (std::size_t i = 1; i < N; ++i) {
            if (!(arr[i - 1].first < arr[i].first)) {
                return false;
            }
        }
        return true;
    }

    static_assert(_is_keyword_table_sorted(_keyword_table),
        "Keyword table must be sorted lexicographically");

    static std::optional<TokKind> _lookup_keyword(std::string_view str) {
        auto first = std::begin(_keyword_table);
        auto last = std::end(_keyword_table);

        auto it = std::lower_bound(
            first, last, str, [](const auto& pair, std::string_view str) {
                return pair.first < str;
            });

        if (it != last && it->first == str)
            return it->second;

        return std::nullopt;
    }

    static bool _is_bin_digit(utf8::Codepoint c) {
        return c == L'0' || c == L'1';
    }

    static bool _is_oct_digit(utf8::Codepoint c) {
        return L'0' <= c && c <= L'7';
    }

    static bool _is_hex_digit(utf8::Codepoint c) {
        return utf8::is_digit(c) || (L'a' <= c && c <= L'f') ||
               (L'A' <= c && c <= L'F');
    }

    static bool _is_sep(utf8::Codepoint c) {
        switch (c) {
        case L'(':
        case L')':
        case L'{':
        case L'}':
        case L'[':
        case L']':
        case L'<':
        case L'>':
        case L'!':
        case L':':
        case L'.':
        case L',':
        case L';':
        case L'=':
        case L'+':
        case L'-':
        case L'*':
        case L'/':
        case L'&':
        case L'|':
        case L'^':
        case L'~':
        case L'%':
        case L'?':
            return true;
        default:
            return false;
        }
    }

    TokEmitter::TokEmitter(TokHandler handler) :
        m_handler(handler) { }

    void TokEmitter::emit(const lex::Tok& tok) { m_handler(tok); }

    Lexer::Lexer(std::string_view src) :
        m_src(src),
        m_src_iter(src),
        m_pos(),
        m_pos_next(),
        m_pos_begin(m_pos),
        m_curr(),
        m_ch(m_src_iter.eof() ? 0 : m_src_iter.peek().value()),
        m_eof(m_src_iter.eof()),
        m_diag_emitter(nullptr),
        m_tok_emitter(nullptr) {
        next();
    }

    void Lexer::set_diag_emitter(diag::DiagEmitter& diag_emitter) {
        m_diag_emitter = &diag_emitter;
    }

    void Lexer::set_tok_emitter(TokEmitter& tok_emitter) {
        m_tok_emitter = &tok_emitter;
    }

    Tok Lexer::next() {
        Tok res = m_curr;
        if (m_tok_emitter) {
            m_tok_emitter->emit(res);
        }
        m_curr = get_token();
        return res;
    }

    Tok Lexer::lex_string() {
        if (m_ch == L'"' || m_ch == L'\'') {
            bool is_char = false;
            if (m_ch == L'\'') {
                is_char = true;
            }
            advance();
            bool escaped = false;
            bool closed = false;
            while (!m_eof) {
                if (!utf8::is_print(m_ch)) {
                    if (is_char) {
                        return create_err_and_emit(diag::ErrKind::
                                NonPrintableCharacterInCharacterLiteral);
                    } else {
                        return create_err_and_emit(diag::ErrKind::
                                NonPrintableCharacterInStringLiteral);
                    }
                }
                if (escaped) {
                    escaped = false;
                } else {
                    if ((m_ch == L'\'' && is_char) ||
                        (m_ch == L'"' && !is_char)) {
                        closed = true;
                        break;
                    } else if (m_ch == L'\\') {
                        escaped = true;
                    }
                }
                advance();
            }
            if (!closed) {
                return create_err_and_emit(diag::ErrKind::UnterminatedString);
            }
            advance();
            if (is_char) {
                return create_tok(TokKind::LitCharacter);
            } else {
                return create_tok(TokKind::LitString);
            }
        }
        return create_tok(TokKind::None);
    }

    Tok Lexer::lex_num(bool negative) {
        if (utf8::is_digit(m_ch)) {
            bool has_dot = false;
            bool has_digit_after_prefix = false;
            bool has_byte_postfix = false;
            int base = 10;
            if (m_ch == L'0') {
                advance();
                switch (m_ch) {
                case L'b':
                case L'B':
                    base = 2;
                    break;
                case L'o':
                case L'O':
                    base = 8;
                    break;
                case L'x':
                case L'X':
                    base = 16;
                    break;
                case L'.':
                    has_dot = true;
                    break;
                default:
                    if (utf8::is_space(m_ch) || _is_sep(m_ch)) {
                        return create_tok(TokKind::LitInteger);
                    }
                    return create_err_and_emit(
                        diag::ErrKind::InvalidIntegerPrefix);
                }
            }

            advance();
            while (!m_eof) {
                if (m_ch == L'.' && base == 10) {
                    if (has_dot) {
                        break;
                    }
                    has_dot = true;
                    Lexer saved_state = *this;
                    advance();
                    if (!utf8::is_digit(m_ch)) {
                        *this = saved_state;
                        has_dot = false;
                        break;
                    }
                } else if (base == 2 && _is_bin_digit(m_ch)) {
                    has_digit_after_prefix = true;
                } else if (base == 8 && _is_oct_digit(m_ch)) {
                    has_digit_after_prefix = true;
                } else if (base == 10 && utf8::is_digit(m_ch)) {
                    // OK
                } else if (base == 16 && _is_hex_digit(m_ch)) {
                    has_digit_after_prefix = true;
                } else if ((_is_sep(m_ch) || utf8::is_space(m_ch))) {
                    if (base != 10 && !has_digit_after_prefix) {
                        return create_err_and_emit(
                            diag::ErrKind::NoDigitsAfterIntegerPrefix);
                    }
                    break;
                } else if (m_ch == L'y' || m_ch == L'Y') {
                    if (has_dot) {
                        return create_err_and_emit(
                            diag::ErrKind::BytePostfixInFloatingPointLiteral);
                    } else {
                        if (base != 10 && !has_digit_after_prefix) {
                            return create_err_and_emit(
                                diag::ErrKind::NoDigitsAfterIntegerPrefix);
                        }
                    }
                    if (negative) {
                        return create_err_and_emit(
                            diag::ErrKind::NegativeByteLiteral);
                    }
                    has_byte_postfix = true;
                    advance();
                    break;
                } else if (m_ch == L'_') {
                    // OK
                } else {
                    return create_err_and_emit(
                        diag::ErrKind::UnexpectedCharacterInNumberLiteral);
                }
                advance();
            }

            TokKind kind;
            if (has_dot) {
                kind = TokKind::LitFloating;
            } else if (has_byte_postfix) {
                kind = TokKind::LitByte;
            } else {
                kind = TokKind::LitInteger;
            }
            return create_tok(kind);
        }
        return create_tok(TokKind::None);
    }

    Tok Lexer::lex_word() {
        if (utf8::is_alpha(m_ch) || m_ch == L'_') {
            advance();
            while (!m_eof) {
                if (utf8::is_alnum(m_ch) || m_ch == L'_') {
                    advance();
                } else {
                    break;
                }
            }
            std::string_view value = get_curr_value();
            std::optional<TokKind> keyword_kind = _lookup_keyword(value);
            if (keyword_kind) {
                return create_tok(*keyword_kind);
            } else {
                if (value == "true" || value == "false") {
                    return create_tok(TokKind::LitBoolean);
                } else {
                    return create_tok(TokKind::Ident);
                }
            }
        }
        return create_tok(TokKind::None);
    }

    Tok Lexer::lex_sym() {
        if (_is_sep(m_ch)) {
            TokKind sym_kind = TokKind::None;
            switch (m_ch) {
                using enum TokKind;
            case L'(':
                sym_kind = LParen;
                advance();
                break;
            case L')':
                sym_kind = RParen;
                advance();
                break;
            case L'{':
                sym_kind = LBrace;
                advance();
                break;
            case L'}':
                sym_kind = RBrace;
                advance();
                break;
            case L'[':
                sym_kind = LBracket;
                advance();
                break;
            case L']':
                sym_kind = RBracket;
                advance();
                break;
            case L'<':
                sym_kind = LAngle;
                advance();
                if (m_ch == L'=' && !m_eof) {
                    sym_kind = LAngleEq;
                    advance();
                    break;
                }
                if (m_ch == L'<' && !m_eof) {
                    sym_kind = LAngleLAngle;
                    advance();
                    if (m_ch == L'=' && !m_eof) {
                        sym_kind = LAngleLAngleEq;
                        advance();
                        break;
                    }
                    break;
                }
                break;
            case L'>':
                sym_kind = RAngle;
                advance();
                if (m_ch == L'=' && !m_eof) {
                    sym_kind = RAngleEq;
                    advance();
                    break;
                }
                if (m_ch == L'>' && !m_eof) {
                    sym_kind = RAngleRAngle;
                    advance();
                    if (m_ch == L'=' && !m_eof) {
                        sym_kind = RAngleRAngleEq;
                        advance();
                        break;
                    }
                    break;
                }
                break;
            case L'!':
                sym_kind = Exclam;
                advance();
                if (m_ch == L'=' && !m_eof) {
                    sym_kind = ExclamEq;
                    advance();
                    break;
                }
                break;
            case L':':
                sym_kind = Colon;
                advance();
                if (m_ch == L':' && !m_eof) {
                    sym_kind = ColonColon;
                    advance();
                    break;
                }
                break;
            case L'.':
                sym_kind = Dot;
                advance();
                break;
            case L',':
                sym_kind = Comma;
                advance();
                break;
            case L';':
                sym_kind = Semicolon;
                advance();
                break;
            case L'=':
                sym_kind = Eq;
                advance();
                if (m_ch == L'=' && !m_eof) {
                    sym_kind = EqEq;
                    advance();
                    break;
                }
                if (m_ch == L'>' && !m_eof) {
                    sym_kind = EqRAngle;
                    advance();
                    break;
                }
                break;
            case L'+':
                sym_kind = Plus;
                advance();
                if (m_ch == L'=' && !m_eof) {
                    sym_kind = PlusEq;
                    advance();
                    break;
                }
                break;
            case L'-': {
                sym_kind = Dash;
                advance();
                if (m_ch == L'=' && !m_eof) {
                    sym_kind = DashEq;
                    advance();
                    break;
                }
                if (m_ch == L'>' && !m_eof) {
                    sym_kind = DashRAngle;
                    advance();
                    break;
                }
#if 0
                Tok num_tok = lex_num(true);
                if (!num_tok.is_none()) {
                    return num_tok;
                }
#endif
            } break;
            case L'*':
                sym_kind = Star;
                advance();
                if (m_ch == L'=' && !m_eof) {
                    sym_kind = StarEq;
                    advance();
                    break;
                }
                break;
            case L'/':
                sym_kind = Slash;
                advance();
                if (m_ch == L'=' && !m_eof) {
                    sym_kind = SlashEq;
                    advance();
                    break;
                }
                // Skip comments
                if (m_ch == L'/') {
                    while (!m_eof && m_ch != L'\n') {
                        advance();
                    }
                    return create_tok(TokKind::Continue);
                }
                break;
            case L'&':
                sym_kind = Amp;
                advance();
                if (m_ch == L'=' && !m_eof) {
                    sym_kind = AmpEq;
                    advance();
                    break;
                }
                if (m_ch == L'&' && !m_eof) {
                    sym_kind = AmpAmp;
                    advance();
                    break;
                }
                break;
            case L'|':
                sym_kind = Pipe;
                advance();
                if (m_ch == L'=' && !m_eof) {
                    sym_kind = PipeEq;
                    advance();
                    break;
                }
                if (m_ch == L'|' && !m_eof) {
                    sym_kind = PipePipe;
                    advance();
                    break;
                }
                break;
            case L'^':
                sym_kind = Caret;
                advance();
                if (m_ch == L'=' && !m_eof) {
                    sym_kind = CaretEq;
                    advance();
                    break;
                }
                break;
            case L'~':
                sym_kind = Tilde;
                advance();
                break;
            case L'%':
                sym_kind = Percent;
                advance();
                if (m_ch == L'=' && !m_eof) {
                    sym_kind = PercentEq;
                    advance();
                    break;
                }
                break;
            case L'?':
                sym_kind = Question;
                advance();
                break;
            }
            return create_tok(sym_kind);
        }
        return create_tok(TokKind::None);
    }

    Tok Lexer::peek() const { return m_curr; }

    Tok Lexer::get_token() {
        while (true) {
            m_pos_begin = m_pos;

            if (m_eof) {
                return create_tok(TokKind::Eof);
            }

            if (utf8::is_space(m_ch)) {
                advance();
                continue;
            }

            Tok string_tok = lex_string();
            if (string_tok.is_continue()) {
                continue;
            }
            if (!string_tok.is_none()) {
                return string_tok;
            }

            Tok num_tok = lex_num(false);
            if (num_tok.is_continue()) {
                continue;
            }
            if (!num_tok.is_none()) {
                return num_tok;
            }

            Tok word_tok = lex_word();
            if (word_tok.is_continue()) {
                continue;
            }
            if (!word_tok.is_none()) {
                return word_tok;
            }

            Tok sym_tok = lex_sym();
            if (sym_tok.is_continue()) {
                continue;
            }
            if (!sym_tok.is_none()) {
                return sym_tok;
            }

            return create_err_and_emit(diag::ErrKind::UnexpectedCharacter);
        }
    }

    void Lexer::advance() {
        m_pos = m_pos_next;
        m_pos_next.offset = m_src_iter.byte_offset();
        std::optional<utf8::Codepoint> codepoint = m_src_iter.next();
        m_pos_next.c++;
        if (codepoint) {
            m_ch = codepoint.value();
            if (codepoint.value() == L'\n') {
                m_pos_next.l++;
                m_pos_next.c = 1;
            }
        } else {
            m_eof = true;
        }
    }

    void Lexer::recover() {
        while (!utf8::is_space(m_ch) && !_is_sep(m_ch) && !m_eof) {
            advance();
        }
    }

    Tok Lexer::create_err_and_emit(diag::Err err) {
        if (m_diag_emitter != nullptr) {
            m_diag_emitter->emit({ err, m_pos });
        }
        recover();
        return create_tok(TokKind::Err);
    }

    Tok Lexer::create_tok(TokKind kind) const {
        return Tok({ m_pos_begin, m_pos }, kind, get_curr_value());
    }

    Tok Lexer::create_tok(TokKind kind, std::string_view view) const {
        return Tok({ m_pos_begin, m_pos }, kind, view);
    }

    std::string_view Lexer::get_curr_value() const {
        return std::string_view(
            m_src.begin() + m_pos_begin.offset, m_src.begin() + m_pos.offset);
    }

}
