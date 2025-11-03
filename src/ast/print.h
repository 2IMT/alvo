#pragma once

#include <iterator>
#include <cstddef>
#include <string_view>
#include <variant>
#include <ostream>
#include <cstdint>
#include <optional>

#include "util.h"

namespace alvo::ast::print {

    template<typename T>
    concept PrinterSink = requires(T& sink, std::string_view sv) {
        { sink.write(sv) } -> std::same_as<void>;
    };

    class OstreamSink {
    public:
        OstreamSink(std::ostream& ostream);

        void write(std::string_view sv);

    private:
        std::ostream* m_ostream;
    };

    static_assert(PrinterSink<OstreamSink>,
        "`OstreamSink` must implement PrintSink concept");

    template<std::output_iterator<char> Out>
    class OutputIteratorSink {
    public:
        OutputIteratorSink(Out out) :
            m_out(out) { }

        void write(std::string_view sv) {
            for (char ch : sv) {
                *m_out++ = ch;
            }
        }

    private:
        Out m_out;
    };

    struct Color {
        std::uint8_t r;
        std::uint8_t g;
        std::uint8_t b;

        bool operator==(Color rhs) const;

        bool operator!=(Color rhs) const;
    };

    class Style {
    public:
        static Style fg_color(Color c);

        static Style bg_color(Color c);

        static Style bold();

        static Style italic();

        static Style underline();

        static Style blink();

        static Style inverse();

        static Style strikethrough();

        Style();

        Style(std::optional<Color> fg_color, std::optional<Color> bg_color,
            bool bold, bool italic, bool underline, bool blink, bool inverse,
            bool strikethrough);

        bool operator==(Style rhs) const;

        bool operator!=(Style rhs) const;

        Style operator|(Style rhs) const;

        Style& operator|=(Style rhs);

        std::optional<Color> get_fg_color() const;

        std::optional<Color> get_bg_color() const;

        bool get_bold() const;

        bool get_italic() const;

        bool get_underline() const;

        bool get_blink() const;

        bool get_inverse() const;

        bool get_strikethrough() const;

    private:
        std::optional<Color> m_fg_color;
        std::optional<Color> m_bg_color;
        bool m_bold;
        bool m_italic;
        bool m_underline;
        bool m_blink;
        bool m_inverse;
        bool m_strikethrough;
    };

    struct StyleConfig {
        Style node_name;
        Style field_name;
        Style string;
        Style boolean;
    };

    template<PrinterSink Sink, typename Derived>
    class PrinterBase {
    public:
        PrinterBase(Sink& sink, std::size_t indent_width) :
            m_sink(&sink),
            m_indent(0),
            m_indent_width(indent_width),
            m_style_config(std::nullopt) { }

        void set_style_config(StyleConfig config) { m_style_config = config; }

    protected:
        void set_style(Style style) {
            uint8_t seq[128] = { 0 };
            std::size_t count = 0;
            if (style.get_fg_color()) {
                auto [r, g, b] = *style.get_fg_color();
                seq[count++] = 38;
                seq[count++] = 2;
                seq[count++] = r;
                seq[count++] = g;
                seq[count++] = b;
            }
            if (style.get_bg_color()) {
                auto [r, g, b] = *style.get_bg_color();
                seq[count++] = 48;
                seq[count++] = 2;
                seq[count++] = r;
                seq[count++] = g;
                seq[count++] = b;
            }
            if (style.get_bold()) {
                seq[count++] = 1;
            }
            if (style.get_italic()) {
                seq[count++] = 3;
            }
            if (style.get_underline()) {
                seq[count++] = 4;
            }
            if (style.get_blink()) {
                seq[count++] = 5;
            }
            if (style.get_inverse()) {
                seq[count++] = 7;
            }
            if (style.get_strikethrough()) {
                seq[count++] = 9;
            }

            if (count == 0) {
                return;
            }

            m_sink->write("\x1b[");

            char buf[3] = { '\0' };
            std::size_t len = 0;
            for (std::size_t i = 0; i < count; i++) {
                uint8_t v = seq[i];
                if (v >= 100) {
                    buf[0] = '0' + v / 100;
                    buf[1] = '0' + (v / 10) % 10;
                    buf[2] = '0' + v % 10;
                    len = 3;
                } else if (v >= 10) {
                    buf[0] = '0' + v / 10;
                    buf[1] = '0' + v % 10;
                    len = 2;
                } else {
                    buf[0] = '0' + v;
                    len = 1;
                }
                std::string_view num(buf, len);
                m_sink->write(num);
                if (i < count - 1) {
                    m_sink->write(";");
                }
            }

            m_sink->write("m");
        }

        void reset_style() { m_sink->write("\x1b[0m"); }

        void set_style_node_name() {
            if (m_style_config) {
                set_style(m_style_config->node_name);
            }
        }

        void set_style_field_name() {
            if (m_style_config) {
                set_style(m_style_config->field_name);
            }
        }

        void set_style_string() {
            if (m_style_config) {
                set_style(m_style_config->string);
            }
        }

        void set_style_boolean() {
            if (m_style_config) {
                set_style(m_style_config->boolean);
            }
        }

        void indent() {
            for (std::size_t i = 0; i < m_indent * m_indent_width; i++) {
                m_sink->write(" ");
            }
        }

        void node_begin(std::string_view name) {
            set_style_node_name();
            m_sink->write(name);
            reset_style();
            m_sink->write(" {\n");
            m_indent++;
        }

        void node(std::string_view name) {
            set_style_node_name();
            m_sink->write(name);
            reset_style();
        }

        void node_end() {
            m_indent--;
            indent();
            m_sink->write("}");
        }

        void field_name(std::string_view name) {
            indent();
            set_style_field_name();
            m_sink->write(name);
            reset_style();
            m_sink->write(": ");
        }

        template<typename T>
        void field(std::string_view name, const T& value) {
            field_name(name);
            static_cast<Derived*>(this)->print_node(value);
            m_sink->write("\n");
        }

        template<typename... Args>
        void field(std::string_view name, const std::variant<Args...>& value) {
            std::visit(
                [this, name](const auto& n) { this->field(name, n); }, value);
        }

        template<typename T>
        void field(std::string_view name, const std::optional<T>& value) {
            if (!value) {
                field_name(name);
                m_sink->write("()\n");
            } else {
                field(name, *value);
            }
        }

        template<typename T>
        void field(std::string_view name, const util::List<T>& value) {
            field_name(name);
            if (value.empty()) {
                m_sink->write("[]\n");
            } else {
                m_sink->write("[\n");
                m_indent++;
                for (const T& node : value) {
                    indent();
                    static_cast<Derived*>(this)->print_node(node);
                    m_sink->write(",\n");
                }
                m_indent--;
                indent();
                m_sink->write("]\n");
            }
        }

        template<typename T>
        void field(std::string_view name, const util::Ptr<T>& value) {
            if (value.get_ptr()) {
                field(name, *value);
            } else {
                field(name, std::string_view("null"));
            }
        }

        void field(std::string_view name, bool value) {
            field_name(name);
            set_style_boolean();
            if (value) {
                m_sink->write("true");
            } else {
                m_sink->write("false");
            }
            reset_style();
            m_sink->write("\n");
        }

        void field(std::string_view name, std::string_view value) {
            field_name(name);
            set_style_string();
            m_sink->write("`");
            m_sink->write(value);
            m_sink->write("`");
            reset_style();
            m_sink->write("\n");
        }

        Sink* m_sink;
        std::size_t m_indent;
        std::size_t m_indent_width;
        std::optional<StyleConfig> m_style_config;
    };

}
