#pragma once

#include <cstdint>

#include <fmt/format.h>

namespace alvo::loc {

    struct Pos {
        std::uint32_t l;
        std::uint32_t c;
        std::uint32_t offset;

        Pos() :
            l(1),
            c(1),
            offset(0) { }

        Pos(std::uint32_t l, std::uint32_t c, std::uint32_t offset) :
            l(l),
            c(c),
            offset(offset) { }
    };

    struct Loc {
        Pos s;
        Pos e;

        Loc() :
            s(),
            e() { }

        Loc(Pos s, Pos e) :
            s(s),
            e(e) { }

        Loc merge(Loc other) { return Loc(s, other.e); }
    };

}

template<>
struct fmt::formatter<alvo::loc::Pos> {
    constexpr auto parse(fmt::format_parse_context& ctx) { return ctx.begin(); }

    auto format(const alvo::loc::Pos& obj, fmt::format_context& ctx) const {
        return fmt::format_to(ctx.out(), "{}:{}", obj.l, obj.c);
    }
};

template<>
struct fmt::formatter<alvo::loc::Loc> {
    constexpr auto parse(fmt::format_parse_context& ctx) { return ctx.begin(); }

    auto format(const alvo::loc::Loc& obj, fmt::format_context& ctx) const {
        return fmt::format_to(ctx.out(), "{}:{}", obj.s, obj.e);
    }
};
