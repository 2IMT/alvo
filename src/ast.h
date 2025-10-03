// This file is generated using `astgen.py`.    /
// DO NOT EDIT BY HAND!                         /
//                                              /
// Note: any modifications to the AST should be /
// done by changing the AST schema file         /
// `ast.json` and regenerating the sources.     /

#pragma once

#include <variant>
#include <string_view>
#include <optional>

#include "ast/util.h"
#include "ast/print.h"

namespace alvo::ast {

    struct Invalid;
    struct PathSegment;
    struct Import;
    struct Type;
    struct Expr;
    struct Block;
    struct Stmt;
    struct Func;
    struct Decl;
    struct TopLevel;
    struct Module;

    struct Invalid { };

    struct PathSegment {
        struct Root { };

        struct Super { };

        struct Name {
            std::string_view value;
            util::List<Type> generic_params;
        };

        std::variant<Invalid, Root, Super, Name> val;
    };

    struct Import {
        struct Normal { };

        struct Glob { };

        struct Renamed {
            std::string_view renamed_to;
        };

        std::variant<Invalid, Normal, Glob, Renamed> kind;
        util::List<PathSegment> segments;
    };

    struct Type {
        struct Unit { };

        struct String { };

        struct Char { };

        struct Int { };

        struct Byte { };

        struct Float { };

        struct Bool { };

        struct Array {
            util::Ptr<Type> type;
        };

        struct Tup {
            util::List<Type> types;
        };

        struct Func {
            util::List<Type> params;
            util::Ptr<Type> return_type;
        };

        struct Path {
            util::List<PathSegment> segments;
        };

        std::variant<Invalid, Unit, String, Char, Int, Byte, Float, Bool, Array,
            Tup, Func, Path>
            val;
    };

    struct Expr {
        struct Literal {
            struct Unit { };

            struct Null { };

            struct String {
                std::string_view value;
            };

            struct Character {
                std::string_view value;
            };

            struct Integer {
                std::string_view value;
            };

            struct Byte {
                std::string_view value;
            };

            struct Floating {
                std::string_view value;
            };

            struct Boolean {
                std::string_view value;
            };

            struct Array {
                struct Regular {
                    util::List<Expr> elements;
                };

                struct DefaultNTimes {
                    util::Ptr<Expr> times;
                };

                struct ExprNTimes {
                    util::Ptr<Expr> expr;
                    util::Ptr<Expr> times;
                };

                std::variant<Regular, DefaultNTimes, ExprNTimes> val;
            };

            struct Tup {
                util::List<Expr> exprs;
            };

            std::variant<Invalid, Unit, Null, String, Character, Integer, Byte,
                Floating, Boolean, Array, Tup, util::Ptr<Func>>
                val;
        };

        struct Unop {
            enum class Op {
                Invalid,
                Plus,
                Minus,
                Not,
                BinaryNot,
            };
            util::Ptr<Expr> expr;
            Op op;
        };

        struct Binop {
            enum class Op {
                Invalid,
                Assign,
                PlusAssign,
                MinusAssign,
                MultiplyAssign,
                DivideAssign,
                BinaryAndAssign,
                BinaryOrAssign,
                BinaryXorAssign,
                ModAssign,
                ShiftLeftAssign,
                ShiftRightAssign,
                Or,
                And,
                BinaryOr,
                BinaryXor,
                BinaryAnd,
                Equal,
                NotEqual,
                Less,
                LessEqual,
                Greater,
                GreaterEqual,
                ShiftLeft,
                ShiftRight,
                Plus,
                Minus,
                Multiply,
                Divide,
                Mod,
                Access,
                StaticAccess,
            };
            util::Ptr<Expr> lhs;
            util::Ptr<Expr> rhs;
            Op op;
        };

        struct Index {
            util::Ptr<Expr> expr;
            util::Ptr<Expr> index;
        };

        struct Call {
            util::Ptr<Expr> expr;
            util::List<Expr> args;
        };

        struct Cast {
            util::Ptr<Expr> expr;
            Type type;
        };

        struct TryCast {
            util::Ptr<Expr> expr;
            Type type;
        };

        std::variant<Invalid, Literal, Unop, Binop, Index, Call, Cast, TryCast,
            PathSegment>
            val;
    };

    struct Block {
        util::List<Stmt> stmts;
    };

    struct Stmt {
        struct Let {
            std::string_view name;
            std::optional<Type> type;
            std::optional<Expr> expr;
        };

        struct If {
            struct Elif {
                Expr expr;
                Block block;
            };

            Expr expr;
            Block main;
            util::List<Elif> elifs;
            std::optional<Block> else_;
        };

