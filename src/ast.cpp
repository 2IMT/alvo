// This file is generated using `astgen.py`.    /
// DO NOT EDIT BY HAND!                         /
//                                              /
// Note: any modifications to the AST should be /
// done by changing the AST schema file         /
// `ast.json` and regenerating the sources.     /

#include "ast.h"

namespace alvo::ast {

    bool operator==(
        [[maybe_unused]] const Invalid& l, [[maybe_unused]] const Invalid& r) {
        return true;
    }

    bool operator==(const PathSegment& l, const PathSegment& r) {
        return l.val == r.val;
    }

    bool operator==([[maybe_unused]] const PathSegment::Root& l,
        [[maybe_unused]] const PathSegment::Root& r) {
        return true;
    }

    bool operator==([[maybe_unused]] const PathSegment::Super& l,
        [[maybe_unused]] const PathSegment::Super& r) {
        return true;
    }

    bool operator==(const PathSegment::Name& l, const PathSegment::Name& r) {
        return l.value == r.value && l.generic_params == r.generic_params;
    }

    bool operator==(const Import& l, const Import& r) {
        return l.kind == r.kind && l.segments == r.segments;
    }

    bool operator==([[maybe_unused]] const Import::Normal& l,
        [[maybe_unused]] const Import::Normal& r) {
        return true;
    }

    bool operator==([[maybe_unused]] const Import::Glob& l,
        [[maybe_unused]] const Import::Glob& r) {
        return true;
    }

    bool operator==(const Import::Renamed& l, const Import::Renamed& r) {
        return l.renamed_to == r.renamed_to;
    }

    bool operator==(const Type& l, const Type& r) {
        return l.val == r.val && l.nullable == r.nullable;
    }

    bool operator==([[maybe_unused]] const Type::Unit& l,
        [[maybe_unused]] const Type::Unit& r) {
        return true;
    }

    bool operator==([[maybe_unused]] const Type::String& l,
        [[maybe_unused]] const Type::String& r) {
        return true;
    }

    bool operator==([[maybe_unused]] const Type::Char& l,
        [[maybe_unused]] const Type::Char& r) {
        return true;
    }

    bool operator==([[maybe_unused]] const Type::Int& l,
        [[maybe_unused]] const Type::Int& r) {
        return true;
    }

    bool operator==([[maybe_unused]] const Type::Byte& l,
        [[maybe_unused]] const Type::Byte& r) {
        return true;
    }

    bool operator==([[maybe_unused]] const Type::Float& l,
        [[maybe_unused]] const Type::Float& r) {
        return true;
    }

    bool operator==([[maybe_unused]] const Type::Bool& l,
        [[maybe_unused]] const Type::Bool& r) {
        return true;
    }

    bool operator==(const Type::Array& l, const Type::Array& r) {
        return l.is_invalid == r.is_invalid && l.type == r.type;
    }

    bool operator==(const Type::Tup& l, const Type::Tup& r) {
        return l.is_invalid == r.is_invalid && l.types == r.types;
    }

    bool operator==(const Type::Func& l, const Type::Func& r) {
        return l.is_invalid == r.is_invalid && l.params == r.params &&
               l.return_type == r.return_type;
    }

    bool operator==(const Type::Path& l, const Type::Path& r) {
        return l.is_invalid == r.is_invalid && l.segments == r.segments;
    }

    bool operator==(const Type::Ref& l, const Type::Ref& r) {
        return l.is_invalid == r.is_invalid && l.type == r.type;
    }

    bool operator==(const Expr& l, const Expr& r) { return l.val == r.val; }

    bool operator==(const Expr::Literal& l, const Expr::Literal& r) {
        return l.val == r.val;
    }

    bool operator==([[maybe_unused]] const Expr::Literal::Unit& l,
        [[maybe_unused]] const Expr::Literal::Unit& r) {
        return true;
    }

    bool operator==([[maybe_unused]] const Expr::Literal::Null& l,
        [[maybe_unused]] const Expr::Literal::Null& r) {
        return true;
    }

    bool operator==(
        const Expr::Literal::String& l, const Expr::Literal::String& r) {
        return l.value == r.value;
    }

    bool operator==(
        const Expr::Literal::Character& l, const Expr::Literal::Character& r) {
        return l.value == r.value;
    }

    bool operator==(
        const Expr::Literal::Integer& l, const Expr::Literal::Integer& r) {
        return l.value == r.value;
    }

    bool operator==(
        const Expr::Literal::Byte& l, const Expr::Literal::Byte& r) {
        return l.value == r.value;
    }

    bool operator==(
        const Expr::Literal::Floating& l, const Expr::Literal::Floating& r) {
        return l.value == r.value;
    }

    bool operator==(
        const Expr::Literal::Boolean& l, const Expr::Literal::Boolean& r) {
        return l.value == r.value;
    }

    bool operator==(
        const Expr::Literal::Array& l, const Expr::Literal::Array& r) {
        return l.val == r.val;
    }

    bool operator==(const Expr::Literal::Array::Regular& l,
        const Expr::Literal::Array::Regular& r) {
        return l.elements == r.elements;
    }

    bool operator==(const Expr::Literal::Array::DefaultNTimes& l,
        const Expr::Literal::Array::DefaultNTimes& r) {
        return l.times == r.times;
    }

    bool operator==(const Expr::Literal::Array::ExprNTimes& l,
        const Expr::Literal::Array::ExprNTimes& r) {
        return l.expr == r.expr && l.times == r.times;
    }

    bool operator==(const Expr::Literal::Tup& l, const Expr::Literal::Tup& r) {
        return l.is_invalid == r.is_invalid && l.exprs == r.exprs;
    }

    bool operator==(
        const Expr::Literal::Struct& l, const Expr::Literal::Struct& r) {
        return l.is_invalid == r.is_invalid && l.type == r.type &&
               l.fields == r.fields;
    }

    bool operator==(const Expr::Literal::Struct::Field& l,
        const Expr::Literal::Struct::Field& r) {
        return l.is_invalid == r.is_invalid && l.name == r.name &&
               l.expr == r.expr;
    }

    bool operator==(const Expr::Unop& l, const Expr::Unop& r) {
        return l.expr == r.expr && l.op == r.op;
    }

    bool operator==(const Expr::Binop& l, const Expr::Binop& r) {
        return l.lhs == r.lhs && l.rhs == r.rhs && l.op == r.op;
    }

    bool operator==(const Expr::Index& l, const Expr::Index& r) {
        return l.expr == r.expr && l.index == r.index;
    }

    bool operator==(const Expr::Call& l, const Expr::Call& r) {
        return l.expr == r.expr && l.args == r.args;
    }

    bool operator==(const Expr::Cast& l, const Expr::Cast& r) {
        return l.expr == r.expr && l.type == r.type;
    }

    bool operator==(const Expr::TryCast& l, const Expr::TryCast& r) {
        return l.expr == r.expr && l.type == r.type;
    }

    bool operator==(const Expr::Ref& l, const Expr::Ref& r) {
        return l.is_invalid == r.is_invalid && l.expr == r.expr;
    }

    bool operator==(const Expr::Builtin& l, const Expr::Builtin& r) {
        return l.is_invalid == r.is_invalid && l.name == r.name &&
               l.generic_params == r.generic_params && l.args == r.args;
    }

    bool operator==(const Block& l, const Block& r) {
        return l.is_invalid == r.is_invalid && l.stmts == r.stmts;
    }

    bool operator==(const Stmt& l, const Stmt& r) { return l.val == r.val; }

