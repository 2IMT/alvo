#pragma once

namespace alvo::util {
    template<typename... Ts>
    struct overload : Ts... {
        using Ts::operator()...;
    };
    template<class... Ts>
    overload(Ts...) -> overload<Ts...>;
}
