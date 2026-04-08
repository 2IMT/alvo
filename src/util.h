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

    void abort();

#if ALVO_DEBUG == 1

#  define ALVO_UNREACHABLE() \
      ::alvo::util::unreachable_(__FILE__, __LINE__, __func__)

#  define ALVO_NOT_IMPLEMENTED() \
      ::alvo::util::not_implemented_(__FILE__, __LINE__, __func__)

#  define ALVO_ASSERT(cond_) \
      (cond_) \
          ? (void)0 \
          : ::alvo::util::assert_fail_(#cond_, __FILE__, __LINE__, __func__)

    void unreachable_(const char* file, int line, const char* func);

    void not_implemented_(const char* file, int line, const char* func);

    void assert_fail_(
        const char* cond, const char* file, int line, const char* func);

#else

#  define ALVO_UNREACHABLE()

#  define ALVO_NOT_IMPLEMENTED()

#  define ALVO_ASSERT(cond_)

#endif

}