        struct Switch {
            struct Case {
                std::optional<Expr> expr;
                Block block;
            };

            Expr expr;
            util::List<Case> cases;
        };

        struct Loop {
            Block block;
        };

        struct For {
            std::string_view name;
            Expr expr;
            Block block;
        };

        struct While {
            Expr expr;
            Block block;
        };

        struct Return {
            std::optional<Expr> expr;
        };

        struct Defer {
            Expr expr;
        };

        struct Continue { };

        struct Break { };

        std::variant<Invalid, Expr, Let, If, Switch, Loop, For, While, Return,
            Defer, Block, Continue, Break>
            val;
    };

    struct Func {
        struct Param {
            std::string_view name;
            Type type;
        };

        util::List<Param> params;
        Type ret;
        Block block;
    };

    struct Decl {
        struct GenericParam {
            std::string_view name;
            util::List<Type> interfaces;
        };

        struct Struct {
            struct Field {
                std::string_view name;
                Type type;
                bool is_export;
            };

            util::List<Field> fields;
        };

        struct Enum {
            struct Element {
                std::string_view name;
            };

            util::List<Element> elements;
        };

        struct TypeAlias {
            Type type;
        };

        struct Const {
            Type type;
            Expr expr;
        };

        struct Defines {
            std::optional<Type> interface;
            util::List<Decl> decls;
        };

        bool is_export;
        std::string_view name;
        util::List<GenericParam> generic_params;
        std::variant<Invalid, Func, Struct, Enum, TypeAlias, Const, Defines>
            val;
    };

    struct TopLevel {
        std::variant<Import, Decl> val;
    };

    struct Module {
        util::List<TopLevel> top_levels;
    };