    bool operator==(const Stmt::Let& l, const Stmt::Let& r) {
        return l.is_invalid == r.is_invalid && l.name == r.name &&
               l.type == r.type && l.expr == r.expr;
    }

    bool operator==(const Stmt::If& l, const Stmt::If& r) {
        return l.is_invalid == r.is_invalid && l.expr == r.expr &&
               l.main == r.main && l.elifs == r.elifs && l.else_ == r.else_;
    }

    bool operator==(const Stmt::If::Elif& l, const Stmt::If::Elif& r) {
        return l.expr == r.expr && l.block == r.block;
    }

    bool operator==(const Stmt::Switch& l, const Stmt::Switch& r) {
        return l.is_invalid == r.is_invalid && l.expr == r.expr &&
               l.cases == r.cases;
    }

    bool operator==(const Stmt::Switch::Case& l, const Stmt::Switch::Case& r) {
        return l.is_invalid == r.is_invalid && l.expr == r.expr &&
               l.block == r.block;
    }

    bool operator==(const Stmt::Loop& l, const Stmt::Loop& r) {
        return l.is_invalid == r.is_invalid && l.block == r.block;
    }

    bool operator==(const Stmt::For& l, const Stmt::For& r) {
        return l.is_invalid == r.is_invalid && l.name == r.name &&
               l.expr == r.expr && l.block == r.block;
    }

    bool operator==(const Stmt::While& l, const Stmt::While& r) {
        return l.is_invalid == r.is_invalid && l.expr == r.expr &&
               l.block == r.block;
    }

    bool operator==(const Stmt::Return& l, const Stmt::Return& r) {
        return l.is_invalid == r.is_invalid && l.expr == r.expr;
    }

    bool operator==(const Stmt::Defer& l, const Stmt::Defer& r) {
        return l.is_invalid == r.is_invalid && l.expr == r.expr;
    }

    bool operator==([[maybe_unused]] const Stmt::Continue& l,
        [[maybe_unused]] const Stmt::Continue& r) {
        return true;
    }

    bool operator==([[maybe_unused]] const Stmt::Break& l,
        [[maybe_unused]] const Stmt::Break& r) {
        return true;
    }

    bool operator==(const Func& l, const Func& r) {
        return l.is_invalid == r.is_invalid && l.signature == r.signature &&
               l.block == r.block;
    }

    bool operator==(const Func::Signature& l, const Func::Signature& r) {
        return l.is_invalid == r.is_invalid &&
               l.is_self_func == r.is_self_func && l.params == r.params &&
               l.ret == r.ret;
    }

    bool operator==(
        const Func::Signature::Param& l, const Func::Signature::Param& r) {
        return l.is_invalid == r.is_invalid && l.name == r.name &&
               l.type == r.type;
    }

    bool operator==(const Decl& l, const Decl& r) {
        return l.is_export == r.is_export &&
               l.is_decls_block == r.is_decls_block && l.name == r.name &&
               l.generic_params == r.generic_params && l.val == r.val;
    }

    bool operator==(const Decl::GenericParam& l, const Decl::GenericParam& r) {
        return l.is_invalid == r.is_invalid && l.name == r.name &&
               l.interfaces == r.interfaces;
    }

    bool operator==(const Decl::Struct& l, const Decl::Struct& r) {
        return l.is_invalid == r.is_invalid && l.fields == r.fields;
    }

    bool operator==(
        const Decl::Struct::Field& l, const Decl::Struct::Field& r) {
        return l.is_invalid == r.is_invalid && l.name == r.name &&
               l.type == r.type && l.is_export == r.is_export;
    }

    bool operator==(const Decl::Enum& l, const Decl::Enum& r) {
        return l.is_invalid == r.is_invalid && l.elements == r.elements;
    }

    bool operator==(
        const Decl::Enum::Element& l, const Decl::Enum::Element& r) {
        return l.is_invalid == r.is_invalid && l.name == r.name;
    }

    bool operator==(const Decl::TypeAlias& l, const Decl::TypeAlias& r) {
        return l.is_invalid == r.is_invalid && l.type == r.type;
    }

    bool operator==(const Decl::Const& l, const Decl::Const& r) {
        return l.is_invalid == r.is_invalid && l.type == r.type &&
               l.expr == r.expr;
    }

    bool operator==(const Decl::Decls& l, const Decl::Decls& r) {
        return l.is_invalid == r.is_invalid && l.type == r.type &&
               l.interface == r.interface && l.decls == r.decls;
    }

    bool operator==(const Decl::Interface& l, const Decl::Interface& r) {
        return l.is_invalid == r.is_invalid && l.members == r.members;
    }

    bool operator==(
        const Decl::Interface::Member& l, const Decl::Interface::Member& r) {
        return l.is_invalid == r.is_invalid && l.name == r.name &&
               l.generic_params == r.generic_params &&
               l.signature == r.signature;
    }

    bool operator==(const TopLevel& l, const TopLevel& r) {
        return l.val == r.val;
    }

    bool operator==(const Module& l, const Module& r) {
        return l.top_levels == r.top_levels;
    }

    bool operator!=(
        [[maybe_unused]] const Invalid& l, [[maybe_unused]] const Invalid& r) {
        return true;
    }

    bool operator!=(const PathSegment& l, const PathSegment& r) {
        return l.val != r.val;
    }

    bool operator!=([[maybe_unused]] const PathSegment::Root& l,
        [[maybe_unused]] const PathSegment::Root& r) {
        return true;
    }

    bool operator!=([[maybe_unused]] const PathSegment::Super& l,
        [[maybe_unused]] const PathSegment::Super& r) {
        return true;
    }

    bool operator!=(const PathSegment::Name& l, const PathSegment::Name& r) {
        return l.value != r.value && l.generic_params != r.generic_params;
    }

    bool operator!=(const Import& l, const Import& r) {
        return l.kind != r.kind && l.segments != r.segments;
    }

    bool operator!=([[maybe_unused]] const Import::Normal& l,
        [[maybe_unused]] const Import::Normal& r) {
        return true;
    }

    bool operator!=([[maybe_unused]] const Import::Glob& l,
        [[maybe_unused]] const Import::Glob& r) {
        return true;
    }

    bool operator!=(const Import::Renamed& l, const Import::Renamed& r) {
        return l.renamed_to != r.renamed_to;
    }

    bool operator!=(const Type& l, const Type& r) {
        return l.val != r.val && l.nullable != r.nullable;
    }

    bool operator!=([[maybe_unused]] const Type::Unit& l,
        [[maybe_unused]] const Type::Unit& r) {
        return true;
    }

    bool operator!=([[maybe_unused]] const Type::String& l,
        [[maybe_unused]] const Type::String& r) {
        return true;
    }

    bool operator!=([[maybe_unused]] const Type::Char& l,
        [[maybe_unused]] const Type::Char& r) {
        return true;
    }

    bool operator!=([[maybe_unused]] const Type::Int& l,
        [[maybe_unused]] const Type::Int& r) {
        return true;
    }

    bool operator!=([[maybe_unused]] const Type::Byte& l,
        [[maybe_unused]] const Type::Byte& r) {
        return true;
    }

    bool operator!=([[maybe_unused]] const Type::Float& l,
        [[maybe_unused]] const Type::Float& r) {
        return true;
    }

    bool operator!=([[maybe_unused]] const Type::Bool& l,
        [[maybe_unused]] const Type::Bool& r) {
        return true;
    }

    bool operator!=(const Type::Array& l, const Type::Array& r) {
        return l.is_invalid != r.is_invalid && l.type != r.type;
    }

    bool operator!=(const Type::Tup& l, const Type::Tup& r) {
        return l.is_invalid != r.is_invalid && l.types != r.types;
    }

