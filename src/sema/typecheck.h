#pragma once

#include <string_view>
#include <optional>
#include <utility>

#include "resolve.h"
#include "../ast.h"
#include "../diag.h"
#include "../mem.h"
#include "../ast/util.h"

namespace alvo::sema::typecheck {

    struct Value {
        ast::Type type;
        bool is_assignable;

        Value(const ast::Type& type, bool is_assignable) :
            type(type),
            is_assignable(is_assignable) { }

        Value(const ast::Type& type) :
            type(type),
            is_assignable(false) { }

        Value(ast::Type&& type) :
            type(type),
            is_assignable(false) { }

        operator ast::Type() { return type; }
    };

    class Typechecker {
    public:
        Typechecker(resolve::NameIndex& name_index, mem::Arena& arena);

        void set_diag_sink(diag::DiagSink& sink);

        void typecheck();

        void typecheck_decl(resolve::Decl& decl);

        void typecheck_decl_func(resolve::Decl::Func& func);

        ast::Type typecheck_ast_func(ast::Func& func,
            std::optional<ast::Type> expected_type = std::nullopt);

        std::optional<ast::Type> typecheck_ast_block(
            ast::Block& block, ast::Type expected_return_type);

        std::optional<ast::Type> typecheck_ast_stmt(
            ast::Stmt& stmt, ast::Type expected_return_type);

        void typecheck_ast_stmt_let(ast::Stmt::Let& let);

        std::optional<ast::Type> typecheck_ast_stmt_if(
            ast::Stmt::If& if_, ast::Type expected_return_type);

        std::optional<ast::Type> typecheck_ast_stmt_switch(
            ast::Stmt::Switch& switch_, ast::Type expected_return_type);

        void typecheck_ast_stmt_loop(
            ast::Stmt::Loop& loop, ast::Type expected_return_type);

        void typecheck_ast_stmt_for(
            ast::Stmt::For& for_, ast::Type expected_return_type);

        void typecheck_ast_stmt_while(
            ast::Stmt::While& while_, ast::Type expected_return_type);

        void typecheck_ast_stmt_defer(ast::Stmt::Defer& defer);

        Value typecheck_ast_expr(ast::Expr& expr,
            std::optional<ast::Type> expected_type = std::nullopt);

        Value typecheck_ast_expr_literal(ast::Expr::Literal& lit,
            std::optional<ast::Type> expected_type = std::nullopt);

        Value typecheck_ast_expr_literal_array(ast::Expr::Literal::Array& array,
            std::optional<ast::Type> expected_type = std::nullopt);

        Value typecheck_ast_expr_literal_tup(ast::Expr::Literal::Tup& tup,
            std::optional<ast::Type> expected_type = std::nullopt);

        Value typecheck_ast_expr_literal_resolved_struct(
            ast::Expr::Literal::ResolvedStruct& struct_,
            std::optional<ast::Type> expected_type = std::nullopt);

        Value typecheck_ast_expr_unop(ast::Expr::Unop& unop,
            std::optional<ast::Type> expected_type = std::nullopt);

        Value typecheck_ast_expr_binop(ast::Expr::Binop& binop,
            std::optional<ast::Type> expected_type = std::nullopt);

        Value typecheck_ast_expr_index(ast::Expr::Index& index,
            std::optional<ast::Type> expected_type = std::nullopt);

        Value typecheck_ast_expr_call(ast::Expr::Call& call,
            std::optional<ast::Type> expected_type = std::nullopt);

        Value typecheck_ast_expr_cast(ast::Expr::Cast& cast,
            std::optional<ast::Type> expected_type = std::nullopt);

        Value typecheck_ast_expr_try_cast(ast::Expr::TryCast& try_cast,
            std::optional<ast::Type> expected_type = std::nullopt);

        Value typecheck_ast_expr_builtin(ast::Expr::Builtin& builtin,
            std::optional<ast::Type> expected_type = std::nullopt);

        std::pair<Value, ast::Expr::ResolvedMemberAccess>
        typecheck_ast_expr_member_access(ast::Expr::MemberAccess& member_access,
            std::optional<ast::Type> expected_type = std::nullopt);

        Value typecheck_ast_expr_local_var(ast::Expr::LocalVar& local_var,
            std::optional<ast::Type> expected_type = std::nullopt);

        Value typecheck_ast_expr_func_arg(ast::Expr::FuncArg& func_arg,
            std::optional<ast::Type> expected_type = std::nullopt);

        Value typecheck_ast_expr_resolved_decl(
            ast::Expr::ResolvedDecl& resolved_decl,
            std::optional<ast::Type> expected_type = std::nullopt);

        Value typecheck_ast_expr_resolved_type_member_access(
            ast::Expr::ResolvedTypeMemberAccess& resolved_type_member_access,
            std::optional<ast::Type> expected_type = std::nullopt);

    private:
        resolve::NameIndex* m_index;
        mem::Arena* m_arena;
        ast::util::NodeCtx m_node_ctx;
        diag::DiagEmitter m_diag_emitter;
        resolve::ScopeStack<ast::Type, true> m_scope_stack;
        resolve::ScopeStack<resolve::GenericParam, false> m_generic_scope_stack;
        resolve::ScopeStack<ast::Type, true> m_arg_stack;

        void put_generic_params(const resolve::GenericParams& generic_params);

        void err(const diag::Err::Val& err);

        void err_expected(std::string_view expected);

        void err_expected_type(const ast::Type& expected);

        std::optional<ast::Id> instantiate_type(
            ast::Id id, const ast::util::List<ast::Type> generic_params);

        std::optional<ast::Id> instantiate_member(ast::Id type_id,
            ast::Id member_id, const ast::util::List<ast::Type> generic_params);

        std::optional<ast::Id> instantiate_named_member(ast::Id type_id,
            std::string_view member_name,
            const ast::util::List<ast::Type> generic_params);

        std::optional<ast::Id> instantiate_decl(
            ast::Id decl_id, const ast::util::List<ast::Type> generic_params);

        ast::Type::Func decl_func_to_type_func(const ast::Func& func);
    };

}
