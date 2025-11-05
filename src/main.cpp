#include <iostream>
#include <fstream>
#include <optional>
#include <string>
#include <string_view>
#include <variant>

#include <fmt/format.h>
#include <fmt/ostream.h>

#include "lex.h"
#include "diag.h"
#include "parse.h"
#include "ast.h"
#include "mem.h"
#include "util.h"
#include "args.h"

class Handler {
public:
    struct State {
        int indent;

        State() :
            indent(0) { }
    };

    Handler(State& state, std::string_view file_path) :
        m_state(&state),
        m_file_path(file_path) { };

    void operator()([[maybe_unused]] const alvo::diag::Diag& diag) {
        fmt::print(std::cerr, "{}:{}: ", m_file_path, diag.pos);
        std::visit(
            alvo::util::overload { [](const alvo::diag::Warn& warn) {
                                      using alvo::diag::Warn;
                                      fmt::print(std::cerr, "warning: ");
                                      std::visit(
                                          alvo::util::overload {
                                              [](const Warn::None&) { } },
                                          warn.val);
                                  },
                [](const alvo::diag::Err& err) {
                    using alvo::diag::Err;
                    fmt::print(std::cerr, "error: ");
                    std::visit(
                        alvo::util::overload {
                            [](const Err::None&) { fmt::print(std::cerr,"none"); },
                            [](const Err::UnexpectedCharacter&) {
                                fmt::print(std::cerr,"unexpected character");
                            },
                            [](const Err::
                                    NonPrintableCharacterInCharacterLiteral&) {
                                fmt::print(std::cerr,"unexpected character in character "
                                           "literal");
                            },
                            [](const Err::
                                    NonPrintableCharacterInStringLiteral&) {
                                fmt::print(std::cerr,"non-printable character in string "
                                           "literal");
                            },
                            [](const Err::UnterminatedString&) {
                                fmt::print(std::cerr,"unterminated string literal");
                            },
                            [](const Err::InvalidIntegerPrefix&) {
                                fmt::print(std::cerr,"invalid integer prefix");
                            },
                            [](const Err::NoDigitsAfterIntegerPrefix&) {
                                fmt::print(std::cerr,"no digits after integer prefix");
                            },
                            [](const Err::BytePostfixInFloatingPointLiteral&) {
                                fmt::print(std::cerr,
                                    "byte postfix in floating point literal");
                            },
                            [](const Err::NegativeByteLiteral&) {
                                fmt::print(std::cerr,"negative byte literal");
                            },
                            [](const Err::UnexpectedCharacterInNumberLiteral&) {
                                fmt::print(std::cerr,
                                    "unexpected character in number literal");
                            },
                            [](const Err::UnexpectedToken& v) {
                                fmt::print(std::cerr,
                                    "unexpected token `{}`", v.tok.value);
                            }

                        },
                        err.val);
                } },
            diag.val);
        fmt::print(std::cerr, "\n");
    }

    void operator()(const alvo::tok::Tok& tok) {
        indent();
        fmt::println("Tok: {}", tok);
    }

    void operator()(alvo::parse::SectionEvent event) {
        switch (event.type) {
        case alvo::parse::SectionEvent::Enter:
            indent();
            m_state->indent++;
            fmt::println("{} {{", event.section);
            break;
        case alvo::parse::SectionEvent::Exit:
            m_state->indent--;
            indent();
            fmt::println("}}");
            break;
        }
    }

private:
    void indent() {
        for (int i = 0; i < m_state->indent * 2; i++) {
            fmt::print(" ");
        }
    }

    State* m_state;
    std::string_view m_file_path;
};

std::optional<std::string> read_file(std::string_view path) {
    std::ifstream file(path.data());
    if (!file.is_open()) {
        fmt::println(std::cerr, "error: failed to open file `{}`", path);
        return std::nullopt;
    }
    file.seekg(0, std::ios::end);
    if (file.fail()) {
        fmt::println(std::cerr, "error: failed to read file `{}`", path);
        return std::nullopt;
    }
    std::size_t file_size = file.tellg();
    file.seekg(0, std::ios::beg);
    if (file.fail()) {
        fmt::println(std::cerr, "error: failed to read file `{}`", path);
        return std::nullopt;
    }
    std::string data;
    data.resize(file_size, '\0');
    file.read(data.data(), file_size);
    if (file.fail()) {
        fmt::println(std::cerr, "error: failed to read file `{}`", path);
        return std::nullopt;
    }
    return data;
}

void eat_all_tokens(alvo::lex::Lexer& lexer) {
    while (!lexer.next().is_eof())
        ;
}

int main(int argc, char** argv) {
    using alvo::args::Args;
    using alvo::args::ArgsResult;
    ArgsResult args = alvo::args::parse(argc, argv);
    if (!args) {
        bool err = args.error();
        if (err) {
            return 1;
        } else {
            return 0;
        }
    }

    std::optional<std::string> source = read_file(args->file);
    if (!source) {
        return 1;
    }

    Handler::State handler_state;
    Handler handler(handler_state, args->file);
    alvo::diag::DiagEmitter diag_emitter(handler);
    alvo::lex::TokEmitter tok_emitter(handler);
    alvo::parse::SectionEmitter enter_emitter(handler);

    alvo::lex::Lexer lexer(*source);
    lexer.set_diag_emitter(diag_emitter);
    if (args->emit_tokens) {
        lexer.set_tok_emitter(tok_emitter);
    }

    alvo::mem::Arena node_arena(8192);
    alvo::parse::Parser parser(lexer, node_arena);
    parser.set_diag_emitter(diag_emitter);
    if (args->emit_sections) {
        parser.set_section_emitter(enter_emitter);
    }

    alvo::ast::Module module = parser.parse_module();
    if (args->print_ast) {
        alvo::ast::print::OstreamSink sink(std::cout);
        alvo::ast::Printer printer(sink, 1);
        using alvo::ast::print::StyleConfig;
        using alvo::ast::print::Style;
        using alvo::ast::print::Color;
        printer.set_style_config(StyleConfig {
            .node_name = Style::fg_color(Color { 255, 0, 0 }) | Style::bold() |
                         Style::underline(),
            .field_name =
                Style::fg_color(Color { 0, 0, 255 }) | Style::italic(),
            .string = Style::fg_color(Color { 255, 127, 0 }),
            .boolean = Style::fg_color(Color { 255, 255, 0 }) | Style::bold(),
        });
        printer.print_node(module);
        fmt::print("\n");
    }

    if (args->show_allocs) {
        std::size_t alloced = node_arena.get_total_allocated();
        std::size_t block_count = node_arena.get_block_count();
        std::size_t used = node_arena.get_total_used();
        fmt::println("Arena allocated: {} B", fmt::group_digits(alloced));
        fmt::println("Arena block count: {}", fmt::group_digits(block_count));
        fmt::println("Used for AST: {} B", fmt::group_digits(used));
    }

    return 0;
}
