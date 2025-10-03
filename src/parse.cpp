#include "parse.h"
#include "lex.h"

#include <utility>

namespace alvo::parse {

    using enum lex::TokKind;

    SectionEmitter::SectionEmitter(SectionHandler handler) :
        m_handler(handler) { }

    void SectionEmitter::emit(SectionEvent event) { m_handler(event); }

    Parser::Parser(lex::Lexer& lexer, mem::Arena& arena) :
        m_lexer(&lexer),
        m_arena(&arena),
        m_node_ctx(arena),
        m_section_emitter(nullptr) { }

    void Parser::set_section_emitter(SectionEmitter& section_emitter) {
        m_section_emitter = &section_emitter;
    }

    ast::PathSegment Parser::parse_path_segment() {
        SectionGuard section_guard(this, __func__);

        ast::PathSegment res;
        std::optional<lex::Tok> name;
        if (accept(KwRoot)) {
            res.val = ast::PathSegment::Root {};
        } else if (accept(KwSuper)) {
            res.val = ast::PathSegment::Super {};
        } else if ((name = accept_and_get(Ident)).has_value()) {
            res.val = ast::PathSegment::Name { (*name).value };
        } else {
            res.val = ast::Invalid {};
        }
        return res;
    }

    ast::Import Parser::parse_import() {
        SectionGuard section_guard(this, __func__);

        ast::Import res;
        res.kind = ast::Import::Normal {};
        if (!expect(KwImport)) {
            // Err
        }
        while (true) {
            if (curr_is(KwRoot) || curr_is(KwSuper) || curr_is(Ident)) {
                res.segments.push_back(*m_arena, parse_path_segment());
                if (!accept(ColonColon)) {
                    break;
                }
            } else if (accept(Star)) {
                res.kind = ast::Import::Glob {};
                break;
            } else {
                // Err
            }
        }
        if (accept(KwAs)) {
            if (std::holds_alternative<ast::Import::Glob>(res.kind)) {
                // Err
            }
            std::optional<lex::Tok> name = expect_and_get(Ident);
            if (!name) {
                // Err
            }
            res.kind = ast::Import::Renamed { (*name).value };
        }
        if (!expect(Semicolon)) {
            // Err
        }
        return res;
    }

    ast::Type Parser::parse_type() {
        SectionGuard section_guard(this, __func__);

        ast::Type res;
        if (accept(KwString)) {
            res.val = ast::Type::String {};
        } else if (accept(KwChar)) {
            res.val = ast::Type::Char {};
        } else if (accept(KwInt)) {
            res.val = ast::Type::Int {};
        } else if (accept(KwByte)) {
            res.val = ast::Type::Byte {};
        } else if (accept(KwFloat)) {
            res.val = ast::Type::Float {};
        } else if (accept(KwBool)) {
            res.val = ast::Type::Bool {};
        } else if (accept(KwUnit)) {
            res.val = ast::Type::Unit {};
        } else if (curr_is(LBracket)) {
            res.val = parse_type_array();
        } else if (curr_is(KwTup)) {
            res.val = parse_type_tup();
        } else if (accept(KwFunc)) {
            res.val = parse_type_func();
        } else if (curr_is(Ident) || curr_is(KwRoot) || curr_is(KwSuper)) {
            res.val = parse_type_path();
        } else {
            // Err
        }
        return res;
    }

    ast::Type::Array Parser::parse_type_array() {
        SectionGuard section_guard(this, __func__);

        ast::Type::Array res;
        if (!expect(LBracket)) {
            // Err
        }
        res.type = m_node_ctx.make_node<ast::Type>(parse_type());
        if (!expect(RBracket)) {
            // Err
        }
        return res;
    }

