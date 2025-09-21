#include "print.h"

namespace alvo::ast::print {

    OstreamSink::OstreamSink(std::ostream& ostream) :
        m_ostream(&ostream) { }

    void OstreamSink::write(std::string_view sv) { (*m_ostream) << sv; }

    bool Color::operator==(Color rhs) const {
        return r == rhs.r & g == rhs.g && b == rhs.b;
    }

    bool Color::operator!=(Color rhs) const { return !(*this == rhs); }

    Style Style::fg_color(Color c) {
        return Style(c, std::nullopt, false, false, false, false, false, false);
    }

    Style Style::bg_color(Color c) {
        return Style(std::nullopt, c, false, false, false, false, false, false);
    }

    Style Style::bold() {
        return Style(std::nullopt, std::nullopt, true, false, false, false,
            false, false);
    }

    Style Style::italic() {
        return Style(std::nullopt, std::nullopt, false, true, false, false,
            false, false);
    }

    Style Style::underline() {
        return Style(std::nullopt, std::nullopt, false, false, true, false,
            false, false);
    }

    Style Style::blink() {
        return Style(std::nullopt, std::nullopt, false, false, false, true,
            false, false);
    }

    Style Style::inverse() {
        return Style(std::nullopt, std::nullopt, false, false, false, false,
            true, false);
    }

    Style Style::strikethrough() {
        return Style(std::nullopt, std::nullopt, false, false, false, false,
            false, true);
    }

    Style::Style() :
        m_fg_color(std::nullopt),
        m_bg_color(std::nullopt),
        m_bold(false),
        m_italic(false),
        m_underline(false),
        m_blink(false),
        m_inverse(false),
        m_strikethrough(false) { }

    Style::Style(std::optional<Color> fg_color, std::optional<Color> bg_color,
        bool bold, bool italic, bool underline, bool blink, bool inverse,
        bool strikethrough) :
        m_fg_color(fg_color),
        m_bg_color(bg_color),
        m_bold(bold),
        m_italic(italic),
        m_underline(underline),
        m_blink(blink),
        m_inverse(inverse),
        m_strikethrough(strikethrough) { }

    bool Style::operator==(Style rhs) const {
        return m_fg_color == rhs.m_fg_color && m_bg_color == rhs.m_bg_color &&
               m_bold == rhs.m_bold && m_italic == rhs.m_italic &&
               m_underline == rhs.m_underline && m_blink == rhs.m_blink &&
               m_inverse == rhs.m_inverse &&
               m_strikethrough == rhs.m_strikethrough;
    }

    bool Style::operator!=(Style rhs) const { return !(*this == rhs); }

    Style Style::operator|(Style rhs) const {
        return Style(rhs.m_fg_color ? rhs.m_fg_color : this->m_fg_color,
            rhs.m_bg_color ? rhs.m_bg_color : this->m_bg_color,
            rhs.m_bold ? true : this->m_bold,
            rhs.m_italic ? true : this->m_italic,
            rhs.m_underline ? true : this->m_underline,
            rhs.m_blink ? true : this->m_blink,
            rhs.m_inverse ? true : this->m_inverse,
            rhs.m_strikethrough ? true : this->m_strikethrough);
    }

    Style& Style::operator|=(Style rhs) {
        *this = *this | rhs;
        return *this;
    }

    std::optional<Color> Style::get_fg_color() const { return m_fg_color; }

    std::optional<Color> Style::get_bg_color() const { return m_bg_color; }

    bool Style::get_bold() const { return m_bold; }

    bool Style::get_italic() const { return m_italic; }

    bool Style::get_underline() const { return m_underline; }

    bool Style::get_blink() const { return m_blink; }

    bool Style::get_inverse() const { return m_inverse; }

    bool Style::get_strikethrough() const { return m_strikethrough; }

}
