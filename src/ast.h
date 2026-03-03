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
#include <cstddef>

#include "ast/util.h"
#include "ast/print.h"

namespace alvo::ast {

    struct Invalid;
    struct Id;
    struct Type;
    struct Expr;
    struct Block;
    struct Stmt;
    struct Func;
    struct Decl;
    struct Module;

    struct Invalid { };

    struct Id {
        uint64_t id;

        Id(const uint64_t& id) :
            id(id) { }
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

        struct Name {
            bool is_invalid;
            std::string_view name;
            util::List<Type> generic_params;

            Name(const bool& is_invalid, const std::string_view& name,
                const util::List<Type>& generic_params) :
                is_invalid(is_invalid),
                name(name),
                generic_params(generic_params) { }
        };

        struct Ref {
            bool is_invalid;
            util::Ptr<Type> type;

            Ref(const bool& is_invalid, const util::Ptr<Type>& type) :
                is_invalid(is_invalid),
                type(type) { }
        };

        struct LocalGeneric {
            Id id;

            LocalGeneric(const Id& id) :
                id(id) { }
        };

        struct ResolvedUserDefinedType {
            Id id;
            util::List<Type> generic_params;

            ResolvedUserDefinedType(
                const Id& id, const util::List<Type>& generic_params) :
                id(id),
                generic_params(generic_params) { }
        };

        using Val = std::variant<Invalid, Unit, String, Char, Int, Byte, Float,
            Bool, Array, Tup, Func, Name, Ref, LocalGeneric,
            ResolvedUserDefinedType>;
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

        struct Name {
            bool is_invalid;
            std::string_view name;
            util::List<Type> generic_params;

            Name(const bool& is_invalid, const std::string_view& name,
                const util::List<Type>& generic_params) :
                is_invalid(is_invalid),
                name(name),
                generic_params(generic_params) { }
        };

        struct TypeMemberAccess {
            bool is_invalid;
            Type type;
            Name name;

            TypeMemberAccess(
                const bool& is_invalid, const Type& type, const Name& name) :
                is_invalid(is_invalid),
                type(type),
                name(name) { }
        };

        struct MemberAccess {
            util::Ptr<Expr> expr;
            Name name;

            MemberAccess(const util::Ptr<Expr>& expr, const Name& name) :
                expr(expr),
                name(name) { }
        };

        struct LocalVar {
            Id id;

            LocalVar(const Id& id) :
                id(id) { }
        };

        struct ResolvedDecl {
            Id decl_id;
            util::List<Type> generic_params;

            ResolvedDecl(
                const Id& decl_id, const util::List<Type>& generic_params) :
                decl_id(decl_id),
                generic_params(generic_params) { }
        };

        struct ResolvedMemberAccess {
            util::Ptr<Expr> expr;
            Id member_id;
            util::List<Type> generic_params;

            ResolvedMemberAccess(const util::Ptr<Expr>& expr,
                const Id& member_id, const util::List<Type>& generic_params) :
                expr(expr),
                member_id(member_id),
                generic_params(generic_params) { }
        };

        struct ResolvedTypeMemberAccess {
            Type type;
            Id member_id;
            util::List<Type> generic_params;

            ResolvedTypeMemberAccess(const Type& type, const Id& member_id,
                const util::List<Type>& generic_params) :
                type(type),
                member_id(member_id),
                generic_params(generic_params) { }
        };

        struct ResolvedGenericMemberAccess {
            Id generic_id;
            std::string_view name;
            util::List<Type> generic_params;

            ResolvedGenericMemberAccess(const Id& generic_id,
                const std::string_view& name,
                const util::List<Type>& generic_params) :
                generic_id(generic_id),
                name(name),
                generic_params(generic_params) { }
        };

        using Val = std::variant<Invalid, Literal, Unop, Binop, Index, Call,
            Cast, TryCast, Ref, Builtin, Name, TypeMemberAccess, MemberAccess,
            LocalVar, ResolvedDecl, ResolvedMemberAccess,
            ResolvedTypeMemberAccess, ResolvedGenericMemberAccess>;
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

        struct DeclsBlock {
            bool is_invalid;
            util::List<GenericParam> generic_params;
            std::optional<Type> interface;
            util::List<Decl> decls;