    bool operator==(
        [[maybe_unused]] const Invalid& l, [[maybe_unused]] const Invalid& r);
    bool operator==(const PathSegment& l, const PathSegment& r);
    bool operator==([[maybe_unused]] const PathSegment::Root& l,
        [[maybe_unused]] const PathSegment::Root& r);
    bool operator==([[maybe_unused]] const PathSegment::Super& l,
        [[maybe_unused]] const PathSegment::Super& r);
    bool operator==(const PathSegment::Name& l, const PathSegment::Name& r);
    bool operator==(const Import& l, const Import& r);
    bool operator==([[maybe_unused]] const Import::Normal& l,
        [[maybe_unused]] const Import::Normal& r);
    bool operator==([[maybe_unused]] const Import::Glob& l,
        [[maybe_unused]] const Import::Glob& r);
    bool operator==(const Import::Renamed& l, const Import::Renamed& r);
    bool operator==(const Type& l, const Type& r);
    bool operator==([[maybe_unused]] const Type::Unit& l,
        [[maybe_unused]] const Type::Unit& r);
    bool operator==([[maybe_unused]] const Type::String& l,
        [[maybe_unused]] const Type::String& r);
    bool operator==([[maybe_unused]] const Type::Char& l,
        [[maybe_unused]] const Type::Char& r);
    bool operator==([[maybe_unused]] const Type::Int& l,
        [[maybe_unused]] const Type::Int& r);
    bool operator==([[maybe_unused]] const Type::Byte& l,
        [[maybe_unused]] const Type::Byte& r);
    bool operator==([[maybe_unused]] const Type::Float& l,
        [[maybe_unused]] const Type::Float& r);
    bool operator==([[maybe_unused]] const Type::Bool& l,
        [[maybe_unused]] const Type::Bool& r);
    bool operator==(const Type::Array& l, const Type::Array& r);
    bool operator==(const Type::Tup& l, const Type::Tup& r);
    bool operator==(const Type::Func& l, const Type::Func& r);
    bool operator==(const Type::Path& l, const Type::Path& r);
    bool operator==(const Expr& l, const Expr& r);
    bool operator==(const Expr::Literal& l, const Expr::Literal& r);
    bool operator==([[maybe_unused]] const Expr::Literal::Unit& l,
        [[maybe_unused]] const Expr::Literal::Unit& r);
    bool operator==([[maybe_unused]] const Expr::Literal::Null& l,
        [[maybe_unused]] const Expr::Literal::Null& r);
    bool operator==(
        const Expr::Literal::String& l, const Expr::Literal::String& r);
    bool operator==(
        const Expr::Literal::Character& l, const Expr::Literal::Character& r);
    bool operator==(
        const Expr::Literal::Integer& l, const Expr::Literal::Integer& r);
    bool operator==(const Expr::Literal::Byte& l, const Expr::Literal::Byte& r);
    bool operator==(
        const Expr::Literal::Floating& l, const Expr::Literal::Floating& r);
    bool operator==(
        const Expr::Literal::Boolean& l, const Expr::Literal::Boolean& r);
    bool operator==(
        const Expr::Literal::Array& l, const Expr::Literal::Array& r);
    bool operator==(const Expr::Literal::Array::Regular& l,
        const Expr::Literal::Array::Regular& r);
    bool operator==(const Expr::Literal::Array::DefaultNTimes& l,
        const Expr::Literal::Array::DefaultNTimes& r);
    bool operator==(const Expr::Literal::Array::ExprNTimes& l,
        const Expr::Literal::Array::ExprNTimes& r);
    bool operator==(const Expr::Literal::Tup& l, const Expr::Literal::Tup& r);
    bool operator==(const Expr::Unop& l, const Expr::Unop& r);
    bool operator==(const Expr::Binop& l, const Expr::Binop& r);
    bool operator==(const Expr::Index& l, const Expr::Index& r);
    bool operator==(const Expr::Call& l, const Expr::Call& r);
    bool operator==(const Expr::Cast& l, const Expr::Cast& r);
    bool operator==(const Expr::TryCast& l, const Expr::TryCast& r);
    bool operator==(const Block& l, const Block& r);
    bool operator==(const Stmt& l, const Stmt& r);
    bool operator==(const Stmt::Let& l, const Stmt::Let& r);
    bool operator==(const Stmt::If& l, const Stmt::If& r);
    bool operator==(const Stmt::If::Elif& l, const Stmt::If::Elif& r);
    bool operator==(const Stmt::Switch& l, const Stmt::Switch& r);
    bool operator==(const Stmt::Switch::Case& l, const Stmt::Switch::Case& r);
    bool operator==(const Stmt::Loop& l, const Stmt::Loop& r);
    bool operator==(const Stmt::For& l, const Stmt::For& r);
    bool operator==(const Stmt::While& l, const Stmt::While& r);
    bool operator==(const Stmt::Return& l, const Stmt::Return& r);
    bool operator==(const Stmt::Defer& l, const Stmt::Defer& r);
    bool operator==([[maybe_unused]] const Stmt::Continue& l,
        [[maybe_unused]] const Stmt::Continue& r);
    bool operator==([[maybe_unused]] const Stmt::Break& l,
        [[maybe_unused]] const Stmt::Break& r);
    bool operator==(const Func& l, const Func& r);
    bool operator==(const Func::Param& l, const Func::Param& r);
    bool operator==(const Decl& l, const Decl& r);
    bool operator==(const Decl::GenericParam& l, const Decl::GenericParam& r);
    bool operator==(const Decl::Struct& l, const Decl::Struct& r);
    bool operator==(const Decl::Struct::Field& l, const Decl::Struct::Field& r);
    bool operator==(const Decl::Enum& l, const Decl::Enum& r);
    bool operator==(const Decl::Enum::Element& l, const Decl::Enum::Element& r);
    bool operator==(const Decl::TypeAlias& l, const Decl::TypeAlias& r);
    bool operator==(const Decl::Const& l, const Decl::Const& r);
    bool operator==(const Decl::Defines& l, const Decl::Defines& r);
    bool operator==(const TopLevel& l, const TopLevel& r);
    bool operator==(const Module& l, const Module& r);
    bool operator!=(
        [[maybe_unused]] const Invalid& l, [[maybe_unused]] const Invalid& r);
    bool operator!=(const PathSegment& l, const PathSegment& r);
    bool operator!=([[maybe_unused]] const PathSegment::Root& l,
        [[maybe_unused]] const PathSegment::Root& r);
    bool operator!=([[maybe_unused]] const PathSegment::Super& l,
        [[maybe_unused]] const PathSegment::Super& r);
    bool operator!=(const PathSegment::Name& l, const PathSegment::Name& r);
    bool operator!=(const Import& l, const Import& r);
    bool operator!=([[maybe_unused]] const Import::Normal& l,
        [[maybe_unused]] const Import::Normal& r);
    bool operator!=([[maybe_unused]] const Import::Glob& l,
        [[maybe_unused]] const Import::Glob& r);
    bool operator!=(const Import::Renamed& l, const Import::Renamed& r);
    bool operator!=(const Type& l, const Type& r);
    bool operator!=([[maybe_unused]] const Type::Unit& l,
        [[maybe_unused]] const Type::Unit& r);
    bool operator!=([[maybe_unused]] const Type::String& l,
        [[maybe_unused]] const Type::String& r);
    bool operator!=([[maybe_unused]] const Type::Char& l,
        [[maybe_unused]] const Type::Char& r);
    bool operator!=([[maybe_unused]] const Type::Int& l,
        [[maybe_unused]] const Type::Int& r);
    bool operator!=([[maybe_unused]] const Type::Byte& l,
        [[maybe_unused]] const Type::Byte& r);
    bool operator!=([[maybe_unused]] const Type::Float& l,
        [[maybe_unused]] const Type::Float& r);
    bool operator!=([[maybe_unused]] const Type::Bool& l,
        [[maybe_unused]] const Type::Bool& r);
    bool operator!=(const Type::Array& l, const Type::Array& r);
    bool operator!=(const Type::Tup& l, const Type::Tup& r);
    bool operator!=(const Type::Func& l, const Type::Func& r);
    bool operator!=(const Type::Path& l, const Type::Path& r);
    bool operator!=(const Expr& l, const Expr& r);
    bool operator!=(const Expr::Literal& l, const Expr::Literal& r);
    bool operator!=([[maybe_unused]] const Expr::Literal::Unit& l,
        [[maybe_unused]] const Expr::Literal::Unit& r);
    bool operator!=([[maybe_unused]] const Expr::Literal::Null& l,
        [[maybe_unused]] const Expr::Literal::Null& r);
    bool operator!=(
        const Expr::Literal::String& l, const Expr::Literal::String& r);
    bool operator!=(
        const Expr::Literal::Character& l, const Expr::Literal::Character& r);
    bool operator!=(
        const Expr::Literal::Integer& l, const Expr::Literal::Integer& r);
    bool operator!=(const Expr::Literal::Byte& l, const Expr::Literal::Byte& r);
    bool operator!=(
        const Expr::Literal::Floating& l, const Expr::Literal::Floating& r);
    bool operator!=(
        const Expr::Literal::Boolean& l, const Expr::Literal::Boolean& r);
    bool operator!=(
        const Expr::Literal::Array& l, const Expr::Literal::Array& r);
    bool operator!=(const Expr::Literal::Array::Regular& l,
        const Expr::Literal::Array::Regular& r);
    bool operator!=(const Expr::Literal::Array::DefaultNTimes& l,
        const Expr::Literal::Array::DefaultNTimes& r);
    bool operator!=(const Expr::Literal::Array::ExprNTimes& l,
        const Expr::Literal::Array::ExprNTimes& r);
    bool operator!=(const Expr::Literal::Tup& l, const Expr::Literal::Tup& r);
    bool operator!=(const Expr::Unop& l, const Expr::Unop& r);
    bool operator!=(const Expr::Binop& l, const Expr::Binop& r);
    bool operator!=(const Expr::Index& l, const Expr::Index& r);
    bool operator!=(const Expr::Call& l, const Expr::Call& r);
    bool operator!=(const Expr::Cast& l, const Expr::Cast& r);
    bool operator!=(const Expr::TryCast& l, const Expr::TryCast& r);
    bool operator!=(const Block& l, const Block& r);
    bool operator!=(const Stmt& l, const Stmt& r);
    bool operator!=(const Stmt::Let& l, const Stmt::Let& r);
    bool operator!=(const Stmt::If& l, const Stmt::If& r);
    bool operator!=(const Stmt::If::Elif& l, const Stmt::If::Elif& r);
    bool operator!=(const Stmt::Switch& l, const Stmt::Switch& r);
    bool operator!=(const Stmt::Switch::Case& l, const Stmt::Switch::Case& r);
    bool operator!=(const Stmt::Loop& l, const Stmt::Loop& r);
    bool operator!=(const Stmt::For& l, const Stmt::For& r);
    bool operator!=(const Stmt::While& l, const Stmt::While& r);
    bool operator!=(const Stmt::Return& l, const Stmt::Return& r);
    bool operator!=(const Stmt::Defer& l, const Stmt::Defer& r);
    bool operator!=([[maybe_unused]] const Stmt::Continue& l,
        [[maybe_unused]] const Stmt::Continue& r);
    bool operator!=([[maybe_unused]] const Stmt::Break& l,
        [[maybe_unused]] const Stmt::Break& r);
    bool operator!=(const Func& l, const Func& r);
    bool operator!=(const Func::Param& l, const Func::Param& r);
    bool operator!=(const Decl& l, const Decl& r);
    bool operator!=(const Decl::GenericParam& l, const Decl::GenericParam& r);
    bool operator!=(const Decl::Struct& l, const Decl::Struct& r);
    bool operator!=(const Decl::Struct::Field& l, const Decl::Struct::Field& r);
    bool operator!=(const Decl::Enum& l, const Decl::Enum& r);
    bool operator!=(const Decl::Enum::Element& l, const Decl::Enum::Element& r);
    bool operator!=(const Decl::TypeAlias& l, const Decl::TypeAlias& r);
    bool operator!=(const Decl::Const& l, const Decl::Const& r);
    bool operator!=(const Decl::Defines& l, const Decl::Defines& r);
    bool operator!=(const TopLevel& l, const TopLevel& r);
    bool operator!=(const Module& l, const Module& r);

