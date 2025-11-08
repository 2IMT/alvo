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

            Name(const std::string_view& value,
                const util::List<Type>& generic_params) :
                value(value),
                generic_params(generic_params) { }
        };

        using Val = std::variant<Invalid, Root, Super, Name>;
        Val val;

        PathSegment(const Val& val) :
            val(val) { }
    };

    struct Import {
        struct Normal { };

        struct Glob { };

        struct Renamed {
            std::string_view renamed_to;

            Renamed(const std::string_view& renamed_to) :
                renamed_to(renamed_to) { }
        };

        using Kind = std::variant<Invalid, Normal, Glob, Renamed>;
        Kind kind;
        util::List<PathSegment> segments;

        Import(const Kind& kind, const util::List<PathSegment>& segments) :
            kind(kind),
            segments(segments) { }
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
            bool is_invalid;
            util::Ptr<Type> type;

            Array(const bool& is_invalid, const util::Ptr<Type>& type) :
                is_invalid(is_invalid),
                type(type) { }
        };

        struct Tup {
            bool is_invalid;
            util::List<Type> types;

            Tup(const bool& is_invalid, const util::List<Type>& types) :
                is_invalid(is_invalid),
                types(types) { }
        };

        struct Func {
            bool is_invalid;
            util::List<Type> params;
            util::Ptr<Type> return_type;

            Func(const bool& is_invalid, const util::List<Type>& params,
                const util::Ptr<Type>& return_type) :
                is_invalid(is_invalid),
                params(params),
                return_type(return_type) { }
        };

        struct Path {
            bool is_invalid;
            util::List<PathSegment> segments;

            Path(const bool& is_invalid,
                const util::List<PathSegment>& segments) :
                is_invalid(is_invalid),
                segments(segments) { }
        };

        struct Ref {
            bool is_invalid;
            util::Ptr<Type> type;

            Ref(const bool& is_invalid, const util::Ptr<Type>& type) :
                is_invalid(is_invalid),
                type(type) { }
        };

        using Val = std::variant<Invalid, Unit, String, Char, Int, Byte, Float,
            Bool, Array, Tup, Func, Path, Ref>;
        Val val;
        bool nullable;

        Type(const Val& val, const bool& nullable) :
            val(val),
            nullable(nullable) { }
    };

    struct Expr {
        struct Literal {
            struct Unit { };

            struct Null { };

            struct String {
                std::string_view value;

                String(const std::string_view& value) :
                    value(value) { }
            };

            struct Character {
                std::string_view value;

                Character(const std::string_view& value) :
                    value(value) { }
            };

            struct Integer {
                std::string_view value;

                Integer(const std::string_view& value) :
                    value(value) { }
            };

            struct Byte {
                std::string_view value;

                Byte(const std::string_view& value) :
                    value(value) { }
            };

            struct Floating {
                std::string_view value;

                Floating(const std::string_view& value) :
                    value(value) { }
            };

            struct Boolean {
                std::string_view value;

                Boolean(const std::string_view& value) :
                    value(value) { }
            };

            struct Array {
                struct Regular {
                    util::List<Expr> elements;

                    Regular(const util::List<Expr>& elements) :
                        elements(elements) { }
                };

                struct DefaultNTimes {
                    util::Ptr<Expr> times;

                    DefaultNTimes(const util::Ptr<Expr>& times) :
                        times(times) { }
                };

                struct ExprNTimes {
                    util::Ptr<Expr> expr;
                    util::Ptr<Expr> times;

                    ExprNTimes(const util::Ptr<Expr>& expr,
                        const util::Ptr<Expr>& times) :
                        expr(expr),
                        times(times) { }
                };

                using Val =
                    std::variant<Invalid, Regular, DefaultNTimes, ExprNTimes>;
                Val val;

                Array(const Val& val) :
                    val(val) { }
            };

            struct Tup {
                bool is_invalid;
                util::List<Expr> exprs;

                Tup(const bool& is_invalid, const util::List<Expr>& exprs) :
                    is_invalid(is_invalid),
                    exprs(exprs) { }
            };

            struct Struct {
                struct Field {
                    bool is_invalid;
                    std::string_view name;
                    util::Ptr<Expr> expr;

                    Field(const bool& is_invalid, const std::string_view& name,
                        const util::Ptr<Expr>& expr) :
                        is_invalid(is_invalid),
                        name(name),
                        expr(expr) { }
                };

                bool is_invalid;
                Type type;
                util::List<Field> fields;

                Struct(const bool& is_invalid, const Type& type,
                    const util::List<Field>& fields) :
                    is_invalid(is_invalid),
                    type(type),
                    fields(fields) { }
            };

            using Val = std::variant<Invalid, Unit, Null, String, Character,
                Integer, Byte, Floating, Boolean, Array, Tup, Struct,
                util::Ptr<Func>>;
            Val val;

            Literal(const Val& val) :
                val(val) { }
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

            Unop(const util::Ptr<Expr>& expr, const Op& op) :
                expr(expr),
                op(op) { }
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

            Binop(const util::Ptr<Expr>& lhs, const util::Ptr<Expr>& rhs,
                const Op& op) :
                lhs(lhs),
                rhs(rhs),
                op(op) { }
        };

        struct Index {
            util::Ptr<Expr> expr;
            util::Ptr<Expr> index;

            Index(const util::Ptr<Expr>& expr, const util::Ptr<Expr>& index) :
                expr(expr),
                index(index) { }
        };

        struct Call {
            util::Ptr<Expr> expr;
            util::List<Expr> args;

            Call(const util::Ptr<Expr>& expr, const util::List<Expr>& args) :
                expr(expr),
                args(args) { }
        };

        struct Cast {
            util::Ptr<Expr> expr;
            Type type;

            Cast(const util::Ptr<Expr>& expr, const Type& type) :
                expr(expr),
                type(type) { }
        };

        struct TryCast {
            util::Ptr<Expr> expr;
            Type type;

            TryCast(const util::Ptr<Expr>& expr, const Type& type) :
                expr(expr),
                type(type) { }
        };

        struct Ref {
            bool is_invalid;
            util::Ptr<Expr> expr;

            Ref(const bool& is_invalid, const util::Ptr<Expr>& expr) :
                is_invalid(is_invalid),
                expr(expr) { }
        };

        struct Builtin {
            bool is_invalid;
            std::string_view name;
            util::List<Type> generic_params;
            util::List<Expr> args;

            Builtin(const bool& is_invalid, const std::string_view& name,
                const util::List<Type>& generic_params,
                const util::List<Expr>& args) :
                is_invalid(is_invalid),
                name(name),
                generic_params(generic_params),
                args(args) { }
        };

        using Val = std::variant<Invalid, Literal, Unop, Binop, Index, Call,
            Cast, TryCast, Ref, Builtin, PathSegment>;
        Val val;

        Expr(const Val& val) :
            val(val) { }
    };

    struct Block {
        bool is_invalid;
        util::List<Stmt> stmts;

        Block(const bool& is_invalid, const util::List<Stmt>& stmts) :
            is_invalid(is_invalid),
            stmts(stmts) { }
    };

    struct Stmt {
        struct Let {
            bool is_invalid;
            std::string_view name;
            std::optional<Type> type;
            std::optional<Expr> expr;

            Let(const bool& is_invalid, const std::string_view& name,
                const std::optional<Type>& type,
                const std::optional<Expr>& expr) :
                is_invalid(is_invalid),
                name(name),
                type(type),
                expr(expr) { }
        };

        struct If {
            struct Elif {
                Expr expr;
                Block block;

                Elif(const Expr& expr, const Block& block) :
                    expr(expr),
                    block(block) { }
            };

            bool is_invalid;
            Expr expr;
            Block main;
            util::List<Elif> elifs;
            std::optional<Block> else_;

            If(const bool& is_invalid, const Expr& expr, const Block& main,
                const util::List<Elif>& elifs,
                const std::optional<Block>& else_) :
                is_invalid(is_invalid),
                expr(expr),
                main(main),
                elifs(elifs),
                else_(else_) { }
        };

        struct Switch {
            struct Case {
                bool is_invalid;
                std::optional<Expr> expr;
                Block block;

                Case(const bool& is_invalid, const std::optional<Expr>& expr,
                    const Block& block) :
                    is_invalid(is_invalid),
                    expr(expr),
                    block(block) { }
            };

            bool is_invalid;
            Expr expr;
            util::List<Case> cases;

            Switch(const bool& is_invalid, const Expr& expr,
                const util::List<Case>& cases) :
                is_invalid(is_invalid),
                expr(expr),
                cases(cases) { }
        };

        struct Loop {
            bool is_invalid;
            Block block;

            Loop(const bool& is_invalid, const Block& block) :
                is_invalid(is_invalid),
                block(block) { }
        };

        struct For {
            bool is_invalid;
            std::string_view name;
            Expr expr;
            Block block;

            For(const bool& is_invalid, const std::string_view& name,
                const Expr& expr, const Block& block) :
                is_invalid(is_invalid),
                name(name),
                expr(expr),
                block(block) { }
        };

        struct While {
            bool is_invalid;
            Expr expr;
            Block block;

            While(
                const bool& is_invalid, const Expr& expr, const Block& block) :
                is_invalid(is_invalid),
                expr(expr),
                block(block) { }
        };

        struct Return {
            bool is_invalid;
            std::optional<Expr> expr;

            Return(const bool& is_invalid, const std::optional<Expr>& expr) :
                is_invalid(is_invalid),
                expr(expr) { }
        };

        struct Defer {
            bool is_invalid;
            Expr expr;

            Defer(const bool& is_invalid, const Expr& expr) :
                is_invalid(is_invalid),
                expr(expr) { }
        };

        struct Continue { };

        struct Break { };

        using Val = std::variant<Invalid, Expr, Let, If, Switch, Loop, For,
            While, Return, Defer, Block, Continue, Break>;
        Val val;

        Stmt(const Val& val) :
            val(val) { }
    };

    struct Func {
        struct Signature {
            struct Param {
                bool is_invalid;
                std::string_view name;
                Type type;

                Param(const bool& is_invalid, const std::string_view& name,
                    const Type& type) :
                    is_invalid(is_invalid),
                    name(name),
                    type(type) { }
            };

            bool is_invalid;
            bool is_self_func;
            util::List<Param> params;
            Type ret;

            Signature(const bool& is_invalid, const bool& is_self_func,
                const util::List<Param>& params, const Type& ret) :
                is_invalid(is_invalid),
                is_self_func(is_self_func),
                params(params),
                ret(ret) { }
        };

        bool is_invalid;
        Signature signature;
        Block block;

        Func(const bool& is_invalid, const Signature& signature,
            const Block& block) :
            is_invalid(is_invalid),
            signature(signature),
            block(block) { }
    };

    struct Decl {
        struct GenericParam {
            bool is_invalid;
            std::string_view name;
            util::List<Type> interfaces;

            GenericParam(const bool& is_invalid, const std::string_view& name,
                const util::List<Type>& interfaces) :
                is_invalid(is_invalid),
                name(name),
                interfaces(interfaces) { }
        };

        struct Struct {
            struct Field {
                bool is_invalid;
                std::string_view name;
                Type type;
                bool is_export;

                Field(const bool& is_invalid, const std::string_view& name,
                    const Type& type, const bool& is_export) :
                    is_invalid(is_invalid),
                    name(name),
                    type(type),
                    is_export(is_export) { }
            };

            bool is_invalid;
            util::List<Field> fields;

            Struct(const bool& is_invalid, const util::List<Field>& fields) :
                is_invalid(is_invalid),
                fields(fields) { }
        };

        struct Enum {
            struct Element {
                bool is_invalid;
                std::string_view name;

                Element(const bool& is_invalid, const std::string_view& name) :
                    is_invalid(is_invalid),
                    name(name) { }
            };

            bool is_invalid;
            util::List<Element> elements;

            Enum(const bool& is_invalid, const util::List<Element>& elements) :
                is_invalid(is_invalid),
                elements(elements) { }
        };

        struct TypeAlias {
            bool is_invalid;
            Type type;

            TypeAlias(const bool& is_invalid, const Type& type) :
                is_invalid(is_invalid),
                type(type) { }
        };

        struct Const {
            bool is_invalid;
            Type type;
            Expr expr;

            Const(const bool& is_invalid, const Type& type, const Expr& expr) :
                is_invalid(is_invalid),
                type(type),
                expr(expr) { }
        };

        struct Decls {
            bool is_invalid;
            Type type;
            std::optional<Type> interface;
            util::List<Decl> decls;

            Decls(const bool& is_invalid, const Type& type,
                const std::optional<Type>& interface,
                const util::List<Decl>& decls) :
                is_invalid(is_invalid),
                type(type),
                interface(interface),
                decls(decls) { }
        };

        struct Interface {
            struct Member {
                bool is_invalid;
                std::string_view name;
                util::List<GenericParam> generic_params;
                Func::Signature signature;

                Member(const bool& is_invalid, const std::string_view& name,
                    const util::List<GenericParam>& generic_params,
                    const Func::Signature& signature) :
                    is_invalid(is_invalid),
                    name(name),
                    generic_params(generic_params),
                    signature(signature) { }
            };

            bool is_invalid;
            util::List<Member> members;

            Interface(
                const bool& is_invalid, const util::List<Member>& members) :
                is_invalid(is_invalid),
                members(members) { }
        };

        using Val = std::variant<Invalid, Func, Struct, Enum, TypeAlias, Const,
            Decls, Interface>;
        bool is_export;
        bool is_decls_block;
        std::string_view name;
        util::List<GenericParam> generic_params;
        Val val;

        Decl(const bool& is_export, const bool& is_decls_block,
            const std::string_view& name,
            const util::List<GenericParam>& generic_params, const Val& val) :
            is_export(is_export),
            is_decls_block(is_decls_block),
            name(name),
            generic_params(generic_params),
            val(val) { }
    };

    struct TopLevel {
        using Val = std::variant<Invalid, Import, Decl>;
        Val val;

        TopLevel(const Val& val) :
            val(val) { }
    };

    struct Module {
        util::List<TopLevel> top_levels;

        Module(const util::List<TopLevel>& top_levels) :
            top_levels(top_levels) { }
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
    bool operator==(const Type::Ref& l, const Type::Ref& r);
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
    bool operator==(
        const Expr::Literal::Struct& l, const Expr::Literal::Struct& r);
    bool operator==(const Expr::Literal::Struct::Field& l,
        const Expr::Literal::Struct::Field& r);
    bool operator==(const Expr::Unop& l, const Expr::Unop& r);
    bool operator==(const Expr::Binop& l, const Expr::Binop& r);
    bool operator==(const Expr::Index& l, const Expr::Index& r);
    bool operator==(const Expr::Call& l, const Expr::Call& r);
    bool operator==(const Expr::Cast& l, const Expr::Cast& r);
    bool operator==(const Expr::TryCast& l, const Expr::TryCast& r);
    bool operator==(const Expr::Ref& l, const Expr::Ref& r);
    bool operator==(const Expr::Builtin& l, const Expr::Builtin& r);
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
    bool operator==(const Func::Signature& l, const Func::Signature& r);
    bool operator==(
        const Func::Signature::Param& l, const Func::Signature::Param& r);
    bool operator==(const Decl& l, const Decl& r);
    bool operator==(const Decl::GenericParam& l, const Decl::GenericParam& r);
    bool operator==(const Decl::Struct& l, const Decl::Struct& r);
    bool operator==(const Decl::Struct::Field& l, const Decl::Struct::Field& r);
    bool operator==(const Decl::Enum& l, const Decl::Enum& r);
    bool operator==(const Decl::Enum::Element& l, const Decl::Enum::Element& r);
    bool operator==(const Decl::TypeAlias& l, const Decl::TypeAlias& r);
    bool operator==(const Decl::Const& l, const Decl::Const& r);
    bool operator==(const Decl::Decls& l, const Decl::Decls& r);
    bool operator==(const Decl::Interface& l, const Decl::Interface& r);
    bool operator==(
        const Decl::Interface::Member& l, const Decl::Interface::Member& r);
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
    bool operator!=(const Type::Ref& l, const Type::Ref& r);
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
    bool operator!=(
        const Expr::Literal::Struct& l, const Expr::Literal::Struct& r);
    bool operator!=(const Expr::Literal::Struct::Field& l,
        const Expr::Literal::Struct::Field& r);
    bool operator!=(const Expr::Unop& l, const Expr::Unop& r);
    bool operator!=(const Expr::Binop& l, const Expr::Binop& r);
    bool operator!=(const Expr::Index& l, const Expr::Index& r);
    bool operator!=(const Expr::Call& l, const Expr::Call& r);
    bool operator!=(const Expr::Cast& l, const Expr::Cast& r);
    bool operator!=(const Expr::TryCast& l, const Expr::TryCast& r);
    bool operator!=(const Expr::Ref& l, const Expr::Ref& r);
    bool operator!=(const Expr::Builtin& l, const Expr::Builtin& r);
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
    bool operator!=(const Func::Signature& l, const Func::Signature& r);
    bool operator!=(
        const Func::Signature::Param& l, const Func::Signature::Param& r);
    bool operator!=(const Decl& l, const Decl& r);
    bool operator!=(const Decl::GenericParam& l, const Decl::GenericParam& r);
    bool operator!=(const Decl::Struct& l, const Decl::Struct& r);
    bool operator!=(const Decl::Struct::Field& l, const Decl::Struct::Field& r);
    bool operator!=(const Decl::Enum& l, const Decl::Enum& r);
    bool operator!=(const Decl::Enum::Element& l, const Decl::Enum::Element& r);
    bool operator!=(const Decl::TypeAlias& l, const Decl::TypeAlias& r);
    bool operator!=(const Decl::Const& l, const Decl::Const& r);
    bool operator!=(const Decl::Decls& l, const Decl::Decls& r);
    bool operator!=(const Decl::Interface& l, const Decl::Interface& r);
    bool operator!=(
        const Decl::Interface::Member& l, const Decl::Interface::Member& r);
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
        void print_node(const Type::Ref& n);
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
        void print_node(const Expr::Literal::Struct& n);
        void print_node(const Expr::Literal::Struct::Field& n);
        void print_node(const Expr::Unop& n);
        void print_node(const Expr::Unop::Op& n);
        void print_node(const Expr::Binop& n);
        void print_node(const Expr::Binop::Op& n);
        void print_node(const Expr::Index& n);
        void print_node(const Expr::Call& n);
        void print_node(const Expr::Cast& n);
        void print_node(const Expr::TryCast& n);
        void print_node(const Expr::Ref& n);
        void print_node(const Expr::Builtin& n);
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
        void print_node(const Func::Signature& n);
        void print_node(const Func::Signature::Param& n);
        void print_node(const Decl& n);
        void print_node(const Decl::GenericParam& n);
        void print_node(const Decl::Struct& n);
        void print_node(const Decl::Struct::Field& n);
        void print_node(const Decl::Enum& n);
        void print_node(const Decl::Enum::Element& n);
        void print_node(const Decl::TypeAlias& n);
        void print_node(const Decl::Const& n);
        void print_node(const Decl::Decls& n);
        void print_node(const Decl::Interface& n);
        void print_node(const Decl::Interface::Member& n);
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
        field("nullable", n.nullable);
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
        field("is_invalid", n.is_invalid);
        field("type", n.type);
        node_end();
    }

    template<print::PrinterSink Sink>
    void Printer<Sink>::print_node(const Type::Tup& n) {
        node_begin("Tup");
        field("is_invalid", n.is_invalid);
        field("types", n.types);
        node_end();
    }

    template<print::PrinterSink Sink>
    void Printer<Sink>::print_node(const Type::Func& n) {
        node_begin("Func");
        field("is_invalid", n.is_invalid);
        field("params", n.params);
        field("return_type", n.return_type);
        node_end();
    }

    template<print::PrinterSink Sink>
    void Printer<Sink>::print_node(const Type::Path& n) {
        node_begin("Path");
        field("is_invalid", n.is_invalid);
        field("segments", n.segments);
        node_end();
    }

    template<print::PrinterSink Sink>
    void Printer<Sink>::print_node(const Type::Ref& n) {
        node_begin("Ref");
        field("is_invalid", n.is_invalid);
        field("type", n.type);
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
        field("is_invalid", n.is_invalid);
        field("exprs", n.exprs);
        node_end();
    }

    template<print::PrinterSink Sink>
    void Printer<Sink>::print_node(const Expr::Literal::Struct& n) {
        node_begin("Struct");
        field("is_invalid", n.is_invalid);
        field("type", n.type);
        field("fields", n.fields);
        node_end();
    }

    template<print::PrinterSink Sink>
    void Printer<Sink>::print_node(const Expr::Literal::Struct::Field& n) {
        node_begin("Field");
        field("is_invalid", n.is_invalid);
        field("name", n.name);
        field("expr", n.expr);
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
    void Printer<Sink>::print_node(const Expr::Ref& n) {
        node_begin("Ref");
        field("is_invalid", n.is_invalid);
        field("expr", n.expr);
        node_end();
    }

    template<print::PrinterSink Sink>
    void Printer<Sink>::print_node(const Expr::Builtin& n) {
        node_begin("Builtin");
        field("is_invalid", n.is_invalid);
        field("name", n.name);
        field("generic_params", n.generic_params);
        field("args", n.args);
        node_end();
    }

    template<print::PrinterSink Sink>
    void Printer<Sink>::print_node(const Block& n) {
        node_begin("Block");
        field("is_invalid", n.is_invalid);
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
        field("is_invalid", n.is_invalid);
        field("name", n.name);
        field("type", n.type);
        field("expr", n.expr);
        node_end();
    }

    template<print::PrinterSink Sink>
    void Printer<Sink>::print_node(const Stmt::If& n) {
        node_begin("If");
        field("is_invalid", n.is_invalid);
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
        field("is_invalid", n.is_invalid);
        field("expr", n.expr);
        field("cases", n.cases);
        node_end();
    }

    template<print::PrinterSink Sink>
    void Printer<Sink>::print_node(const Stmt::Switch::Case& n) {
        node_begin("Case");
        field("is_invalid", n.is_invalid);
        field("expr", n.expr);
        field("block", n.block);
        node_end();
    }

    template<print::PrinterSink Sink>
    void Printer<Sink>::print_node(const Stmt::Loop& n) {
        node_begin("Loop");
        field("is_invalid", n.is_invalid);
        field("block", n.block);
        node_end();
    }

    template<print::PrinterSink Sink>
    void Printer<Sink>::print_node(const Stmt::For& n) {
        node_begin("For");
        field("is_invalid", n.is_invalid);
        field("name", n.name);
        field("expr", n.expr);
        field("block", n.block);
        node_end();
    }

    template<print::PrinterSink Sink>
    void Printer<Sink>::print_node(const Stmt::While& n) {
        node_begin("While");
        field("is_invalid", n.is_invalid);
        field("expr", n.expr);
        field("block", n.block);
        node_end();
    }

    template<print::PrinterSink Sink>
    void Printer<Sink>::print_node(const Stmt::Return& n) {
        node_begin("Return");
        field("is_invalid", n.is_invalid);
        field("expr", n.expr);
        node_end();
    }

    template<print::PrinterSink Sink>
    void Printer<Sink>::print_node(const Stmt::Defer& n) {
        node_begin("Defer");
        field("is_invalid", n.is_invalid);
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
        field("is_invalid", n.is_invalid);
        field("signature", n.signature);
        field("block", n.block);
        node_end();
    }

    template<print::PrinterSink Sink>
    void Printer<Sink>::print_node(const Func::Signature& n) {
        node_begin("Signature");
        field("is_invalid", n.is_invalid);
        field("is_self_func", n.is_self_func);
        field("params", n.params);
        field("ret", n.ret);
        node_end();
    }

    template<print::PrinterSink Sink>
    void Printer<Sink>::print_node(const Func::Signature::Param& n) {
        node_begin("Param");
        field("is_invalid", n.is_invalid);
        field("name", n.name);
        field("type", n.type);
        node_end();
    }

    template<print::PrinterSink Sink>
    void Printer<Sink>::print_node(const Decl& n) {
        node_begin("Decl");
        field("is_export", n.is_export);
        field("is_decls_block", n.is_decls_block);
        field("name", n.name);
        field("generic_params", n.generic_params);
        field("val", n.val);
        node_end();
    }

    template<print::PrinterSink Sink>
    void Printer<Sink>::print_node(const Decl::GenericParam& n) {
        node_begin("GenericParam");
        field("is_invalid", n.is_invalid);
        field("name", n.name);
        field("interfaces", n.interfaces);
        node_end();
    }

    template<print::PrinterSink Sink>
    void Printer<Sink>::print_node(const Decl::Struct& n) {
        node_begin("Struct");
        field("is_invalid", n.is_invalid);
        field("fields", n.fields);
        node_end();
    }

    template<print::PrinterSink Sink>
    void Printer<Sink>::print_node(const Decl::Struct::Field& n) {
        node_begin("Field");
        field("is_invalid", n.is_invalid);
        field("name", n.name);
        field("type", n.type);
        field("is_export", n.is_export);
        node_end();
    }

    template<print::PrinterSink Sink>
    void Printer<Sink>::print_node(const Decl::Enum& n) {
        node_begin("Enum");
        field("is_invalid", n.is_invalid);
        field("elements", n.elements);
        node_end();
    }

    template<print::PrinterSink Sink>
    void Printer<Sink>::print_node(const Decl::Enum::Element& n) {
        node_begin("Element");
        field("is_invalid", n.is_invalid);
        field("name", n.name);
        node_end();
    }

    template<print::PrinterSink Sink>
    void Printer<Sink>::print_node(const Decl::TypeAlias& n) {
        node_begin("TypeAlias");
        field("is_invalid", n.is_invalid);
        field("type", n.type);
        node_end();
    }

    template<print::PrinterSink Sink>
    void Printer<Sink>::print_node(const Decl::Const& n) {
        node_begin("Const");
        field("is_invalid", n.is_invalid);
        field("type", n.type);
        field("expr", n.expr);
        node_end();
    }

    template<print::PrinterSink Sink>
    void Printer<Sink>::print_node(const Decl::Decls& n) {
        node_begin("Decls");
        field("is_invalid", n.is_invalid);
        field("type", n.type);
        field("interface", n.interface);
        field("decls", n.decls);
        node_end();
    }

    template<print::PrinterSink Sink>
    void Printer<Sink>::print_node(const Decl::Interface& n) {
        node_begin("Interface");
        field("is_invalid", n.is_invalid);
        field("members", n.members);
        node_end();
    }

    template<print::PrinterSink Sink>
    void Printer<Sink>::print_node(const Decl::Interface::Member& n) {
        node_begin("Member");
        field("is_invalid", n.is_invalid);
        field("name", n.name);
        field("generic_params", n.generic_params);
        field("signature", n.signature);
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
