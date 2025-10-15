#include "parse.h"

#include <utility>

namespace alvo::parse {

    using enum lex::TokKind;
    using namespace ast;

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

    PathSegment Parser::parse_path_segment() {
        SectionGuard section_guard(this, __func__);

        PathSegment::Val val;
        std::optional<lex::Tok> name;
        if (accept(KwRoot)) {
            val = PathSegment::Root {};
        } else if (accept(KwSuper)) {
            val = PathSegment::Super {};
        } else if ((name = accept_and_get(Ident)).has_value()) {
            util::List<Type> generic_params;
            std::string_view value = (*name).value;
            if (accept(LAngle)) {
                generic_params.push_back(*m_arena, parse_type());
                while (accept(Comma)) {
                    generic_params.push_back(*m_arena, parse_type());
                }
                if (!expect(RAngle)) {
                    // Err
                }
            }
            val = PathSegment::Name(value, generic_params);
        } else {
            // Err
        }
        return PathSegment(val);
    }

    Import Parser::parse_import() {
        SectionGuard section_guard(this, __func__);

        Import::Kind kind = Import::Normal {};
        util::List<PathSegment> segments;
        if (!expect(KwImport)) {
            // Err
        }
        while (true) {
            if (curr_is(KwRoot) || curr_is(KwSuper) || curr_is(Ident)) {
                segments.push_back(*m_arena, parse_path_segment());
                if (!accept(ColonColon)) {
                    break;
                }
            } else if (accept(Star)) {
                kind = Import::Glob {};
                break;
            } else {
                // Err
            }
        }
        if (accept(KwAs)) {
            if (std::holds_alternative<Import::Glob>(kind)) {
                // Err
            }
            std::optional<lex::Tok> name = expect_and_get(Ident);
            if (!name) {
                // Err
            }
            kind = Import::Renamed { (*name).value };
        }
        if (!expect(Semicolon)) {
            // Err
        }
        return Import(kind, segments);
    }

    Type Parser::parse_type() {
        SectionGuard section_guard(this, __func__);

        Type::Val val;
        if (accept(KwString)) {
            val = Type::String {};
        } else if (accept(KwChar)) {
            val = Type::Char {};
        } else if (accept(KwInt)) {
            val = Type::Int {};
        } else if (accept(KwByte)) {
            val = Type::Byte {};
        } else if (accept(KwFloat)) {
            val = Type::Float {};
        } else if (accept(KwBool)) {
            val = Type::Bool {};
        } else if (accept(KwUnit)) {
            val = Type::Unit {};
        } else if (curr_is(LBracket)) {
            val = parse_type_array();
        } else if (curr_is(KwTup)) {
            val = parse_type_tup();
        } else if (accept(KwFunc)) {
            val = parse_type_func();
        } else if (curr_is(Ident) || curr_is(KwRoot) || curr_is(KwSuper)) {
            val = parse_type_path();
        } else {
            // Err
        }
        bool nullable = false;
        if (accept(Question)) {
            nullable = true;
        }
        return Type(val, nullable);
    }

    Type::Array Parser::parse_type_array() {
        SectionGuard section_guard(this, __func__);

        if (!expect(LBracket)) {
            // Err
        }
        util::Ptr<Type> type = m_node_ctx.make_node<Type>(parse_type());
        if (!expect(RBracket)) {
            // Err
        }
        return Type::Array(type);
    }

    Type::Tup Parser::parse_type_tup() {
        SectionGuard section_guard(this, __func__);

        util::List<Type> types;
        if (!expect(KwTup)) {
            // Err
        }
        if (!expect(LParen)) {
            // Err
        }
        types.push_back(*m_arena, parse_type());
        while (accept(Comma)) {
            if (curr_is(RParen)) {
                break;
            }
            types.push_back(*m_arena, parse_type());
        }
        if (!expect(RParen)) {
            // Err
        }
        return Type::Tup(types);
    }