    ast::Type::Tup Parser::parse_type_tup() {
        SectionGuard section_guard(this, __func__);

        ast::Type::Tup res;
        if (!expect(KwTup)) {
            // Err
        }
        if (!expect(LParen)) {
            // Err
        }
        res.types.push_back(*m_arena, parse_type());
        while (accept(Comma)) {
            if (curr_is(RParen)) {
                break;
            }
            res.types.push_back(*m_arena, parse_type());
        }
        if (!expect(RParen)) {
            // Err
        }
        return res;
    }

    ast::Type::Func Parser::parse_type_func() {
        SectionGuard section_guard(this, __func__);

        ast::Type::Func res;
        ast::Type return_type;
        return_type.val = ast::Type::Unit {};
        if (!expect(KwFunc)) {
            // Err
        }
        if (!expect(LParen)) {
            // Err
        }
        if (!curr_is(RParen)) {
            res.params.push_back(*m_arena, parse_type());
            while (accept(Comma)) {
                if (curr_is(RParen)) {
                    break;
                }
                res.params.push_back(*m_arena, parse_type());
            }
        }
        if (!expect(RParen)) {
            // Err
        }
        if (accept(DashRAngle)) {
            return_type = parse_type();
        }
        res.return_type = m_node_ctx.make_node<ast::Type>(return_type);
        return res;
    }

    ast::Type::Path Parser::parse_type_path() {
        SectionGuard section_guard(this, __func__);

        ast::Type::Path res;
        while (curr_is(KwRoot) || curr_is(KwSuper) || curr_is(Ident)) {
            res.segments.push_back(*m_arena, parse_path_segment());
            if (!accept(ColonColon)) {
                break;
            }
        }
        return res;
    }

    ast::Expr Parser::parse_expr() {
        SectionGuard section_guard(this, __func__);

        return parse_expr_bp(0);
    }

    ast::Expr::Literal Parser::parse_expr_literal() {
        SectionGuard section_guard(this, __func__);

        ast::Expr::Literal res;
        std::optional<lex::Tok> tok;
        if (accept(KwUnit)) {
            res.val = ast::Expr::Literal::Unit {};
        } else if (accept(KwNull)) {
            res.val = ast::Expr::Literal::Null {};
        } else if ((tok = accept_and_get(LitString)).has_value()) {
            res.val = ast::Expr::Literal::String { (*tok).value };
        } else if ((tok = accept_and_get(LitCharacter)).has_value()) {
            res.val = ast::Expr::Literal::Character { (*tok).value };
        } else if ((tok = accept_and_get(LitInteger)).has_value()) {
            res.val = ast::Expr::Literal::Integer { (*tok).value };
        } else if ((tok = accept_and_get(LitByte)).has_value()) {
            res.val = ast::Expr::Literal::Byte { (*tok).value };
        } else if ((tok = accept_and_get(LitFloating)).has_value()) {
            res.val = ast::Expr::Literal::Floating { (*tok).value };
        } else if ((tok = accept_and_get(LitBoolean)).has_value()) {
            res.val = ast::Expr::Literal::Boolean { (*tok).value };
        } else if (curr_is(LBracket)) {
            res.val = parse_expr_literal_array();
        } else if (curr_is(KwTup)) {
            res.val = parse_expr_literal_tup();
        } else if (curr_is(KwFunc)) {
            res.val = m_node_ctx.make_node<ast::Func>(parse_func());
        } else {
            // Err
        }
        return res;
    }

