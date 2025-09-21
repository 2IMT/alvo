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
        return l.value == r.value;
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
        return l.type == r.type;
    }

    bool operator==(const Type::Tup& l, const Type::Tup& r) {
        return l.types == r.types;
    }

    bool operator==(const Type::Func& l, const Type::Func& r) {
        return l.params == r.params && l.return_type == r.return_type;
    }

    bool operator==(const Type::Path& l, const Type::Path& r) {
        return l.segments == r.segments;
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
        return l.exprs == r.exprs;
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

    bool operator==(const Block& l, const Block& r) {
        return l.stmts == r.stmts;
    }

    bool operator==(const Stmt& l, const Stmt& r) { return l.val == r.val; }

    bool operator==(const Stmt::Let& l, const Stmt::Let& r) {
        return l.name == r.name && l.expr == r.expr && l.type == r.type;
    }

    bool operator==(const Stmt::If& l, const Stmt::If& r) {
        return l.expr == r.expr && l.main == r.main && l.elifs == r.elifs &&
               l.else_ == r.else_;
    }

    bool operator==(const Stmt::If::Elif& l, const Stmt::If::Elif& r) {
        return l.expr == r.expr && l.block == r.block;
    }

    bool operator==(const Stmt::Switch& l, const Stmt::Switch& r) {
        return l.expr == r.expr && l.cases == r.cases;
    }

    bool operator==(const Stmt::Switch::Case& l, const Stmt::Switch::Case& r) {
        return l.expr == r.expr && l.block == r.block;
    }

    bool operator==(const Stmt::Loop& l, const Stmt::Loop& r) {
        return l.block == r.block;
    }

    bool operator==(const Stmt::For& l, const Stmt::For& r) {
        return l.name == r.name && l.expr == r.expr && l.block == r.block;
    }

    bool operator==(const Stmt::While& l, const Stmt::While& r) {
        return l.expr == r.expr && l.block == r.block;
    }

    bool operator==(const Stmt::Return& l, const Stmt::Return& r) {
        return l.expr == r.expr;
    }

    bool operator==(const Stmt::Defer& l, const Stmt::Defer& r) {
        return l.expr == r.expr;
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
        return l.params == r.params && l.ret == r.ret && l.block == r.block;
    }

    bool operator==(const Func::Param& l, const Func::Param& r) {
        return l.name == r.name && l.type == r.type;
    }

    bool operator==(const Decl& l, const Decl& r) {
        return l.is_export == r.is_export && l.name == r.name && l.val == r.val;
    }

    bool operator==(const Decl::Struct& l, const Decl::Struct& r) {
        return l.items == r.items;
    }

    bool operator==(const Decl::Struct::Item& l, const Decl::Struct::Item& r) {
        return l.is_export == r.is_export && l.val == r.val;
    }

    bool operator==(const Decl::Struct::Item::Field& l,
        const Decl::Struct::Item::Field& r) {
        return l.name == r.name && l.type == r.type;
    }

    bool operator==(const Decl::Enum& l, const Decl::Enum& r) {
        return l.items == r.items;
    }

    bool operator==(const Decl::Enum::Item& l, const Decl::Enum::Item& r) {
        return l.val == r.val;
    }

    bool operator==(
        const Decl::Enum::Item::Field& l, const Decl::Enum::Item::Field& r) {
        return l.name == r.name;
    }

    bool operator==(const Decl::TypeAlias& l, const Decl::TypeAlias& r) {
        return l.type == r.type;
    }

    bool operator==(const Decl::Const& l, const Decl::Const& r) {
        return l.type == r.type && l.expr == r.expr;
    }

    bool operator==(const TopLevel& l, const TopLevel& r) {
        return l.val == r.val;
    }

    bool operator==(const Module& l, const Module& r) {
        return l.top_levels == r.top_levels;
    }

#define NOT_EQ(l, r) (!((l) == (r)))

    bool operator!=(const Invalid& l, const Invalid& r) { return NOT_EQ(l, r); }

    bool operator!=(const PathSegment& l, const PathSegment& r) {
        return NOT_EQ(l, r);
    }

    bool operator!=(const PathSegment::Root& l, const PathSegment::Root& r) {
        return NOT_EQ(l, r);
    }

    bool operator!=(const PathSegment::Super& l, const PathSegment::Super& r) {
        return NOT_EQ(l, r);
    }

    bool operator!=(const PathSegment::Name& l, const PathSegment::Name& r) {
        return NOT_EQ(l, r);
    }

    bool operator!=(const Import& l, const Import& r) { return NOT_EQ(l, r); }

    bool operator!=(const Import::Normal& l, const Import::Normal& r) {
        return NOT_EQ(l, r);
    }

    bool operator!=(const Import::Glob& l, const Import::Glob& r) {
        return NOT_EQ(l, r);
    }

    bool operator!=(const Import::Renamed& l, const Import::Renamed& r) {
        return NOT_EQ(l, r);
    }

    bool operator!=(const Type& l, const Type& r) { return NOT_EQ(l, r); }

    bool operator!=(const Type::Unit& l, const Type::Unit& r) {
        return NOT_EQ(l, r);
    }

    bool operator!=(const Type::String& l, const Type::String& r) {
        return NOT_EQ(l, r);
    }

    bool operator!=(const Type::Char& l, const Type::Char& r) {
        return NOT_EQ(l, r);
    }

    bool operator!=(const Type::Int& l, const Type::Int& r) {
        return NOT_EQ(l, r);
    }

    bool operator!=(const Type::Byte& l, const Type::Byte& r) {
        return NOT_EQ(l, r);
    }

    bool operator!=(const Type::Float& l, const Type::Float& r) {
        return NOT_EQ(l, r);
    }

    bool operator!=(const Type::Bool& l, const Type::Bool& r) {
        return NOT_EQ(l, r);
    }

    bool operator!=(const Type::Array& l, const Type::Array& r) {
        return NOT_EQ(l, r);
    }

    bool operator!=(const Type::Tup& l, const Type::Tup& r) {
        return NOT_EQ(l, r);
    }

    bool operator!=(const Type::Func& l, const Type::Func& r) {
        return NOT_EQ(l, r);
    }

    bool operator!=(const Type::Path& l, const Type::Path& r) {
        return NOT_EQ(l, r);
    }

    bool operator!=(const Expr& l, const Expr& r) { return NOT_EQ(l, r); }

    bool operator!=(const Expr::Literal& l, const Expr::Literal& r) {
        return NOT_EQ(l, r);
    }

    bool operator!=(
        const Expr::Literal::Unit& l, const Expr::Literal::Unit& r) {
        return NOT_EQ(l, r);
    }

    bool operator!=(
        const Expr::Literal::Null& l, const Expr::Literal::Null& r) {
        return NOT_EQ(l, r);
    }

    bool operator!=(
        const Expr::Literal::String& l, const Expr::Literal::String& r) {
        return NOT_EQ(l, r);
    }

    bool operator!=(
        const Expr::Literal::Character& l, const Expr::Literal::Character& r) {
        return NOT_EQ(l, r);
    }

    bool operator!=(
        const Expr::Literal::Integer& l, const Expr::Literal::Integer& r) {
        return NOT_EQ(l, r);
    }

    bool operator!=(
        const Expr::Literal::Byte& l, const Expr::Literal::Byte& r) {
        return NOT_EQ(l, r);
    }

    bool operator!=(
        const Expr::Literal::Floating& l, const Expr::Literal::Floating& r) {
        return NOT_EQ(l, r);
    }

    bool operator!=(
        const Expr::Literal::Boolean& l, const Expr::Literal::Boolean& r) {
        return NOT_EQ(l, r);
    }

    bool operator!=(
        const Expr::Literal::Array& l, const Expr::Literal::Array& r) {
        return NOT_EQ(l, r);
    }

    bool operator!=(const Expr::Literal::Array::Regular& l,
        const Expr::Literal::Array::Regular& r) {
        return NOT_EQ(l, r);
    }

    bool operator!=(const Expr::Literal::Array::DefaultNTimes& l,
        const Expr::Literal::Array::DefaultNTimes& r) {
        return NOT_EQ(l, r);
    }

    bool operator!=(const Expr::Literal::Array::ExprNTimes& l,
        const Expr::Literal::Array::ExprNTimes& r) {
        return NOT_EQ(l, r);
    }

    bool operator!=(const Expr::Literal::Tup& l, const Expr::Literal::Tup& r) {
        return NOT_EQ(l, r);
    }

    bool operator!=(const Expr::Unop& l, const Expr::Unop& r) {
        return NOT_EQ(l, r);
    }

    bool operator!=(const Expr::Binop& l, const Expr::Binop& r) {
        return NOT_EQ(l, r);
    }

    bool operator!=(const Expr::Index& l, const Expr::Index& r) {
        return NOT_EQ(l, r);
    }

    bool operator!=(const Expr::Call& l, const Expr::Call& r) {
        return NOT_EQ(l, r);
    }

    bool operator!=(const Expr::Cast& l, const Expr::Cast& r) {
        return NOT_EQ(l, r);
    }

    bool operator!=(const Expr::TryCast& l, const Expr::TryCast& r) {
        return NOT_EQ(l, r);
    }

    bool operator!=(const Block& l, const Block& r) { return NOT_EQ(l, r); }

    bool operator!=(const Stmt& l, const Stmt& r) { return NOT_EQ(l, r); }

    bool operator!=(const Stmt::Let& l, const Stmt::Let& r) {
        return NOT_EQ(l, r);
    }

    bool operator!=(const Stmt::If& l, const Stmt::If& r) {
        return NOT_EQ(l, r);
    }

    bool operator!=(const Stmt::If::Elif& l, const Stmt::If::Elif& r) {
        return NOT_EQ(l, r);
    }

    bool operator!=(const Stmt::Switch& l, const Stmt::Switch& r) {
        return NOT_EQ(l, r);
    }

    bool operator!=(const Stmt::Switch::Case& l, const Stmt::Switch::Case& r) {
        return NOT_EQ(l, r);
    }

    bool operator!=(const Stmt::Loop& l, const Stmt::Loop& r) {
        return NOT_EQ(l, r);
    }

    bool operator!=(const Stmt::For& l, const Stmt::For& r) {
        return NOT_EQ(l, r);
    }

    bool operator!=(const Stmt::While& l, const Stmt::While& r) {
        return NOT_EQ(l, r);
    }

    bool operator!=(const Stmt::Return& l, const Stmt::Return& r) {
        return NOT_EQ(l, r);
    }

    bool operator!=(const Stmt::Defer& l, const Stmt::Defer& r) {
        return NOT_EQ(l, r);
    }

    bool operator!=(const Stmt::Continue& l, const Stmt::Continue& r) {
        return NOT_EQ(l, r);
    }

    bool operator!=(const Stmt::Break& l, const Stmt::Break& r) {
        return NOT_EQ(l, r);
    }

    bool operator!=(const Func& l, const Func& r) { return NOT_EQ(l, r); }

    bool operator!=(const Func::Param& l, const Func::Param& r) {
        return NOT_EQ(l, r);
    }

    bool operator!=(const Decl& l, const Decl& r) { return NOT_EQ(l, r); }

    bool operator!=(const Decl::Struct& l, const Decl::Struct& r) {
        return NOT_EQ(l, r);
    }

    bool operator!=(const Decl::Struct::Item& l, const Decl::Struct::Item& r) {
        return NOT_EQ(l, r);
    }

    bool operator!=(const Decl::Struct::Item::Field& l,
        const Decl::Struct::Item::Field& r) {
        return NOT_EQ(l, r);
    }

    bool operator!=(const Decl::Enum& l, const Decl::Enum& r) {
        return NOT_EQ(l, r);
    }

    bool operator!=(const Decl::Enum::Item& l, const Decl::Enum::Item& r) {
        return NOT_EQ(l, r);
    }

    bool operator!=(
        const Decl::Enum::Item::Field& l, const Decl::Enum::Item::Field& r) {
        return NOT_EQ(l, r);
    }

    bool operator!=(const Decl::TypeAlias& l, const Decl::TypeAlias& r) {
        return NOT_EQ(l, r);
    }

    bool operator!=(const Decl::Const& l, const Decl::Const& r) {
        return NOT_EQ(l, r);
    }

    bool operator!=(const TopLevel& l, const TopLevel& r) {
        return NOT_EQ(l, r);
    }

    bool operator!=(const Module& l, const Module& r) { return NOT_EQ(l, r); }

}