    template<print::PrinterSink Sink>
    class Printer : public print::PrinterBase<Sink, Printer<Sink>> {
    private:
        using Base = print::PrinterBase<Sink, Printer<Sink>>;
        using Base::field;
        using Base::node;
        using Base::node_begin;
        using Base::node_end;

    public:
        Printer(Sink& sink, std::size_t indent_width) :
            Base(sink, indent_width) { }

        void print_node(const Invalid& n);
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
        void print_node(const Decl::GenericParam& n);
        void print_node(const Decl::Struct& n);
        void print_node(const Decl::Struct::Field& n);
        void print_node(const Decl::Enum& n);
        void print_node(const Decl::Enum::Element& n);
        void print_node(const Decl::TypeAlias& n);
        void print_node(const Decl::Const& n);
        void print_node(const Decl::Defines& n);
        void print_node(const TopLevel& n);
        void print_node(const Module& n);
    };

    template<print::PrinterSink Sink>
    void Printer<Sink>::print_node([[maybe_unused]] const Invalid& n) {
        node("Invalid");
    }

    template<print::PrinterSink Sink>
    void Printer<Sink>::print_node(const PathSegment& n) {
        node_begin("PathSegment");
        field("val", n.val);
        node_end();
    }

    template<print::PrinterSink Sink>
    void Printer<Sink>::print_node(
        [[maybe_unused]] const PathSegment::Root& n) {
        node("Root");
    }