    Type::Func Parser::parse_type_func() {
        SectionGuard section_guard(this, __func__);

        util::List<Type> params;
        util::Ptr<Type> return_type =
            m_node_ctx.make_node<Type>(Type::Unit {}, false);
        if (!expect(KwFunc)) {
            // Err
        }
        if (!expect(LParen)) {
            // Err
        }
        if (!curr_is(RParen)) {
            params.push_back(*m_arena, parse_type());
            while (accept(Comma)) {
                if (curr_is(RParen)) {
                    break;
                }
                params.push_back(*m_arena, parse_type());
            }
        }
        if (!expect(RParen)) {
            // Err
        }
        if (accept(DashRAngle)) {
            *return_type = parse_type();
        }
        return Type::Func(params, return_type);
    }

    Type::Path Parser::parse_type_path() {
        SectionGuard section_guard(this, __func__);

        util::List<PathSegment> segments;
        while (curr_is(KwRoot) || curr_is(KwSuper) || curr_is(Ident)) {
            segments.push_back(*m_arena, parse_path_segment());
            if (!accept(ColonColon)) {
                break;
            }
        }
        return Type::Path(segments);
    }

    Expr Parser::parse_expr() {
        SectionGuard section_guard(this, __func__);

        return parse_expr_bp(0);
    }

    Expr::Literal Parser::parse_expr_literal() {
        SectionGuard section_guard(this, __func__);

        Expr::Literal::Val val;
        std::optional<lex::Tok> tok;
        if (accept(KwUnit)) {
            val = Expr::Literal::Unit {};
        } else if (accept(KwNull)) {
            val = Expr::Literal::Null {};
        } else if ((tok = accept_and_get(LitString)).has_value()) {
            val = Expr::Literal::String { (*tok).value };
        } else if ((tok = accept_and_get(LitCharacter)).has_value()) {
            val = Expr::Literal::Character { (*tok).value };
        } else if ((tok = accept_and_get(LitInteger)).has_value()) {
            val = Expr::Literal::Integer { (*tok).value };
        } else if ((tok = accept_and_get(LitByte)).has_value()) {
            val = Expr::Literal::Byte { (*tok).value };
        } else if ((tok = accept_and_get(LitFloating)).has_value()) {
            val = Expr::Literal::Floating { (*tok).value };
        } else if ((tok = accept_and_get(LitBoolean)).has_value()) {
            val = Expr::Literal::Boolean { (*tok).value };
        } else if (curr_is(LBracket)) {
            val = parse_expr_literal_array();
        } else if (curr_is(KwTup)) {
            val = parse_expr_literal_tup();
        } else if (curr_is(KwFunc)) {
            val = m_node_ctx.make_node<Func>(parse_func());
        } else {
            // Err
        }
        return Expr::Literal(val);
    }

    Expr::Literal::Array Parser::parse_expr_literal_array() {
        SectionGuard section_guard(this, __func__);

        Expr::Literal::Array::Val val;
        if (!expect(LBracket)) {
            // Err
        }
        if (accept(RBracket)) {
            val = Expr::Literal::Array::Regular { util::List<Expr>() };
        } else {
            if (accept(KwDefault)) {
                if (!expect(KwFor)) {
                    // Err
                }
                val = Expr::Literal::Array::DefaultNTimes {
                    m_node_ctx.make_node<Expr>(parse_expr())
                };
            } else {
                Expr first = parse_expr();
                if (accept(KwFor)) {
                    Expr times = parse_expr();
                    val = Expr::Literal::Array::ExprNTimes(
                        m_node_ctx.make_node<Expr>(first),
                        m_node_ctx.make_node<Expr>(times));
                } else {
                    util::List<Expr> elements;
                    elements.push_back(*m_arena, first);
                    if (curr_is(Comma)) {
                        while (accept(Comma)) {
                            if (curr_is(RBracket)) {
                                break;
                            }
                            elements.push_back(*m_arena, parse_expr());
                        }
                    }
                    val = Expr::Literal::Array::Regular(elements);
                }
            }

            if (!expect(RBracket)) {
                // Err
            }
        }
        return Expr::Literal::Array(val);
    }

