#include "args.h"

#include <cstddef>
#include <optional>
#include <iostream>
#include <ostream>

#include <fmt/format.h>
#include <fmt/ostream.h>

namespace alvo::args {

    static void _usage(std::string_view program_name, std::ostream& os) {
        fmt::println(os, "Usage: {} [Options]", program_name);
        fmt::println(os, "       {} -f <file>", program_name);
    }

    static void _version() { fmt::println("alvo v{}", ALVO_VERSION); }

    static void _help(std::string_view program_name) {
        _version();
        _usage(program_name, std::cout);
        fmt::println("Options:");
        fmt::println("  -h, --help          Display help message and exit.");
        fmt::println("  -v, --version       Display version and exit.");
        fmt::println("  -a, --print-ast     Display AST.");
        fmt::println("  -s, --emit-sections Emit parsing sections.");
        fmt::println("  -t, --emit-tokens   Emit tokens (lexems).");
        fmt::println("  -m, --show-allocs   Show allocation statistics.");
        fmt::println("  -f, --file <file>   Source file path. (REQUIRED)");
    }

    static void _err_usage(std::string_view program_name) {
        _usage(program_name, std::cerr);
        fmt::println(
            std::cerr, "Use `{} --help` for more information.", program_name);
    }

    tl::expected<Args, bool> parse(int argc, char** argv) {
        std::string_view program_name = argv[0];
        bool help = false;
        bool print_ast = false;
        bool emit_sections = false;
        bool emit_tokens = false;
        bool show_allocs = false;
        std::optional<std::string_view> file = std::nullopt;

        for (std::size_t i = 1; i < (std::size_t)argc; i++) {
            std::string_view arg = argv[i];
            if (arg == "-h" || arg == "--help") {
                _help(program_name);
                return tl::make_unexpected(false);
            } else if (arg == "-v" || arg == "--version") {
                _version();
                return tl::make_unexpected(false);
            } else if (arg == "-a" || arg == "--print-ast") {
                print_ast = true;
            } else if (arg == "-s" || arg == "--emit-sections") {
                emit_sections = true;
            } else if (arg == "-t" || arg == "--emit-tokens") {
                emit_tokens = true;
            } else if (arg == "-m" || arg == "--show-allocs") {
                show_allocs = true;
            } else if (arg == "-f" || arg == "--file") {
                if (i == (std::size_t)argc - 1) {
                    fmt::println(
                        std::cerr, "error: `{}` requires an argument", arg);
                    _err_usage(program_name);
                    return tl::make_unexpected(true);
                }
                i++;
                file = argv[i];
            } else {
                _err_usage(program_name);
                fmt::println(std::cerr, "error: invalid argument: `{}`", arg);
                return tl::make_unexpected(true);
            }
        }

        if (!file) {
            _err_usage(program_name);
            fmt::println(std::cerr, "error: source file not provided");
            return tl::make_unexpected(true);
        }

        return Args(program_name, help, print_ast, emit_sections, emit_tokens,
            show_allocs, *file);
    }

}