    template<print::PrinterSink Sink>
    void Printer<Sink>::print_node(
        [[maybe_unused]] const PathSegment::Super& n) {
        node("Super");
    }

    template<print::PrinterSink Sink>
    void Printer<Sink>::print_node(const PathSegment::Name& n) {
        node_begin("Name");
        field("value", n.value);
        field("generic_params", n.generic_params);
        node_end();
    }

    template<print::PrinterSink Sink>
    void Printer<Sink>::print_node(const Import& n) {
        node_begin("Import");
        field("kind", n.kind);
        field("segments", n.segments);
        node_end();
    }

    template<print::PrinterSink Sink>
    void Printer<Sink>::print_node([[maybe_unused]] const Import::Normal& n) {
        node("Normal");
    }

    template<print::PrinterSink Sink>
    void Printer<Sink>::print_node([[maybe_unused]] const Import::Glob& n) {
        node("Glob");
    }

    template<print::PrinterSink Sink>
    void Printer<Sink>::print_node(const Import::Renamed& n) {
        node_begin("Renamed");
        field("renamed_to", n.renamed_to);
        node_end();
    }

    template<print::PrinterSink Sink>
    void Printer<Sink>::print_node(const Type& n) {
        node_begin("Type");
        field("val", n.val);
        node_end();
    }

    template<print::PrinterSink Sink>
    void Printer<Sink>::print_node([[maybe_unused]] const Type::Unit& n) {
        node("Unit");
    }

    template<print::PrinterSink Sink>
    void Printer<Sink>::print_node([[maybe_unused]] const Type::String& n) {
        node("String");
    }

    template<print::PrinterSink Sink>
    void Printer<Sink>::print_node([[maybe_unused]] const Type::Char& n) {
        node("Char");
    }

    template<print::PrinterSink Sink>
    void Printer<Sink>::print_node([[maybe_unused]] const Type::Int& n) {
        node("Int");
    }

    template<print::PrinterSink Sink>
    void Printer<Sink>::print_node([[maybe_unused]] const Type::Byte& n) {
        node("Byte");
    }

    template<print::PrinterSink Sink>
    void Printer<Sink>::print_node([[maybe_unused]] const Type::Float& n) {
        node("Float");
    }

    template<print::PrinterSink Sink>
    void Printer<Sink>::print_node([[maybe_unused]] const Type::Bool& n) {
        node("Bool");
    }

    template<print::PrinterSink Sink>
    void Printer<Sink>::print_node(const Type::Array& n) {
        node_begin("Array");
        field("type", n.type);
        node_end();
    }

    template<print::PrinterSink Sink>
    void Printer<Sink>::print_node(const Type::Tup& n) {
        node_begin("Tup");
        field("types", n.types);
        node_end();
    }