    Expr::Literal::Tup Parser::parse_expr_literal_tup() {
        SectionGuard section_guard(this, __func__);

        util::List<Expr> exprs;
        if (!expect(KwTup)) {
            // Err
        }
        if (!expect(LParen)) {
            // Err
        }
        exprs.push_back(*m_arena, parse_expr());
        while (accept(Comma)) {
            if (curr_is(RParen)) {
                break;
            }
            exprs.push_back(*m_arena, parse_expr());
        }
        if (!expect(RParen)) {
            // Err
        }
        return Expr::Literal::Tup(exprs);
    }

    Block Parser::parse_block() {
        SectionGuard section_guard(this, __func__);

        util::List<Stmt> stmts;
        if (!expect(LBrace)) {
            // Err
        }
        if (!curr_is(RBrace)) {
            stmts.push_back(*m_arena, parse_stmt());
            while (!curr_is(RBrace)) {
                stmts.push_back(*m_arena, parse_stmt());
            }
        }
        if (!expect(RBrace)) {
            // Err
        }
        return Block(stmts);
    }

    Stmt Parser::parse_stmt() {
        SectionGuard section_guard(this, __func__);

        Stmt::Val val;
        while (accept(Semicolon))
            ;
        if (curr_is(KwLet)) {
            val = parse_stmt_let();
        } else if (curr_is(KwIf)) {
            val = parse_stmt_if();
        } else if (curr_is(KwSwitch)) {
            val = parse_stmt_switch();
        } else if (curr_is(KwLoop)) {
            val = parse_stmt_loop();
        } else if (curr_is(KwFor)) {
            val = parse_stmt_for();
        } else if (curr_is(KwWhile)) {
            val = parse_stmt_while();
        } else if (curr_is(KwReturn)) {
            val = parse_stmt_return();
        } else if (curr_is(KwDefer)) {
            val = parse_stmt_defer();
        } else if (curr_is(LBrace)) {
            val = parse_block();
        } else if (accept(KwContinue)) {
            val = Stmt::Continue {};
        } else if (accept(KwBreak)) {
            val = Stmt::Break {};
        } else {
            val = parse_expr();
            if (!expect(Semicolon)) {
                // Err
            }
        }
        return Stmt(val);
    }

    Stmt::Let Parser::parse_stmt_let() {
        SectionGuard section_guard(this, __func__);

        std::string_view name;
        std::optional<Type> type = std::nullopt;
        std::optional<Expr> expr = std::nullopt;
        if (!expect(KwLet)) {
            // Err
        }
        std::optional<lex::Tok> name_tok = expect_and_get(Ident);
        if (!name_tok) {
            // Err
        }
        name = (*name_tok).value;
        if (accept(Colon)) {
            type = parse_type();
        }
        if (accept(Eq)) {
            expr = parse_expr();
        }
        if (!expect(Semicolon)) {
            // Err
        }
        return Stmt::Let(name, type, expr);
    }

    Stmt::If Parser::parse_stmt_if() {
        SectionGuard section_guard(this, __func__);

        if (!expect(KwIf)) {
            // Err
        }
        Expr expr = parse_expr();
        Block main = parse_block();
        util::List<Stmt::If::Elif> elifs;
        while (accept(KwElif)) {
            Expr expr = parse_expr();
            Block block = parse_block();
            elifs.push_back(*m_arena, Stmt::If::Elif(expr, block));
        }
        std::optional<Block> else_ = std::nullopt;
        if (accept(KwElse)) {
            else_ = parse_block();
        }
        return Stmt::If(expr, main, elifs, else_);
    }

    Stmt::Switch Parser::parse_stmt_switch() {
        SectionGuard section_guard(this, __func__);

        util::List<Stmt::Switch::Case> cases;
        if (!expect(KwSwitch)) {
            // Err
        }
        Expr expr = parse_expr();
        if (!expect(LBrace)) {
            // Err
        }
        if (!curr_is(RBrace)) {
            cases.push_back(*m_arena, parse_stmt_switch_case());
            while (!curr_is(RBrace)) {
                cases.push_back(*m_arena, parse_stmt_switch_case());
            }
        }
        if (!expect(RBrace)) {
            // Err
        }
        return Stmt::Switch(expr, cases);
    }