            DeclsBlock(const bool& is_invalid,
                const util::List<GenericParam>& generic_params,
                const std::optional<Type>& interface,
                const util::List<Decl>& decls) :
                is_invalid(is_invalid),
                generic_params(generic_params),
                interface(interface),
                decls(decls) { }
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
            util::List<DeclsBlock> decls_blocks;

            Struct(const bool& is_invalid, const util::List<Field>& fields,
                const util::List<DeclsBlock>& decls_blocks) :
                is_invalid(is_invalid),
                fields(fields),
                decls_blocks(decls_blocks) { }
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
            util::List<DeclsBlock> decls_blocks;

            Enum(const bool& is_invalid, const util::List<Element>& elements,
                const util::List<DeclsBlock>& decls_blocks) :
                is_invalid(is_invalid),
                elements(elements),
                decls_blocks(decls_blocks) { }
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
            Interface>;
        bool is_export;
        std::string_view name;
        util::List<GenericParam> generic_params;
        Val val;

        Decl(const bool& is_export, const std::string_view& name,
            const util::List<GenericParam>& generic_params, const Val& val) :
            is_export(is_export),
            name(name),
            generic_params(generic_params),
            val(val) { }
    };

    struct Module {
        util::List<Decl> decls;

        Module(const util::List<Decl>& decls) :
            decls(decls) { }
    };