    template<print::PrinterSink Sink>
    void Printer<Sink>::print_node(const Type::Func& n) {
        node_begin("Func");
        field("params", n.params);
        field("return_type", n.return_type);
        node_end();
    }

    template<print::PrinterSink Sink>
    void Printer<Sink>::print_node(const Type::Path& n) {
        node_begin("Path");
        field("segments", n.segments);
        node_end();
    }

    template<print::PrinterSink Sink>
    void Printer<Sink>::print_node(const Expr& n) {
        node_begin("Expr");
        field("val", n.val);
        node_end();
    }

    template<print::PrinterSink Sink>
    void Printer<Sink>::print_node(const Expr::Literal& n) {
        node_begin("Literal");
        field("val", n.val);
        node_end();
    }

    template<print::PrinterSink Sink>
    void Printer<Sink>::print_node(
        [[maybe_unused]] const Expr::Literal::Unit& n) {
        node("Unit");
    }

    template<print::PrinterSink Sink>
    void Printer<Sink>::print_node(
        [[maybe_unused]] const Expr::Literal::Null& n) {
        node("Null");
    }

    template<print::PrinterSink Sink>
    void Printer<Sink>::print_node(const Expr::Literal::String& n) {
        node_begin("String");
        field("value", n.value);
        node_end();
    }

    template<print::PrinterSink Sink>
    void Printer<Sink>::print_node(const Expr::Literal::Character& n) {
        node_begin("Character");
        field("value", n.value);
        node_end();
    }

    template<print::PrinterSink Sink>
    void Printer<Sink>::print_node(const Expr::Literal::Integer& n) {
        node_begin("Integer");
        field("value", n.value);
        node_end();
    }

    template<print::PrinterSink Sink>
    void Printer<Sink>::print_node(const Expr::Literal::Byte& n) {
        node_begin("Byte");
        field("value", n.value);
        node_end();
    }

    template<print::PrinterSink Sink>
    void Printer<Sink>::print_node(const Expr::Literal::Floating& n) {
        node_begin("Floating");
        field("value", n.value);
        node_end();
    }

    template<print::PrinterSink Sink>
    void Printer<Sink>::print_node(const Expr::Literal::Boolean& n) {
        node_begin("Boolean");
        field("value", n.value);
        node_end();
    }

    template<print::PrinterSink Sink>
    void Printer<Sink>::print_node(const Expr::Literal::Array& n) {
        node_begin("Array");
        field("val", n.val);
        node_end();
    }

    template<print::PrinterSink Sink>
    void Printer<Sink>::print_node(const Expr::Literal::Array::Regular& n) {
        node_begin("Regular");
        field("elements", n.elements);
        node_end();
    }

    template<print::PrinterSink Sink>
    void Printer<Sink>::print_node(
        const Expr::Literal::Array::DefaultNTimes& n) {
        node_begin("DefaultNTimes");
        field("times", n.times);
        node_end();
    }

    template<print::PrinterSink Sink>
    void Printer<Sink>::print_node(const Expr::Literal::Array::ExprNTimes& n) {
        node_begin("ExprNTimes");
        field("expr", n.expr);
        field("times", n.times);
        node_end();
    }

    template<print::PrinterSink Sink>
    void Printer<Sink>::print_node(const Expr::Literal::Tup& n) {
        node_begin("Tup");
        field("exprs", n.exprs);
        node_end();
    }

    template<print::PrinterSink Sink>
    void Printer<Sink>::print_node(const Expr::Unop& n) {
        node_begin("Unop");
        field("expr", n.expr);
        field("op", n.op);
        node_end();
    }

    template<print::PrinterSink Sink>
    void Printer<Sink>::print_node(const Expr::Unop::Op& n) {
        switch (n) {
        case Expr::Unop::Op::Invalid:
            node("Invalid");
            break;
        case Expr::Unop::Op::Plus:
            node("Plus");
            break;
        case Expr::Unop::Op::Minus:
            node("Minus");
            break;
        case Expr::Unop::Op::Not:
            node("Not");
            break;
        case Expr::Unop::Op::BinaryNot:
            node("BinaryNot");
            break;
        }
    }

    template<print::PrinterSink Sink>
    void Printer<Sink>::print_node(const Expr::Binop& n) {
        node_begin("Binop");
        field("lhs", n.lhs);
        field("rhs", n.rhs);
        field("op", n.op);
        node_end();
    }

