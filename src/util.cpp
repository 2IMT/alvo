#include "util.h"

#include <iostream>
#include <stdlib.h>

#include <fmt/ostream.h>

namespace alvo::util {

    void hash_combine(std::size_t& seed, std::size_t value) {
        seed ^= value + 0x9e3779b97f4a7c15ull + (seed << 6) + (seed >> 2);
    }

    void unreachable(const char* file, int line, const char* func) {
        fmt::print(std::cerr, "{}:{} encountered unreachable code in `{}`",
            file, line, func);
        alvo::util::abort();
    }

    void not_implemented(const char* file, int line, const char* func) {
        fmt::print(std::cerr, "{}:{} encountered not implemented in `{}`", file,
            line, func);
        alvo::util::abort();
    }

    void abort() { std::abort(); }

}