    bool operator!=(const Type::Func& l, const Type::Func& r) {
        return l.is_invalid != r.is_invalid && l.params != r.params &&
               l.return_type != r.return_type;
    }

    bool operator!=(const Type::Path& l, const Type::Path& r) {
        return l.is_invalid != r.is_invalid && l.segments != r.segments;
    }

    bool operator!=(const Type::Ref& l, const Type::Ref& r) {
        return l.is_invalid != r.is_invalid && l.type != r.type;
    }

    bool operator!=(const Expr& l, const Expr& r) { return l.val != r.val; }

    bool operator!=(const Expr::Literal& l, const Expr::Literal& r) {
        return l.val != r.val;
    }

    bool operator!=([[maybe_unused]] const Expr::Literal::Unit& l,
        [[maybe_unused]] const Expr::Literal::Unit& r) {
        return true;
    }

    bool operator!=([[maybe_unused]] const Expr::Literal::Null& l,
        [[maybe_unused]] const Expr::Literal::Null& r) {
        return true;
    }

    bool operator!=(
        const Expr::Literal::String& l, const Expr::Literal::String& r) {
        return l.value != r.value;
    }

    bool operator!=(
        const Expr::Literal::Character& l, const Expr::Literal::Character& r) {
        return l.value != r.value;
    }

    bool operator!=(
        const Expr::Literal::Integer& l, const Expr::Literal::Integer& r) {
        return l.value != r.value;
    }

    bool operator!=(
        const Expr::Literal::Byte& l, const Expr::Literal::Byte& r) {
        return l.value != r.value;
    }

    bool operator!=(
        const Expr::Literal::Floating& l, const Expr::Literal::Floating& r) {
        return l.value != r.value;
    }

    bool operator!=(
        const Expr::Literal::Boolean& l, const Expr::Literal::Boolean& r) {
        return l.value != r.value;
    }

    bool operator!=(
        const Expr::Literal::Array& l, const Expr::Literal::Array& r) {
        return l.val != r.val;
    }

    bool operator!=(const Expr::Literal::Array::Regular& l,
        const Expr::Literal::Array::Regular& r) {
        return l.elements != r.elements;
    }

    bool operator!=(const Expr::Literal::Array::DefaultNTimes& l,
        const Expr::Literal::Array::DefaultNTimes& r) {
        return l.times != r.times;
    }

    bool operator!=(const Expr::Literal::Array::ExprNTimes& l,
        const Expr::Literal::Array::ExprNTimes& r) {
        return l.expr != r.expr && l.times != r.times;
    }

    bool operator!=(const Expr::Literal::Tup& l, const Expr::Literal::Tup& r) {
        return l.is_invalid != r.is_invalid && l.exprs != r.exprs;
    }

    bool operator!=(
        const Expr::Literal::Struct& l, const Expr::Literal::Struct& r) {
        return l.is_invalid != r.is_invalid && l.type != r.type &&
               l.fields != r.fields;
    }

    bool operator!=(const Expr::Literal::Struct::Field& l,
        const Expr::Literal::Struct::Field& r) {
        return l.is_invalid != r.is_invalid && l.name != r.name &&
               l.expr != r.expr;
    }

    bool operator!=(const Expr::Unop& l, const Expr::Unop& r) {
        return l.expr != r.expr && l.op != r.op;
    }

    bool operator!=(const Expr::Binop& l, const Expr::Binop& r) {
        return l.lhs != r.lhs && l.rhs != r.rhs && l.op != r.op;
    }

    bool operator!=(const Expr::Index& l, const Expr::Index& r) {
        return l.expr != r.expr && l.index != r.index;
    }

    bool operator!=(const Expr::Call& l, const Expr::Call& r) {
        return l.expr != r.expr && l.args != r.args;
    }

    bool operator!=(const Expr::Cast& l, const Expr::Cast& r) {
        return l.expr != r.expr && l.type != r.type;
    }

    bool operator!=(const Expr::TryCast& l, const Expr::TryCast& r) {
        return l.expr != r.expr && l.type != r.type;
    }

    bool operator!=(const Expr::Ref& l, const Expr::Ref& r) {
        return l.is_invalid != r.is_invalid && l.expr != r.expr;
    }

    bool operator!=(const Expr::Builtin& l, const Expr::Builtin& r) {
        return l.is_invalid != r.is_invalid && l.name != r.name &&
               l.generic_params != r.generic_params && l.args != r.args;
    }

    bool operator!=(const Block& l, const Block& r) {
        return l.is_invalid != r.is_invalid && l.stmts != r.stmts;
    }

    bool operator!=(const Stmt& l, const Stmt& r) { return l.val != r.val; }

    bool operator!=(const Stmt::Let& l, const Stmt::Let& r) {
        return l.is_invalid != r.is_invalid && l.name != r.name &&
               l.type != r.type && l.expr != r.expr;
    }

    bool operator!=(const Stmt::If& l, const Stmt::If& r) {
        return l.is_invalid != r.is_invalid && l.expr != r.expr &&
               l.main != r.main && l.elifs != r.elifs && l.else_ != r.else_;
    }

    bool operator!=(const Stmt::If::Elif& l, const Stmt::If::Elif& r) {
        return l.expr != r.expr && l.block != r.block;
    }

    bool operator!=(const Stmt::Switch& l, const Stmt::Switch& r) {
        return l.is_invalid != r.is_invalid && l.expr != r.expr &&
               l.cases != r.cases;
    }

    bool operator!=(const Stmt::Switch::Case& l, const Stmt::Switch::Case& r) {
        return l.is_invalid != r.is_invalid && l.expr != r.expr &&
               l.block != r.block;
    }

    bool operator!=(const Stmt::Loop& l, const Stmt::Loop& r) {
        return l.is_invalid != r.is_invalid && l.block != r.block;
    }

    bool operator!=(const Stmt::For& l, const Stmt::For& r) {
        return l.is_invalid != r.is_invalid && l.name != r.name &&
               l.expr != r.expr && l.block != r.block;
    }

    bool operator!=(const Stmt::While& l, const Stmt::While& r) {
        return l.is_invalid != r.is_invalid && l.expr != r.expr &&
               l.block != r.block;
    }

    bool operator!=(const Stmt::Return& l, const Stmt::Return& r) {
        return l.is_invalid != r.is_invalid && l.expr != r.expr;
    }

    bool operator!=(const Stmt::Defer& l, const Stmt::Defer& r) {
        return l.is_invalid != r.is_invalid && l.expr != r.expr;
    }

    bool operator!=([[maybe_unused]] const Stmt::Continue& l,
        [[maybe_unused]] const Stmt::Continue& r) {
        return true;
    }

    bool operator!=([[maybe_unused]] const Stmt::Break& l,
        [[maybe_unused]] const Stmt::Break& r) {
        return true;
    }

    bool operator!=(const Func& l, const Func& r) {
        return l.is_invalid != r.is_invalid && l.signature != r.signature &&
               l.block != r.block;
    }

    bool operator!=(const Func::Signature& l, const Func::Signature& r) {
        return l.is_invalid != r.is_invalid &&
               l.is_self_func != r.is_self_func && l.params != r.params &&
               l.ret != r.ret;
    }

    bool operator!=(
        const Func::Signature::Param& l, const Func::Signature::Param& r) {
        return l.is_invalid != r.is_invalid && l.name != r.name &&
               l.type != r.type;
    }

    bool operator!=(const Decl& l, const Decl& r) {
        return l.is_export != r.is_export &&
               l.is_decls_block != r.is_decls_block && l.name != r.name &&
               l.generic_params != r.generic_params && l.val != r.val;
    }