    bool operator==(
        [[maybe_unused]] const Invalid& l, [[maybe_unused]] const Invalid& r);
    bool operator==(const Id& l, const Id& r);
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
    bool operator==(const Type::Name& l, const Type::Name& r);
    bool operator==(const Type::Ref& l, const Type::Ref& r);
    bool operator==(const Type::LocalGeneric& l, const Type::LocalGeneric& r);
    bool operator==(const Type::ResolvedUserDefinedType& l,
        const Type::ResolvedUserDefinedType& r);
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
    bool operator==(const Expr::Name& l, const Expr::Name& r);
    bool operator==(
        const Expr::TypeMemberAccess& l, const Expr::TypeMemberAccess& r);
    bool operator==(const Expr::MemberAccess& l, const Expr::MemberAccess& r);
    bool operator==(const Expr::LocalVar& l, const Expr::LocalVar& r);
    bool operator==(const Expr::ResolvedDecl& l, const Expr::ResolvedDecl& r);
    bool operator==(const Expr::ResolvedMemberAccess& l,
        const Expr::ResolvedMemberAccess& r);
    bool operator==(const Expr::ResolvedTypeMemberAccess& l,
        const Expr::ResolvedTypeMemberAccess& r);
    bool operator==(const Expr::ResolvedGenericMemberAccess& l,
        const Expr::ResolvedGenericMemberAccess& r);
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
    bool operator==(const Decl::DeclsBlock& l, const Decl::DeclsBlock& r);
    bool operator==(const Decl::Struct& l, const Decl::Struct& r);
    bool operator==(const Decl::Struct::Field& l, const Decl::Struct::Field& r);
    bool operator==(const Decl::Enum& l, const Decl::Enum& r);
    bool operator==(const Decl::Enum::Element& l, const Decl::Enum::Element& r);
    bool operator==(const Decl::TypeAlias& l, const Decl::TypeAlias& r);
    bool operator==(const Decl::Const& l, const Decl::Const& r);
    bool operator==(const Decl::Interface& l, const Decl::Interface& r);
    bool operator==(
        const Decl::Interface::Member& l, const Decl::Interface::Member& r);
    bool operator==(const Module& l, const Module& r);
    bool operator!=(
        [[maybe_unused]] const Invalid& l, [[maybe_unused]] const Invalid& r);
    bool operator!=(const Id& l, const Id& r);
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
    bool operator!=(const Type::Name& l, const Type::Name& r);
    bool operator!=(const Type::Ref& l, const Type::Ref& r);
    bool operator!=(const Type::LocalGeneric& l, const Type::LocalGeneric& r);
    bool operator!=(const Type::ResolvedUserDefinedType& l,
        const Type::ResolvedUserDefinedType& r);
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
    bool operator!=(const Expr::Name& l, const Expr::Name& r);
    bool operator!=(
        const Expr::TypeMemberAccess& l, const Expr::TypeMemberAccess& r);
    bool operator!=(const Expr::MemberAccess& l, const Expr::MemberAccess& r);
    bool operator!=(const Expr::LocalVar& l, const Expr::LocalVar& r);
    bool operator!=(const Expr::ResolvedDecl& l, const Expr::ResolvedDecl& r);
    bool operator!=(const Expr::ResolvedMemberAccess& l,
        const Expr::ResolvedMemberAccess& r);
    bool operator!=(const Expr::ResolvedTypeMemberAccess& l,
        const Expr::ResolvedTypeMemberAccess& r);
    bool operator!=(const Expr::ResolvedGenericMemberAccess& l,
        const Expr::ResolvedGenericMemberAccess& r);
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
    bool operator!=(const Decl::DeclsBlock& l, const Decl::DeclsBlock& r);
    bool operator!=(const Decl::Struct& l, const Decl::Struct& r);
    bool operator!=(const Decl::Struct::Field& l, const Decl::Struct::Field& r);
    bool operator!=(const Decl::Enum& l, const Decl::Enum& r);
    bool operator!=(const Decl::Enum::Element& l, const Decl::Enum::Element& r);
    bool operator!=(const Decl::TypeAlias& l, const Decl::TypeAlias& r);
    bool operator!=(const Decl::Const& l, const Decl::Const& r);
    bool operator!=(const Decl::Interface& l, const Decl::Interface& r);
    bool operator!=(
        const Decl::Interface::Member& l, const Decl::Interface::Member& r);
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
        void print_node(const Id& n);
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
        void print_node(const Type::Name& n);
        void print_node(const Type::Ref& n);
        void print_node(const Type::LocalGeneric& n);
        void print_node(const Type::ResolvedUserDefinedType& n);
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
        void print_node(const Expr::Name& n);
        void print_node(const Expr::TypeMemberAccess& n);
        void print_node(const Expr::MemberAccess& n);
        void print_node(const Expr::LocalVar& n);
        void print_node(const Expr::ResolvedDecl& n);
        void print_node(const Expr::ResolvedMemberAccess& n);
        void print_node(const Expr::ResolvedTypeMemberAccess& n);
        void print_node(const Expr::ResolvedGenericMemberAccess& n);
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
        void print_node(const Decl::DeclsBlock& n);
        void print_node(const Decl::Struct& n);
        void print_node(const Decl::Struct::Field& n);
        void print_node(const Decl::Enum& n);
        void print_node(const Decl::Enum::Element& n);
        void print_node(const Decl::TypeAlias& n);
        void print_node(const Decl::Const& n);
        void print_node(const Decl::Interface& n);
        void print_node(const Decl::Interface::Member& n);
        void print_node(const Module& n);
    };

    template<print::PrinterSink Sink>
    void Printer<Sink>::print_node([[maybe_unused]] const Invalid& n) {
        node("Invalid");
    }