    Stmt::Switch::Case Parser::parse_stmt_switch_case() {
        SectionGuard section_guard(this, __func__);

        std::optional<Expr> expr = std::nullopt;
        if (accept(KwDefault)) {
            expr = std::nullopt;
        } else {
            expr = parse_expr();
        }
        if (!expect(EqRAngle)) {
            // Err
        }
        Block block = parse_block();
        return Stmt::Switch::Case(expr, block);
    }

    Stmt::Loop Parser::parse_stmt_loop() {
        SectionGuard section_guard(this, __func__);

        if (!expect(KwLoop)) {
            // Err
        }
        Block block = parse_block();
        return Stmt::Loop(block);
    }

    Stmt::For Parser::parse_stmt_for() {
        SectionGuard section_guard(this, __func__);

        if (!expect(KwFor)) {
            // Err
        }
        std::optional<lex::Tok> tok_name = expect_and_get(Ident);
        if (!tok_name) {
            // Err
        }
        std::string_view name = (*tok_name).value;
        if (!expect(Colon)) {
            // Err
        }
        Expr expr = parse_expr();
        Block block = parse_block();
        return Stmt::For(name, expr, block);
    }

    Stmt::While Parser::parse_stmt_while() {
        SectionGuard section_guard(this, __func__);

        if (!expect(KwWhile)) {
            // Err
        }
        Expr expr = parse_expr();
        Block block = parse_block();
        return Stmt::While(expr, block);
    }

    Stmt::Return Parser::parse_stmt_return() {
        SectionGuard section_guard(this, __func__);

        std::optional<Expr> expr = std::nullopt;
        if (!expect(KwReturn)) {
            // Err
        }
        if (accept(Semicolon)) {
            return Stmt::Return(expr);
        }
        expr = parse_expr();
        if (!expect(Semicolon)) {
            // Err
        }
        return Stmt::Return(expr);
    }

    Stmt::Defer Parser::parse_stmt_defer() {
        SectionGuard section_guard(this, __func__);

        if (!expect(KwDefer)) {
            // Err
        }
        Expr expr = parse_expr();
        if (!expect(Semicolon)) {
            // Err
        }
        return Stmt::Defer(expr);
    }

    Func Parser::parse_func() {
        SectionGuard section_guard(this, __func__);

        util::List<Func::Param> params;
        Type ret(Type::Unit {}, false);
        if (!expect(KwFunc)) {
            // Err
        }
        if (!expect(LParen)) {
            // Err
        }
        if (curr_is(Ident)) {
            params.push_back(*m_arena, parse_func_param());
            while (accept(Comma)) {
                if (curr_is(RParen)) {
                    break;
                }
                params.push_back(*m_arena, parse_func_param());
            }
        }
        if (!expect(RParen)) {
            // Err
        }
        if (accept(DashRAngle)) {
            ret = parse_type();
        }
        Block block = parse_block();
        return Func(params, ret, block);
    }

    Func::Param Parser::parse_func_param() {
        SectionGuard section_guard(this, __func__);

        std::optional<lex::Tok> tok_name = expect_and_get(Ident);
        if (!tok_name) {
            // Err
        }
        std::string_view name = (*tok_name).value;
        if (!expect(Colon)) {
            // Err
        }
        Type type = parse_type();
        return Func::Param(name, type);
    }

