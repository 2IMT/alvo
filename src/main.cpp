#include <iostream>
#include <fstream>
#include <optional>
#include <string>
#include <string_view>

#include <fmt/format.h>
#include <fmt/ostream.h>

#include "lex.h"
#include "diag.h"
#include "parse.h"
#include "ast.h"
#include "mem.h"

class Handler {
public:
    struct State {
        int indent;

        State() :
            indent(0) { }
    };

    Handler(State& state) :
        m_state(&state) { };

    void operator()([[maybe_unused]] alvo::diag::Diag diag) {
        fmt::println(std::cerr, "error");
    }

    void operator()(const alvo::lex::Tok& tok) {
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
    if (argc < 2) {
        fmt::println(std::cerr, "error: source file not provided");
        return 1;
    }
    if (argc > 2) {
        fmt::println(std::cerr, "error: too many arguments");
        return 1;
    }
    std::optional<std::string> source = read_file(argv[1]);
    if (!source) {
        return 1;
    }

    Handler::State handler_state;
    Handler handler(handler_state);
    alvo::lex::Lexer lexer(*source);
    alvo::diag::DiagEmitter diag_emitter(handler);
    alvo::lex::TokEmitter tok_emitter(handler);
    lexer.set_diag_emitter(diag_emitter);
    lexer.set_tok_emitter(tok_emitter);

    alvo::mem::Arena node_arena(8192);
    alvo::parse::Parser parser(lexer, node_arena);
    alvo::parse::SectionEmitter enter_emitter(handler);
    parser.set_section_emitter(enter_emitter);

    alvo::ast::Module module = parser.parse_module();
    alvo::ast::print::OstreamSink sink(std::cout);
    alvo::ast::Printer printer(sink, 1);
    using alvo::ast::print::StyleConfig;
    using alvo::ast::print::Style;
    using alvo::ast::print::Color;
    printer.set_style_config(StyleConfig {
        .node_name = Style::fg_color(Color { 255, 0, 0 }) | Style::bold() |
                     Style::underline(),
        .field_name = Style::fg_color(Color { 0, 0, 255 }) | Style::italic(),
        .string = Style::fg_color(Color { 255, 127, 0 }),
        .boolean = Style::fg_color(Color { 255, 255, 0 }) | Style::bold(),
    });
    printer.print_node(module);

    std::size_t alloced = node_arena.get_total_allocated();
    std::size_t block_count = node_arena.get_block_count();
    std::size_t used = node_arena.get_total_used();
    fmt::println("Arena allocated: {} B", fmt::group_digits(alloced));
    fmt::println("Arena block count: {}", fmt::group_digits(block_count));
    fmt::println("Used for AST: {} B", fmt::group_digits(used));

    return 0;
}
