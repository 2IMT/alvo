#include "parse.h"

#include <utility>

namespace alvo::parse {

    using enum tok::TokKind;
    using namespace ast;
    using namespace ast::util;

    static constexpr std::initializer_list<tok::TokKind> TYPE_CTX_SYNC = {
        Comma,
        Semicolon,
        RParen,
        RBracket,
        RAngle,
        RBrace,
        Colon,
        Eq,
        DashRAngle,
        EqRAngle,
    };

    static constexpr std::initializer_list<tok::TokKind> EXPR_CTX_SYNC = {
        Semicolon,
        RParen,
        RBrace,
        RAngle,
        RBracket,
        LParen,
        KwRoot,
        KwSuper,
        Ident,
        Plus,
        Dash,
        Exclam,
        Tilde,
        KwUnit,
        KwNull,
        LitString,
        LitCharacter,
        LitInteger,
        LitByte,
        LitFloating,
        LitBoolean,
        LBracket,
        KwTup,
        KwStruct,
        KwFunc,
        EqRAngle,
        DashRAngle,
        KwElse,
        KwElif,
        KwRef,
        KwBuiltin,
    };

    static constexpr std::initializer_list<tok::TokKind> STMT_CTX_SYNC = {
        Semicolon,
        RBrace,
        KwLet,
        KwIf,
        KwSwitch,
        KwLoop,
        KwFor,
        KwWhile,
        KwReturn,
        KwDefer,
        LBrace,
        KwElse,
        KwElif,
    };

    static constexpr std::initializer_list<tok::TokKind> TOP_LEVEL_CTX_SYNC = {
        Eof,
        Ident,
        KwExport,
        KwImport,
        RBrace,
    };

    static constexpr std::initializer_list<tok::TokKind> ARGLIST_CTX_SYNC = {
        Comma,
        RParen,
        RAngle,
        RBrace,
        Semicolon,
    };

    static constexpr std::initializer_list<tok::TokKind> UDTYPE_CTX_SYNC = {
        Comma,
        RBrace,
        Semicolon,
        Eof,
    };

    SectionEmitter::SectionEmitter(SectionHandler handler) :
        m_handler(handler) { }

    void SectionEmitter::emit(SectionEvent event) { m_handler(event); }

    Parser::Parser(
        std::string_view filename, lex::Lexer& lexer, mem::Arena& arena) :
        m_filename(filename),
        m_lexer(&lexer),
        m_arena(&arena),
        m_node_ctx(arena),
        m_section_emitter(nullptr),
        m_diag_emitter(),
        m_lexer_next_pushed() {
        m_lexer_next_pushed.reserve(10);
    }

    void Parser::set_section_emitter(SectionEmitter& section_emitter) {
        m_section_emitter = &section_emitter;
    }

    void Parser::set_diag_sink(diag::DiagSink& sink) {
        m_diag_emitter.set_sink(sink);
    }

    PathSegment Parser::parse_path_segment() {
        SectionGuard section_guard(this, __func__);

        PathSegment::Val val;
        std::optional<tok::Tok> name;
        if (accept(KwRoot)) {
            val = PathSegment::Root {};
        } else if (accept(KwSuper)) {
            val = PathSegment::Super {};
        } else if ((name = accept_and_get(Ident)).has_value()) {
            List<Type> generic_params;
            std::string_view value = (*name).value;
            if (accept(LAngle)) {
                generic_params.push_back(*m_arena, parse_type());
                while (accept(Comma)) {
                    generic_params.push_back(*m_arena, parse_type());
                }
                if (!expect(RAngle)) {
                    synchronize({ ColonColon, Dot, Semicolon, Comma, RAngle });
                    return PathSegment(Invalid {});
                }
            }
            val = PathSegment::Name(value, generic_params);
        } else {
            synchronize({ ColonColon, Dot, Semicolon });
            return PathSegment(Invalid {});
        }
        return PathSegment(val);
    }