    bool operator!=(const Decl::GenericParam& l, const Decl::GenericParam& r) {
        return l.is_invalid != r.is_invalid && l.name != r.name &&
               l.interfaces != r.interfaces;
    }

    bool operator!=(const Decl::Struct& l, const Decl::Struct& r) {
        return l.is_invalid != r.is_invalid && l.fields != r.fields;
    }

    bool operator!=(
        const Decl::Struct::Field& l, const Decl::Struct::Field& r) {
        return l.is_invalid != r.is_invalid && l.name != r.name &&
               l.type != r.type && l.is_export != r.is_export;
    }

    bool operator!=(const Decl::Enum& l, const Decl::Enum& r) {
        return l.is_invalid != r.is_invalid && l.elements != r.elements;
    }

    bool operator!=(
        const Decl::Enum::Element& l, const Decl::Enum::Element& r) {
        return l.is_invalid != r.is_invalid && l.name != r.name;
    }

    bool operator!=(const Decl::TypeAlias& l, const Decl::TypeAlias& r) {
        return l.is_invalid != r.is_invalid && l.type != r.type;
    }

    bool operator!=(const Decl::Const& l, const Decl::Const& r) {
        return l.is_invalid != r.is_invalid && l.type != r.type &&
               l.expr != r.expr;
    }

    bool operator!=(const Decl::Decls& l, const Decl::Decls& r) {
        return l.is_invalid != r.is_invalid && l.type != r.type &&
               l.interface != r.interface && l.decls != r.decls;
    }

    bool operator!=(const Decl::Interface& l, const Decl::Interface& r) {
        return l.is_invalid != r.is_invalid && l.members != r.members;
    }

    bool operator!=(
        const Decl::Interface::Member& l, const Decl::Interface::Member& r) {
        return l.is_invalid != r.is_invalid && l.name != r.name &&
               l.generic_params != r.generic_params &&
               l.signature != r.signature;
    }

    bool operator!=(const TopLevel& l, const TopLevel& r) {
        return l.val != r.val;
    }

    bool operator!=(const Module& l, const Module& r) {
        return l.top_levels != r.top_levels;
    }

}

namespace std {

    std::size_t hash<alvo::ast::Invalid>::operator()(
        [[maybe_unused]] const alvo::ast::Invalid& n) const noexcept {
        return 0;
    }

    std::size_t hash<alvo::ast::PathSegment>::operator()(
        const alvo::ast::PathSegment& n) const noexcept {
        return std::hash<decltype(n.val)>()(n.val);
    }

    std::size_t hash<alvo::ast::PathSegment::Root>::operator()(
        [[maybe_unused]] const alvo::ast::PathSegment::Root& n) const noexcept {
        return 0;
    }

    std::size_t hash<alvo::ast::PathSegment::Super>::operator()(
        [[maybe_unused]] const alvo::ast::PathSegment::Super& n)
        const noexcept {
        return 0;
    }

    std::size_t hash<alvo::ast::PathSegment::Name>::operator()(
        const alvo::ast::PathSegment::Name& n) const noexcept {
        std::size_t value_hash = std::hash<decltype(n.value)>()(n.value);
        std::size_t generic_params_hash =
            std::hash<decltype(n.generic_params)>()(n.generic_params);
        std::size_t res = value_hash;
        res ^= generic_params_hash + 0x9e3779b97f4a7c15ull + (res << 6) +
               (res >> 2);
        return res;
    }

    std::size_t hash<alvo::ast::Import>::operator()(
        const alvo::ast::Import& n) const noexcept {
        std::size_t kind_hash = std::hash<decltype(n.kind)>()(n.kind);
        std::size_t segments_hash =
            std::hash<decltype(n.segments)>()(n.segments);
        std::size_t res = kind_hash;
        res ^= segments_hash + 0x9e3779b97f4a7c15ull + (res << 6) + (res >> 2);
        return res;
    }

    std::size_t hash<alvo::ast::Import::Normal>::operator()(
        [[maybe_unused]] const alvo::ast::Import::Normal& n) const noexcept {
        return 0;
    }

    std::size_t hash<alvo::ast::Import::Glob>::operator()(
        [[maybe_unused]] const alvo::ast::Import::Glob& n) const noexcept {
        return 0;
    }

    std::size_t hash<alvo::ast::Import::Renamed>::operator()(
        const alvo::ast::Import::Renamed& n) const noexcept {
        return std::hash<decltype(n.renamed_to)>()(n.renamed_to);
    }

    std::size_t hash<alvo::ast::Type>::operator()(
        const alvo::ast::Type& n) const noexcept {
        std::size_t val_hash = std::hash<decltype(n.val)>()(n.val);
        std::size_t nullable_hash =
            std::hash<decltype(n.nullable)>()(n.nullable);
        std::size_t res = val_hash;
        res ^= nullable_hash + 0x9e3779b97f4a7c15ull + (res << 6) + (res >> 2);
        return res;
    }

    std::size_t hash<alvo::ast::Type::Unit>::operator()(
        [[maybe_unused]] const alvo::ast::Type::Unit& n) const noexcept {
        return 0;
    }

    std::size_t hash<alvo::ast::Type::String>::operator()(
        [[maybe_unused]] const alvo::ast::Type::String& n) const noexcept {
        return 0;
    }

    std::size_t hash<alvo::ast::Type::Char>::operator()(
        [[maybe_unused]] const alvo::ast::Type::Char& n) const noexcept {
        return 0;
    }

    std::size_t hash<alvo::ast::Type::Int>::operator()(
        [[maybe_unused]] const alvo::ast::Type::Int& n) const noexcept {
        return 0;
    }

    std::size_t hash<alvo::ast::Type::Byte>::operator()(
        [[maybe_unused]] const alvo::ast::Type::Byte& n) const noexcept {
        return 0;
    }

    std::size_t hash<alvo::ast::Type::Float>::operator()(
        [[maybe_unused]] const alvo::ast::Type::Float& n) const noexcept {
        return 0;
    }

    std::size_t hash<alvo::ast::Type::Bool>::operator()(
        [[maybe_unused]] const alvo::ast::Type::Bool& n) const noexcept {
        return 0;
    }

    std::size_t hash<alvo::ast::Type::Array>::operator()(
        const alvo::ast::Type::Array& n) const noexcept {
        std::size_t is_invalid_hash =
            std::hash<decltype(n.is_invalid)>()(n.is_invalid);
        std::size_t type_hash = std::hash<decltype(n.type)>()(n.type);
        std::size_t res = is_invalid_hash;
        res ^= type_hash + 0x9e3779b97f4a7c15ull + (res << 6) + (res >> 2);
        return res;
    }

    std::size_t hash<alvo::ast::Type::Tup>::operator()(
        const alvo::ast::Type::Tup& n) const noexcept {
        std::size_t is_invalid_hash =
            std::hash<decltype(n.is_invalid)>()(n.is_invalid);
        std::size_t types_hash = std::hash<decltype(n.types)>()(n.types);
        std::size_t res = is_invalid_hash;
        res ^= types_hash + 0x9e3779b97f4a7c15ull + (res << 6) + (res >> 2);
        return res;
    }

    std::size_t hash<alvo::ast::Type::Func>::operator()(
        const alvo::ast::Type::Func& n) const noexcept {
        std::size_t is_invalid_hash =
            std::hash<decltype(n.is_invalid)>()(n.is_invalid);
        std::size_t params_hash = std::hash<decltype(n.params)>()(n.params);
        std::size_t return_type_hash =
            std::hash<decltype(n.return_type)>()(n.return_type);
        std::size_t res = is_invalid_hash;
        res ^= params_hash + 0x9e3779b97f4a7c15ull + (res << 6) + (res >> 2);
        res ^=
            return_type_hash + 0x9e3779b97f4a7c15ull + (res << 6) + (res >> 2);
        return res;
    }

