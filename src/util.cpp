#include "util.h"

#include <iostream>
#include <cstdlib>

#include <fmt/ostream.h>

namespace alvo::util {

    void hash_combine(std::size_t& seed, std::size_t value) {
        seed ^= value + 0x9e3779b97f4a7c15ull + (seed << 6) + (seed >> 2);
    }

    void abort() { std::abort(); }

#if ALVO_DEBUG == 1

    void unreachable_(const char* file, int line, const char* func) {
        fmt::println(std::cerr, "{}:{} encountered unreachable code in `{}`",
            file, line, func);
        alvo::util::abort();
    }

    void not_implemented_(const char* file, int line, const char* func) {
        fmt::println(std::cerr, "{}:{} encountered not implemented in `{}`",
            file, line, func);
        alvo::util::abort();
    }

    void assert_fail_(
        const char* cond, const char* file, int line, const char* func) {
        fmt::println(std::cerr, "{}:{} assertion `{}` failed in `{}`", file,
            line, cond, func);
        alvo::util::abort();
    }

#endif

}