    template<print::PrinterSink Sink>
    void Printer<Sink>::print_node(const Id& n) {
        node_begin("Id");
        field("id", n.id);
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
    void Printer<Sink>::print_node(const Type::Name& n) {
        node_begin("Name");
        field("is_invalid", n.is_invalid);
        field("name", n.name);
        field("generic_params", n.generic_params);
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
    void Printer<Sink>::print_node(const Type::LocalGeneric& n) {
        node_begin("LocalGeneric");
        field("id", n.id);
        node_end();
    }

    template<print::PrinterSink Sink>
    void Printer<Sink>::print_node(const Type::ResolvedUserDefinedType& n) {
        node_begin("ResolvedUserDefinedType");
        field("id", n.id);
        field("generic_params", n.generic_params);
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
    void Printer<Sink>::print_node(const Expr::Name& n) {
        node_begin("Name");
        field("is_invalid", n.is_invalid);
        field("name", n.name);
        field("generic_params", n.generic_params);
        node_end();
    }

    template<print::PrinterSink Sink>
    void Printer<Sink>::print_node(const Expr::TypeMemberAccess& n) {
        node_begin("TypeMemberAccess");
        field("is_invalid", n.is_invalid);
        field("type", n.type);
        field("name", n.name);
        node_end();
    }

    template<print::PrinterSink Sink>
    void Printer<Sink>::print_node(const Expr::MemberAccess& n) {
        node_begin("MemberAccess");
        field("expr", n.expr);
        field("name", n.name);
        node_end();
    }

    template<print::PrinterSink Sink>
    void Printer<Sink>::print_node(const Expr::LocalVar& n) {
        node_begin("LocalVar");
        field("id", n.id);
        node_end();
    }

    template<print::PrinterSink Sink>
    void Printer<Sink>::print_node(const Expr::ResolvedDecl& n) {
        node_begin("ResolvedDecl");
        field("decl_id", n.decl_id);
        field("generic_params", n.generic_params);
        node_end();
    }

    template<print::PrinterSink Sink>
    void Printer<Sink>::print_node(const Expr::ResolvedMemberAccess& n) {
        node_begin("ResolvedMemberAccess");
        field("expr", n.expr);
        field("member_id", n.member_id);
        field("generic_params", n.generic_params);
        node_end();
    }

    template<print::PrinterSink Sink>
    void Printer<Sink>::print_node(const Expr::ResolvedTypeMemberAccess& n) {
        node_begin("ResolvedTypeMemberAccess");
        field("type", n.type);
        field("member_id", n.member_id);
        field("generic_params", n.generic_params);
        node_end();
    }

    template<print::PrinterSink Sink>
    void Printer<Sink>::print_node(const Expr::ResolvedGenericMemberAccess& n) {
        node_begin("ResolvedGenericMemberAccess");
        field("generic_id", n.generic_id);
        field("name", n.name);
        field("generic_params", n.generic_params);
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
    void Printer<Sink>::print_node(const Decl::DeclsBlock& n) {
        node_begin("DeclsBlock");
        field("is_invalid", n.is_invalid);
        field("generic_params", n.generic_params);
        field("interface", n.interface);
        field("decls", n.decls);
        node_end();
    }

    template<print::PrinterSink Sink>
    void Printer<Sink>::print_node(const Decl::Struct& n) {
        node_begin("Struct");
        field("is_invalid", n.is_invalid);
        field("fields", n.fields);
        field("decls_blocks", n.decls_blocks);
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
        field("decls_blocks", n.decls_blocks);
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
    void Printer<Sink>::print_node(const Module& n) {
        node_begin("Module");
        field("decls", n.decls);
        node_end();
    }

}

namespace std {

    template<>
    struct hash<alvo::ast::Invalid> {
        std::size_t operator()(const alvo::ast::Invalid& n) const noexcept;
    };

    template<>
    struct hash<alvo::ast::Id> {
        std::size_t operator()(const alvo::ast::Id& n) const noexcept;
    };

    template<>
    struct hash<alvo::ast::Type> {
        std::size_t operator()(const alvo::ast::Type& n) const noexcept;
    };

    template<>
    struct hash<alvo::ast::Type::Unit> {
        std::size_t operator()(const alvo::ast::Type::Unit& n) const noexcept;
    };

    template<>
    struct hash<alvo::ast::Type::String> {
        std::size_t operator()(const alvo::ast::Type::String& n) const noexcept;
    };

    template<>
    struct hash<alvo::ast::Type::Char> {
        std::size_t operator()(const alvo::ast::Type::Char& n) const noexcept;
    };

    template<>
    struct hash<alvo::ast::Type::Int> {
        std::size_t operator()(const alvo::ast::Type::Int& n) const noexcept;
    };

    template<>
    struct hash<alvo::ast::Type::Byte> {
        std::size_t operator()(const alvo::ast::Type::Byte& n) const noexcept;
    };

    template<>
    struct hash<alvo::ast::Type::Float> {
        std::size_t operator()(const alvo::ast::Type::Float& n) const noexcept;
    };

    template<>
    struct hash<alvo::ast::Type::Bool> {
        std::size_t operator()(const alvo::ast::Type::Bool& n) const noexcept;
    };

    template<>
    struct hash<alvo::ast::Type::Array> {
        std::size_t operator()(const alvo::ast::Type::Array& n) const noexcept;
    };

    template<>
    struct hash<alvo::ast::Type::Tup> {
        std::size_t operator()(const alvo::ast::Type::Tup& n) const noexcept;
    };

    template<>
    struct hash<alvo::ast::Type::Func> {
        std::size_t operator()(const alvo::ast::Type::Func& n) const noexcept;
    };

    template<>
    struct hash<alvo::ast::Type::Name> {
        std::size_t operator()(const alvo::ast::Type::Name& n) const noexcept;
    };

    template<>
    struct hash<alvo::ast::Type::Ref> {
        std::size_t operator()(const alvo::ast::Type::Ref& n) const noexcept;
    };

    template<>
    struct hash<alvo::ast::Type::LocalGeneric> {
        std::size_t operator()(
            const alvo::ast::Type::LocalGeneric& n) const noexcept;
    };

    template<>
    struct hash<alvo::ast::Type::ResolvedUserDefinedType> {
        std::size_t operator()(
            const alvo::ast::Type::ResolvedUserDefinedType& n) const noexcept;
    };

    template<>
    struct hash<alvo::ast::Expr> {
        std::size_t operator()(const alvo::ast::Expr& n) const noexcept;
    };

    template<>
    struct hash<alvo::ast::Expr::Literal> {
        std::size_t operator()(
            const alvo::ast::Expr::Literal& n) const noexcept;
    };

    template<>
    struct hash<alvo::ast::Expr::Literal::Unit> {
        std::size_t operator()(
            const alvo::ast::Expr::Literal::Unit& n) const noexcept;
    };

    template<>
    struct hash<alvo::ast::Expr::Literal::Null> {
        std::size_t operator()(
            const alvo::ast::Expr::Literal::Null& n) const noexcept;
    };

    template<>
    struct hash<alvo::ast::Expr::Literal::String> {
        std::size_t operator()(
            const alvo::ast::Expr::Literal::String& n) const noexcept;
    };

    template<>
    struct hash<alvo::ast::Expr::Literal::Character> {
        std::size_t operator()(
            const alvo::ast::Expr::Literal::Character& n) const noexcept;
    };

    template<>
    struct hash<alvo::ast::Expr::Literal::Integer> {
        std::size_t operator()(
            const alvo::ast::Expr::Literal::Integer& n) const noexcept;
    };

    template<>
    struct hash<alvo::ast::Expr::Literal::Byte> {
        std::size_t operator()(
            const alvo::ast::Expr::Literal::Byte& n) const noexcept;
    };

    template<>
    struct hash<alvo::ast::Expr::Literal::Floating> {
        std::size_t operator()(
            const alvo::ast::Expr::Literal::Floating& n) const noexcept;
    };

    template<>
    struct hash<alvo::ast::Expr::Literal::Boolean> {
        std::size_t operator()(
            const alvo::ast::Expr::Literal::Boolean& n) const noexcept;
    };

    template<>
    struct hash<alvo::ast::Expr::Literal::Array> {
        std::size_t operator()(
            const alvo::ast::Expr::Literal::Array& n) const noexcept;
    };

    template<>
    struct hash<alvo::ast::Expr::Literal::Array::Regular> {
        std::size_t operator()(
            const alvo::ast::Expr::Literal::Array::Regular& n) const noexcept;
    };

    template<>
    struct hash<alvo::ast::Expr::Literal::Array::DefaultNTimes> {
        std::size_t operator()(
            const alvo::ast::Expr::Literal::Array::DefaultNTimes& n)
            const noexcept;
    };

    template<>
    struct hash<alvo::ast::Expr::Literal::Array::ExprNTimes> {
        std::size_t operator()(
            const alvo::ast::Expr::Literal::Array::ExprNTimes& n)
            const noexcept;
    };

    template<>
    struct hash<alvo::ast::Expr::Literal::Tup> {
        std::size_t operator()(
            const alvo::ast::Expr::Literal::Tup& n) const noexcept;
    };

    template<>
    struct hash<alvo::ast::Expr::Literal::Struct> {
        std::size_t operator()(
            const alvo::ast::Expr::Literal::Struct& n) const noexcept;
    };

    template<>
    struct hash<alvo::ast::Expr::Literal::Struct::Field> {
        std::size_t operator()(
            const alvo::ast::Expr::Literal::Struct::Field& n) const noexcept;
    };

    template<>
    struct hash<alvo::ast::Expr::Unop> {
        std::size_t operator()(const alvo::ast::Expr::Unop& n) const noexcept;
    };

    template<>
    struct hash<alvo::ast::Expr::Unop::Op> {
        std::size_t operator()(
            const alvo::ast::Expr::Unop::Op& n) const noexcept;
    };

    template<>
    struct hash<alvo::ast::Expr::Binop> {
        std::size_t operator()(const alvo::ast::Expr::Binop& n) const noexcept;
    };

    template<>
    struct hash<alvo::ast::Expr::Binop::Op> {
        std::size_t operator()(
            const alvo::ast::Expr::Binop::Op& n) const noexcept;
    };

    template<>
    struct hash<alvo::ast::Expr::Index> {
        std::size_t operator()(const alvo::ast::Expr::Index& n) const noexcept;
    };

    template<>
    struct hash<alvo::ast::Expr::Call> {
        std::size_t operator()(const alvo::ast::Expr::Call& n) const noexcept;
    };

    template<>
    struct hash<alvo::ast::Expr::Cast> {
        std::size_t operator()(const alvo::ast::Expr::Cast& n) const noexcept;
    };

    template<>
    struct hash<alvo::ast::Expr::TryCast> {
        std::size_t operator()(
            const alvo::ast::Expr::TryCast& n) const noexcept;
    };

    template<>
    struct hash<alvo::ast::Expr::Ref> {
        std::size_t operator()(const alvo::ast::Expr::Ref& n) const noexcept;
    };

    template<>
    struct hash<alvo::ast::Expr::Builtin> {
        std::size_t operator()(
            const alvo::ast::Expr::Builtin& n) const noexcept;
    };

    template<>
    struct hash<alvo::ast::Expr::Name> {
        std::size_t operator()(const alvo::ast::Expr::Name& n) const noexcept;
    };

    template<>
    struct hash<alvo::ast::Expr::TypeMemberAccess> {
        std::size_t operator()(
            const alvo::ast::Expr::TypeMemberAccess& n) const noexcept;
    };

    template<>
    struct hash<alvo::ast::Expr::MemberAccess> {
        std::size_t operator()(
            const alvo::ast::Expr::MemberAccess& n) const noexcept;
    };

    template<>
    struct hash<alvo::ast::Expr::LocalVar> {
        std::size_t operator()(
            const alvo::ast::Expr::LocalVar& n) const noexcept;
    };

    template<>
    struct hash<alvo::ast::Expr::ResolvedDecl> {
        std::size_t operator()(
            const alvo::ast::Expr::ResolvedDecl& n) const noexcept;
    };

    template<>
    struct hash<alvo::ast::Expr::ResolvedMemberAccess> {
        std::size_t operator()(
            const alvo::ast::Expr::ResolvedMemberAccess& n) const noexcept;
    };

    template<>
    struct hash<alvo::ast::Expr::ResolvedTypeMemberAccess> {
        std::size_t operator()(
            const alvo::ast::Expr::ResolvedTypeMemberAccess& n) const noexcept;
    };

    template<>
    struct hash<alvo::ast::Expr::ResolvedGenericMemberAccess> {
        std::size_t operator()(
            const alvo::ast::Expr::ResolvedGenericMemberAccess& n)
            const noexcept;
    };

    template<>
    struct hash<alvo::ast::Block> {
        std::size_t operator()(const alvo::ast::Block& n) const noexcept;
    };

    template<>
    struct hash<alvo::ast::Stmt> {
        std::size_t operator()(const alvo::ast::Stmt& n) const noexcept;
    };

    template<>
    struct hash<alvo::ast::Stmt::Let> {
        std::size_t operator()(const alvo::ast::Stmt::Let& n) const noexcept;
    };

    template<>
    struct hash<alvo::ast::Stmt::If> {
        std::size_t operator()(const alvo::ast::Stmt::If& n) const noexcept;
    };

    template<>
    struct hash<alvo::ast::Stmt::If::Elif> {
        std::size_t operator()(
            const alvo::ast::Stmt::If::Elif& n) const noexcept;
    };

    template<>
    struct hash<alvo::ast::Stmt::Switch> {
        std::size_t operator()(const alvo::ast::Stmt::Switch& n) const noexcept;
    };

    template<>
    struct hash<alvo::ast::Stmt::Switch::Case> {
        std::size_t operator()(
            const alvo::ast::Stmt::Switch::Case& n) const noexcept;
    };

    template<>
    struct hash<alvo::ast::Stmt::Loop> {
        std::size_t operator()(const alvo::ast::Stmt::Loop& n) const noexcept;
    };

    template<>
    struct hash<alvo::ast::Stmt::For> {
        std::size_t operator()(const alvo::ast::Stmt::For& n) const noexcept;
    };

    template<>
    struct hash<alvo::ast::Stmt::While> {
        std::size_t operator()(const alvo::ast::Stmt::While& n) const noexcept;
    };

    template<>
    struct hash<alvo::ast::Stmt::Return> {
        std::size_t operator()(const alvo::ast::Stmt::Return& n) const noexcept;
    };

    template<>
    struct hash<alvo::ast::Stmt::Defer> {
        std::size_t operator()(const alvo::ast::Stmt::Defer& n) const noexcept;
    };

    template<>
    struct hash<alvo::ast::Stmt::Continue> {
        std::size_t operator()(
            const alvo::ast::Stmt::Continue& n) const noexcept;
    };

    template<>
    struct hash<alvo::ast::Stmt::Break> {
        std::size_t operator()(const alvo::ast::Stmt::Break& n) const noexcept;
    };

    template<>
    struct hash<alvo::ast::Func> {
        std::size_t operator()(const alvo::ast::Func& n) const noexcept;
    };

    template<>
    struct hash<alvo::ast::Func::Signature> {
        std::size_t operator()(
            const alvo::ast::Func::Signature& n) const noexcept;
    };

    template<>
    struct hash<alvo::ast::Func::Signature::Param> {
        std::size_t operator()(
            const alvo::ast::Func::Signature::Param& n) const noexcept;
    };

    template<>
    struct hash<alvo::ast::Decl> {
        std::size_t operator()(const alvo::ast::Decl& n) const noexcept;
    };

    template<>
    struct hash<alvo::ast::Decl::GenericParam> {
        std::size_t operator()(
            const alvo::ast::Decl::GenericParam& n) const noexcept;
    };

    template<>
    struct hash<alvo::ast::Decl::DeclsBlock> {
        std::size_t operator()(
            const alvo::ast::Decl::DeclsBlock& n) const noexcept;
    };

    template<>
    struct hash<alvo::ast::Decl::Struct> {
        std::size_t operator()(const alvo::ast::Decl::Struct& n) const noexcept;
    };

    template<>
    struct hash<alvo::ast::Decl::Struct::Field> {
        std::size_t operator()(
            const alvo::ast::Decl::Struct::Field& n) const noexcept;
    };

    template<>
    struct hash<alvo::ast::Decl::Enum> {
        std::size_t operator()(const alvo::ast::Decl::Enum& n) const noexcept;
    };

    template<>
    struct hash<alvo::ast::Decl::Enum::Element> {
        std::size_t operator()(
            const alvo::ast::Decl::Enum::Element& n) const noexcept;
    };

    template<>
    struct hash<alvo::ast::Decl::TypeAlias> {
        std::size_t operator()(
            const alvo::ast::Decl::TypeAlias& n) const noexcept;
    };

    template<>
    struct hash<alvo::ast::Decl::Const> {
        std::size_t operator()(const alvo::ast::Decl::Const& n) const noexcept;
    };

    template<>
    struct hash<alvo::ast::Decl::Interface> {
        std::size_t operator()(
            const alvo::ast::Decl::Interface& n) const noexcept;
    };

    template<>
    struct hash<alvo::ast::Decl::Interface::Member> {
        std::size_t operator()(
            const alvo::ast::Decl::Interface::Member& n) const noexcept;
    };

    template<>
    struct hash<alvo::ast::Module> {
        std::size_t operator()(const alvo::ast::Module& n) const noexcept;
    };

}