    Import Parser::parse_import() {
        SectionGuard section_guard(this, __func__);

        Import::Kind kind = Import::Normal {};
        List<PathSegment> segments;
        if (!expect(KwImport)) {
            synchronize({ Semicolon, Eof });
            return Import(Invalid {}, segments);
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
                synchronize({ Semicolon, Eof });
                return Import(Invalid {}, segments);
            }
        }
        if (accept(KwAs)) {
            if (std::holds_alternative<Import::Glob>(kind)) {
                // Err
                synchronize({ Semicolon, Eof });
                return Import(Invalid {}, segments);
            }
            std::optional<tok::Tok> name = expect_and_get(Ident);
            if (!name) {
                // Err
                synchronize({ Semicolon, Eof });
                return Import(Invalid {}, segments);
            }
            kind = Import::Renamed { (*name).value };
        }
        if (!expect(Semicolon)) {
            synchronize({ Semicolon, Eof });
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
        } else if (curr_is(KwRef)) {
            val = parse_type_ref();
        } else {
            // Err
            synchronize({ Ident, KwRoot, KwSuper, LBracket, KwTup, KwFunc,
                Question, Semicolon, Comma, RAngle, RParen, RBracket });
            return Type(Invalid {}, false);
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
            synchronize(TYPE_CTX_SYNC);
            return Type::Array(true, Ptr<Type>::null());
        }
        Ptr<Type> type = m_node_ctx.make_node<Type>(parse_type());
        if (!expect(RBracket)) {
            synchronize(TYPE_CTX_SYNC);
        }
        return Type::Array(false, type);
    }

    Type::Tup Parser::parse_type_tup() {
        SectionGuard section_guard(this, __func__);

        List<Type> types;
        if (!expect(KwTup)) {
            synchronize(TYPE_CTX_SYNC);
            return Type::Tup(true, types);
        }
        if (!expect(LParen)) {
            synchronize(TYPE_CTX_SYNC);
            return Type::Tup(true, types);
        }
        types.push_back(*m_arena, parse_type());
        while (accept(Comma)) {
            if (curr_is(RParen)) {
                break;
            }
            types.push_back(*m_arena, parse_type());
        }
        if (!expect(RParen)) {
            synchronize(TYPE_CTX_SYNC);
        }
        return Type::Tup(false, types);
    }

    Type::Func Parser::parse_type_func() {
        SectionGuard section_guard(this, __func__);

        List<Type> params;
        Type return_type(Type::Unit {}, false);
        if (!expect(KwFunc)) {
            synchronize(TYPE_CTX_SYNC);
            return Type::Func(true, params, Ptr<Type>::null());
        }
        if (!expect(LParen)) {
            synchronize(TYPE_CTX_SYNC);
            return Type::Func(true, params, Ptr<Type>::null());
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
            synchronize(TYPE_CTX_SYNC);
            return Type::Func(true, params, Ptr<Type>::null());
        }
        if (accept(DashRAngle)) {
            return_type = parse_type();
        }
        return Type::Func(
            false, params, m_node_ctx.make_node<Type>(return_type));
    }

    Type::Path Parser::parse_type_path() {
        SectionGuard section_guard(this, __func__);

        List<PathSegment> segments;
        while (curr_is(KwRoot) || curr_is(KwSuper) || curr_is(Ident)) {
            segments.push_back(*m_arena, parse_path_segment());
            if (!accept(ColonColon)) {
                break;
            }
        }
        return Type::Path(false, segments);
    }

    Type::Ref Parser::parse_type_ref() {
        SectionGuard section_guard(this, __func__);

        if (!expect(KwRef)) {
            synchronize(TYPE_CTX_SYNC);
            return Type::Ref(true, Ptr<Type>::null());
        }

        if (!expect(LParen)) {
            synchronize(TYPE_CTX_SYNC);
            return Type::Ref(true, Ptr<Type>::null());
        }

        Ptr<Type> type = m_node_ctx.make_node<Type>(parse_type());

        if (!expect(RParen)) {
            synchronize(TYPE_CTX_SYNC);
        }

        return Type::Ref(false, type);
    }

    Expr Parser::parse_expr() {
        SectionGuard section_guard(this, __func__);

        return parse_expr_bp(0);
    }

    Expr::Literal Parser::parse_expr_literal() {
        SectionGuard section_guard(this, __func__);

        Expr::Literal::Val val;
        std::optional<tok::Tok> tok;
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
        } else if (curr_is(KwStruct)) {
            val = parse_expr_literal_struct();
        } else {
            synchronize(EXPR_CTX_SYNC);
            return Expr::Literal(Invalid {});
        }
        return Expr::Literal(val);
    }

    Expr::Literal::Array Parser::parse_expr_literal_array() {
        SectionGuard section_guard(this, __func__);

        Expr::Literal::Array::Val val;
        if (!expect(LBracket)) {
            synchronize(EXPR_CTX_SYNC);
            return Expr::Literal::Array(Invalid {});
        }
        if (accept(RBracket)) {
            val = Expr::Literal::Array::Regular { List<Expr>() };
        } else {
            if (accept(KwDefault)) {
                if (!expect(KwFor)) {
                    synchronize(EXPR_CTX_SYNC);
                    return Expr::Literal::Array(Invalid {});
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
                    List<Expr> elements;
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
                synchronize(EXPR_CTX_SYNC);
            }
        }
        return Expr::Literal::Array(val);
    }

    Expr::Literal::Tup Parser::parse_expr_literal_tup() {
        SectionGuard section_guard(this, __func__);

        List<Expr> exprs;
        if (!expect(KwTup)) {
            synchronize(EXPR_CTX_SYNC);
            return Expr::Literal::Tup(true, exprs);
        }
        if (!expect(LParen)) {
            synchronize(EXPR_CTX_SYNC);
            return Expr::Literal::Tup(true, exprs);
        }
        exprs.push_back(*m_arena, parse_expr());
        while (accept(Comma)) {
            if (curr_is(RParen)) {
                break;
            }
            exprs.push_back(*m_arena, parse_expr());
        }
        if (!expect(RParen)) {
            synchronize(EXPR_CTX_SYNC);
            return Expr::Literal::Tup(true, exprs);
        }
        return Expr::Literal::Tup(false, exprs);
    }

    Expr::Literal::Struct Parser::parse_expr_literal_struct() {
        SectionGuard section_guard(this, __func__);

        List<Expr::Literal::Struct::Field> fields;
        if (!expect(KwStruct)) {
            synchronize(EXPR_CTX_SYNC);
            return Expr::Literal::Struct(true, Type(Invalid {}, false), fields);
        }
        Type type = parse_type();
        if (!expect(LBrace)) {
            synchronize(EXPR_CTX_SYNC);
            return Expr::Literal::Struct(true, Type(Invalid {}, false), fields);
        }
        if (!curr_is(RBrace)) {
            fields.push_back(*m_arena, parse_expr_literal_struct_field());
            while (accept(Comma)) {
                if (curr_is(RBrace)) {
                    break;
                }
                fields.push_back(*m_arena, parse_expr_literal_struct_field());
            }
        }
        if (!expect(RBrace)) {
            synchronize(EXPR_CTX_SYNC);
        }
        return Expr::Literal::Struct(false, type, fields);
    }

    Expr::Literal::Struct::Field Parser::parse_expr_literal_struct_field() {
        SectionGuard section_guard(this, __func__);

        std::optional<tok::Tok> tok_name = accept_and_get(Ident);
        if (!tok_name) {
            synchronize(EXPR_CTX_SYNC);
            return Expr::Literal::Struct::Field(true, "", Ptr<Expr>::null());
        }
        std::string_view name = (*tok_name).value;
        if (!expect(Colon)) {
            synchronize(EXPR_CTX_SYNC);
            return Expr::Literal::Struct::Field(true, "", Ptr<Expr>::null());
        }
        Ptr<Expr> expr = m_node_ctx.make_node<Expr>(parse_expr());
        return Expr::Literal::Struct::Field(false, name, expr);
    }

    Expr::Ref Parser::parse_expr_ref() {
        SectionGuard section_guard(this, __func__);

        if (!expect(KwRef)) {
            synchronize(EXPR_CTX_SYNC);
            return Expr::Ref(true, Ptr<Expr>::null());
        }

        if (!expect(LParen)) {
            synchronize(EXPR_CTX_SYNC);
            return Expr::Ref(true, Ptr<Expr>::null());
        }

        Ptr<Expr> expr = m_node_ctx.make_node<Expr>(parse_expr());

        if (!expect(RParen)) {
            synchronize(EXPR_CTX_SYNC);
        }

        return Expr::Ref(false, expr);
    }

    Expr::Builtin Parser::parse_expr_builtin() {
        SectionGuard section_guard(this, __func__);

        std::string_view name = "";
        List<Type> generic_params;
        List<Expr> args;

        if (!expect(KwBuiltin)) {
            synchronize(EXPR_CTX_SYNC);
            return Expr::Builtin(true, name, generic_params, args);
        }

        if (!expect(LBracket)) {
            synchronize(EXPR_CTX_SYNC);
            return Expr::Builtin(true, name, generic_params, args);
        }
        std::optional<tok::Tok> name_tok = expect_and_get(Ident);
        if (!name_tok) {
            synchronize(EXPR_CTX_SYNC);
            return Expr::Builtin(true, name, generic_params, args);
        }
        name = name_tok->value;
        if (!expect(RBracket)) {
            synchronize(EXPR_CTX_SYNC);
            return Expr::Builtin(true, name, generic_params, args);
        }

        if (accept(LAngle)) {
            generic_params.push_back(*m_arena, parse_type());
            while (accept(Comma)) {
                generic_params.push_back(*m_arena, parse_type());
            }
            if (!expect(RAngle)) {
                synchronize(EXPR_CTX_SYNC);
                return Expr::Builtin(true, name, generic_params, args);
            }
        }

        if (!expect(LParen)) {
            synchronize(EXPR_CTX_SYNC);
            return Expr::Builtin(true, name, generic_params, args);
        }
        if (!curr_is(RParen)) {
            args.push_back(*m_arena, parse_expr());
            while (accept(Comma)) {
                if (curr_is(RParen)) {
                    break;
                }
                args.push_back(*m_arena, parse_expr());
            }
        }
        if (!expect(RParen)) {
            synchronize(EXPR_CTX_SYNC);
            return Expr::Builtin(true, name, generic_params, args);
        }

        return Expr::Builtin(false, name, generic_params, args);
    }

    Block Parser::parse_block() {
        SectionGuard section_guard(this, __func__);

        List<Stmt> stmts;
        if (!expect(LBrace)) {
            synchronize(STMT_CTX_SYNC);
            return Block(true, stmts);
        }
        if (!curr_is(RBrace)) {
            stmts.push_back(*m_arena, parse_stmt());
            while (!curr_is(RBrace)) {
                stmts.push_back(*m_arena, parse_stmt());
            }
        }
        if (!expect(RBrace)) {
            synchronize(STMT_CTX_SYNC);
            return Block(true, stmts);
        }
        return Block(false, stmts);
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
                synchronize(STMT_CTX_SYNC);
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
            synchronize(STMT_CTX_SYNC);
            return Stmt::Let(true, name, type, expr);
        }
        std::optional<tok::Tok> name_tok = expect_and_get(Ident);
        if (!name_tok) {
            synchronize(STMT_CTX_SYNC);
            return Stmt::Let(true, name, type, expr);
        }
        name = (*name_tok).value;
        if (accept(Colon)) {
            type = parse_type();
        }
        if (accept(Eq)) {
            expr = parse_expr();
        }
        if (!expect(Semicolon)) {
            synchronize(STMT_CTX_SYNC);
        }
        return Stmt::Let(false, name, type, expr);
    }

    Stmt::If Parser::parse_stmt_if() {
        SectionGuard section_guard(this, __func__);

        List<Stmt::If::Elif> elifs;
        std::optional<Block> else_ = std::nullopt;
        if (!expect(KwIf)) {
            synchronize(STMT_CTX_SYNC);
            return Stmt::If(false, Expr(Invalid {}), Block(true, List<Stmt>()),
                elifs, else_);
        }
        Expr expr = parse_expr();
        Block main = parse_block();
        while (accept(KwElif)) {
            Expr expr = parse_expr();
            Block block = parse_block();
            elifs.push_back(*m_arena, Stmt::If::Elif(expr, block));
        }
        if (accept(KwElse)) {
            else_ = parse_block();
        }
        return Stmt::If(false, expr, main, elifs, else_);
    }

    Stmt::Switch Parser::parse_stmt_switch() {
        SectionGuard section_guard(this, __func__);

        List<Stmt::Switch::Case> cases;
        if (!expect(KwSwitch)) {
            synchronize(STMT_CTX_SYNC);
            return Stmt::Switch(true, Expr(Invalid {}), cases);
        }
        Expr expr = parse_expr();
        if (!expect(LBrace)) {
            synchronize(STMT_CTX_SYNC);
            return Stmt::Switch(true, Expr(Invalid {}), cases);
        }
        if (!curr_is(RBrace)) {
            cases.push_back(*m_arena, parse_stmt_switch_case());
            while (!curr_is(RBrace)) {
                cases.push_back(*m_arena, parse_stmt_switch_case());
            }
        }
        if (!expect(RBrace)) {
            synchronize(STMT_CTX_SYNC);
        }
        return Stmt::Switch(false, expr, cases);
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
            synchronize(STMT_CTX_SYNC);
            return Stmt::Switch::Case(true, expr, Block(true, List<Stmt>()));
            // Err
        }
        Block block = parse_block();
        return Stmt::Switch::Case(false, expr, block);
    }

    Stmt::Loop Parser::parse_stmt_loop() {
        SectionGuard section_guard(this, __func__);

        if (!expect(KwLoop)) {
            synchronize(STMT_CTX_SYNC);
            return Stmt::Loop(true, Block(true, List<Stmt>()));
        }
        Block block = parse_block();
        return Stmt::Loop(false, block);
    }

    Stmt::For Parser::parse_stmt_for() {
        SectionGuard section_guard(this, __func__);

        if (!expect(KwFor)) {
            synchronize(STMT_CTX_SYNC);
            return Stmt::For(
                true, "", Expr(Invalid {}), Block(true, List<Stmt>()));
        }
        std::optional<tok::Tok> tok_name = expect_and_get(Ident);
        if (!tok_name) {
            synchronize(STMT_CTX_SYNC);
            return Stmt::For(
                true, "", Expr(Invalid {}), Block(true, List<Stmt>()));
        }
        std::string_view name = (*tok_name).value;
        if (!expect(Colon)) {
            synchronize(STMT_CTX_SYNC);
            return Stmt::For(
                true, "", Expr(Invalid {}), Block(true, List<Stmt>()));
        }
        Expr expr = parse_expr();
        Block block = parse_block();
        return Stmt::For(false, name, expr, block);
    }

    Stmt::While Parser::parse_stmt_while() {
        SectionGuard section_guard(this, __func__);

        if (!expect(KwWhile)) {
            synchronize(STMT_CTX_SYNC);
            return Stmt::While(
                true, Expr(Invalid {}), Block(true, List<Stmt>()));
        }
        Expr expr = parse_expr();
        Block block = parse_block();
        return Stmt::While(false, expr, block);
    }

    Stmt::Return Parser::parse_stmt_return() {
        SectionGuard section_guard(this, __func__);

        std::optional<Expr> expr = std::nullopt;
        if (!expect(KwReturn)) {
            synchronize(STMT_CTX_SYNC);
            return Stmt::Return(true, expr);
        }
        if (accept(Semicolon)) {
            return Stmt::Return(false, expr);
        }
        expr = parse_expr();
        if (!expect(Semicolon)) {
            synchronize(STMT_CTX_SYNC);
        }
        return Stmt::Return(false, expr);
    }

    Stmt::Defer Parser::parse_stmt_defer() {
        SectionGuard section_guard(this, __func__);

        if (!expect(KwDefer)) {
            synchronize(STMT_CTX_SYNC);
            return Stmt::Defer(true, Expr(Invalid {}));
        }
        Expr expr = parse_expr();
        if (!expect(Semicolon)) {
            synchronize(STMT_CTX_SYNC);
        }
        return Stmt::Defer(false, expr);
    }

    Func Parser::parse_func() {
        SectionGuard section_guard(this, __func__);

        Func::Signature signature = parse_func_signature();
        Block block = parse_block();
        return Func(false, signature, block);
    }

    Func::Signature Parser::parse_func_signature() {
        SectionGuard section_guard(this, __func__);

        List<Func::Signature::Param> params;
        Type ret(Type::Unit {}, false);
        bool is_self_func = false;
        if (!expect(KwFunc)) {
            synchronize(EXPR_CTX_SYNC);
            return Func::Signature(true, is_self_func, params, ret);
        }
        if (!expect(LParen)) {
            synchronize(EXPR_CTX_SYNC);
            return Func::Signature(true, is_self_func, params, ret);
        }
        if (accept(KwSelf)) {
            is_self_func = true;
        }
        // if `is_self_func accept comma and continue parsing the remaining
        // arguments. If not `is_self_func`, parse arguments normally
        // this allows us to do:
        // `func(self)`
        // `func(self,)`
        // `func(self, foo: int)`
        if (!is_self_func || (is_self_func && (accept(Comma)))) {
            if (curr_is(Ident)) {
                params.push_back(*m_arena, parse_func_signature_param());
                while (accept(Comma)) {
                    if (curr_is(RParen)) {
                        break;
                    }
                    params.push_back(*m_arena, parse_func_signature_param());
                }
            }
        }
        if (!expect(RParen)) {
            synchronize(EXPR_CTX_SYNC);
            return Func::Signature(true, is_self_func, params, ret);
        }
        if (accept(DashRAngle)) {
            ret = parse_type();
        }
        return Func::Signature(false, is_self_func, params, ret);
    }

    Func::Signature::Param Parser::parse_func_signature_param() {
        SectionGuard section_guard(this, __func__);

        std::optional<tok::Tok> tok_name = expect_and_get(Ident);
        if (!tok_name) {
            synchronize(EXPR_CTX_SYNC);
            return Func::Signature::Param(true, "", Type(Invalid {}, false));
        }
        std::string_view name = (*tok_name).value;
        if (!expect(Colon)) {
            synchronize(EXPR_CTX_SYNC);
            return Func::Signature::Param(true, "", Type(Invalid {}, false));
        }
        Type type = parse_type();
        return Func::Signature::Param(false, name, type);
    }

    Decl Parser::parse_decl() {
        SectionGuard section_guard(this, __func__);

        bool is_decls_block = false;

        bool is_export = false;
        List<Decl::GenericParam> generic_params;
        if (accept(KwExport)) {
            is_export = true;
        }

        std::string_view name = "";

        if (accept(KwDecls)) {
            is_decls_block = true;
        } else {
            std::optional<tok::Tok> tok_name = expect_and_get(Ident);
            if (!tok_name) {
                synchronize(TOP_LEVEL_CTX_SYNC);
                return Decl(
                    is_export, is_decls_block, "", generic_params, Invalid {});
            }
            name = (*tok_name).value;
        }

        if (accept(LAngle)) {
            generic_params.push_back(*m_arena, parse_decl_generic_param());
            while (accept(Comma)) {
                if (curr_is(RAngle)) {
                    break;
                }
                generic_params.push_back(*m_arena, parse_decl_generic_param());
            }
            if (!expect(RAngle)) {
                synchronize(TOP_LEVEL_CTX_SYNC);
                return Decl(is_export, false, "", generic_params, Invalid {});
            }
        }

        if (!expect(ColonColon)) {
            synchronize(TOP_LEVEL_CTX_SYNC);
            return Decl(is_export, false, "", generic_params, Invalid {});
        }

        Decl::Val val;
        if (is_decls_block) {
            val = parse_decl_decls();
        } else {
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
            } else if (curr_is(KwInterface)) {
                val = parse_decl_interface();
            } else {
                val = Invalid {};
                synchronize(TOP_LEVEL_CTX_SYNC);
            }
        }

        return Decl(is_export, is_decls_block, name, generic_params, val);
    }

    Decl::GenericParam Parser::parse_decl_generic_param() {
        SectionGuard section_guard(this, __func__);

        List<Type> interfaces;
        std::optional<tok::Tok> tok_name = expect_and_get(Ident);
        if (!tok_name) {
            synchronize(ARGLIST_CTX_SYNC);
            return Decl::GenericParam(true, "", interfaces);
        }
        std::string_view name = (*tok_name).value;
        if (accept(Colon)) {
            interfaces.push_back(*m_arena, parse_type());
            while (accept(Plus)) {
                interfaces.push_back(*m_arena, parse_type());
            }
        }
        return Decl::GenericParam(false, name, interfaces);
    }

    Decl::Struct Parser::parse_decl_struct() {
        SectionGuard section_guard(this, __func__);

        List<Decl::Struct::Field> fields;
        if (!expect(KwStruct)) {
            synchronize(UDTYPE_CTX_SYNC);
            return Decl::Struct(true, fields);
        }
        if (!expect(LBrace)) {
            synchronize(UDTYPE_CTX_SYNC);
            return Decl::Struct(true, fields);
        }
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
            synchronize(UDTYPE_CTX_SYNC);
        }
        return Decl::Struct(false, fields);
    }

    Decl::Struct::Field Parser::parse_decl_struct_field() {
        SectionGuard section_guard(this, __func__);

        bool is_export = false;
        if (accept(KwExport)) {
            is_export = true;
        }
        std::optional<tok::Tok> tok_name = expect_and_get(Ident);
        if (!tok_name) {
            synchronize(UDTYPE_CTX_SYNC);
            return Decl::Struct::Field(
                true, "", Type(Invalid {}, false), is_export);
        }
        std::string_view name = (*tok_name).value;
        if (!expect(Colon)) {
            synchronize(UDTYPE_CTX_SYNC);
            return Decl::Struct::Field(
                true, "", Type(Invalid {}, false), is_export);
        }
        Type type = parse_type();
        return Decl::Struct::Field(false, name, type, is_export);
    }

    Decl::Enum Parser::parse_decl_enum() {
        SectionGuard section_guard(this, __func__);

        List<Decl::Enum::Element> elements;
        if (!expect(KwEnum)) {
            synchronize(UDTYPE_CTX_SYNC);
            return Decl::Enum(true, elements);
        }
        if (!expect(LBrace)) {
            synchronize(UDTYPE_CTX_SYNC);
            return Decl::Enum(true, elements);
        }
        elements.push_back(*m_arena, parse_decl_enum_element());
        while (accept(Comma)) {
            if (curr_is(RBrace)) {
                break;
            }
            elements.push_back(*m_arena, parse_decl_enum_element());
        }
        if (!expect(RBrace)) {
            synchronize(UDTYPE_CTX_SYNC);
        }
        return Decl::Enum(false, elements);
    }

    Decl::Enum::Element Parser::parse_decl_enum_element() {
        SectionGuard section_guard(this, __func__);

        std::optional<tok::Tok> name = expect_and_get(Ident);
        if (!name) {
            synchronize(UDTYPE_CTX_SYNC);
            return Decl::Enum::Element(true, "");
        }
        return Decl::Enum::Element(false, (*name).value);
    }

    Decl::TypeAlias Parser::parse_decl_type_alias() {
        SectionGuard section_guard(this, __func__);

        if (!expect(KwType)) {
            synchronize(UDTYPE_CTX_SYNC);
            return Decl::TypeAlias(true, Type(Invalid {}, false));
        }
        if (!expect(Eq)) {
            synchronize(UDTYPE_CTX_SYNC);
            return Decl::TypeAlias(true, Type(Invalid {}, false));
        }
        Type type = parse_type();
        if (!expect(Semicolon)) {
            synchronize(UDTYPE_CTX_SYNC);
        }
        return Decl::TypeAlias(false, type);
    }

    Decl::Const Parser::parse_decl_const() {
        SectionGuard section_guard(this, __func__);

        if (!expect(KwConst)) {
            synchronize(UDTYPE_CTX_SYNC);
            return Decl::Const(true, Type(Invalid {}, false), Expr(Invalid {}));
        }
        if (!expect(Colon)) {
            synchronize(UDTYPE_CTX_SYNC);
            return Decl::Const(true, Type(Invalid {}, false), Expr(Invalid {}));
        }
        Type type = parse_type();
        if (!expect(Eq)) {
            synchronize(UDTYPE_CTX_SYNC);
            return Decl::Const(true, Type(Invalid {}, false), Expr(Invalid {}));
        }
        Expr expr = parse_expr();
        if (!expect(Semicolon)) {
            synchronize(UDTYPE_CTX_SYNC);
        }
        return Decl::Const(false, type, expr);
    }

    Decl::Decls Parser::parse_decl_decls() {
        SectionGuard section_guard(this, __func__);

        Type type = parse_type();
        std::optional<Type> interface = std::nullopt;
        List<Decl> decls;

        if (accept(KwFor)) {
            interface = parse_type();
        }
        if (!expect(LBrace)) {
            synchronize(UDTYPE_CTX_SYNC);
            return Decl::Decls(true, type, interface, decls);
        }
        while (!curr_is(RBrace)) {
            decls.push_back(*m_arena, parse_decl());
        }
        if (!expect(RBrace)) {
            synchronize(UDTYPE_CTX_SYNC);
        }
        return Decl::Decls(false, type, interface, decls);
    }

    Decl::Interface Parser::parse_decl_interface() {
        SectionGuard section_guard(this, __func__);

        List<Decl::Interface::Member> members;
        if (!expect(KwInterface)) {
            synchronize(UDTYPE_CTX_SYNC);
            return Decl::Interface(true, members);
        }
        if (!expect(LBrace)) {
            synchronize(UDTYPE_CTX_SYNC);
            return Decl::Interface(true, members);
        }
        while (!curr_is(RBrace)) {
            members.push_back(*m_arena, parse_decl_interface_member());
        }
        if (!expect(RBrace)) {
            synchronize(UDTYPE_CTX_SYNC);
            return Decl::Interface(true, members);
        }
        return Decl::Interface(false, members);
    }

    Decl::Interface::Member Parser::parse_decl_interface_member() {
        SectionGuard section_guard(this, __func__);

        Func::Signature signature_invalid(true, false,
            List<Func::Signature::Param>(), Type(Invalid {}, false));
        List<Decl::GenericParam> generic_params;
        std::optional<tok::Tok> tok_name = expect_and_get(Ident);
        if (!tok_name) {
            synchronize(UDTYPE_CTX_SYNC);
            return Decl::Interface::Member(
                false, "", generic_params, signature_invalid);
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
                synchronize(UDTYPE_CTX_SYNC);
                return Decl::Interface::Member(
                    false, "", generic_params, signature_invalid);
            }
        }

        if (!expect(ColonColon)) {
            synchronize(UDTYPE_CTX_SYNC);
            return Decl::Interface::Member(
                false, "", generic_params, signature_invalid);
        }

        Func::Signature signature = parse_func_signature();

        if (!expect(Semicolon)) {
            synchronize(UDTYPE_CTX_SYNC);
        }

        return Decl::Interface::Member(false, name, generic_params, signature);
    }

    TopLevel Parser::parse_top_level() {
        SectionGuard section_guard(this, __func__);

        TopLevel::Val val;
        if (curr_is(KwImport)) {
            val = parse_import();
        } else if (curr_is(Ident) || curr_is(KwDecls) || curr_is(KwExport)) {
            val = parse_decl();
        } else {
            // TODO: Err
            synchronize(TOP_LEVEL_CTX_SYNC);
            return TopLevel(Invalid {});
        }
        return TopLevel(val);
    }

    Module Parser::parse_module() {
        SectionGuard section_guard(this, __func__);

        List<TopLevel> top_levels;
        while (!curr_is(Eof)) {
            if (curr_is(KwImport) || curr_is(Ident) || curr_is(KwDecls) ||
                curr_is(KwExport)) {
                top_levels.push_back(*m_arena, parse_top_level());
            } else {
                synchronize({ KwImport, Ident, KwDecls, KwExport });
                // TODO: Err
            }
        }
        return Module(top_levels);
    }

    std::optional<int> Parser::prefix_bp(tok::TokKind kind) {
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

    std::optional<int> Parser::postfix_bp(tok::TokKind kind) {
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

    std::optional<std::pair<int, int>> Parser::infix_bp(tok::TokKind kind) {
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
        } else if ((bp_prefix = prefix_bp(lexer_peek().kind)).has_value()) {
            Expr::Unop::Op op = parse_unop_op();
            Ptr<Expr> expr =
                m_node_ctx.make_node<Expr>(parse_expr_bp(*bp_prefix));
            lhs.val = Expr::Unop(expr, op);
        } else if (curr_is(KwUnit) || curr_is(KwNull) || curr_is(LitString) ||
                   curr_is(LitCharacter) || curr_is(LitInteger) ||
                   curr_is(LitByte) || curr_is(LitByte) ||
                   curr_is(LitFloating) || curr_is(LitBoolean) ||
                   curr_is(LBracket) || curr_is(KwTup) || curr_is(KwStruct) ||
                   curr_is(KwFunc)) {
            lhs.val = parse_expr_literal();
        } else if (curr_is(KwRef)) {
            lhs.val = parse_expr_ref();
        } else if (curr_is(KwBuiltin)) {
            lhs.val = parse_expr_builtin();
        } else {
            synchronize(EXPR_CTX_SYNC);
            return Expr(Invalid {});
        }

        while (true) {
            if (accept(Eof)) {
                break;
            }

            Expr res(Invalid {});
            Ptr<Expr> res_lhs = m_node_ctx.make_node<Expr>(lhs);

            if ((bp_postfix = postfix_bp(lexer_peek().kind)).has_value()) {
                if (*bp_postfix < min_bp) {
                    break;
                }

                if (accept(LBracket)) {
                    res.val = Expr::Index { res_lhs,
                        m_node_ctx.make_node<Expr>(parse_expr()) };
                    lhs = res;
                    if (!expect(RBracket)) {
                        synchronize(EXPR_CTX_SYNC);
                        return Expr(Invalid {});
                    }
                } else if (accept(LParen)) {
                    Ptr<Expr> expr = res_lhs;
                    List<Expr> args;
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
                        synchronize(EXPR_CTX_SYNC);
                        return Expr(Invalid {});
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

            if ((bp_infix = infix_bp(lexer_peek().kind)).has_value()) {
                if ((*bp_infix).first < min_bp) {
                    break;
                }
                Expr::Binop::Op op = parse_binop_op();
                Ptr<Expr> res_rhs = m_node_ctx.make_node<Expr>(
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

        tok::Tok tok = lexer_next();
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

        tok::Tok tok = lexer_next();
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

    bool Parser::curr_is(tok::TokKind kind) const {
        return lexer_peek().kind == kind;
    }

    std::optional<tok::Tok> Parser::accept_and_get(tok::TokKind kind) {
        // We need this to resolve cases like this:
        // The current token on the lexer is `>>=`
        // The expected (accepted) token is `>`
        // Split `>>=` into `>` and `>=`, then push `>=` as the next token
        std::optional<std::pair<tok::Tok, tok::Tok>> split =
            lexer_peek().split_sep();
        if (split && (kind == split->first.kind)) {
            lexer_next();
            lexer_push_next(split->first);
            lexer_push_next(split->second);
        }

        if (lexer_peek().kind == kind) {
            return lexer_next();
        }
        return std::nullopt;
    }

    std::optional<tok::Tok> Parser::expect_and_get(tok::TokKind kind) {
        std::optional<tok::Tok> res = accept_and_get(kind);
        if (!res) {
            m_diag_emitter.err(m_filename, lexer_peek().loc.s,
                { diag::Err::UnexpectedToken { lexer_peek() } });
        }
        return res;
    }

    bool Parser::accept(tok::TokKind kind) {
        return accept_and_get(kind).has_value();
    }

    bool Parser::expect(tok::TokKind kind) {
        return expect_and_get(kind).has_value();
    }

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

    void Parser::synchronize(std::initializer_list<tok::TokKind> kinds) {
        while (!curr_is(Eof)) {
            bool found = false;
            for (const auto& kind : kinds) {
                if (curr_is(kind)) {
                    found = true;
                    break;
                }
            }
            if (found) {
                break;
            }
            lexer_next();
        }
    }

    tok::Tok Parser::lexer_peek() const {
        if (!m_lexer_next_pushed.empty()) {
            return m_lexer_next_pushed[0];
        }
        return m_lexer->peek();
    }

    tok::Tok Parser::lexer_next() {
        if (!m_lexer_next_pushed.empty()) {
            tok::Tok res = m_lexer_next_pushed[0];
            m_lexer_next_pushed.erase(m_lexer_next_pushed.begin());
            return res;
        }
        return m_lexer->next();
    }

    void Parser::lexer_push_next(tok::Tok tok) {
        m_lexer_next_pushed.push_back(tok);
    }

}
