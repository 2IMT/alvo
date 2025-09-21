#pragma once

#include <iterator>
#include <cstddef>
#include <string_view>
#include <variant>
#include <ostream>
#include <cstdint>
#include <optional>

#include "../ast.h"

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

    template<PrinterSink Sink>
    class Printer {
    public:
        Printer(Sink& sink, std::size_t indent_width) :
            m_sink(&sink),
            m_indent(0),
            m_indent_width(indent_width),
            m_style_config(std::nullopt) { }

        void set_style_config(StyleConfig config) { m_style_config = config; }

        void print_node(const Invalid&);
        void print_node(const PathSegment& n);
        void print_node(const PathSegment::Root& n);
        void print_node(const PathSegment::Super& n);
        void print_node(const PathSegment::Name& n);
        void print_node(const Import& n);
        void print_node(const Import::Normal& n);
        void print_node(const Import::Glob& n);
        void print_node(const Import::Renamed& n);
        void print_node(const Type& n);
        void print_node(const Type::Unit& n);
        void print_node(const Type::String& n);
        void print_node(const Type::Char& n);
        void print_node(const Type::Int& n);
        void print_node(const Type::Byte& n);
        void print_node(const Type::Float& n);
        void print_node(const Type::Bool& n);
        void print_node(const Type::Array& n);
        void print_node(const Type::Tup& n);
        void print_node(const Type::Func& n);
        void print_node(const Type::Path& n);
        void print_node(const Expr& n);
        void print_node(const Expr::Literal& n);
        void print_node(const Expr::Literal::Unit& n);
        void print_node(const Expr::Literal::Null& n);
        void print_node(const Expr::Literal::String& n);
        void print_node(const Expr::Literal::Character& n);
        void print_node(const Expr::Literal::Integer& n);
        void print_node(const Expr::Literal::Byte& n);
        void print_node(const Expr::Literal::Floating& n);
        void print_node(const Expr::Literal::Boolean& n);
        void print_node(const Expr::Literal::Array& n);
        void print_node(const Expr::Literal::Array::Regular& n);
        void print_node(const Expr::Literal::Array::DefaultNTimes& n);
        void print_node(const Expr::Literal::Array::ExprNTimes& n);
        void print_node(const Expr::Literal::Tup& n);
        void print_node(const Expr::Unop& n);
        void print_node(const Expr::Unop::Op& n);
        void print_node(const Expr::Binop& n);
        void print_node(const Expr::Binop::Op& n);
        void print_node(const Expr::Index& n);
        void print_node(const Expr::Call& n);
        void print_node(const Expr::Cast& n);
        void print_node(const Expr::TryCast& n);
        void print_node(const Block& n);
        void print_node(const Stmt& n);
        void print_node(const Stmt::Let& n);
        void print_node(const Stmt::If& n);
        void print_node(const Stmt::If::Elif& n);
        void print_node(const Stmt::Switch& n);
        void print_node(const Stmt::Switch::Case& n);
        void print_node(const Stmt::Loop& n);
        void print_node(const Stmt::For& n);
        void print_node(const Stmt::While& n);
        void print_node(const Stmt::Return& n);
        void print_node(const Stmt::Defer& n);
        void print_node(const Stmt::Continue& n);
        void print_node(const Stmt::Break& n);
        void print_node(const Func& n);
        void print_node(const Func::Param& n);
        void print_node(const Decl& n);
        void print_node(const Decl::Struct& n);
        void print_node(const Decl::Struct::Item& n);
        void print_node(const Decl::Struct::Item::Field& n);
        void print_node(const Decl::Enum& n);
        void print_node(const Decl::Enum::Item& n);
        void print_node(const Decl::Enum::Item::Field& n);
        void print_node(const Decl::TypeAlias& n);
        void print_node(const Decl::Const& n);
        void print_node(const TopLevel& n);
        void print_node(const Module& n);

    private:
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
            print_node(value);
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
        void field(std::string_view name, const List<T>& value) {
            field_name(name);
            if (value.empty()) {
                m_sink->write("[]\n");
            } else {
                m_sink->write("[\n");
                m_indent++;
                for (const T& node : value) {
                    indent();
                    print_node(node);
                    m_sink->write(",\n");
                }
                m_indent--;
                indent();
                m_sink->write("]\n");
            }
        }

        template<typename T>
        void field(std::string_view name, const Ptr<T>& value) {
            field(name, *value);
        }

        template<>
        void field(std::string_view name, const bool& value) {
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

        template<>
        void field(std::string_view name, const std::string_view& value) {
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

    template<PrinterSink Sink>
    void Printer<Sink>::print_node(const Invalid&) {
        node("Invalid");
    }

    template<PrinterSink Sink>
    void Printer<Sink>::print_node(const PathSegment& n) {
        node_begin("PathSegment");

        field("val", n.val);

        node_end();
    }

    template<PrinterSink Sink>
    void Printer<Sink>::print_node(const PathSegment::Root&) {
        node("Root");
    }

    template<PrinterSink Sink>
    void Printer<Sink>::print_node(const PathSegment::Super&) {
        node("Super");
    }

    template<PrinterSink Sink>
    void Printer<Sink>::print_node(const PathSegment::Name& n) {
        node_begin("Name");

        field("value", n.value);

        node_end();
    }

    template<PrinterSink Sink>
    void Printer<Sink>::print_node(const Import& n) {
        node_begin("Import");

        field("kind", n.kind);
        field("segments", n.segments);

        node_end();
    }

    template<PrinterSink Sink>
    void Printer<Sink>::print_node(const Import::Normal&) {
        node("Normal");
    }

    template<PrinterSink Sink>
    void Printer<Sink>::print_node(const Import::Glob&) {
        node("Glob");
    }

    template<PrinterSink Sink>
    void Printer<Sink>::print_node(const Import::Renamed& n) {
        node_begin("Renamed");

        field("renamed_to", n.renamed_to);

        node_end();
    }

    template<PrinterSink Sink>
    void Printer<Sink>::print_node(const Type& n) {
        node_begin("Type");

        field("val", n.val);

        node_end();
    }

    template<PrinterSink Sink>
    void Printer<Sink>::print_node(const Type::Unit&) {
        node("Unit");
    }

    template<PrinterSink Sink>
    void Printer<Sink>::print_node(const Type::String&) {
        node("String");
    }

    template<PrinterSink Sink>
    void Printer<Sink>::print_node(const Type::Char&) {
        node("Char");
    }

    template<PrinterSink Sink>
    void Printer<Sink>::print_node(const Type::Int&) {
        node("Int");
    }

    template<PrinterSink Sink>
    void Printer<Sink>::print_node(const Type::Byte&) {
        node("Byte");
    }

    template<PrinterSink Sink>
    void Printer<Sink>::print_node(const Type::Float&) {
        node("Float");
    }

    template<PrinterSink Sink>
    void Printer<Sink>::print_node(const Type::Bool&) {
        node("Bool");
    }

    template<PrinterSink Sink>
    void Printer<Sink>::print_node(const Type::Array& n) {
        node_begin("Array");

        field("type", n.type);

        node_end();
    }

    template<PrinterSink Sink>
    void Printer<Sink>::print_node(const Type::Tup& n) {
        node_begin("Tup");

        field("types", n.types);

        node_end();
    }

    template<PrinterSink Sink>
    void Printer<Sink>::print_node(const Type::Func& n) {
        node_begin("Func");

        field("params", n.params);
        field("return_type", n.return_type);

        node_end();
    }

    template<PrinterSink Sink>
    void Printer<Sink>::print_node(const Type::Path& n) {
        node_begin("Path");

        field("segments", n.segments);

        node_end();
    }

    template<PrinterSink Sink>
    void Printer<Sink>::print_node(const Expr& n) {
        node_begin("Expr");

        field("val", n.val);

        node_end();
    }

    template<PrinterSink Sink>
    void Printer<Sink>::print_node(const Expr::Literal& n) {
        node_begin("Literal");

        field("val", n.val);

        node_end();
    }

    template<PrinterSink Sink>
    void Printer<Sink>::print_node(const Expr::Literal::Unit&) {
        node("Unit");
    }

    template<PrinterSink Sink>
    void Printer<Sink>::print_node(const Expr::Literal::Null&) {
        node("Null");
    }

    template<PrinterSink Sink>
    void Printer<Sink>::print_node(const Expr::Literal::String& n) {
        node_begin("String");

        field("value", n.value);

        node_end();
    }

    template<PrinterSink Sink>
    void Printer<Sink>::print_node(const Expr::Literal::Character& n) {
        node_begin("Character");

        field("value", n.value);

        node_end();
    }

    template<PrinterSink Sink>
    void Printer<Sink>::print_node(const Expr::Literal::Integer& n) {
        node_begin("Integer");

        field("value", n.value);

        node_end();
    }

    template<PrinterSink Sink>
    void Printer<Sink>::print_node(const Expr::Literal::Byte& n) {
        node_begin("Byte");

        field("value", n.value);

        node_end();
    }

    template<PrinterSink Sink>
    void Printer<Sink>::print_node(const Expr::Literal::Floating& n) {
        node_begin("Floating");

        field("value", n.value);

        node_end();
    }

    template<PrinterSink Sink>
    void Printer<Sink>::print_node(const Expr::Literal::Boolean& n) {
        node_begin("Boolean");

        field("value", n.value);

        node_end();
    }

    template<PrinterSink Sink>
    void Printer<Sink>::print_node(const Expr::Literal::Array& n) {
        node_begin("Array");

        field("val", n.val);

        node_end();
    }

    template<PrinterSink Sink>
    void Printer<Sink>::print_node(const Expr::Literal::Array::Regular& n) {
        node_begin("Regular");

        field("elements", n.elements);

        node_end();
    }

    template<PrinterSink Sink>
    void Printer<Sink>::print_node(
        const Expr::Literal::Array::DefaultNTimes& n) {
        node_begin("DefaultNTimes");

        field("times", n.times);

        node_end();
    }

    template<PrinterSink Sink>
    void Printer<Sink>::print_node(const Expr::Literal::Array::ExprNTimes& n) {
        node_begin("ExprNTimes");

        field("expr", n.expr);
        field("times", n.times);

        node_end();
    }

    template<PrinterSink Sink>
    void Printer<Sink>::print_node(const Expr::Literal::Tup& n) {
        node_begin("Tup");

        field("exprs", n.exprs);

        node_end();
    }

    template<PrinterSink Sink>
    void Printer<Sink>::print_node(const Expr::Unop& n) {
        node_begin("Unop");

        field("op", n.op);
        field("expr", n.expr);

        node_end();
    }

    template<PrinterSink Sink>
    void Printer<Sink>::print_node(const Expr::Unop::Op& n) {
        switch (n) {
            using enum Expr::Unop::Op;
        case Invalid:
            node("Invalid");
            break;
        case Plus:
            node("Plus");
            break;
        case Minus:
            node("Minus");
            break;
        case Not:
            node("Not");
            break;
        case BinaryNot:
            node("BinaryNot");
            break;
        }
    }

    template<PrinterSink Sink>
    void Printer<Sink>::print_node(const Expr::Binop& n) {
        node_begin("Binop");

        field("op", n.op);
        field("lhs", n.lhs);
        field("rhs", n.rhs);

        node_end();
    }

    template<PrinterSink Sink>
    void Printer<Sink>::print_node(const Expr::Binop::Op& n) {
        switch (n) {
            using enum Expr::Binop::Op;
        case Invalid:
            node("Invalid");
            break;
        case Assign:
            node("Assign");
            break;
        case PlusAssign:
            node("PlusAssign");
            break;
        case MinusAssign:
            node("MinusAssign");
            break;
        case MultiplyAssign:
            node("MultiplyAssign");
            break;
        case DivideAssign:
            node("DivideAssign");
            break;
        case BinaryAndAssign:
            node("BinaryAndAssign");
            break;
        case BinaryOrAssign:
            node("BinaryOrAssign");
            break;
        case BinaryXorAssign:
            node("BinaryXorAssign");
            break;
        case ModAssign:
            node("ModAssign");
            break;
        case ShiftLeftAssign:
            node("ShiftLeftAssign");
            break;
        case ShiftRightAssign:
            node("ShiftRightAssign");
            break;
        case Or:
            node("Or");
            break;
        case And:
            node("And");
            break;
        case BinaryOr:
            node("BinaryOr");
            break;
        case BinaryXor:
            node("BinaryXor");
            break;
        case BinaryAnd:
            node("BinaryAnd");
            break;
        case Equal:
            node("Equal");
            break;
        case NotEqual:
            node("NotEqual");
            break;
        case Less:
            node("Less");
            break;
        case LessEqual:
            node("LessEqual");
            break;
        case Greater:
            node("Greater");
            break;
        case GreaterEqual:
            node("GreaterEqual");
            break;
        case ShiftLeft:
            node("ShiftLeft");
            break;
        case ShiftRight:
            node("ShiftRight");
            break;
        case Plus:
            node("Plus");
            break;
        case Minus:
            node("Minus");
            break;
        case Multiply:
            node("Multiply");
            break;
        case Divide:
            node("Divide");
            break;
        case Mod:
            node("Mod");
            break;
        case Access:
            node("Access");
            break;
        case StaticAccess:
            node("StaticAccess");
            break;
        }
    }

    template<PrinterSink Sink>
    void Printer<Sink>::print_node(const Expr::Index& n) {
        node_begin("Index");

        field("expr", n.expr);
        field("index", n.index);

        node_end();
    }

    template<PrinterSink Sink>
    void Printer<Sink>::print_node(const Expr::Call& n) {
        node_begin("Call");

        field("expr", n.expr);
        field("args", n.args);

        node_end();
    }

    template<PrinterSink Sink>
    void Printer<Sink>::print_node(const Expr::Cast& n) {
        node_begin("Cast");

        field("expr", n.expr);
        field("type", n.type);

        node_end();
    }

    template<PrinterSink Sink>
    void Printer<Sink>::print_node(const Expr::TryCast& n) {
        node_begin("TryCast");

        field("expr", n.expr);
        field("type", n.type);

        node_end();
    }

    template<PrinterSink Sink>
    void Printer<Sink>::print_node(const Block& n) {
        node_begin("Block");

        field("stmts", n.stmts);

        node_end();
    }

    template<PrinterSink Sink>
    void Printer<Sink>::print_node(const Stmt& n) {
        node_begin("Stmt");

        field("val", n.val);

        node_end();
    }

    template<PrinterSink Sink>
    void Printer<Sink>::print_node(const Stmt::Let& n) {
        node_begin("Let");

        field("name", n.name);
        field("type", n.type);
        field("expr", n.expr);

        node_end();
    }

    template<PrinterSink Sink>
    void Printer<Sink>::print_node(const Stmt::If& n) {
        node_begin("If");

        field("expr", n.expr);
        field("main", n.main);
        field("elifs", n.elifs);
        field("else_", n.else_);

        node_end();
    }

    template<PrinterSink Sink>
    void Printer<Sink>::print_node(const Stmt::If::Elif& n) {
        node_begin("Elif");

        field("expr", n.expr);
        field("block", n.block);

        node_end();
    }

    template<PrinterSink Sink>
    void Printer<Sink>::print_node(const Stmt::Switch& n) {
        node_begin("Switch");

        field("expr", n.expr);
        field("cases", n.cases);

        node_end();
    }

    template<PrinterSink Sink>
    void Printer<Sink>::print_node(const Stmt::Switch::Case& n) {
        node_begin("Case");

        field("expr", n.expr);
        field("block", n.block);

        node_end();
    }

    template<PrinterSink Sink>
    void Printer<Sink>::print_node(const Stmt::Loop& n) {
        node_begin("Loop");

        field("block", n.block);

        node_end();
    }

    template<PrinterSink Sink>
    void Printer<Sink>::print_node(const Stmt::For& n) {
        node_begin("For");

        field("name", n.name);
        field("expr", n.expr);
        field("block", n.block);

        node_end();
    }

    template<PrinterSink Sink>
    void Printer<Sink>::print_node(const Stmt::While& n) {
        node_begin("While");

        field("expr", n.expr);
        field("block", n.block);

        node_end();
    }

    template<PrinterSink Sink>
    void Printer<Sink>::print_node(const Stmt::Return& n) {
        node_begin("Return");

        field("expr", n.expr);

        node_end();
    }

    template<PrinterSink Sink>
    void Printer<Sink>::print_node(const Stmt::Defer& n) {
        node_begin("Defer");

        field("expr", n.expr);

        node_end();
    }

    template<PrinterSink Sink>
    void Printer<Sink>::print_node(const Stmt::Continue&) {
        node("Continue");
    }

    template<PrinterSink Sink>
    void Printer<Sink>::print_node(const Stmt::Break&) {
        node("Break");
    }

    template<PrinterSink Sink>
    void Printer<Sink>::print_node(const Func& n) {
        node_begin("Func");

        field("params", n.params);
        field("ret", n.ret);
        field("block", n.block);

        node_end();
    }

    template<PrinterSink Sink>
    void Printer<Sink>::print_node(const Func::Param& n) {
        node_begin("Param");

        field("name", n.name);
        field("type", n.type);

        node_end();
    }

    template<PrinterSink Sink>
    void Printer<Sink>::print_node(const Decl& n) {
        node_begin("Decl");

        field("is_export", n.is_export);
        field("name", n.name);
        field("val", n.val);

        node_end();
    }

    template<PrinterSink Sink>
    void Printer<Sink>::print_node(const Decl::Struct& n) {
        node_begin("Struct");

        field("items", n.items);

        node_end();
    }

    template<PrinterSink Sink>
    void Printer<Sink>::print_node(const Decl::Struct::Item& n) {
        node_begin("Item");

        field("is_export", n.is_export);
        field("val", n.val);

        node_end();
    }

    template<PrinterSink Sink>
    void Printer<Sink>::print_node(const Decl::Struct::Item::Field& n) {
        node_begin("Field");

        field("name", n.name);
        field("type", n.type);

        node_end();
    }

    template<PrinterSink Sink>
    void Printer<Sink>::print_node(const Decl::Enum& n) {
        node_begin("Enum");

        field("items", n.items);

        node_end();
    }

    template<PrinterSink Sink>
    void Printer<Sink>::print_node(const Decl::Enum::Item& n) {
        node_begin("Item");

        field("is_export", n.is_export);
        field("val", n.val);

        node_end();
    }

    template<PrinterSink Sink>
    void Printer<Sink>::print_node(const Decl::Enum::Item::Field& n) {
        node_begin("Field");

        field("name", n.name);

        node_end();
    }

    template<PrinterSink Sink>
    void Printer<Sink>::print_node(const Decl::TypeAlias& n) {
        node_begin("TypeAlias");

        field("type", n.type);

        node_end();
    }

    template<PrinterSink Sink>
    void Printer<Sink>::print_node(const Decl::Const& n) {
        node_begin("Const");

        field("type", n.type);
        field("expr", n.expr);

        node_end();
    }

    template<PrinterSink Sink>
    void Printer<Sink>::print_node(const TopLevel& n) {
        node_begin("TopLevel");

        field("val", n.val);

        node_end();
    }

    template<PrinterSink Sink>
    void Printer<Sink>::print_node(const Module& n) {
        node_begin("Module");

        field("top_levels", n.top_levels);

        node_end();
    }

}