    std::size_t hash<alvo::ast::Type::Path>::operator()(
        const alvo::ast::Type::Path& n) const noexcept {
        std::size_t is_invalid_hash =
            std::hash<decltype(n.is_invalid)>()(n.is_invalid);
        std::size_t segments_hash =
            std::hash<decltype(n.segments)>()(n.segments);
        std::size_t res = is_invalid_hash;
        res ^= segments_hash + 0x9e3779b97f4a7c15ull + (res << 6) + (res >> 2);
        return res;
    }

    std::size_t hash<alvo::ast::Type::Ref>::operator()(
        const alvo::ast::Type::Ref& n) const noexcept {
        std::size_t is_invalid_hash =
            std::hash<decltype(n.is_invalid)>()(n.is_invalid);
        std::size_t type_hash = std::hash<decltype(n.type)>()(n.type);
        std::size_t res = is_invalid_hash;
        res ^= type_hash + 0x9e3779b97f4a7c15ull + (res << 6) + (res >> 2);
        return res;
    }

    std::size_t hash<alvo::ast::Expr>::operator()(
        const alvo::ast::Expr& n) const noexcept {
        return std::hash<decltype(n.val)>()(n.val);
    }

    std::size_t hash<alvo::ast::Expr::Literal>::operator()(
        const alvo::ast::Expr::Literal& n) const noexcept {
        return std::hash<decltype(n.val)>()(n.val);
    }

    std::size_t hash<alvo::ast::Expr::Literal::Unit>::operator()(
        [[maybe_unused]] const alvo::ast::Expr::Literal::Unit& n)
        const noexcept {
        return 0;
    }

    std::size_t hash<alvo::ast::Expr::Literal::Null>::operator()(
        [[maybe_unused]] const alvo::ast::Expr::Literal::Null& n)
        const noexcept {
        return 0;
    }

    std::size_t hash<alvo::ast::Expr::Literal::String>::operator()(
        const alvo::ast::Expr::Literal::String& n) const noexcept {
        return std::hash<decltype(n.value)>()(n.value);
    }

    std::size_t hash<alvo::ast::Expr::Literal::Character>::operator()(
        const alvo::ast::Expr::Literal::Character& n) const noexcept {
        return std::hash<decltype(n.value)>()(n.value);
    }

    std::size_t hash<alvo::ast::Expr::Literal::Integer>::operator()(
        const alvo::ast::Expr::Literal::Integer& n) const noexcept {
        return std::hash<decltype(n.value)>()(n.value);
    }

    std::size_t hash<alvo::ast::Expr::Literal::Byte>::operator()(
        const alvo::ast::Expr::Literal::Byte& n) const noexcept {
        return std::hash<decltype(n.value)>()(n.value);
    }

    std::size_t hash<alvo::ast::Expr::Literal::Floating>::operator()(
        const alvo::ast::Expr::Literal::Floating& n) const noexcept {
        return std::hash<decltype(n.value)>()(n.value);
    }

    std::size_t hash<alvo::ast::Expr::Literal::Boolean>::operator()(
        const alvo::ast::Expr::Literal::Boolean& n) const noexcept {
        return std::hash<decltype(n.value)>()(n.value);
    }

    std::size_t hash<alvo::ast::Expr::Literal::Array>::operator()(
        const alvo::ast::Expr::Literal::Array& n) const noexcept {
        return std::hash<decltype(n.val)>()(n.val);
    }

    std::size_t hash<alvo::ast::Expr::Literal::Array::Regular>::operator()(
        const alvo::ast::Expr::Literal::Array::Regular& n) const noexcept {
        return std::hash<decltype(n.elements)>()(n.elements);
    }

    std::size_t
    hash<alvo::ast::Expr::Literal::Array::DefaultNTimes>::operator()(
        const alvo::ast::Expr::Literal::Array::DefaultNTimes& n)
        const noexcept {
        return std::hash<decltype(n.times)>()(n.times);
    }

    std::size_t hash<alvo::ast::Expr::Literal::Array::ExprNTimes>::operator()(
        const alvo::ast::Expr::Literal::Array::ExprNTimes& n) const noexcept {
        std::size_t expr_hash = std::hash<decltype(n.expr)>()(n.expr);
        std::size_t times_hash = std::hash<decltype(n.times)>()(n.times);
        std::size_t res = expr_hash;
        res ^= times_hash + 0x9e3779b97f4a7c15ull + (res << 6) + (res >> 2);
        return res;
    }

    std::size_t hash<alvo::ast::Expr::Literal::Tup>::operator()(
        const alvo::ast::Expr::Literal::Tup& n) const noexcept {
        std::size_t is_invalid_hash =
            std::hash<decltype(n.is_invalid)>()(n.is_invalid);
        std::size_t exprs_hash = std::hash<decltype(n.exprs)>()(n.exprs);
        std::size_t res = is_invalid_hash;
        res ^= exprs_hash + 0x9e3779b97f4a7c15ull + (res << 6) + (res >> 2);
        return res;
    }

    std::size_t hash<alvo::ast::Expr::Literal::Struct>::operator()(
        const alvo::ast::Expr::Literal::Struct& n) const noexcept {
        std::size_t is_invalid_hash =
            std::hash<decltype(n.is_invalid)>()(n.is_invalid);
        std::size_t type_hash = std::hash<decltype(n.type)>()(n.type);
        std::size_t fields_hash = std::hash<decltype(n.fields)>()(n.fields);
        std::size_t res = is_invalid_hash;
        res ^= type_hash + 0x9e3779b97f4a7c15ull + (res << 6) + (res >> 2);
        res ^= fields_hash + 0x9e3779b97f4a7c15ull + (res << 6) + (res >> 2);
        return res;
    }

    std::size_t hash<alvo::ast::Expr::Literal::Struct::Field>::operator()(
        const alvo::ast::Expr::Literal::Struct::Field& n) const noexcept {
        std::size_t is_invalid_hash =
            std::hash<decltype(n.is_invalid)>()(n.is_invalid);
        std::size_t name_hash = std::hash<decltype(n.name)>()(n.name);
        std::size_t expr_hash = std::hash<decltype(n.expr)>()(n.expr);
        std::size_t res = is_invalid_hash;
        res ^= name_hash + 0x9e3779b97f4a7c15ull + (res << 6) + (res >> 2);
        res ^= expr_hash + 0x9e3779b97f4a7c15ull + (res << 6) + (res >> 2);
        return res;
    }

    std::size_t hash<alvo::ast::Expr::Unop>::operator()(
        const alvo::ast::Expr::Unop& n) const noexcept {
        std::size_t expr_hash = std::hash<decltype(n.expr)>()(n.expr);
        std::size_t op_hash = std::hash<decltype(n.op)>()(n.op);
        std::size_t res = expr_hash;
        res ^= op_hash + 0x9e3779b97f4a7c15ull + (res << 6) + (res >> 2);
        return res;
    }

    std::size_t hash<alvo::ast::Expr::Unop::Op>::operator()(
        [[maybe_unused]] const alvo::ast::Expr::Unop::Op& n) const noexcept {
        return 0;
    }