    ast::Expr::Literal::Array Parser::parse_expr_literal_array() {
        SectionGuard section_guard(this, __func__);

        ast::Expr::Literal::Array res;
        if (!expect(LBracket)) {
            // Err
        }
        if (accept(RBracket)) {
            res.val = ast::Expr::Literal::Array::Regular {};
        } else {
            if (accept(KwDefault)) {
                if (!expect(KwFor)) {
                    // Err
                }
                res.val = ast::Expr::Literal::Array::DefaultNTimes {
                    m_node_ctx.make_node<ast::Expr>(parse_expr())
                };
            } else {
                ast::Expr first = parse_expr();
                if (accept(KwFor)) {
                    ast::Expr times = parse_expr();
                    res.val = ast::Expr::Literal::Array::ExprNTimes {
                        m_node_ctx.make_node<ast::Expr>(first),
                        m_node_ctx.make_node<ast::Expr>(times)
                    };
                } else {
                    ast::Expr::Literal::Array::Regular regular;
                    regular.elements.push_back(*m_arena, first);
                    if (curr_is(Comma)) {
                        while (accept(Comma)) {
                            if (curr_is(RBracket)) {
                                break;
                            }
                            regular.elements.push_back(*m_arena, parse_expr());
                        }
                    }
                    res.val = regular;
                }
            }

            if (!expect(RBracket)) {
                // Err
            }
        }
        return res;
    }

    ast::Expr::Literal::Tup Parser::parse_expr_literal_tup() {
        SectionGuard section_guard(this, __func__);

        ast::Expr::Literal::Tup res;
        if (!expect(KwTup)) {
            // Err
        }
        if (!expect(LParen)) {
            // Err
        }
        res.exprs.push_back(*m_arena, parse_expr());
        while (accept(Comma)) {
            if (curr_is(RParen)) {
                break;
            }
            res.exprs.push_back(*m_arena, parse_expr());
        }
        if (!expect(RParen)) {
            // Err
        }
        return res;
    }

    ast::Block Parser::parse_block() {
        SectionGuard section_guard(this, __func__);

        ast::Block res;
        if (!expect(LBrace)) {
            // Err
        }
        if (!curr_is(RBrace)) {
            res.stmts.push_back(*m_arena, parse_stmt());
            while (!curr_is(RBrace)) {
                res.stmts.push_back(*m_arena, parse_stmt());
            }
        }
        if (!expect(RBrace)) {
            // Err
        }
        return res;
    }

    ast::Stmt Parser::parse_stmt() {
        SectionGuard section_guard(this, __func__);

        ast::Stmt res;
        while (accept(Semicolon))
            ;
        if (curr_is(KwLet)) {
            res.val = parse_stmt_let();
        } else if (curr_is(KwIf)) {
            res.val = parse_stmt_if();
        } else if (curr_is(KwSwitch)) {
            res.val = parse_stmt_switch();
        } else if (curr_is(KwLoop)) {
            res.val = parse_stmt_loop();
        } else if (curr_is(KwFor)) {
            res.val = parse_stmt_for();
        } else if (curr_is(KwWhile)) {
            res.val = parse_stmt_while();
        } else if (curr_is(KwReturn)) {
            res.val = parse_stmt_return();
        } else if (curr_is(KwDefer)) {
            res.val = parse_stmt_defer();
        } else if (curr_is(LBrace)) {
            res.val = parse_block();
        } else if (accept(KwContinue)) {
            res.val = ast::Stmt::Continue {};
        } else if (accept(KwBreak)) {
            res.val = ast::Stmt::Break {};
        } else {
            res.val = parse_expr();
            if (!expect(Semicolon)) {
                // Err
            }
        }
        return res;
    }

    ast::Stmt::Let Parser::parse_stmt_let() {
        SectionGuard section_guard(this, __func__);

        ast::Stmt::Let res;
        res.type = std::nullopt;
        res.expr = std::nullopt;
        if (!expect(KwLet)) {
            // Err
        }
        std::optional<lex::Tok> name = expect_and_get(Ident);
        if (!name) {
            // Err
        }
        res.name = (*name).value;
        if (accept(Colon)) {
            res.type = parse_type();
        }
        if (accept(Eq)) {
            res.expr = parse_expr();
        }
        if (!expect(Semicolon)) {
            // Err
        }
        return res;
    }

