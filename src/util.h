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
}