    std::size_t hash<alvo::ast::Expr::Binop>::operator()(
        const alvo::ast::Expr::Binop& n) const noexcept {
        std::size_t lhs_hash = std::hash<decltype(n.lhs)>()(n.lhs);
        std::size_t rhs_hash = std::hash<decltype(n.rhs)>()(n.rhs);
        std::size_t op_hash = std::hash<decltype(n.op)>()(n.op);
        std::size_t res = lhs_hash;
        res ^= rhs_hash + 0x9e3779b97f4a7c15ull + (res << 6) + (res >> 2);
        res ^= op_hash + 0x9e3779b97f4a7c15ull + (res << 6) + (res >> 2);
        return res;
    }

    std::size_t hash<alvo::ast::Expr::Binop::Op>::operator()(
        [[maybe_unused]] const alvo::ast::Expr::Binop::Op& n) const noexcept {
        return 0;
    }

    std::size_t hash<alvo::ast::Expr::Index>::operator()(
        const alvo::ast::Expr::Index& n) const noexcept {
        std::size_t expr_hash = std::hash<decltype(n.expr)>()(n.expr);
        std::size_t index_hash = std::hash<decltype(n.index)>()(n.index);
        std::size_t res = expr_hash;
        res ^= index_hash + 0x9e3779b97f4a7c15ull + (res << 6) + (res >> 2);
        return res;
    }

    std::size_t hash<alvo::ast::Expr::Call>::operator()(
        const alvo::ast::Expr::Call& n) const noexcept {
        std::size_t expr_hash = std::hash<decltype(n.expr)>()(n.expr);
        std::size_t args_hash = std::hash<decltype(n.args)>()(n.args);
        std::size_t res = expr_hash;
        res ^= args_hash + 0x9e3779b97f4a7c15ull + (res << 6) + (res >> 2);
        return res;
    }

    std::size_t hash<alvo::ast::Expr::Cast>::operator()(
        const alvo::ast::Expr::Cast& n) const noexcept {
        std::size_t expr_hash = std::hash<decltype(n.expr)>()(n.expr);
        std::size_t type_hash = std::hash<decltype(n.type)>()(n.type);
        std::size_t res = expr_hash;
        res ^= type_hash + 0x9e3779b97f4a7c15ull + (res << 6) + (res >> 2);
        return res;
    }

    std::size_t hash<alvo::ast::Expr::TryCast>::operator()(
        const alvo::ast::Expr::TryCast& n) const noexcept {
        std::size_t expr_hash = std::hash<decltype(n.expr)>()(n.expr);
        std::size_t type_hash = std::hash<decltype(n.type)>()(n.type);
        std::size_t res = expr_hash;
        res ^= type_hash + 0x9e3779b97f4a7c15ull + (res << 6) + (res >> 2);
        return res;
    }

    std::size_t hash<alvo::ast::Expr::Ref>::operator()(
        const alvo::ast::Expr::Ref& n) const noexcept {
        std::size_t is_invalid_hash =
            std::hash<decltype(n.is_invalid)>()(n.is_invalid);
        std::size_t expr_hash = std::hash<decltype(n.expr)>()(n.expr);
        std::size_t res = is_invalid_hash;
        res ^= expr_hash + 0x9e3779b97f4a7c15ull + (res << 6) + (res >> 2);
        return res;
    }

    std::size_t hash<alvo::ast::Expr::Builtin>::operator()(
        const alvo::ast::Expr::Builtin& n) const noexcept {
        std::size_t is_invalid_hash =
            std::hash<decltype(n.is_invalid)>()(n.is_invalid);
        std::size_t name_hash = std::hash<decltype(n.name)>()(n.name);
        std::size_t generic_params_hash =
            std::hash<decltype(n.generic_params)>()(n.generic_params);
        std::size_t args_hash = std::hash<decltype(n.args)>()(n.args);
        std::size_t res = is_invalid_hash;
        res ^= name_hash + 0x9e3779b97f4a7c15ull + (res << 6) + (res >> 2);
        res ^= generic_params_hash + 0x9e3779b97f4a7c15ull + (res << 6) +
               (res >> 2);
        res ^= args_hash + 0x9e3779b97f4a7c15ull + (res << 6) + (res >> 2);
        return res;
    }

    std::size_t hash<alvo::ast::Block>::operator()(
        const alvo::ast::Block& n) const noexcept {
        std::size_t is_invalid_hash =
            std::hash<decltype(n.is_invalid)>()(n.is_invalid);
        std::size_t stmts_hash = std::hash<decltype(n.stmts)>()(n.stmts);
        std::size_t res = is_invalid_hash;
        res ^= stmts_hash + 0x9e3779b97f4a7c15ull + (res << 6) + (res >> 2);
        return res;
    }

    std::size_t hash<alvo::ast::Stmt>::operator()(
        const alvo::ast::Stmt& n) const noexcept {
        return std::hash<decltype(n.val)>()(n.val);
    }

    std::size_t hash<alvo::ast::Stmt::Let>::operator()(
        const alvo::ast::Stmt::Let& n) const noexcept {
        std::size_t is_invalid_hash =
            std::hash<decltype(n.is_invalid)>()(n.is_invalid);
        std::size_t name_hash = std::hash<decltype(n.name)>()(n.name);
        std::size_t type_hash = std::hash<decltype(n.type)>()(n.type);
        std::size_t expr_hash = std::hash<decltype(n.expr)>()(n.expr);
        std::size_t res = is_invalid_hash;
        res ^= name_hash + 0x9e3779b97f4a7c15ull + (res << 6) + (res >> 2);
        res ^= type_hash + 0x9e3779b97f4a7c15ull + (res << 6) + (res >> 2);
        res ^= expr_hash + 0x9e3779b97f4a7c15ull + (res << 6) + (res >> 2);
        return res;
    }

    std::size_t hash<alvo::ast::Stmt::If>::operator()(
        const alvo::ast::Stmt::If& n) const noexcept {
        std::size_t is_invalid_hash =
            std::hash<decltype(n.is_invalid)>()(n.is_invalid);
        std::size_t expr_hash = std::hash<decltype(n.expr)>()(n.expr);
        std::size_t main_hash = std::hash<decltype(n.main)>()(n.main);
        std::size_t elifs_hash = std::hash<decltype(n.elifs)>()(n.elifs);
        std::size_t else__hash = std::hash<decltype(n.else_)>()(n.else_);
        std::size_t res = is_invalid_hash;
        res ^= expr_hash + 0x9e3779b97f4a7c15ull + (res << 6) + (res >> 2);
        res ^= main_hash + 0x9e3779b97f4a7c15ull + (res << 6) + (res >> 2);
        res ^= elifs_hash + 0x9e3779b97f4a7c15ull + (res << 6) + (res >> 2);
        res ^= else__hash + 0x9e3779b97f4a7c15ull + (res << 6) + (res >> 2);
        return res;
    }

    std::size_t hash<alvo::ast::Stmt::If::Elif>::operator()(
        const alvo::ast::Stmt::If::Elif& n) const noexcept {
        std::size_t expr_hash = std::hash<decltype(n.expr)>()(n.expr);
        std::size_t block_hash = std::hash<decltype(n.block)>()(n.block);
        std::size_t res = expr_hash;
        res ^= block_hash + 0x9e3779b97f4a7c15ull + (res << 6) + (res >> 2);
        return res;
    }

    std::size_t hash<alvo::ast::Stmt::Switch>::operator()(
        const alvo::ast::Stmt::Switch& n) const noexcept {
        std::size_t is_invalid_hash =
            std::hash<decltype(n.is_invalid)>()(n.is_invalid);
        std::size_t expr_hash = std::hash<decltype(n.expr)>()(n.expr);
        std::size_t cases_hash = std::hash<decltype(n.cases)>()(n.cases);
        std::size_t res = is_invalid_hash;
        res ^= expr_hash + 0x9e3779b97f4a7c15ull + (res << 6) + (res >> 2);
        res ^= cases_hash + 0x9e3779b97f4a7c15ull + (res << 6) + (res >> 2);
        return res;
    }