    ast::Stmt::If Parser::parse_stmt_if() {
        SectionGuard section_guard(this, __func__);

        ast::Stmt::If res;
        res.else_ = std::nullopt;
        if (!expect(KwIf)) {
            // Err
        }
        res.expr = parse_expr();
        res.main = parse_block();
        while (accept(KwElif)) {
            ast::Stmt::If::Elif elif;
            elif.expr = parse_expr();
            elif.block = parse_block();
            res.elifs.push_back(*m_arena, elif);
        }
        if (accept(KwElse)) {
            res.else_ = parse_block();
        }
        return res;
    }

    ast::Stmt::Switch Parser::parse_stmt_switch() {
        SectionGuard section_guard(this, __func__);

        ast::Stmt::Switch res;
        if (!expect(KwSwitch)) {
            // Err
        }
        if (!expect(LBrace)) {
            // Err
        }
        if (!curr_is(RBrace)) {
            res.cases.push_back(*m_arena, parse_stmt_switch_case());
            while (!curr_is(RBrace)) {
                res.cases.push_back(*m_arena, parse_stmt_switch_case());
            }
        }
        if (!expect(RBrace)) {
            // Err
        }
        return res;
    }

    ast::Stmt::Switch::Case Parser::parse_stmt_switch_case() {
        SectionGuard section_guard(this, __func__);

        ast::Stmt::Switch::Case res;
        if (accept(KwDefault)) {
            res.expr = std::nullopt;
        } else {
            res.expr = parse_expr();
        }
        if (!expect(EqRAngle)) {
            // Err
        }
        res.block = parse_block();
        return res;
    }

    ast::Stmt::Loop Parser::parse_stmt_loop() {
        SectionGuard section_guard(this, __func__);

        ast::Stmt::Loop res;
        if (!expect(KwLoop)) {
            // Err
        }
        res.block = parse_block();
        return res;
    }

    ast::Stmt::For Parser::parse_stmt_for() {
        SectionGuard section_guard(this, __func__);

        ast::Stmt::For res;
        if (!expect(KwFor)) {
            // Err
        }
        std::optional<lex::Tok> name = expect_and_get(Ident);
        if (!name) {
            // Err
        }
        res.name = (*name).value;
        if (!expect(Colon)) {
            // Err
        }
        res.expr = parse_expr();
        res.block = parse_block();
        return res;
    }

    ast::Stmt::While Parser::parse_stmt_while() {
        SectionGuard section_guard(this, __func__);

        ast::Stmt::While res;
        if (!expect(KwWhile)) {
            // Err
        }
        res.expr = parse_expr();
        res.block = parse_block();
        return res;
    }

    ast::Stmt::Return Parser::parse_stmt_return() {
        SectionGuard section_guard(this, __func__);

        ast::Stmt::Return res;
        res.expr = std::nullopt;
        if (!expect(KwReturn)) {
            // Err
        }
        if (accept(Semicolon)) {
            return res;
        }
        res.expr = parse_expr();
        if (!expect(Semicolon)) {
            // Err
        }
        return res;
    }

    ast::Stmt::Defer Parser::parse_stmt_defer() {
        SectionGuard section_guard(this, __func__);

        ast::Stmt::Defer res;
        if (!expect(KwDefer)) {
            // Err
        }
        res.expr = parse_expr();
        if (!expect(Semicolon)) {
            // Err
        }
        return res;
    }

    ast::Func Parser::parse_func() {
        SectionGuard section_guard(this, __func__);

        ast::Func res;
        res.ret.val = ast::Type::Unit {};
        if (!expect(KwFunc)) {
            // Err
        }
        if (!expect(LParen)) {
            // Err
        }
        if (curr_is(Ident)) {
            res.params.push_back(*m_arena, parse_func_param());
            while (accept(Comma)) {
                if (curr_is(RParen)) {
                    break;
                }
                res.params.push_back(*m_arena, parse_func_param());
            }
        }
        if (!expect(RParen)) {
            // Err
        }
        if (accept(DashRAngle)) {
            res.ret = parse_type();
        }
        res.block = parse_block();
        return res;
    }

