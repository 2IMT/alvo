#pragma once

#include <string_view>
#include <cstddef>
#include <cstdint>
#include <optional>

namespace alvo::utf8 {

    using Codepoint = int32_t;

    class Utf8Iter {
    public:
        Utf8Iter(std::string_view view);

        std::optional<Codepoint> next();

        std::optional<Codepoint> peek() const;

        std::size_t byte_offset() const;

        bool eof() const;

    private:
        std::string_view m_view;
        std::size_t m_byte_offset;
        std::optional<Codepoint> m_curr;
    };

    bool is_space(Codepoint c);

    bool is_alpha(Codepoint c);

    bool is_alnum(Codepoint c);

    bool is_upper(Codepoint c);

    bool is_lower(Codepoint c);

    bool is_digit(Codepoint c);

    bool is_print(Codepoint c);

}
