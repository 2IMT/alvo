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
#include "args.h"
#include "sema/resolve.h"
#include "sema/typecheck.h"

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

    using namespace alvo;
    using args::Args;
    using args::ArgsResult;
    ArgsResult args = args::parse(argc, argv);
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
    diag::OstreamSink diag_sink(std::cerr);
    Handler handler(handler_state, args->file);
    lex::TokEmitter tok_emitter(handler);
    parse::SectionEmitter enter_emitter(handler);

    lex::Lexer lexer(args->file, *source);
    lexer.set_diag_sink(diag_sink);
    if (args->emit_tokens) {
        lexer.set_tok_emitter(tok_emitter);
    }

    mem::Arena node_arena(8192);
    parse::Parser parser(args->file, lexer, node_arena);
    parser.set_diag_sink(diag_sink);
    if (args->emit_sections) {
        parser.set_section_emitter(enter_emitter);
    }

    ast::Module module = parser.parse_module();
    if (args->print_ast) {
        ast::print::OstreamSink sink(std::cout);
        ast::Printer printer(sink, 1);
        using ast::print::StyleConfig;
        using ast::print::Style;
        using ast::print::Color;
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

    using namespace sema;

    resolve::NameIndex name_index;
    resolve::NameResolver name_resolver(name_index, node_arena);
    name_resolver.set_diag_sink(diag_sink);

    name_resolver.resolve(module);

    typecheck::Typechecker typechecker(name_index, node_arena);
    typechecker.set_diag_sink(diag_sink);

    typechecker.typecheck();

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