    ast::Func::Param Parser::parse_func_param() {
        SectionGuard section_guard(this, __func__);

        ast::Func::Param res;
        std::optional<lex::Tok> name = expect_and_get(Ident);
        if (!name) {
            // Err
        }
        res.name = (*name).value;
        if (!expect(Colon)) {
            // Err
        }
        res.type = parse_type();
        return res;
    }

    ast::Decl Parser::parse_decl() {
        SectionGuard section_guard(this, __func__);

        ast::Decl res;
        res.is_export = false;
        if (accept(KwExport)) {
            res.is_export = true;
        }

        std::optional<lex::Tok> name = expect_and_get(Ident);
        if (!name) {
            // Err
        }
        res.name = (*name).value;

        if (accept(LAngle)) {
            res.generic_params.push_back(*m_arena, parse_decl_generic_param());
            while (accept(Comma)) {
                if (curr_is(RAngle)) {
                    break;
                }
                res.generic_params.push_back(
                    *m_arena, parse_decl_generic_param());
            }
            if (!expect(RAngle)) {
                // Err
            }
        }

        if (!expect(ColonColon)) {
            // Err
        }

        if (curr_is(KwFunc)) {
            res.val = parse_func();
        } else if (curr_is(KwStruct)) {
            res.val = parse_decl_struct();
        } else if (curr_is(KwEnum)) {
            res.val = parse_decl_enum();
        } else if (curr_is(KwType)) {
            res.val = parse_decl_type_alias();
        } else if (curr_is(KwConst)) {
            res.val = parse_decl_const();
        } else if (curr_is(KwDefines)) {
            res.val = parse_decl_defines();
        } else {
            // Err
        }

        return res;
    }

    ast::Decl::GenericParam Parser::parse_decl_generic_param() {
        SectionGuard section_guard(this, __func__);

        ast::Decl::GenericParam res;
        std::optional<lex::Tok> name = expect_and_get(Ident);
        if (!name) {
            // Err
        }
        res.name = (*name).value;
        if (accept(Colon)) {
            res.interfaces.push_back(*m_arena, parse_type());
            while (accept(Plus)) {
                res.interfaces.push_back(*m_arena, parse_type());
            }
        }
        return res;
    }

    ast::Decl::Struct Parser::parse_decl_struct() {
        SectionGuard section_guard(this, __func__);

        ast::Decl::Struct res;
        if (!expect(KwStruct)) {
            // Err
        }
        if (!expect(LBrace)) {
            // Err
        }
        if (curr_is(KwExport) || curr_is(Ident)) {
            res.fields.push_back(*m_arena, parse_decl_struct_field());
            while (accept(Comma)) {
                if (curr_is(RBrace)) {
                    break;
                }
                res.fields.push_back(*m_arena, parse_decl_struct_field());
            }
        }
        if (!expect(RBrace)) {
            // Err
        }
        return res;
    }

    ast::Decl::Struct::Field Parser::parse_decl_struct_field() {
        SectionGuard section_guard(this, __func__);

        ast::Decl::Struct::Field res;
        res.is_export = false;
        if (accept(KwExport)) {
            res.is_export = true;
        }
        std::optional<lex::Tok> name = expect_and_get(Ident);
        if (!name) {
            // Err
        }
        res.name = (*name).value;
        if (!expect(Colon)) {
            // Err
        }
        res.type = parse_type();
        return res;
    }

    ast::Decl::Enum Parser::parse_decl_enum() {
        SectionGuard section_guard(this, __func__);

        ast::Decl::Enum res;
        if (!expect(KwEnum)) {
            // Err
        }
        if (!expect(LBrace)) {
            // Err
        }
        std::optional<lex::Tok> item = expect_and_get(Ident);
        if (!item) {
            // Err
        }
        res.elements.push_back(*m_arena, parse_decl_enum_element());
        while (accept(Comma)) {
            if (curr_is(RBrace)) {
                break;
            }
            res.elements.push_back(*m_arena, parse_decl_enum_element());
        }
        return res;
    }

