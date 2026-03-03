#pragma once

#include <cstddef>

namespace alvo::util {
    template<typename... Ts>
    struct overload : Ts... {
        using Ts::operator()...;
    };
    template<class... Ts>
    overload(Ts...) -> overload<Ts...>;

    void hash_combine(std::size_t& seed, std::size_t value);

    void unreachable(const char* file, int line, const char* func);

    void not_implemented(const char* file, int line, const char* func);

    void abort();
}

#if ALVO_DEBUG == 1

#  define ALVO_UNREACHABLE() \
      ::alvo::util::unreachable(__FILE__, __LINE__, __func__)

#  define ALVO_NOT_IMPLEMENTED() \
      ::alvo::util::not_implemented(__FILE__, __LINE__, __func__)

#else

#  define ALVO_UNREACHABLE()

#  define ALVO_NOT_IMPLEMENTED() ::alvo::util::abort()

#endif