    template<print::PrinterSink Sink>
    void Printer<Sink>::print_node(const Expr::Binop::Op& n) {
        switch (n) {
        case Expr::Binop::Op::Invalid:
            node("Invalid");
            break;
        case Expr::Binop::Op::Assign:
            node("Assign");
            break;
        case Expr::Binop::Op::PlusAssign:
            node("PlusAssign");
            break;
        case Expr::Binop::Op::MinusAssign:
            node("MinusAssign");
            break;
        case Expr::Binop::Op::MultiplyAssign:
            node("MultiplyAssign");
            break;
        case Expr::Binop::Op::DivideAssign:
            node("DivideAssign");
            break;
        case Expr::Binop::Op::BinaryAndAssign:
            node("BinaryAndAssign");
            break;
        case Expr::Binop::Op::BinaryOrAssign:
            node("BinaryOrAssign");
            break;
        case Expr::Binop::Op::BinaryXorAssign:
            node("BinaryXorAssign");
            break;
        case Expr::Binop::Op::ModAssign:
            node("ModAssign");
            break;
        case Expr::Binop::Op::ShiftLeftAssign:
            node("ShiftLeftAssign");
            break;
        case Expr::Binop::Op::ShiftRightAssign:
            node("ShiftRightAssign");
            break;
        case Expr::Binop::Op::Or:
            node("Or");
            break;
        case Expr::Binop::Op::And:
            node("And");
            break;
        case Expr::Binop::Op::BinaryOr:
            node("BinaryOr");
            break;
        case Expr::Binop::Op::BinaryXor:
            node("BinaryXor");
            break;
        case Expr::Binop::Op::BinaryAnd:
            node("BinaryAnd");
            break;
        case Expr::Binop::Op::Equal:
            node("Equal");
            break;
        case Expr::Binop::Op::NotEqual:
            node("NotEqual");
            break;
        case Expr::Binop::Op::Less:
            node("Less");
            break;
        case Expr::Binop::Op::LessEqual:
            node("LessEqual");
            break;
        case Expr::Binop::Op::Greater:
            node("Greater");
            break;
        case Expr::Binop::Op::GreaterEqual:
            node("GreaterEqual");
            break;
        case Expr::Binop::Op::ShiftLeft:
            node("ShiftLeft");
            break;
        case Expr::Binop::Op::ShiftRight:
            node("ShiftRight");
            break;
        case Expr::Binop::Op::Plus:
            node("Plus");
            break;
        case Expr::Binop::Op::Minus:
            node("Minus");
            break;
        case Expr::Binop::Op::Multiply:
            node("Multiply");
            break;
        case Expr::Binop::Op::Divide:
            node("Divide");
            break;
        case Expr::Binop::Op::Mod:
            node("Mod");
            break;
        case Expr::Binop::Op::Access:
            node("Access");
            break;
        case Expr::Binop::Op::StaticAccess:
            node("StaticAccess");
            break;
        }
    }

    template<print::PrinterSink Sink>
    void Printer<Sink>::print_node(const Expr::Index& n) {
        node_begin("Index");
        field("expr", n.expr);
        field("index", n.index);
        node_end();
    }

    template<print::PrinterSink Sink>
    void Printer<Sink>::print_node(const Expr::Call& n) {
        node_begin("Call");
        field("expr", n.expr);
        field("args", n.args);
        node_end();
    }

    template<print::PrinterSink Sink>
    void Printer<Sink>::print_node(const Expr::Cast& n) {
        node_begin("Cast");
        field("expr", n.expr);
        field("type", n.type);
        node_end();
    }

    template<print::PrinterSink Sink>
    void Printer<Sink>::print_node(const Expr::TryCast& n) {
        node_begin("TryCast");
        field("expr", n.expr);
        field("type", n.type);
        node_end();
    }

    template<print::PrinterSink Sink>
    void Printer<Sink>::print_node(const Block& n) {
        node_begin("Block");
        field("stmts", n.stmts);
        node_end();
    }

    template<print::PrinterSink Sink>
    void Printer<Sink>::print_node(const Stmt& n) {
        node_begin("Stmt");
        field("val", n.val);
        node_end();
    }

    template<print::PrinterSink Sink>
    void Printer<Sink>::print_node(const Stmt::Let& n) {
        node_begin("Let");
        field("name", n.name);
        field("type", n.type);
        field("expr", n.expr);
        node_end();
    }

    template<print::PrinterSink Sink>
    void Printer<Sink>::print_node(const Stmt::If& n) {
        node_begin("If");
        field("expr", n.expr);
        field("main", n.main);
        field("elifs", n.elifs);
        field("else_", n.else_);
        node_end();
    }

    template<print::PrinterSink Sink>
    void Printer<Sink>::print_node(const Stmt::If::Elif& n) {
        node_begin("Elif");
        field("expr", n.expr);
        field("block", n.block);
        node_end();
    }