    ast::Decl::Enum::Element Parser::parse_decl_enum_element() {
        SectionGuard section_guard(this, __func__);

        ast::Decl::Enum::Element res;
        std::optional<lex::Tok> name = expect_and_get(Ident);
        if (!name) {
            // Err
        }
        res.name = (*name).value;
        return res;
    }

    ast::Decl::TypeAlias Parser::parse_decl_type_alias() {
        SectionGuard section_guard(this, __func__);

        ast::Decl::TypeAlias res;
        if (!expect(KwType)) {
            // Err
        }
        if (!expect(Eq)) {
            // Err
        }
        res.type = parse_type();
        if (!expect(Semicolon)) {
            // Err
        }
        return res;
    }

    ast::Decl::Const Parser::parse_decl_const() {
        SectionGuard section_guard(this, __func__);

        ast::Decl::Const res;
        if (!expect(KwConst)) {
            // Err
        }
        if (!expect(Colon)) {
            // Err
        }
        res.type = parse_type();
        if (!expect(Eq)) {
            // Err
        }
        res.expr = parse_expr();
        if (!expect(Semicolon)) {
            // Err
        }
        return res;
    }

    ast::Decl::Defines Parser::parse_decl_defines() {
        SectionGuard section_guard(this, __func__);

        ast::Decl::Defines res;
        if (!expect(KwDefines)) {
            // Err
        }
        if (!accept(LBrace)) {
            res.interface = parse_type();
            if (!expect(LBrace)) {
                // Err
            }
        }
        while (!curr_is(RBrace)) {
            res.decls.push_back(*m_arena, parse_decl());
        }
        if (!expect(RBrace)) {
            // Err
        }
        return res;
    }

    ast::TopLevel Parser::parse_top_level() {
        SectionGuard section_guard(this, __func__);

        ast::TopLevel res;
        if (curr_is(KwImport)) {
            res.val = parse_import();
        } else if (curr_is(Ident) || curr_is(KwExport)) {
            res.val = parse_decl();
        }
        return res;
    }

    ast::Module Parser::parse_module() {
        SectionGuard section_guard(this, __func__);

        ast::Module res;
        while (!curr_is(Eof)) {
            res.top_levels.push_back(*m_arena, parse_top_level());
        }
        return res;
    }

    std::optional<int> Parser::prefix_bp(lex::TokKind kind) {
        switch (kind) {
        case Plus:
        case Dash:
        case Exclam:
        case Tilde:
            return 23;
        default:
            return std::nullopt;
        }
    }

    std::optional<int> Parser::postfix_bp(lex::TokKind kind) {
        switch (kind) {
        case LParen:
        case LBracket:
        case KwAs:
        case KwTryAs:
            return 25;
        default:
            return std::nullopt;
        }
    }

    std::optional<std::pair<int, int>> Parser::infix_bp(lex::TokKind kind) {
        switch (kind) {
        // Assignment
        case Eq:
        case PlusEq:
        case DashEq:
        case StarEq:
        case SlashEq:
        case AmpEq:
        case PipeEq:
        case CaretEq:
        case PercentEq:
        case LAngleLAngleEq:
        case RAngleRAngleEq:
            return std::make_pair(2, 1);

        // Logical Or/And
        case PipePipe:
            return std::make_pair(5, 6);
        case AmpAmp:
            return std::make_pair(7, 8);

        // Bitwise
        case Pipe:
            return std::make_pair(9, 10);
        case Caret:
            return std::make_pair(11, 12);
        case Amp:
            return std::make_pair(13, 14);

        // Comparison
        case EqEq:
        case ExclamEq:
        case LAngle:
        case LAngleEq:
        case RAngle:
        case RAngleEq:
            return std::make_pair(15, 16);

        // Bit Shifts
        case LAngleLAngle:
        case RAngleRAngle:
            return std::make_pair(17, 18);

        // Arithmetic
        case Plus:
        case Dash:
            return std::make_pair(19, 20);
        case Star:
        case Slash:
        case Percent:
            return std::make_pair(21, 22);

        // Member Access
        case Dot:
        case ColonColon:
            return std::make_pair(27, 28);

        default:
            return std::nullopt;
        }
    }