    Decl Parser::parse_decl() {
        SectionGuard section_guard(this, __func__);

        bool is_export = false;
        util::List<Decl::GenericParam> generic_params;
        if (accept(KwExport)) {
            is_export = true;
        }

        std::optional<lex::Tok> tok_name = expect_and_get(Ident);
        if (!tok_name) {
            // Err
        }
        std::string_view name = (*tok_name).value;

        if (accept(LAngle)) {
            generic_params.push_back(*m_arena, parse_decl_generic_param());
            while (accept(Comma)) {
                if (curr_is(RAngle)) {
                    break;
                }
                generic_params.push_back(*m_arena, parse_decl_generic_param());
            }
            if (!expect(RAngle)) {
                // Err
            }
        }

        if (!expect(ColonColon)) {
            // Err
        }

        Decl::Val val;
        if (curr_is(KwFunc)) {
            val = parse_func();
        } else if (curr_is(KwStruct)) {
            val = parse_decl_struct();
        } else if (curr_is(KwEnum)) {
            val = parse_decl_enum();
        } else if (curr_is(KwType)) {
            val = parse_decl_type_alias();
        } else if (curr_is(KwConst)) {
            val = parse_decl_const();
        } else if (curr_is(KwDefines)) {
            val = parse_decl_defines();
        } else {
            // Err
        }

        return Decl(is_export, name, generic_params, val);
    }

    Decl::GenericParam Parser::parse_decl_generic_param() {
        SectionGuard section_guard(this, __func__);

        std::optional<lex::Tok> tok_name = expect_and_get(Ident);
        if (!tok_name) {
            // Err
        }
        std::string_view name = (*tok_name).value;
        util::List<Type> interfaces;
        if (accept(Colon)) {
            interfaces.push_back(*m_arena, parse_type());
            while (accept(Plus)) {
                interfaces.push_back(*m_arena, parse_type());
            }
        }
        return Decl::GenericParam(name, interfaces);
    }

    Decl::Struct Parser::parse_decl_struct() {
        SectionGuard section_guard(this, __func__);

        if (!expect(KwStruct)) {
            // Err
        }
        if (!expect(LBrace)) {
            // Err
        }
        util::List<Decl::Struct::Field> fields;
        if (curr_is(KwExport) || curr_is(Ident)) {
            fields.push_back(*m_arena, parse_decl_struct_field());
            while (accept(Comma)) {
                if (curr_is(RBrace)) {
                    break;
                }
                fields.push_back(*m_arena, parse_decl_struct_field());
            }
        }
        if (!expect(RBrace)) {
            // Err
        }
        return Decl::Struct(fields);
    }

    Decl::Struct::Field Parser::parse_decl_struct_field() {
        SectionGuard section_guard(this, __func__);

        bool is_export = false;
        if (accept(KwExport)) {
            is_export = true;
        }
        std::optional<lex::Tok> tok_name = expect_and_get(Ident);
        if (!tok_name) {
            // Err
        }
        std::string_view name = (*tok_name).value;
        if (!expect(Colon)) {
            // Err
        }
        Type type = parse_type();
        return Decl::Struct::Field(name, type, is_export);
    }

    Decl::Enum Parser::parse_decl_enum() {
        SectionGuard section_guard(this, __func__);

        if (!expect(KwEnum)) {
            // Err
        }
        if (!expect(LBrace)) {
            // Err
        }
        util::List<Decl::Enum::Element> elements;
        elements.push_back(*m_arena, parse_decl_enum_element());
        while (accept(Comma)) {
            if (curr_is(RBrace)) {
                break;
            }
            elements.push_back(*m_arena, parse_decl_enum_element());
        }
        if (!expect(RBrace)) {
            // Err
        }
        return Decl::Enum(elements);
    }

    Decl::Enum::Element Parser::parse_decl_enum_element() {
        SectionGuard section_guard(this, __func__);

        std::optional<lex::Tok> name = expect_and_get(Ident);
        if (!name) {
            // Err
        }
        return Decl::Enum::Element((*name).value);
    }

    Decl::TypeAlias Parser::parse_decl_type_alias() {
        SectionGuard section_guard(this, __func__);

        if (!expect(KwType)) {
            // Err
        }
        if (!expect(Eq)) {
            // Err
        }
        Type type = parse_type();
        if (!expect(Semicolon)) {
            // Err
        }
        return Decl::TypeAlias(type);
    }

    Decl::Const Parser::parse_decl_const() {
        SectionGuard section_guard(this, __func__);

        if (!expect(KwConst)) {
            // Err
        }
        if (!expect(Colon)) {
            // Err
        }
        Type type = parse_type();
        if (!expect(Eq)) {
            // Err
        }
        Expr expr = parse_expr();
        if (!expect(Semicolon)) {
            // Err
        }
        return Decl::Const(type, expr);
    }

