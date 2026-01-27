#include "util.h"

namespace alvo::util {

    void hash_combine(std::size_t& seed, std::size_t value) {
        seed ^= value + 0x9e3779b97f4a7c15ull + (seed << 6) + (seed >> 2);
    }

}