    ast::Expr Parser::parse_expr_bp(int min_bp) {
        ast::Expr lhs;
        std::optional<int> bp_prefix;
        std::optional<int> bp_postfix;
        std::optional<std::pair<int, int>> bp_infix;
        if (accept(LParen)) {
            lhs = parse_expr();
            if (!expect(RParen)) {
                // Err
            }
        } else if (curr_is(KwRoot) || curr_is(KwSuper) || curr_is(Ident)) {
            lhs.val = parse_path_segment();
        } else if ((bp_prefix = prefix_bp(m_lexer->peek().kind)).has_value()) {
            ast::Expr::Unop unop;
            unop.op = parse_unop_op();
            unop.expr =
                m_node_ctx.make_node<ast::Expr>(parse_expr_bp(*bp_prefix));
            lhs.val = unop;
        } else if (curr_is(KwUnit) || curr_is(KwNull) || curr_is(LitString) ||
                   curr_is(LitCharacter) || curr_is(LitInteger) ||
                   curr_is(LitByte) || curr_is(LitByte) ||
                   curr_is(LitFloating) || curr_is(LitBoolean) ||
                   curr_is(LBracket) || curr_is(KwTup) || curr_is(KwFunc)) {
            lhs.val = parse_expr_literal();
        } else {
            // Err
        }

        while (true) {
            if (accept(Eof)) {
                break;
            }

            ast::Expr res;
            ast::util::Ptr<ast::Expr> res_lhs =
                m_node_ctx.make_node<ast::Expr>(lhs);

            if ((bp_postfix = postfix_bp(m_lexer->peek().kind)).has_value()) {
                if (*bp_postfix < min_bp) {
                    break;
                }

                if (accept(LBracket)) {
                    res.val = ast::Expr::Index { res_lhs,
                        m_node_ctx.make_node<ast::Expr>(parse_expr()) };
                    lhs = res;
                    if (!expect(RBracket)) {
                        // Err
                    }
                } else if (accept(LParen)) {
                    ast::Expr::Call call;
                    call.expr = res_lhs;
                    if (accept(RParen)) {
                        res.val = call;
                        lhs = res;
                        continue;
                    }
                    call.args.push_back(*m_arena, parse_expr());
                    while (accept(Comma)) {
                        if (curr_is(RParen)) {
                            break;
                        }
                        call.args.push_back(*m_arena, parse_expr());
                    }
                    if (!expect(RParen)) {
                        // Err
                    }
                    res.val = call;
                    lhs = res;
                } else if (accept(KwAs)) {
                    res.val = ast::Expr::Cast { res_lhs, parse_type() };
                    lhs = res;
                } else if (accept(KwTryAs)) {
                    res.val = ast::Expr::TryCast { res_lhs, parse_type() };
                    lhs = res;
                }
                continue;
            }

            if ((bp_infix = infix_bp(m_lexer->peek().kind)).has_value()) {
                if ((*bp_infix).first < min_bp) {
                    break;
                }
                ast::Expr::Binop::Op op = parse_binop_op();
                ast::util::Ptr<ast::Expr> res_rhs =
                    m_node_ctx.make_node<ast::Expr>(
                        parse_expr_bp((*bp_infix).second));

                res.val = ast::Expr::Binop { res_lhs, res_rhs, op };
                lhs = res;
                continue;
            }
            break;
        }
        return lhs;
    }