    Decl::Defines Parser::parse_decl_defines() {
        SectionGuard section_guard(this, __func__);

        if (!expect(KwDefines)) {
            // Err
        }
        std::optional<Type> interface = std::nullopt;
        if (!accept(LBrace)) {
            interface = parse_type();
            if (!expect(LBrace)) {
                // Err
            }
        }
        util::List<Decl> decls;
        while (!curr_is(RBrace)) {
            decls.push_back(*m_arena, parse_decl());
        }
        if (!expect(RBrace)) {
            // Err
        }
        return Decl::Defines(interface, decls);
    }

    TopLevel Parser::parse_top_level() {
        SectionGuard section_guard(this, __func__);

        TopLevel::Val val;
        if (curr_is(KwImport)) {
            val = parse_import();
        } else if (curr_is(Ident) || curr_is(KwExport)) {
            val = parse_decl();
        } else {
            // Err
        }
        return TopLevel(val);
    }

    Module Parser::parse_module() {
        SectionGuard section_guard(this, __func__);

        util::List<TopLevel> top_levels;
        while (!curr_is(Eof)) {
            top_levels.push_back(*m_arena, parse_top_level());
        }
        return Module(top_levels);
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

    Expr Parser::parse_expr_bp(int min_bp) {
        Expr lhs(Invalid {});
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
            Expr::Unop::Op op = parse_unop_op();
            util::Ptr<Expr> expr =
                m_node_ctx.make_node<Expr>(parse_expr_bp(*bp_prefix));
            lhs.val = Expr::Unop(expr, op);
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

            Expr res(Invalid {});
            util::Ptr<Expr> res_lhs = m_node_ctx.make_node<Expr>(lhs);

            if ((bp_postfix = postfix_bp(m_lexer->peek().kind)).has_value()) {
                if (*bp_postfix < min_bp) {
                    break;
                }

                if (accept(LBracket)) {
                    res.val = Expr::Index { res_lhs,
                        m_node_ctx.make_node<Expr>(parse_expr()) };
                    lhs = res;
                    if (!expect(RBracket)) {
                        // Err
                    }
                } else if (accept(LParen)) {
                    util::Ptr<Expr> expr = res_lhs;
                    util::List<Expr> args;
                    if (accept(RParen)) {
                        res.val = Expr::Call(expr, args);
                        lhs = res;
                        continue;
                    }
                    args.push_back(*m_arena, parse_expr());
                    while (accept(Comma)) {
                        if (curr_is(RParen)) {
                            break;
                        }
                        args.push_back(*m_arena, parse_expr());
                    }
                    if (!expect(RParen)) {
                        // Err
                    }
                    res.val = Expr::Call(expr, args);
                    lhs = res;
                } else if (accept(KwAs)) {
                    res.val = Expr::Cast { res_lhs, parse_type() };
                    lhs = res;
                } else if (accept(KwTryAs)) {
                    res.val = Expr::TryCast { res_lhs, parse_type() };
                    lhs = res;
                }
                continue;
            }

            if ((bp_infix = infix_bp(m_lexer->peek().kind)).has_value()) {
                if ((*bp_infix).first < min_bp) {
                    break;
                }
                Expr::Binop::Op op = parse_binop_op();
                util::Ptr<Expr> res_rhs = m_node_ctx.make_node<Expr>(
                    parse_expr_bp((*bp_infix).second));

                res.val = Expr::Binop { res_lhs, res_rhs, op };
                lhs = res;
                continue;
            }
            break;
        }
        return lhs;
    }

    Expr::Unop::Op Parser::parse_unop_op() {
        SectionGuard section_guard(this, __func__);

        lex::Tok tok = m_lexer->next();
        switch (tok.kind) {
            using Unop = Expr::Unop::Op;
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

    Expr::Binop::Op Parser::parse_binop_op() {
        SectionGuard section_guard(this, __func__);

        lex::Tok tok = m_lexer->next();
        switch (tok.kind) {
            using Binop = Expr::Binop::Op;
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