    std::size_t hash<alvo::ast::Stmt::Switch::Case>::operator()(
        const alvo::ast::Stmt::Switch::Case& n) const noexcept {
        std::size_t is_invalid_hash =
            std::hash<decltype(n.is_invalid)>()(n.is_invalid);
        std::size_t expr_hash = std::hash<decltype(n.expr)>()(n.expr);
        std::size_t block_hash = std::hash<decltype(n.block)>()(n.block);
        std::size_t res = is_invalid_hash;
        res ^= expr_hash + 0x9e3779b97f4a7c15ull + (res << 6) + (res >> 2);
        res ^= block_hash + 0x9e3779b97f4a7c15ull + (res << 6) + (res >> 2);
        return res;
    }

    std::size_t hash<alvo::ast::Stmt::Loop>::operator()(
        const alvo::ast::Stmt::Loop& n) const noexcept {
        std::size_t is_invalid_hash =
            std::hash<decltype(n.is_invalid)>()(n.is_invalid);
        std::size_t block_hash = std::hash<decltype(n.block)>()(n.block);
        std::size_t res = is_invalid_hash;
        res ^= block_hash + 0x9e3779b97f4a7c15ull + (res << 6) + (res >> 2);
        return res;
    }

    std::size_t hash<alvo::ast::Stmt::For>::operator()(
        const alvo::ast::Stmt::For& n) const noexcept {
        std::size_t is_invalid_hash =
            std::hash<decltype(n.is_invalid)>()(n.is_invalid);
        std::size_t name_hash = std::hash<decltype(n.name)>()(n.name);
        std::size_t expr_hash = std::hash<decltype(n.expr)>()(n.expr);
        std::size_t block_hash = std::hash<decltype(n.block)>()(n.block);
        std::size_t res = is_invalid_hash;
        res ^= name_hash + 0x9e3779b97f4a7c15ull + (res << 6) + (res >> 2);
        res ^= expr_hash + 0x9e3779b97f4a7c15ull + (res << 6) + (res >> 2);
        res ^= block_hash + 0x9e3779b97f4a7c15ull + (res << 6) + (res >> 2);
        return res;
    }

    std::size_t hash<alvo::ast::Stmt::While>::operator()(
        const alvo::ast::Stmt::While& n) const noexcept {
        std::size_t is_invalid_hash =
            std::hash<decltype(n.is_invalid)>()(n.is_invalid);
        std::size_t expr_hash = std::hash<decltype(n.expr)>()(n.expr);
        std::size_t block_hash = std::hash<decltype(n.block)>()(n.block);
        std::size_t res = is_invalid_hash;
        res ^= expr_hash + 0x9e3779b97f4a7c15ull + (res << 6) + (res >> 2);
        res ^= block_hash + 0x9e3779b97f4a7c15ull + (res << 6) + (res >> 2);
        return res;
    }

    std::size_t hash<alvo::ast::Stmt::Return>::operator()(
        const alvo::ast::Stmt::Return& n) const noexcept {
        std::size_t is_invalid_hash =
            std::hash<decltype(n.is_invalid)>()(n.is_invalid);
        std::size_t expr_hash = std::hash<decltype(n.expr)>()(n.expr);
        std::size_t res = is_invalid_hash;
        res ^= expr_hash + 0x9e3779b97f4a7c15ull + (res << 6) + (res >> 2);
        return res;
    }

    std::size_t hash<alvo::ast::Stmt::Defer>::operator()(
        const alvo::ast::Stmt::Defer& n) const noexcept {
        std::size_t is_invalid_hash =
            std::hash<decltype(n.is_invalid)>()(n.is_invalid);
        std::size_t expr_hash = std::hash<decltype(n.expr)>()(n.expr);
        std::size_t res = is_invalid_hash;
        res ^= expr_hash + 0x9e3779b97f4a7c15ull + (res << 6) + (res >> 2);
        return res;
    }

    std::size_t hash<alvo::ast::Stmt::Continue>::operator()(
        [[maybe_unused]] const alvo::ast::Stmt::Continue& n) const noexcept {
        return 0;
    }

    std::size_t hash<alvo::ast::Stmt::Break>::operator()(
        [[maybe_unused]] const alvo::ast::Stmt::Break& n) const noexcept {
        return 0;
    }

    std::size_t hash<alvo::ast::Func>::operator()(
        const alvo::ast::Func& n) const noexcept {
        std::size_t is_invalid_hash =
            std::hash<decltype(n.is_invalid)>()(n.is_invalid);
        std::size_t signature_hash =
            std::hash<decltype(n.signature)>()(n.signature);
        std::size_t block_hash = std::hash<decltype(n.block)>()(n.block);
        std::size_t res = is_invalid_hash;
        res ^= signature_hash + 0x9e3779b97f4a7c15ull + (res << 6) + (res >> 2);
        res ^= block_hash + 0x9e3779b97f4a7c15ull + (res << 6) + (res >> 2);
        return res;
    }

    std::size_t hash<alvo::ast::Func::Signature>::operator()(
        const alvo::ast::Func::Signature& n) const noexcept {
        std::size_t is_invalid_hash =
            std::hash<decltype(n.is_invalid)>()(n.is_invalid);
        std::size_t is_self_func_hash =
            std::hash<decltype(n.is_self_func)>()(n.is_self_func);
        std::size_t params_hash = std::hash<decltype(n.params)>()(n.params);
        std::size_t ret_hash = std::hash<decltype(n.ret)>()(n.ret);
        std::size_t res = is_invalid_hash;
        res ^=
            is_self_func_hash + 0x9e3779b97f4a7c15ull + (res << 6) + (res >> 2);
        res ^= params_hash + 0x9e3779b97f4a7c15ull + (res << 6) + (res >> 2);
        res ^= ret_hash + 0x9e3779b97f4a7c15ull + (res << 6) + (res >> 2);
        return res;
    }

    std::size_t hash<alvo::ast::Func::Signature::Param>::operator()(
        const alvo::ast::Func::Signature::Param& n) const noexcept {
        std::size_t is_invalid_hash =
            std::hash<decltype(n.is_invalid)>()(n.is_invalid);
        std::size_t name_hash = std::hash<decltype(n.name)>()(n.name);
        std::size_t type_hash = std::hash<decltype(n.type)>()(n.type);
        std::size_t res = is_invalid_hash;
        res ^= name_hash + 0x9e3779b97f4a7c15ull + (res << 6) + (res >> 2);
        res ^= type_hash + 0x9e3779b97f4a7c15ull + (res << 6) + (res >> 2);
        return res;
    }

    std::size_t hash<alvo::ast::Decl>::operator()(
        const alvo::ast::Decl& n) const noexcept {
        std::size_t is_export_hash =
            std::hash<decltype(n.is_export)>()(n.is_export);
        std::size_t is_decls_block_hash =
            std::hash<decltype(n.is_decls_block)>()(n.is_decls_block);
        std::size_t name_hash = std::hash<decltype(n.name)>()(n.name);
        std::size_t generic_params_hash =
            std::hash<decltype(n.generic_params)>()(n.generic_params);
        std::size_t val_hash = std::hash<decltype(n.val)>()(n.val);
        std::size_t res = is_export_hash;
        res ^= is_decls_block_hash + 0x9e3779b97f4a7c15ull + (res << 6) +
               (res >> 2);
        res ^= name_hash + 0x9e3779b97f4a7c15ull + (res << 6) + (res >> 2);
        res ^= generic_params_hash + 0x9e3779b97f4a7c15ull + (res << 6) +
               (res >> 2);
        res ^= val_hash + 0x9e3779b97f4a7c15ull + (res << 6) + (res >> 2);
        return res;
    }