    ast::Expr::Unop::Op Parser::parse_unop_op() {
        SectionGuard section_guard(this, __func__);

        lex::Tok tok = m_lexer->next();
        switch (tok.kind) {
            using Unop = ast::Expr::Unop::Op;
        case Plus:
            return Unop::Plus;
        case Dash:
            return Unop::Minus;
        case Exclam:
            return Unop::Not;
        case Tilde:
            return Unop::BinaryNot;
        default:
            return Unop::Invalid;
        }
    }

    ast::Expr::Binop::Op Parser::parse_binop_op() {
        SectionGuard section_guard(this, __func__);

        lex::Tok tok = m_lexer->next();
        switch (tok.kind) {
            using Binop = ast::Expr::Binop::Op;
        case Eq:
            return Binop::Assign;
        case PlusEq:
            return Binop::PlusAssign;
        case DashEq:
            return Binop::MinusAssign;
        case StarEq:
            return Binop::MultiplyAssign;
        case SlashEq:
            return Binop::DivideAssign;
        case AmpEq:
            return Binop::BinaryAndAssign;
        case PipeEq:
            return Binop::BinaryOrAssign;
        case CaretEq:
            return Binop::BinaryXorAssign;
        case PercentEq:
            return Binop::ModAssign;
        case LAngleLAngleEq:
            return Binop::ShiftLeftAssign;
        case RAngleRAngleEq:
            return Binop::ShiftRightAssign;
        case PipePipe:
            return Binop::Or;
        case AmpAmp:
            return Binop::And;
        case Pipe:
            return Binop::BinaryOr;
        case Caret:
            return Binop::BinaryXor;
        case Amp:
            return Binop::BinaryAnd;
        case EqEq:
            return Binop::Equal;
        case ExclamEq:
            return Binop::NotEqual;
        case LAngle:
            return Binop::Less;
        case LAngleEq:
            return Binop::LessEqual;
        case RAngle:
            return Binop::Greater;
        case RAngleEq:
            return Binop::GreaterEqual;
        case LAngleLAngle:
            return Binop::ShiftLeft;
        case RAngleRAngle:
            return Binop::ShiftRight;
        case Plus:
            return Binop::Plus;
        case Dash:
            return Binop::Minus;
        case Star:
            return Binop::Multiply;
        case Slash:
            return Binop::Divide;
        case Percent:
            return Binop::Mod;
        case Dot:
            return Binop::Access;
        case ColonColon:
            return Binop::StaticAccess;
        default:
            return Binop::Invalid;
        }
    }

    bool Parser::curr_is(lex::TokKind kind) const {
        return m_lexer->peek().kind == kind;
    }

    std::optional<lex::Tok> Parser::accept_and_get(lex::TokKind kind) {
        if (m_lexer->peek().kind == kind) {
            return m_lexer->next();
        }
        return std::nullopt;
    }

    std::optional<lex::Tok> Parser::expect_and_get(lex::TokKind kind) {
        return accept_and_get(kind);
    }

    bool Parser::accept(lex::TokKind kind) {
        if (m_lexer->peek().kind == kind) {
            m_lexer->next();
            return true;
        }
        return false;
    }

    bool Parser::expect(lex::TokKind kind) { return accept(kind); }

    void Parser::section_enter(std::string_view name) {
        if (m_section_emitter) {
            m_section_emitter->emit(SectionEvent { SectionEvent::Enter, name });
        }
    }

    void Parser::section_exit(std::string_view name) {
        if (m_section_emitter) {
            m_section_emitter->emit(SectionEvent { SectionEvent::Exit, name });
        }
    }

    void Parser::synchronize(std::initializer_list<lex::TokKind> kinds) {
        while (!curr_is(Eof)) {
            for (const auto& kind : kinds) {
                if (curr_is(kind)) {
                    break;
                }
            }
            m_lexer->next();
        }
    }

}
