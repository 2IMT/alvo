#pragma once

#include <optional>
#include <functional>

#include "lex.h"
#include "ast.h"
#include "mem.h"

namespace alvo::parse {

    struct SectionEvent {
        enum Type {
            Enter,
            Exit,
        };

        Type type;
        std::string_view section;
    };

    using SectionHandler = std::function<void(SectionEvent)>;

    class SectionEmitter {
    public:
        SectionEmitter(SectionHandler handler);

        void emit(SectionEvent event);

    private:
        SectionHandler m_handler;
    };

    class Parser {
    public:
        Parser(lex::Lexer& lexer, mem::Arena& arena);

        void set_section_emitter(SectionEmitter& section_emitter);

        ast::PathSegment parse_path_segment();

        ast::Import parse_import();

        ast::Type parse_type();

        ast::Type::Array parse_type_array();

        ast::Type::Tup parse_type_tup();

        ast::Type::Func parse_type_func();

        ast::Type::Path parse_type_path();

        ast::Expr parse_expr();

        ast::Expr::Literal parse_expr_literal();

        ast::Expr::Literal::Array parse_expr_literal_array();

        ast::Expr::Literal::Tup parse_expr_literal_tup();

        ast::Block parse_block();

        ast::Stmt parse_stmt();

        ast::Stmt::Let parse_stmt_let();

        ast::Stmt::If parse_stmt_if();

        ast::Stmt::Switch parse_stmt_switch();

        ast::Stmt::Switch::Case parse_stmt_switch_case();

        ast::Stmt::Loop parse_stmt_loop();

        ast::Stmt::For parse_stmt_for();

        ast::Stmt::While parse_stmt_while();

        ast::Stmt::Return parse_stmt_return();

        ast::Stmt::Defer parse_stmt_defer();

        ast::Func parse_func();

        ast::Func::Param parse_func_param();

        ast::Decl parse_decl();

        ast::Decl::Struct parse_decl_struct();

        ast::Decl::Struct::Item parse_decl_struct_item();

        ast::Decl::Enum parse_decl_enum();

        ast::Decl::Enum::Item parse_decl_enum_item();

        ast::Decl::TypeAlias parse_decl_type_alias();

        ast::Decl::Const parse_decl_const();

        ast::TopLevel parse_top_level();

        ast::Module parse_module();

    private:
        class SectionGuard {
        public:
            SectionGuard(Parser* parser, std::string_view section) :
                m_parser(parser),
                m_section(section) {
                m_parser->section_enter(m_section);
            }

            SectionGuard(const SectionGuard&) = delete;

            SectionGuard(SectionGuard&&) = delete;

            SectionGuard& operator=(const SectionGuard&) = delete;

            SectionGuard& operator=(SectionGuard&&) = delete;

            ~SectionGuard() { m_parser->section_exit(m_section); }

        private:
            Parser* m_parser;
            std::string_view m_section;
        };

        static std::optional<int> prefix_bp(lex::TokKind kind);

        static std::optional<int> postfix_bp(lex::TokKind kind);

        static std::optional<std::pair<int, int>> infix_bp(lex::TokKind kind);

        ast::Expr parse_expr_bp(int min_bp);

        ast::Expr::Unop::Op parse_unop_op();

        ast::Expr::Binop::Op parse_binop_op();

        bool curr_is(lex::TokKind kind) const;

        std::optional<lex::Tok> accept_and_get(lex::TokKind kind);

        std::optional<lex::Tok> expect_and_get(lex::TokKind kind);

        bool accept(lex::TokKind kind);

        bool expect(lex::TokKind kind);

        void section_enter(std::string_view name);

        void section_exit(std::string_view name);

        lex::Lexer* m_lexer;
        mem::Arena* m_arena;
        ast::NodeCtx m_node_ctx;
        SectionEmitter* m_section_emitter;
    };

}