    std::size_t hash<alvo::ast::Decl::GenericParam>::operator()(
        const alvo::ast::Decl::GenericParam& n) const noexcept {
        std::size_t is_invalid_hash =
            std::hash<decltype(n.is_invalid)>()(n.is_invalid);
        std::size_t name_hash = std::hash<decltype(n.name)>()(n.name);
        std::size_t interfaces_hash =
            std::hash<decltype(n.interfaces)>()(n.interfaces);
        std::size_t res = is_invalid_hash;
        res ^= name_hash + 0x9e3779b97f4a7c15ull + (res << 6) + (res >> 2);
        res ^=
            interfaces_hash + 0x9e3779b97f4a7c15ull + (res << 6) + (res >> 2);
        return res;
    }

    std::size_t hash<alvo::ast::Decl::Struct>::operator()(
        const alvo::ast::Decl::Struct& n) const noexcept {
        std::size_t is_invalid_hash =
            std::hash<decltype(n.is_invalid)>()(n.is_invalid);
        std::size_t fields_hash = std::hash<decltype(n.fields)>()(n.fields);
        std::size_t res = is_invalid_hash;
        res ^= fields_hash + 0x9e3779b97f4a7c15ull + (res << 6) + (res >> 2);
        return res;
    }

    std::size_t hash<alvo::ast::Decl::Struct::Field>::operator()(
        const alvo::ast::Decl::Struct::Field& n) const noexcept {
        std::size_t is_invalid_hash =
            std::hash<decltype(n.is_invalid)>()(n.is_invalid);
        std::size_t name_hash = std::hash<decltype(n.name)>()(n.name);
        std::size_t type_hash = std::hash<decltype(n.type)>()(n.type);
        std::size_t is_export_hash =
            std::hash<decltype(n.is_export)>()(n.is_export);
        std::size_t res = is_invalid_hash;
        res ^= name_hash + 0x9e3779b97f4a7c15ull + (res << 6) + (res >> 2);
        res ^= type_hash + 0x9e3779b97f4a7c15ull + (res << 6) + (res >> 2);
        res ^= is_export_hash + 0x9e3779b97f4a7c15ull + (res << 6) + (res >> 2);
        return res;
    }

    std::size_t hash<alvo::ast::Decl::Enum>::operator()(
        const alvo::ast::Decl::Enum& n) const noexcept {
        std::size_t is_invalid_hash =
            std::hash<decltype(n.is_invalid)>()(n.is_invalid);
        std::size_t elements_hash =
            std::hash<decltype(n.elements)>()(n.elements);
        std::size_t res = is_invalid_hash;
        res ^= elements_hash + 0x9e3779b97f4a7c15ull + (res << 6) + (res >> 2);
        return res;
    }

    std::size_t hash<alvo::ast::Decl::Enum::Element>::operator()(
        const alvo::ast::Decl::Enum::Element& n) const noexcept {
        std::size_t is_invalid_hash =
            std::hash<decltype(n.is_invalid)>()(n.is_invalid);
        std::size_t name_hash = std::hash<decltype(n.name)>()(n.name);
        std::size_t res = is_invalid_hash;
        res ^= name_hash + 0x9e3779b97f4a7c15ull + (res << 6) + (res >> 2);
        return res;
    }

    std::size_t hash<alvo::ast::Decl::TypeAlias>::operator()(
        const alvo::ast::Decl::TypeAlias& n) const noexcept {
        std::size_t is_invalid_hash =
            std::hash<decltype(n.is_invalid)>()(n.is_invalid);
        std::size_t type_hash = std::hash<decltype(n.type)>()(n.type);
        std::size_t res = is_invalid_hash;
        res ^= type_hash + 0x9e3779b97f4a7c15ull + (res << 6) + (res >> 2);
        return res;
    }

    std::size_t hash<alvo::ast::Decl::Const>::operator()(
        const alvo::ast::Decl::Const& n) const noexcept {
        std::size_t is_invalid_hash =
            std::hash<decltype(n.is_invalid)>()(n.is_invalid);
        std::size_t type_hash = std::hash<decltype(n.type)>()(n.type);
        std::size_t expr_hash = std::hash<decltype(n.expr)>()(n.expr);
        std::size_t res = is_invalid_hash;
        res ^= type_hash + 0x9e3779b97f4a7c15ull + (res << 6) + (res >> 2);
        res ^= expr_hash + 0x9e3779b97f4a7c15ull + (res << 6) + (res >> 2);
        return res;
    }

    std::size_t hash<alvo::ast::Decl::Decls>::operator()(
        const alvo::ast::Decl::Decls& n) const noexcept {
        std::size_t is_invalid_hash =
            std::hash<decltype(n.is_invalid)>()(n.is_invalid);
        std::size_t type_hash = std::hash<decltype(n.type)>()(n.type);
        std::size_t interface_hash =
            std::hash<decltype(n.interface)>()(n.interface);
        std::size_t decls_hash = std::hash<decltype(n.decls)>()(n.decls);
        std::size_t res = is_invalid_hash;
        res ^= type_hash + 0x9e3779b97f4a7c15ull + (res << 6) + (res >> 2);
        res ^= interface_hash + 0x9e3779b97f4a7c15ull + (res << 6) + (res >> 2);
        res ^= decls_hash + 0x9e3779b97f4a7c15ull + (res << 6) + (res >> 2);
        return res;
    }

    std::size_t hash<alvo::ast::Decl::Interface>::operator()(
        const alvo::ast::Decl::Interface& n) const noexcept {
        std::size_t is_invalid_hash =
            std::hash<decltype(n.is_invalid)>()(n.is_invalid);
        std::size_t members_hash = std::hash<decltype(n.members)>()(n.members);
        std::size_t res = is_invalid_hash;
        res ^= members_hash + 0x9e3779b97f4a7c15ull + (res << 6) + (res >> 2);
        return res;
    }

    std::size_t hash<alvo::ast::Decl::Interface::Member>::operator()(
        const alvo::ast::Decl::Interface::Member& n) const noexcept {
        std::size_t is_invalid_hash =
            std::hash<decltype(n.is_invalid)>()(n.is_invalid);
        std::size_t name_hash = std::hash<decltype(n.name)>()(n.name);
        std::size_t generic_params_hash =
            std::hash<decltype(n.generic_params)>()(n.generic_params);
        std::size_t signature_hash =
            std::hash<decltype(n.signature)>()(n.signature);
        std::size_t res = is_invalid_hash;
        res ^= name_hash + 0x9e3779b97f4a7c15ull + (res << 6) + (res >> 2);
        res ^= generic_params_hash + 0x9e3779b97f4a7c15ull + (res << 6) +
               (res >> 2);
        res ^= signature_hash + 0x9e3779b97f4a7c15ull + (res << 6) + (res >> 2);
        return res;
    }

    std::size_t hash<alvo::ast::TopLevel>::operator()(
        const alvo::ast::TopLevel& n) const noexcept {
        return std::hash<decltype(n.val)>()(n.val);
    }

    std::size_t hash<alvo::ast::Module>::operator()(
        const alvo::ast::Module& n) const noexcept {
        return std::hash<decltype(n.top_levels)>()(n.top_levels);
    }

}
