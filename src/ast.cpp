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

    bool operator==(const Id& l, const Id& r) { return l.id == r.id; }

    bool operator==(const Type& l, const Type& r) { return l.val == r.val; }

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

    bool operator==(const Type::Name& l, const Type::Name& r) {
        return l.is_invalid == r.is_invalid && l.name == r.name &&
               l.generic_params == r.generic_params;
    }

    bool operator==(const Type::LocalGeneric& l, const Type::LocalGeneric& r) {
        return l.id == r.id && l.name == r.name;
    }

    bool operator==(const Type::ResolvedUserDefinedType& l,
        const Type::ResolvedUserDefinedType& r) {
        return l.id == r.id && l.generic_params == r.generic_params;
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

    bool operator==(const Expr::Literal::ResolvedStruct& l,
        const Expr::Literal::ResolvedStruct& r) {
        return l.type_id == r.type_id && l.generic_params == r.generic_params &&
               l.fields == r.fields;
    }

    bool operator==(const Expr::Literal::ResolvedStruct::Field& l,
        const Expr::Literal::ResolvedStruct::Field& r) {
        return l.member_id == r.member_id && l.expr == r.expr &&
               l.type == r.type;
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

    bool operator==(const Expr::Builtin& l, const Expr::Builtin& r) {
        return l.is_invalid == r.is_invalid && l.name == r.name &&
               l.generic_params == r.generic_params && l.args == r.args;
    }

    bool operator==(const Expr::Name& l, const Expr::Name& r) {
        return l.is_invalid == r.is_invalid && l.name == r.name &&
               l.generic_params == r.generic_params;
    }

    bool operator==(
        const Expr::TypeMemberAccess& l, const Expr::TypeMemberAccess& r) {
        return l.is_invalid == r.is_invalid && l.type == r.type &&
               l.name == r.name;
    }

    bool operator==(const Expr::MemberAccess& l, const Expr::MemberAccess& r) {
        return l.expr == r.expr && l.name == r.name;
    }

    bool operator==(const Expr::LocalVar& l, const Expr::LocalVar& r) {
        return l.id == r.id;
    }

    bool operator==(const Expr::FuncArg& l, const Expr::FuncArg& r) {
        return l.name == r.name;
    }

    bool operator==(const Expr::ResolvedDecl& l, const Expr::ResolvedDecl& r) {
        return l.decl_id == r.decl_id && l.generic_params == r.generic_params;
    }

    bool operator==(const Expr::ResolvedTypeMemberAccess& l,
        const Expr::ResolvedTypeMemberAccess& r) {
        return l.type == r.type && l.member_id == r.member_id &&
               l.generic_params == r.generic_params;
    }

    bool operator==(const Expr::ResolvedMemberAccess& l,
        const Expr::ResolvedMemberAccess& r) {
        return l.expr == r.expr && l.member_id == r.member_id;
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

    bool operator==(const Stmt::Print& l, const Stmt::Print& r) {
        return l.is_invalid == r.is_invalid && l.exprs == r.exprs;
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
        return l.is_export == r.is_export && l.name == r.name &&
               l.generic_params == r.generic_params && l.val == r.val;
    }

    bool operator==(const Decl::GenericParam& l, const Decl::GenericParam& r) {
        return l.is_invalid == r.is_invalid && l.name == r.name &&
               l.interfaces == r.interfaces;
    }

    bool operator==(const Decl::DeclsBlock& l, const Decl::DeclsBlock& r) {
        return l.is_invalid == r.is_invalid &&
               l.generic_params == r.generic_params &&
               l.interface == r.interface && l.decls == r.decls;
    }

    bool operator==(const Decl::Struct& l, const Decl::Struct& r) {
        return l.is_invalid == r.is_invalid && l.fields == r.fields &&
               l.decls_blocks == r.decls_blocks;
    }

    bool operator==(
        const Decl::Struct::Field& l, const Decl::Struct::Field& r) {
        return l.is_invalid == r.is_invalid && l.name == r.name &&
               l.type == r.type && l.is_export == r.is_export;
    }

    bool operator==(const Decl::Enum& l, const Decl::Enum& r) {
        return l.is_invalid == r.is_invalid && l.elements == r.elements &&
               l.decls_blocks == r.decls_blocks;
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

    bool operator==(const Decl::Interface& l, const Decl::Interface& r) {
        return l.is_invalid == r.is_invalid && l.members == r.members;
    }

    bool operator==(
        const Decl::Interface::Member& l, const Decl::Interface::Member& r) {
        return l.is_invalid == r.is_invalid && l.name == r.name &&
               l.generic_params == r.generic_params &&
               l.signature == r.signature;
    }

    bool operator==(const Module& l, const Module& r) {
        return l.decls == r.decls;
    }

    bool operator!=(
        [[maybe_unused]] const Invalid& l, [[maybe_unused]] const Invalid& r) {
        return false;
    }

    bool operator!=(const Id& l, const Id& r) { return l.id != r.id; }

    bool operator!=(const Type& l, const Type& r) { return l.val != r.val; }

    bool operator!=([[maybe_unused]] const Type::Unit& l,
        [[maybe_unused]] const Type::Unit& r) {
        return false;
    }

    bool operator!=([[maybe_unused]] const Type::String& l,
        [[maybe_unused]] const Type::String& r) {
        return false;
    }

    bool operator!=([[maybe_unused]] const Type::Char& l,
        [[maybe_unused]] const Type::Char& r) {
        return false;
    }

    bool operator!=([[maybe_unused]] const Type::Int& l,
        [[maybe_unused]] const Type::Int& r) {
        return false;
    }

    bool operator!=([[maybe_unused]] const Type::Byte& l,
        [[maybe_unused]] const Type::Byte& r) {
        return false;
    }

    bool operator!=([[maybe_unused]] const Type::Float& l,
        [[maybe_unused]] const Type::Float& r) {
        return false;
    }

    bool operator!=([[maybe_unused]] const Type::Bool& l,
        [[maybe_unused]] const Type::Bool& r) {
        return false;
    }

    bool operator!=(const Type::Array& l, const Type::Array& r) {
        return l.is_invalid != r.is_invalid || l.type != r.type;
    }

    bool operator!=(const Type::Tup& l, const Type::Tup& r) {
        return l.is_invalid != r.is_invalid || l.types != r.types;
    }

    bool operator!=(const Type::Func& l, const Type::Func& r) {
        return l.is_invalid != r.is_invalid || l.params != r.params ||
               l.return_type != r.return_type;
    }

    bool operator!=(const Type::Name& l, const Type::Name& r) {
        return l.is_invalid != r.is_invalid || l.name != r.name ||
               l.generic_params != r.generic_params;
    }

    bool operator!=(const Type::LocalGeneric& l, const Type::LocalGeneric& r) {
        return l.id != r.id || l.name != r.name;
    }

    bool operator!=(const Type::ResolvedUserDefinedType& l,
        const Type::ResolvedUserDefinedType& r) {
        return l.id != r.id || l.generic_params != r.generic_params;
    }

    bool operator!=(const Expr& l, const Expr& r) { return l.val != r.val; }

    bool operator!=(const Expr::Literal& l, const Expr::Literal& r) {
        return l.val != r.val;
    }

    bool operator!=([[maybe_unused]] const Expr::Literal::Unit& l,
        [[maybe_unused]] const Expr::Literal::Unit& r) {
        return false;
    }

    bool operator!=([[maybe_unused]] const Expr::Literal::Null& l,
        [[maybe_unused]] const Expr::Literal::Null& r) {
        return false;
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
        return l.expr != r.expr || l.times != r.times;
    }

    bool operator!=(const Expr::Literal::Tup& l, const Expr::Literal::Tup& r) {
        return l.is_invalid != r.is_invalid || l.exprs != r.exprs;
    }

    bool operator!=(
        const Expr::Literal::Struct& l, const Expr::Literal::Struct& r) {
        return l.is_invalid != r.is_invalid || l.type != r.type ||
               l.fields != r.fields;
    }

    bool operator!=(const Expr::Literal::Struct::Field& l,
        const Expr::Literal::Struct::Field& r) {
        return l.is_invalid != r.is_invalid || l.name != r.name ||
               l.expr != r.expr;
    }

    bool operator!=(const Expr::Literal::ResolvedStruct& l,
        const Expr::Literal::ResolvedStruct& r) {
        return l.type_id != r.type_id || l.generic_params != r.generic_params ||
               l.fields != r.fields;
    }

    bool operator!=(const Expr::Literal::ResolvedStruct::Field& l,
        const Expr::Literal::ResolvedStruct::Field& r) {
        return l.member_id != r.member_id || l.expr != r.expr ||
               l.type != r.type;
    }

    bool operator!=(const Expr::Unop& l, const Expr::Unop& r) {
        return l.expr != r.expr || l.op != r.op;
    }

    bool operator!=(const Expr::Binop& l, const Expr::Binop& r) {
        return l.lhs != r.lhs || l.rhs != r.rhs || l.op != r.op;
    }

    bool operator!=(const Expr::Index& l, const Expr::Index& r) {
        return l.expr != r.expr || l.index != r.index;
    }

    bool operator!=(const Expr::Call& l, const Expr::Call& r) {
        return l.expr != r.expr || l.args != r.args;
    }

    bool operator!=(const Expr::Cast& l, const Expr::Cast& r) {
        return l.expr != r.expr || l.type != r.type;
    }

    bool operator!=(const Expr::TryCast& l, const Expr::TryCast& r) {
        return l.expr != r.expr || l.type != r.type;
    }

    bool operator!=(const Expr::Builtin& l, const Expr::Builtin& r) {
        return l.is_invalid != r.is_invalid || l.name != r.name ||
               l.generic_params != r.generic_params || l.args != r.args;
    }

    bool operator!=(const Expr::Name& l, const Expr::Name& r) {
        return l.is_invalid != r.is_invalid || l.name != r.name ||
               l.generic_params != r.generic_params;
    }

    bool operator!=(
        const Expr::TypeMemberAccess& l, const Expr::TypeMemberAccess& r) {
        return l.is_invalid != r.is_invalid || l.type != r.type ||
               l.name != r.name;
    }

    bool operator!=(const Expr::MemberAccess& l, const Expr::MemberAccess& r) {
        return l.expr != r.expr || l.name != r.name;
    }

    bool operator!=(const Expr::LocalVar& l, const Expr::LocalVar& r) {
        return l.id != r.id;
    }

    bool operator!=(const Expr::FuncArg& l, const Expr::FuncArg& r) {
        return l.name != r.name;
    }

    bool operator!=(const Expr::ResolvedDecl& l, const Expr::ResolvedDecl& r) {
        return l.decl_id != r.decl_id || l.generic_params != r.generic_params;
    }

    bool operator!=(const Expr::ResolvedTypeMemberAccess& l,
        const Expr::ResolvedTypeMemberAccess& r) {
        return l.type != r.type || l.member_id != r.member_id ||
               l.generic_params != r.generic_params;
    }

    bool operator!=(const Expr::ResolvedMemberAccess& l,
        const Expr::ResolvedMemberAccess& r) {
        return l.expr != r.expr || l.member_id != r.member_id;
    }

    bool operator!=(const Block& l, const Block& r) {
        return l.is_invalid != r.is_invalid || l.stmts != r.stmts;
    }

    bool operator!=(const Stmt& l, const Stmt& r) { return l.val != r.val; }

    bool operator!=(const Stmt::Let& l, const Stmt::Let& r) {
        return l.is_invalid != r.is_invalid || l.name != r.name ||
               l.type != r.type || l.expr != r.expr;
    }

    bool operator!=(const Stmt::If& l, const Stmt::If& r) {
        return l.is_invalid != r.is_invalid || l.expr != r.expr ||
               l.main != r.main || l.elifs != r.elifs || l.else_ != r.else_;
    }

    bool operator!=(const Stmt::If::Elif& l, const Stmt::If::Elif& r) {
        return l.expr != r.expr || l.block != r.block;
    }

    bool operator!=(const Stmt::Switch& l, const Stmt::Switch& r) {
        return l.is_invalid != r.is_invalid || l.expr != r.expr ||
               l.cases != r.cases;
    }

    bool operator!=(const Stmt::Switch::Case& l, const Stmt::Switch::Case& r) {
        return l.is_invalid != r.is_invalid || l.expr != r.expr ||
               l.block != r.block;
    }

    bool operator!=(const Stmt::Loop& l, const Stmt::Loop& r) {
        return l.is_invalid != r.is_invalid || l.block != r.block;
    }

    bool operator!=(const Stmt::For& l, const Stmt::For& r) {
        return l.is_invalid != r.is_invalid || l.name != r.name ||
               l.expr != r.expr || l.block != r.block;
    }

    bool operator!=(const Stmt::While& l, const Stmt::While& r) {
        return l.is_invalid != r.is_invalid || l.expr != r.expr ||
               l.block != r.block;
    }

    bool operator!=(const Stmt::Return& l, const Stmt::Return& r) {
        return l.is_invalid != r.is_invalid || l.expr != r.expr;
    }

    bool operator!=(const Stmt::Defer& l, const Stmt::Defer& r) {
        return l.is_invalid != r.is_invalid || l.expr != r.expr;
    }

    bool operator!=([[maybe_unused]] const Stmt::Continue& l,
        [[maybe_unused]] const Stmt::Continue& r) {
        return false;
    }

    bool operator!=([[maybe_unused]] const Stmt::Break& l,
        [[maybe_unused]] const Stmt::Break& r) {
        return false;
    }

    bool operator!=(const Stmt::Print& l, const Stmt::Print& r) {
        return l.is_invalid != r.is_invalid || l.exprs != r.exprs;
    }

    bool operator!=(const Func& l, const Func& r) {
        return l.is_invalid != r.is_invalid || l.signature != r.signature ||
               l.block != r.block;
    }

    bool operator!=(const Func::Signature& l, const Func::Signature& r) {
        return l.is_invalid != r.is_invalid ||
               l.is_self_func != r.is_self_func || l.params != r.params ||
               l.ret != r.ret;
    }

    bool operator!=(
        const Func::Signature::Param& l, const Func::Signature::Param& r) {
        return l.is_invalid != r.is_invalid || l.name != r.name ||
               l.type != r.type;
    }

    bool operator!=(const Decl& l, const Decl& r) {
        return l.is_export != r.is_export || l.name != r.name ||
               l.generic_params != r.generic_params || l.val != r.val;
    }

    bool operator!=(const Decl::GenericParam& l, const Decl::GenericParam& r) {
        return l.is_invalid != r.is_invalid || l.name != r.name ||
               l.interfaces != r.interfaces;
    }

    bool operator!=(const Decl::DeclsBlock& l, const Decl::DeclsBlock& r) {
        return l.is_invalid != r.is_invalid ||
               l.generic_params != r.generic_params ||
               l.interface != r.interface || l.decls != r.decls;
    }

    bool operator!=(const Decl::Struct& l, const Decl::Struct& r) {
        return l.is_invalid != r.is_invalid || l.fields != r.fields ||
               l.decls_blocks != r.decls_blocks;
    }

    bool operator!=(
        const Decl::Struct::Field& l, const Decl::Struct::Field& r) {
        return l.is_invalid != r.is_invalid || l.name != r.name ||
               l.type != r.type || l.is_export != r.is_export;
    }

    bool operator!=(const Decl::Enum& l, const Decl::Enum& r) {
        return l.is_invalid != r.is_invalid || l.elements != r.elements ||
               l.decls_blocks != r.decls_blocks;
    }

    bool operator!=(
        const Decl::Enum::Element& l, const Decl::Enum::Element& r) {
        return l.is_invalid != r.is_invalid || l.name != r.name;
    }

    bool operator!=(const Decl::TypeAlias& l, const Decl::TypeAlias& r) {
        return l.is_invalid != r.is_invalid || l.type != r.type;
    }

    bool operator!=(const Decl::Const& l, const Decl::Const& r) {
        return l.is_invalid != r.is_invalid || l.type != r.type ||
               l.expr != r.expr;
    }

    bool operator!=(const Decl::Interface& l, const Decl::Interface& r) {
        return l.is_invalid != r.is_invalid || l.members != r.members;
    }

    bool operator!=(
        const Decl::Interface::Member& l, const Decl::Interface::Member& r) {
        return l.is_invalid != r.is_invalid || l.name != r.name ||
               l.generic_params != r.generic_params ||
               l.signature != r.signature;
    }

    bool operator!=(const Module& l, const Module& r) {
        return l.decls != r.decls;
    }

    alvo::ast::Invalid util::Clone<alvo::ast::Invalid>::operator()(
        [[maybe_unused]] const alvo::ast::Invalid& n,
        [[maybe_unused]] mem::Arena& arena) {
        return alvo::ast::Invalid(

        );
    }

    alvo::ast::Id util::Clone<alvo::ast::Id>::operator()(
        const alvo::ast::Id& n, mem::Arena& arena) {
        return alvo::ast::Id(util::Clone<decltype(n.id)>()(n.id, arena));
    }

    alvo::ast::Type util::Clone<alvo::ast::Type>::operator()(
        const alvo::ast::Type& n, mem::Arena& arena) {
        return alvo::ast::Type(util::Clone<decltype(n.val)>()(n.val, arena));
    }

    alvo::ast::Type::Unit util::Clone<alvo::ast::Type::Unit>::operator()(
        [[maybe_unused]] const alvo::ast::Type::Unit& n,
        [[maybe_unused]] mem::Arena& arena) {
        return alvo::ast::Type::Unit(

        );
    }

    alvo::ast::Type::String util::Clone<alvo::ast::Type::String>::operator()(
        [[maybe_unused]] const alvo::ast::Type::String& n,
        [[maybe_unused]] mem::Arena& arena) {
        return alvo::ast::Type::String(

        );
    }

    alvo::ast::Type::Char util::Clone<alvo::ast::Type::Char>::operator()(
        [[maybe_unused]] const alvo::ast::Type::Char& n,
        [[maybe_unused]] mem::Arena& arena) {
        return alvo::ast::Type::Char(

        );
    }

    alvo::ast::Type::Int util::Clone<alvo::ast::Type::Int>::operator()(
        [[maybe_unused]] const alvo::ast::Type::Int& n,
        [[maybe_unused]] mem::Arena& arena) {
        return alvo::ast::Type::Int(

        );
    }

    alvo::ast::Type::Byte util::Clone<alvo::ast::Type::Byte>::operator()(
        [[maybe_unused]] const alvo::ast::Type::Byte& n,
        [[maybe_unused]] mem::Arena& arena) {
        return alvo::ast::Type::Byte(

        );
    }

    alvo::ast::Type::Float util::Clone<alvo::ast::Type::Float>::operator()(
        [[maybe_unused]] const alvo::ast::Type::Float& n,
        [[maybe_unused]] mem::Arena& arena) {
        return alvo::ast::Type::Float(

        );
    }

    alvo::ast::Type::Bool util::Clone<alvo::ast::Type::Bool>::operator()(
        [[maybe_unused]] const alvo::ast::Type::Bool& n,
        [[maybe_unused]] mem::Arena& arena) {
        return alvo::ast::Type::Bool(

        );
    }

    alvo::ast::Type::Array util::Clone<alvo::ast::Type::Array>::operator()(
        const alvo::ast::Type::Array& n, mem::Arena& arena) {
        return alvo::ast::Type::Array(
            util::Clone<decltype(n.is_invalid)>()(n.is_invalid, arena),
            util::Clone<decltype(n.type)>()(n.type, arena));
    }

    alvo::ast::Type::Tup util::Clone<alvo::ast::Type::Tup>::operator()(
        const alvo::ast::Type::Tup& n, mem::Arena& arena) {
        return alvo::ast::Type::Tup(
            util::Clone<decltype(n.is_invalid)>()(n.is_invalid, arena),
            util::Clone<decltype(n.types)>()(n.types, arena));
    }

    alvo::ast::Type::Func util::Clone<alvo::ast::Type::Func>::operator()(
        const alvo::ast::Type::Func& n, mem::Arena& arena) {
        return alvo::ast::Type::Func(
            util::Clone<decltype(n.is_invalid)>()(n.is_invalid, arena),
            util::Clone<decltype(n.params)>()(n.params, arena),
            util::Clone<decltype(n.return_type)>()(n.return_type, arena));
    }

    alvo::ast::Type::Name util::Clone<alvo::ast::Type::Name>::operator()(
        const alvo::ast::Type::Name& n, mem::Arena& arena) {
        return alvo::ast::Type::Name(
            util::Clone<decltype(n.is_invalid)>()(n.is_invalid, arena),
            util::Clone<decltype(n.name)>()(n.name, arena),
            util::Clone<decltype(n.generic_params)>()(n.generic_params, arena));
    }

    alvo::ast::Type::LocalGeneric
    util::Clone<alvo::ast::Type::LocalGeneric>::operator()(
        const alvo::ast::Type::LocalGeneric& n, mem::Arena& arena) {
        return alvo::ast::Type::LocalGeneric(
            util::Clone<decltype(n.id)>()(n.id, arena),
            util::Clone<decltype(n.name)>()(n.name, arena));
    }

    alvo::ast::Type::ResolvedUserDefinedType
    util::Clone<alvo::ast::Type::ResolvedUserDefinedType>::operator()(
        const alvo::ast::Type::ResolvedUserDefinedType& n, mem::Arena& arena) {
        return alvo::ast::Type::ResolvedUserDefinedType(
            util::Clone<decltype(n.id)>()(n.id, arena),
            util::Clone<decltype(n.generic_params)>()(n.generic_params, arena));
    }

    alvo::ast::Expr util::Clone<alvo::ast::Expr>::operator()(
        const alvo::ast::Expr& n, mem::Arena& arena) {
        return alvo::ast::Expr(util::Clone<decltype(n.val)>()(n.val, arena));
    }

    alvo::ast::Expr::Literal util::Clone<alvo::ast::Expr::Literal>::operator()(
        const alvo::ast::Expr::Literal& n, mem::Arena& arena) {
        return alvo::ast::Expr::Literal(
            util::Clone<decltype(n.val)>()(n.val, arena));
    }

    alvo::ast::Expr::Literal::Unit
    util::Clone<alvo::ast::Expr::Literal::Unit>::operator()(
        [[maybe_unused]] const alvo::ast::Expr::Literal::Unit& n,
        [[maybe_unused]] mem::Arena& arena) {
        return alvo::ast::Expr::Literal::Unit(

        );
    }

    alvo::ast::Expr::Literal::Null
    util::Clone<alvo::ast::Expr::Literal::Null>::operator()(
        [[maybe_unused]] const alvo::ast::Expr::Literal::Null& n,
        [[maybe_unused]] mem::Arena& arena) {
        return alvo::ast::Expr::Literal::Null(

        );
    }

    alvo::ast::Expr::Literal::String
    util::Clone<alvo::ast::Expr::Literal::String>::operator()(
        const alvo::ast::Expr::Literal::String& n, mem::Arena& arena) {
        return alvo::ast::Expr::Literal::String(
            util::Clone<decltype(n.value)>()(n.value, arena));
    }

    alvo::ast::Expr::Literal::Character
    util::Clone<alvo::ast::Expr::Literal::Character>::operator()(
        const alvo::ast::Expr::Literal::Character& n, mem::Arena& arena) {
        return alvo::ast::Expr::Literal::Character(
            util::Clone<decltype(n.value)>()(n.value, arena));
    }

    alvo::ast::Expr::Literal::Integer
    util::Clone<alvo::ast::Expr::Literal::Integer>::operator()(
        const alvo::ast::Expr::Literal::Integer& n, mem::Arena& arena) {
        return alvo::ast::Expr::Literal::Integer(
            util::Clone<decltype(n.value)>()(n.value, arena));
    }

    alvo::ast::Expr::Literal::Byte
    util::Clone<alvo::ast::Expr::Literal::Byte>::operator()(
        const alvo::ast::Expr::Literal::Byte& n, mem::Arena& arena) {
        return alvo::ast::Expr::Literal::Byte(
            util::Clone<decltype(n.value)>()(n.value, arena));
    }

    alvo::ast::Expr::Literal::Floating
    util::Clone<alvo::ast::Expr::Literal::Floating>::operator()(
        const alvo::ast::Expr::Literal::Floating& n, mem::Arena& arena) {
        return alvo::ast::Expr::Literal::Floating(
            util::Clone<decltype(n.value)>()(n.value, arena));
    }

    alvo::ast::Expr::Literal::Boolean
    util::Clone<alvo::ast::Expr::Literal::Boolean>::operator()(
        const alvo::ast::Expr::Literal::Boolean& n, mem::Arena& arena) {
        return alvo::ast::Expr::Literal::Boolean(
            util::Clone<decltype(n.value)>()(n.value, arena));
    }

    alvo::ast::Expr::Literal::Array
    util::Clone<alvo::ast::Expr::Literal::Array>::operator()(
        const alvo::ast::Expr::Literal::Array& n, mem::Arena& arena) {
        return alvo::ast::Expr::Literal::Array(
            util::Clone<decltype(n.val)>()(n.val, arena));
    }

    alvo::ast::Expr::Literal::Array::Regular
    util::Clone<alvo::ast::Expr::Literal::Array::Regular>::operator()(
        const alvo::ast::Expr::Literal::Array::Regular& n, mem::Arena& arena) {
        return alvo::ast::Expr::Literal::Array::Regular(
            util::Clone<decltype(n.elements)>()(n.elements, arena));
    }

    alvo::ast::Expr::Literal::Array::DefaultNTimes
    util::Clone<alvo::ast::Expr::Literal::Array::DefaultNTimes>::operator()(
        const alvo::ast::Expr::Literal::Array::DefaultNTimes& n,
        mem::Arena& arena) {
        return alvo::ast::Expr::Literal::Array::DefaultNTimes(
            util::Clone<decltype(n.times)>()(n.times, arena));
    }

    alvo::ast::Expr::Literal::Array::ExprNTimes
    util::Clone<alvo::ast::Expr::Literal::Array::ExprNTimes>::operator()(
        const alvo::ast::Expr::Literal::Array::ExprNTimes& n,
        mem::Arena& arena) {
        return alvo::ast::Expr::Literal::Array::ExprNTimes(
            util::Clone<decltype(n.expr)>()(n.expr, arena),
            util::Clone<decltype(n.times)>()(n.times, arena));
    }

    alvo::ast::Expr::Literal::Tup
    util::Clone<alvo::ast::Expr::Literal::Tup>::operator()(
        const alvo::ast::Expr::Literal::Tup& n, mem::Arena& arena) {
        return alvo::ast::Expr::Literal::Tup(
            util::Clone<decltype(n.is_invalid)>()(n.is_invalid, arena),
            util::Clone<decltype(n.exprs)>()(n.exprs, arena));
    }

    alvo::ast::Expr::Literal::Struct
    util::Clone<alvo::ast::Expr::Literal::Struct>::operator()(
        const alvo::ast::Expr::Literal::Struct& n, mem::Arena& arena) {
        return alvo::ast::Expr::Literal::Struct(
            util::Clone<decltype(n.is_invalid)>()(n.is_invalid, arena),
            util::Clone<decltype(n.type)>()(n.type, arena),
            util::Clone<decltype(n.fields)>()(n.fields, arena));
    }

    alvo::ast::Expr::Literal::Struct::Field
    util::Clone<alvo::ast::Expr::Literal::Struct::Field>::operator()(
        const alvo::ast::Expr::Literal::Struct::Field& n, mem::Arena& arena) {
        return alvo::ast::Expr::Literal::Struct::Field(
            util::Clone<decltype(n.is_invalid)>()(n.is_invalid, arena),
            util::Clone<decltype(n.name)>()(n.name, arena),
            util::Clone<decltype(n.expr)>()(n.expr, arena));
    }

    alvo::ast::Expr::Literal::ResolvedStruct
    util::Clone<alvo::ast::Expr::Literal::ResolvedStruct>::operator()(
        const alvo::ast::Expr::Literal::ResolvedStruct& n, mem::Arena& arena) {
        return alvo::ast::Expr::Literal::ResolvedStruct(
            util::Clone<decltype(n.type_id)>()(n.type_id, arena),
            util::Clone<decltype(n.generic_params)>()(n.generic_params, arena),
            util::Clone<decltype(n.fields)>()(n.fields, arena));
    }

    alvo::ast::Expr::Literal::ResolvedStruct::Field
    util::Clone<alvo::ast::Expr::Literal::ResolvedStruct::Field>::operator()(
        const alvo::ast::Expr::Literal::ResolvedStruct::Field& n,
        mem::Arena& arena) {
        return alvo::ast::Expr::Literal::ResolvedStruct::Field(
            util::Clone<decltype(n.member_id)>()(n.member_id, arena),
            util::Clone<decltype(n.expr)>()(n.expr, arena),
            util::Clone<decltype(n.type)>()(n.type, arena));
    }

    alvo::ast::Expr::Unop util::Clone<alvo::ast::Expr::Unop>::operator()(
        const alvo::ast::Expr::Unop& n, mem::Arena& arena) {
        return alvo::ast::Expr::Unop(
            util::Clone<decltype(n.expr)>()(n.expr, arena),
            util::Clone<decltype(n.op)>()(n.op, arena));
    }

    alvo::ast::Expr::Unop::Op
    util::Clone<alvo::ast::Expr::Unop::Op>::operator()(
        [[maybe_unused]] const alvo::ast::Expr::Unop::Op& n,
        [[maybe_unused]] mem::Arena& arena) {
        return alvo::ast::Expr::Unop::Op(

        );
    }

    alvo::ast::Expr::Binop util::Clone<alvo::ast::Expr::Binop>::operator()(
        const alvo::ast::Expr::Binop& n, mem::Arena& arena) {
        return alvo::ast::Expr::Binop(
            util::Clone<decltype(n.lhs)>()(n.lhs, arena),
            util::Clone<decltype(n.rhs)>()(n.rhs, arena),
            util::Clone<decltype(n.op)>()(n.op, arena));
    }

    alvo::ast::Expr::Binop::Op
    util::Clone<alvo::ast::Expr::Binop::Op>::operator()(
        [[maybe_unused]] const alvo::ast::Expr::Binop::Op& n,
        [[maybe_unused]] mem::Arena& arena) {
        return alvo::ast::Expr::Binop::Op(

        );
    }

    alvo::ast::Expr::Index util::Clone<alvo::ast::Expr::Index>::operator()(
        const alvo::ast::Expr::Index& n, mem::Arena& arena) {
        return alvo::ast::Expr::Index(
            util::Clone<decltype(n.expr)>()(n.expr, arena),
            util::Clone<decltype(n.index)>()(n.index, arena));
    }

    alvo::ast::Expr::Call util::Clone<alvo::ast::Expr::Call>::operator()(
        const alvo::ast::Expr::Call& n, mem::Arena& arena) {
        return alvo::ast::Expr::Call(
            util::Clone<decltype(n.expr)>()(n.expr, arena),
            util::Clone<decltype(n.args)>()(n.args, arena));
    }

    alvo::ast::Expr::Cast util::Clone<alvo::ast::Expr::Cast>::operator()(
        const alvo::ast::Expr::Cast& n, mem::Arena& arena) {
        return alvo::ast::Expr::Cast(
            util::Clone<decltype(n.expr)>()(n.expr, arena),
            util::Clone<decltype(n.type)>()(n.type, arena));
    }

    alvo::ast::Expr::TryCast util::Clone<alvo::ast::Expr::TryCast>::operator()(
        const alvo::ast::Expr::TryCast& n, mem::Arena& arena) {
        return alvo::ast::Expr::TryCast(
            util::Clone<decltype(n.expr)>()(n.expr, arena),
            util::Clone<decltype(n.type)>()(n.type, arena));
    }

    alvo::ast::Expr::Builtin util::Clone<alvo::ast::Expr::Builtin>::operator()(
        const alvo::ast::Expr::Builtin& n, mem::Arena& arena) {
        return alvo::ast::Expr::Builtin(
            util::Clone<decltype(n.is_invalid)>()(n.is_invalid, arena),
            util::Clone<decltype(n.name)>()(n.name, arena),
            util::Clone<decltype(n.generic_params)>()(n.generic_params, arena),
            util::Clone<decltype(n.args)>()(n.args, arena));
    }

    alvo::ast::Expr::Name util::Clone<alvo::ast::Expr::Name>::operator()(
        const alvo::ast::Expr::Name& n, mem::Arena& arena) {
        return alvo::ast::Expr::Name(
            util::Clone<decltype(n.is_invalid)>()(n.is_invalid, arena),
            util::Clone<decltype(n.name)>()(n.name, arena),
            util::Clone<decltype(n.generic_params)>()(n.generic_params, arena));
    }

    alvo::ast::Expr::TypeMemberAccess
    util::Clone<alvo::ast::Expr::TypeMemberAccess>::operator()(
        const alvo::ast::Expr::TypeMemberAccess& n, mem::Arena& arena) {
        return alvo::ast::Expr::TypeMemberAccess(
            util::Clone<decltype(n.is_invalid)>()(n.is_invalid, arena),
            util::Clone<decltype(n.type)>()(n.type, arena),
            util::Clone<decltype(n.name)>()(n.name, arena));
    }

    alvo::ast::Expr::MemberAccess
    util::Clone<alvo::ast::Expr::MemberAccess>::operator()(
        const alvo::ast::Expr::MemberAccess& n, mem::Arena& arena) {
        return alvo::ast::Expr::MemberAccess(
            util::Clone<decltype(n.expr)>()(n.expr, arena),
            util::Clone<decltype(n.name)>()(n.name, arena));
    }

    alvo::ast::Expr::LocalVar
    util::Clone<alvo::ast::Expr::LocalVar>::operator()(
        const alvo::ast::Expr::LocalVar& n, mem::Arena& arena) {
        return alvo::ast::Expr::LocalVar(
            util::Clone<decltype(n.id)>()(n.id, arena));
    }

    alvo::ast::Expr::FuncArg util::Clone<alvo::ast::Expr::FuncArg>::operator()(
        const alvo::ast::Expr::FuncArg& n, mem::Arena& arena) {
        return alvo::ast::Expr::FuncArg(
            util::Clone<decltype(n.name)>()(n.name, arena));
    }

    alvo::ast::Expr::ResolvedDecl
    util::Clone<alvo::ast::Expr::ResolvedDecl>::operator()(
        const alvo::ast::Expr::ResolvedDecl& n, mem::Arena& arena) {
        return alvo::ast::Expr::ResolvedDecl(
            util::Clone<decltype(n.decl_id)>()(n.decl_id, arena),
            util::Clone<decltype(n.generic_params)>()(n.generic_params, arena));
    }

    alvo::ast::Expr::ResolvedTypeMemberAccess
    util::Clone<alvo::ast::Expr::ResolvedTypeMemberAccess>::operator()(
        const alvo::ast::Expr::ResolvedTypeMemberAccess& n, mem::Arena& arena) {
        return alvo::ast::Expr::ResolvedTypeMemberAccess(
            util::Clone<decltype(n.type)>()(n.type, arena),
            util::Clone<decltype(n.member_id)>()(n.member_id, arena),
            util::Clone<decltype(n.generic_params)>()(n.generic_params, arena));
    }

    alvo::ast::Expr::ResolvedMemberAccess
    util::Clone<alvo::ast::Expr::ResolvedMemberAccess>::operator()(
        const alvo::ast::Expr::ResolvedMemberAccess& n, mem::Arena& arena) {
        return alvo::ast::Expr::ResolvedMemberAccess(
            util::Clone<decltype(n.expr)>()(n.expr, arena),
            util::Clone<decltype(n.member_id)>()(n.member_id, arena));
    }

    alvo::ast::Block util::Clone<alvo::ast::Block>::operator()(
        const alvo::ast::Block& n, mem::Arena& arena) {
        return alvo::ast::Block(
            util::Clone<decltype(n.is_invalid)>()(n.is_invalid, arena),
            util::Clone<decltype(n.stmts)>()(n.stmts, arena));
    }

    alvo::ast::Stmt util::Clone<alvo::ast::Stmt>::operator()(
        const alvo::ast::Stmt& n, mem::Arena& arena) {
        return alvo::ast::Stmt(util::Clone<decltype(n.val)>()(n.val, arena));
    }

    alvo::ast::Stmt::Let util::Clone<alvo::ast::Stmt::Let>::operator()(
        const alvo::ast::Stmt::Let& n, mem::Arena& arena) {
        return alvo::ast::Stmt::Let(
            util::Clone<decltype(n.is_invalid)>()(n.is_invalid, arena),
            util::Clone<decltype(n.name)>()(n.name, arena),
            util::Clone<decltype(n.type)>()(n.type, arena),
            util::Clone<decltype(n.expr)>()(n.expr, arena));
    }

    alvo::ast::Stmt::If util::Clone<alvo::ast::Stmt::If>::operator()(
        const alvo::ast::Stmt::If& n, mem::Arena& arena) {
        return alvo::ast::Stmt::If(
            util::Clone<decltype(n.is_invalid)>()(n.is_invalid, arena),
            util::Clone<decltype(n.expr)>()(n.expr, arena),
            util::Clone<decltype(n.main)>()(n.main, arena),
            util::Clone<decltype(n.elifs)>()(n.elifs, arena),
            util::Clone<decltype(n.else_)>()(n.else_, arena));
    }

    alvo::ast::Stmt::If::Elif
    util::Clone<alvo::ast::Stmt::If::Elif>::operator()(
        const alvo::ast::Stmt::If::Elif& n, mem::Arena& arena) {
        return alvo::ast::Stmt::If::Elif(
            util::Clone<decltype(n.expr)>()(n.expr, arena),
            util::Clone<decltype(n.block)>()(n.block, arena));
    }

    alvo::ast::Stmt::Switch util::Clone<alvo::ast::Stmt::Switch>::operator()(
        const alvo::ast::Stmt::Switch& n, mem::Arena& arena) {
        return alvo::ast::Stmt::Switch(
            util::Clone<decltype(n.is_invalid)>()(n.is_invalid, arena),
            util::Clone<decltype(n.expr)>()(n.expr, arena),
            util::Clone<decltype(n.cases)>()(n.cases, arena));
    }

    alvo::ast::Stmt::Switch::Case
    util::Clone<alvo::ast::Stmt::Switch::Case>::operator()(
        const alvo::ast::Stmt::Switch::Case& n, mem::Arena& arena) {
        return alvo::ast::Stmt::Switch::Case(
            util::Clone<decltype(n.is_invalid)>()(n.is_invalid, arena),
            util::Clone<decltype(n.expr)>()(n.expr, arena),
            util::Clone<decltype(n.block)>()(n.block, arena));
    }

    alvo::ast::Stmt::Loop util::Clone<alvo::ast::Stmt::Loop>::operator()(
        const alvo::ast::Stmt::Loop& n, mem::Arena& arena) {
        return alvo::ast::Stmt::Loop(
            util::Clone<decltype(n.is_invalid)>()(n.is_invalid, arena),
            util::Clone<decltype(n.block)>()(n.block, arena));
    }

    alvo::ast::Stmt::For util::Clone<alvo::ast::Stmt::For>::operator()(
        const alvo::ast::Stmt::For& n, mem::Arena& arena) {
        return alvo::ast::Stmt::For(
            util::Clone<decltype(n.is_invalid)>()(n.is_invalid, arena),
            util::Clone<decltype(n.name)>()(n.name, arena),
            util::Clone<decltype(n.expr)>()(n.expr, arena),
            util::Clone<decltype(n.block)>()(n.block, arena));
    }

    alvo::ast::Stmt::While util::Clone<alvo::ast::Stmt::While>::operator()(
        const alvo::ast::Stmt::While& n, mem::Arena& arena) {
        return alvo::ast::Stmt::While(
            util::Clone<decltype(n.is_invalid)>()(n.is_invalid, arena),
            util::Clone<decltype(n.expr)>()(n.expr, arena),
            util::Clone<decltype(n.block)>()(n.block, arena));
    }

    alvo::ast::Stmt::Return util::Clone<alvo::ast::Stmt::Return>::operator()(
        const alvo::ast::Stmt::Return& n, mem::Arena& arena) {
        return alvo::ast::Stmt::Return(
            util::Clone<decltype(n.is_invalid)>()(n.is_invalid, arena),
            util::Clone<decltype(n.expr)>()(n.expr, arena));
    }

    alvo::ast::Stmt::Defer util::Clone<alvo::ast::Stmt::Defer>::operator()(
        const alvo::ast::Stmt::Defer& n, mem::Arena& arena) {
        return alvo::ast::Stmt::Defer(
            util::Clone<decltype(n.is_invalid)>()(n.is_invalid, arena),
            util::Clone<decltype(n.expr)>()(n.expr, arena));
    }

    alvo::ast::Stmt::Continue
    util::Clone<alvo::ast::Stmt::Continue>::operator()(
        [[maybe_unused]] const alvo::ast::Stmt::Continue& n,
        [[maybe_unused]] mem::Arena& arena) {
        return alvo::ast::Stmt::Continue(

        );
    }

    alvo::ast::Stmt::Break util::Clone<alvo::ast::Stmt::Break>::operator()(
        [[maybe_unused]] const alvo::ast::Stmt::Break& n,
        [[maybe_unused]] mem::Arena& arena) {
        return alvo::ast::Stmt::Break(

        );
    }

    alvo::ast::Stmt::Print util::Clone<alvo::ast::Stmt::Print>::operator()(
        const alvo::ast::Stmt::Print& n, mem::Arena& arena) {
        return alvo::ast::Stmt::Print(
            util::Clone<decltype(n.is_invalid)>()(n.is_invalid, arena),
            util::Clone<decltype(n.exprs)>()(n.exprs, arena));
    }

    alvo::ast::Func util::Clone<alvo::ast::Func>::operator()(
        const alvo::ast::Func& n, mem::Arena& arena) {
        return alvo::ast::Func(
            util::Clone<decltype(n.is_invalid)>()(n.is_invalid, arena),
            util::Clone<decltype(n.signature)>()(n.signature, arena),
            util::Clone<decltype(n.block)>()(n.block, arena));
    }

    alvo::ast::Func::Signature
    util::Clone<alvo::ast::Func::Signature>::operator()(
        const alvo::ast::Func::Signature& n, mem::Arena& arena) {
        return alvo::ast::Func::Signature(
            util::Clone<decltype(n.is_invalid)>()(n.is_invalid, arena),
            util::Clone<decltype(n.is_self_func)>()(n.is_self_func, arena),
            util::Clone<decltype(n.params)>()(n.params, arena),
            util::Clone<decltype(n.ret)>()(n.ret, arena));
    }

    alvo::ast::Func::Signature::Param
    util::Clone<alvo::ast::Func::Signature::Param>::operator()(
        const alvo::ast::Func::Signature::Param& n, mem::Arena& arena) {
        return alvo::ast::Func::Signature::Param(
            util::Clone<decltype(n.is_invalid)>()(n.is_invalid, arena),
            util::Clone<decltype(n.name)>()(n.name, arena),
            util::Clone<decltype(n.type)>()(n.type, arena));
    }

    alvo::ast::Decl util::Clone<alvo::ast::Decl>::operator()(
        const alvo::ast::Decl& n, mem::Arena& arena) {
        return alvo::ast::Decl(
            util::Clone<decltype(n.is_export)>()(n.is_export, arena),
            util::Clone<decltype(n.name)>()(n.name, arena),
            util::Clone<decltype(n.generic_params)>()(n.generic_params, arena),
            util::Clone<decltype(n.val)>()(n.val, arena));
    }

    alvo::ast::Decl::GenericParam
    util::Clone<alvo::ast::Decl::GenericParam>::operator()(
        const alvo::ast::Decl::GenericParam& n, mem::Arena& arena) {
        return alvo::ast::Decl::GenericParam(
            util::Clone<decltype(n.is_invalid)>()(n.is_invalid, arena),
            util::Clone<decltype(n.name)>()(n.name, arena),
            util::Clone<decltype(n.interfaces)>()(n.interfaces, arena));
    }

    alvo::ast::Decl::DeclsBlock
    util::Clone<alvo::ast::Decl::DeclsBlock>::operator()(
        const alvo::ast::Decl::DeclsBlock& n, mem::Arena& arena) {
        return alvo::ast::Decl::DeclsBlock(
            util::Clone<decltype(n.is_invalid)>()(n.is_invalid, arena),
            util::Clone<decltype(n.generic_params)>()(n.generic_params, arena),
            util::Clone<decltype(n.interface)>()(n.interface, arena),
            util::Clone<decltype(n.decls)>()(n.decls, arena));
    }

    alvo::ast::Decl::Struct util::Clone<alvo::ast::Decl::Struct>::operator()(
        const alvo::ast::Decl::Struct& n, mem::Arena& arena) {
        return alvo::ast::Decl::Struct(
            util::Clone<decltype(n.is_invalid)>()(n.is_invalid, arena),
            util::Clone<decltype(n.fields)>()(n.fields, arena),
            util::Clone<decltype(n.decls_blocks)>()(n.decls_blocks, arena));
    }

    alvo::ast::Decl::Struct::Field
    util::Clone<alvo::ast::Decl::Struct::Field>::operator()(
        const alvo::ast::Decl::Struct::Field& n, mem::Arena& arena) {
        return alvo::ast::Decl::Struct::Field(
            util::Clone<decltype(n.is_invalid)>()(n.is_invalid, arena),
            util::Clone<decltype(n.name)>()(n.name, arena),
            util::Clone<decltype(n.type)>()(n.type, arena),
            util::Clone<decltype(n.is_export)>()(n.is_export, arena));
    }

    alvo::ast::Decl::Enum util::Clone<alvo::ast::Decl::Enum>::operator()(
        const alvo::ast::Decl::Enum& n, mem::Arena& arena) {
        return alvo::ast::Decl::Enum(
            util::Clone<decltype(n.is_invalid)>()(n.is_invalid, arena),
            util::Clone<decltype(n.elements)>()(n.elements, arena),
            util::Clone<decltype(n.decls_blocks)>()(n.decls_blocks, arena));
    }

    alvo::ast::Decl::Enum::Element
    util::Clone<alvo::ast::Decl::Enum::Element>::operator()(
        const alvo::ast::Decl::Enum::Element& n, mem::Arena& arena) {
        return alvo::ast::Decl::Enum::Element(
            util::Clone<decltype(n.is_invalid)>()(n.is_invalid, arena),
            util::Clone<decltype(n.name)>()(n.name, arena));
    }

    alvo::ast::Decl::TypeAlias
    util::Clone<alvo::ast::Decl::TypeAlias>::operator()(
        const alvo::ast::Decl::TypeAlias& n, mem::Arena& arena) {
        return alvo::ast::Decl::TypeAlias(
            util::Clone<decltype(n.is_invalid)>()(n.is_invalid, arena),
            util::Clone<decltype(n.type)>()(n.type, arena));
    }

    alvo::ast::Decl::Const util::Clone<alvo::ast::Decl::Const>::operator()(
        const alvo::ast::Decl::Const& n, mem::Arena& arena) {
        return alvo::ast::Decl::Const(
            util::Clone<decltype(n.is_invalid)>()(n.is_invalid, arena),
            util::Clone<decltype(n.type)>()(n.type, arena),
            util::Clone<decltype(n.expr)>()(n.expr, arena));
    }

    alvo::ast::Decl::Interface
    util::Clone<alvo::ast::Decl::Interface>::operator()(
        const alvo::ast::Decl::Interface& n, mem::Arena& arena) {
        return alvo::ast::Decl::Interface(
            util::Clone<decltype(n.is_invalid)>()(n.is_invalid, arena),
            util::Clone<decltype(n.members)>()(n.members, arena));
    }

    alvo::ast::Decl::Interface::Member
    util::Clone<alvo::ast::Decl::Interface::Member>::operator()(
        const alvo::ast::Decl::Interface::Member& n, mem::Arena& arena) {
        return alvo::ast::Decl::Interface::Member(
            util::Clone<decltype(n.is_invalid)>()(n.is_invalid, arena),
            util::Clone<decltype(n.name)>()(n.name, arena),
            util::Clone<decltype(n.generic_params)>()(n.generic_params, arena),
            util::Clone<decltype(n.signature)>()(n.signature, arena));
    }

    alvo::ast::Module util::Clone<alvo::ast::Module>::operator()(
        const alvo::ast::Module& n, mem::Arena& arena) {
        return alvo::ast::Module(
            util::Clone<decltype(n.decls)>()(n.decls, arena));
    }

}

namespace std {

    std::size_t hash<alvo::ast::Invalid>::operator()(
        [[maybe_unused]] const alvo::ast::Invalid& n) const noexcept {
        return 0;
    }

    std::size_t hash<alvo::ast::Id>::operator()(
        const alvo::ast::Id& n) const noexcept {
        return std::hash<decltype(n.id)>()(n.id);
    }

    std::size_t hash<alvo::ast::Type>::operator()(
        const alvo::ast::Type& n) const noexcept {
        return std::hash<decltype(n.val)>()(n.val);
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

    std::size_t hash<alvo::ast::Type::Name>::operator()(
        const alvo::ast::Type::Name& n) const noexcept {
        std::size_t is_invalid_hash =
            std::hash<decltype(n.is_invalid)>()(n.is_invalid);
        std::size_t name_hash = std::hash<decltype(n.name)>()(n.name);
        std::size_t generic_params_hash =
            std::hash<decltype(n.generic_params)>()(n.generic_params);
        std::size_t res = is_invalid_hash;
        res ^= name_hash + 0x9e3779b97f4a7c15ull + (res << 6) + (res >> 2);
        res ^= generic_params_hash + 0x9e3779b97f4a7c15ull + (res << 6) +
               (res >> 2);
        return res;
    }

    std::size_t hash<alvo::ast::Type::LocalGeneric>::operator()(
        const alvo::ast::Type::LocalGeneric& n) const noexcept {
        std::size_t id_hash = std::hash<decltype(n.id)>()(n.id);
        std::size_t name_hash = std::hash<decltype(n.name)>()(n.name);
        std::size_t res = id_hash;
        res ^= name_hash + 0x9e3779b97f4a7c15ull + (res << 6) + (res >> 2);
        return res;
    }

    std::size_t hash<alvo::ast::Type::ResolvedUserDefinedType>::operator()(
        const alvo::ast::Type::ResolvedUserDefinedType& n) const noexcept {
        std::size_t id_hash = std::hash<decltype(n.id)>()(n.id);
        std::size_t generic_params_hash =
            std::hash<decltype(n.generic_params)>()(n.generic_params);
        std::size_t res = id_hash;
        res ^= generic_params_hash + 0x9e3779b97f4a7c15ull + (res << 6) +
               (res >> 2);
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

    std::size_t hash<alvo::ast::Expr::Literal::ResolvedStruct>::operator()(
        const alvo::ast::Expr::Literal::ResolvedStruct& n) const noexcept {
        std::size_t type_id_hash = std::hash<decltype(n.type_id)>()(n.type_id);
        std::size_t generic_params_hash =
            std::hash<decltype(n.generic_params)>()(n.generic_params);
        std::size_t fields_hash = std::hash<decltype(n.fields)>()(n.fields);
        std::size_t res = type_id_hash;
        res ^= generic_params_hash + 0x9e3779b97f4a7c15ull + (res << 6) +
               (res >> 2);
        res ^= fields_hash + 0x9e3779b97f4a7c15ull + (res << 6) + (res >> 2);
        return res;
    }

    std::size_t
    hash<alvo::ast::Expr::Literal::ResolvedStruct::Field>::operator()(
        const alvo::ast::Expr::Literal::ResolvedStruct::Field& n)
        const noexcept {
        std::size_t member_id_hash =
            std::hash<decltype(n.member_id)>()(n.member_id);
        std::size_t expr_hash = std::hash<decltype(n.expr)>()(n.expr);
        std::size_t type_hash = std::hash<decltype(n.type)>()(n.type);
        std::size_t res = member_id_hash;
        res ^= expr_hash + 0x9e3779b97f4a7c15ull + (res << 6) + (res >> 2);
        res ^= type_hash + 0x9e3779b97f4a7c15ull + (res << 6) + (res >> 2);
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

    std::size_t hash<alvo::ast::Expr::Name>::operator()(
        const alvo::ast::Expr::Name& n) const noexcept {
        std::size_t is_invalid_hash =
            std::hash<decltype(n.is_invalid)>()(n.is_invalid);
        std::size_t name_hash = std::hash<decltype(n.name)>()(n.name);
        std::size_t generic_params_hash =
            std::hash<decltype(n.generic_params)>()(n.generic_params);
        std::size_t res = is_invalid_hash;
        res ^= name_hash + 0x9e3779b97f4a7c15ull + (res << 6) + (res >> 2);
        res ^= generic_params_hash + 0x9e3779b97f4a7c15ull + (res << 6) +
               (res >> 2);
        return res;
    }

    std::size_t hash<alvo::ast::Expr::TypeMemberAccess>::operator()(
        const alvo::ast::Expr::TypeMemberAccess& n) const noexcept {
        std::size_t is_invalid_hash =
            std::hash<decltype(n.is_invalid)>()(n.is_invalid);
        std::size_t type_hash = std::hash<decltype(n.type)>()(n.type);
        std::size_t name_hash = std::hash<decltype(n.name)>()(n.name);
        std::size_t res = is_invalid_hash;
        res ^= type_hash + 0x9e3779b97f4a7c15ull + (res << 6) + (res >> 2);
        res ^= name_hash + 0x9e3779b97f4a7c15ull + (res << 6) + (res >> 2);
        return res;
    }

    std::size_t hash<alvo::ast::Expr::MemberAccess>::operator()(
        const alvo::ast::Expr::MemberAccess& n) const noexcept {
        std::size_t expr_hash = std::hash<decltype(n.expr)>()(n.expr);
        std::size_t name_hash = std::hash<decltype(n.name)>()(n.name);
        std::size_t res = expr_hash;
        res ^= name_hash + 0x9e3779b97f4a7c15ull + (res << 6) + (res >> 2);
        return res;
    }

    std::size_t hash<alvo::ast::Expr::LocalVar>::operator()(
        const alvo::ast::Expr::LocalVar& n) const noexcept {
        return std::hash<decltype(n.id)>()(n.id);
    }

    std::size_t hash<alvo::ast::Expr::FuncArg>::operator()(
        const alvo::ast::Expr::FuncArg& n) const noexcept {
        return std::hash<decltype(n.name)>()(n.name);
    }

    std::size_t hash<alvo::ast::Expr::ResolvedDecl>::operator()(
        const alvo::ast::Expr::ResolvedDecl& n) const noexcept {
        std::size_t decl_id_hash = std::hash<decltype(n.decl_id)>()(n.decl_id);
        std::size_t generic_params_hash =
            std::hash<decltype(n.generic_params)>()(n.generic_params);
        std::size_t res = decl_id_hash;
        res ^= generic_params_hash + 0x9e3779b97f4a7c15ull + (res << 6) +
               (res >> 2);
        return res;
    }

    std::size_t hash<alvo::ast::Expr::ResolvedTypeMemberAccess>::operator()(
        const alvo::ast::Expr::ResolvedTypeMemberAccess& n) const noexcept {
        std::size_t type_hash = std::hash<decltype(n.type)>()(n.type);
        std::size_t member_id_hash =
            std::hash<decltype(n.member_id)>()(n.member_id);
        std::size_t generic_params_hash =
            std::hash<decltype(n.generic_params)>()(n.generic_params);
        std::size_t res = type_hash;
        res ^= member_id_hash + 0x9e3779b97f4a7c15ull + (res << 6) + (res >> 2);
        res ^= generic_params_hash + 0x9e3779b97f4a7c15ull + (res << 6) +
               (res >> 2);
        return res;
    }

    std::size_t hash<alvo::ast::Expr::ResolvedMemberAccess>::operator()(
        const alvo::ast::Expr::ResolvedMemberAccess& n) const noexcept {
        std::size_t expr_hash = std::hash<decltype(n.expr)>()(n.expr);
        std::size_t member_id_hash =
            std::hash<decltype(n.member_id)>()(n.member_id);
        std::size_t res = expr_hash;
        res ^= member_id_hash + 0x9e3779b97f4a7c15ull + (res << 6) + (res >> 2);
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

    std::size_t hash<alvo::ast::Stmt::Print>::operator()(
        const alvo::ast::Stmt::Print& n) const noexcept {
        std::size_t is_invalid_hash =
            std::hash<decltype(n.is_invalid)>()(n.is_invalid);
        std::size_t exprs_hash = std::hash<decltype(n.exprs)>()(n.exprs);
        std::size_t res = is_invalid_hash;
        res ^= exprs_hash + 0x9e3779b97f4a7c15ull + (res << 6) + (res >> 2);
        return res;
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
        std::size_t name_hash = std::hash<decltype(n.name)>()(n.name);
        std::size_t generic_params_hash =
            std::hash<decltype(n.generic_params)>()(n.generic_params);
        std::size_t val_hash = std::hash<decltype(n.val)>()(n.val);
        std::size_t res = is_export_hash;
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

    std::size_t hash<alvo::ast::Decl::DeclsBlock>::operator()(
        const alvo::ast::Decl::DeclsBlock& n) const noexcept {
        std::size_t is_invalid_hash =
            std::hash<decltype(n.is_invalid)>()(n.is_invalid);
        std::size_t generic_params_hash =
            std::hash<decltype(n.generic_params)>()(n.generic_params);
        std::size_t interface_hash =
            std::hash<decltype(n.interface)>()(n.interface);
        std::size_t decls_hash = std::hash<decltype(n.decls)>()(n.decls);
        std::size_t res = is_invalid_hash;
        res ^= generic_params_hash + 0x9e3779b97f4a7c15ull + (res << 6) +
               (res >> 2);
        res ^= interface_hash + 0x9e3779b97f4a7c15ull + (res << 6) + (res >> 2);
        res ^= decls_hash + 0x9e3779b97f4a7c15ull + (res << 6) + (res >> 2);
        return res;
    }

    std::size_t hash<alvo::ast::Decl::Struct>::operator()(
        const alvo::ast::Decl::Struct& n) const noexcept {
        std::size_t is_invalid_hash =
            std::hash<decltype(n.is_invalid)>()(n.is_invalid);
        std::size_t fields_hash = std::hash<decltype(n.fields)>()(n.fields);
        std::size_t decls_blocks_hash =
            std::hash<decltype(n.decls_blocks)>()(n.decls_blocks);
        std::size_t res = is_invalid_hash;
        res ^= fields_hash + 0x9e3779b97f4a7c15ull + (res << 6) + (res >> 2);
        res ^=
            decls_blocks_hash + 0x9e3779b97f4a7c15ull + (res << 6) + (res >> 2);
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
        std::size_t decls_blocks_hash =
            std::hash<decltype(n.decls_blocks)>()(n.decls_blocks);
        std::size_t res = is_invalid_hash;
        res ^= elements_hash + 0x9e3779b97f4a7c15ull + (res << 6) + (res >> 2);
        res ^=
            decls_blocks_hash + 0x9e3779b97f4a7c15ull + (res << 6) + (res >> 2);
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

    std::size_t hash<alvo::ast::Module>::operator()(
        const alvo::ast::Module& n) const noexcept {
        return std::hash<decltype(n.decls)>()(n.decls);
    }

}
