#pragma once

#include <string_view>

#include <tl/expected.hpp>

namespace alvo::args {

    struct Args {
        std::string_view program_name;
        bool help;
        bool print_ast;
        bool emit_sections;
        bool emit_tokens;
        bool show_allocs;
        std::string_view file;

        Args(std::string_view program_name, bool help, bool print_ast,
            bool emit_sections, bool emit_tokens, bool show_allocs,
            std::string_view file) :
            program_name(program_name),
            help(help),
            print_ast(print_ast),
            emit_sections(emit_sections),
            emit_tokens(emit_tokens),
            show_allocs(show_allocs),
            file(file) { }

        std::string_view help_text();
    };

    using ArgsResult = tl::expected<Args, bool>;

    ArgsResult parse(int argc, char** argv);
}