    template<print::PrinterSink Sink>
    void Printer<Sink>::print_node(const Stmt::Switch& n) {
        node_begin("Switch");
        field("expr", n.expr);
        field("cases", n.cases);
        node_end();
    }

    template<print::PrinterSink Sink>
    void Printer<Sink>::print_node(const Stmt::Switch::Case& n) {
        node_begin("Case");
        field("expr", n.expr);
        field("block", n.block);
        node_end();
    }

    template<print::PrinterSink Sink>
    void Printer<Sink>::print_node(const Stmt::Loop& n) {
        node_begin("Loop");
        field("block", n.block);
        node_end();
    }

    template<print::PrinterSink Sink>
    void Printer<Sink>::print_node(const Stmt::For& n) {
        node_begin("For");
        field("name", n.name);
        field("expr", n.expr);
        field("block", n.block);
        node_end();
    }

    template<print::PrinterSink Sink>
    void Printer<Sink>::print_node(const Stmt::While& n) {
        node_begin("While");
        field("expr", n.expr);
        field("block", n.block);
        node_end();
    }

    template<print::PrinterSink Sink>
    void Printer<Sink>::print_node(const Stmt::Return& n) {
        node_begin("Return");
        field("expr", n.expr);
        node_end();
    }

    template<print::PrinterSink Sink>
    void Printer<Sink>::print_node(const Stmt::Defer& n) {
        node_begin("Defer");
        field("expr", n.expr);
        node_end();
    }

    template<print::PrinterSink Sink>
    void Printer<Sink>::print_node([[maybe_unused]] const Stmt::Continue& n) {
        node("Continue");
    }

    template<print::PrinterSink Sink>
    void Printer<Sink>::print_node([[maybe_unused]] const Stmt::Break& n) {
        node("Break");
    }

    template<print::PrinterSink Sink>
    void Printer<Sink>::print_node(const Func& n) {
        node_begin("Func");
        field("params", n.params);
        field("ret", n.ret);
        field("block", n.block);
        node_end();
    }

    template<print::PrinterSink Sink>
    void Printer<Sink>::print_node(const Func::Param& n) {
        node_begin("Param");
        field("name", n.name);
        field("type", n.type);
        node_end();
    }

    template<print::PrinterSink Sink>
    void Printer<Sink>::print_node(const Decl& n) {
        node_begin("Decl");
        field("is_export", n.is_export);
        field("name", n.name);
        field("generic_params", n.generic_params);
        field("val", n.val);
        node_end();
    }

    template<print::PrinterSink Sink>
    void Printer<Sink>::print_node(const Decl::GenericParam& n) {
        node_begin("GenericParam");
        field("name", n.name);
        field("interfaces", n.interfaces);
        node_end();
    }

    template<print::PrinterSink Sink>
    void Printer<Sink>::print_node(const Decl::Struct& n) {
        node_begin("Struct");
        field("fields", n.fields);
        node_end();
    }

    template<print::PrinterSink Sink>
    void Printer<Sink>::print_node(const Decl::Struct::Field& n) {
        node_begin("Field");
        field("name", n.name);
        field("type", n.type);
        field("is_export", n.is_export);
        node_end();
    }

    template<print::PrinterSink Sink>
    void Printer<Sink>::print_node(const Decl::Enum& n) {
        node_begin("Enum");
        field("elements", n.elements);
        node_end();
    }

    template<print::PrinterSink Sink>
    void Printer<Sink>::print_node(const Decl::Enum::Element& n) {
        node_begin("Element");
        field("name", n.name);
        node_end();
    }

    template<print::PrinterSink Sink>
    void Printer<Sink>::print_node(const Decl::TypeAlias& n) {
        node_begin("TypeAlias");
        field("type", n.type);
        node_end();
    }

    template<print::PrinterSink Sink>
    void Printer<Sink>::print_node(const Decl::Const& n) {
        node_begin("Const");
        field("type", n.type);
        field("expr", n.expr);
        node_end();
    }

    template<print::PrinterSink Sink>
    void Printer<Sink>::print_node(const Decl::Defines& n) {
        node_begin("Defines");
        field("interface", n.interface);
        field("decls", n.decls);
        node_end();
    }

    template<print::PrinterSink Sink>
    void Printer<Sink>::print_node(const TopLevel& n) {
        node_begin("TopLevel");
        field("val", n.val);
        node_end();
    }

    template<print::PrinterSink Sink>
    void Printer<Sink>::print_node(const Module& n) {
        node_begin("Module");
        field("top_levels", n.top_levels);
        node_end();
    }

}
