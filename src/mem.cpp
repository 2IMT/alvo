#include "mem.h"

#include <cstring>

namespace alvo::mem {

    static std::size_t _align_up(std::size_t p, std::size_t alignment) {
        return (p + alignment - 1) & ~(alignment - 1);
    }

    static bool _is_power_of_two(std::size_t x) {
        return x != 0 && (x & (x - 1)) == 0;
    }

    ArenaBlock::ArenaBlock(std::size_t size) :
        m_data(new char[size]),
        m_size(size),
        m_used(0) {
        std::memset(m_data.get(), 0, size);
    }

    std::size_t ArenaBlock::size() const { return m_size; }

    std::size_t ArenaBlock::used() const { return m_used; }

    bool ArenaBlock::can_fit(std::size_t size, std::size_t alignment) const {
        if (!_is_power_of_two(alignment)) {
            return false;
        }
        std::size_t used = _align_up(m_used, alignment);
        return used + size <= m_size;
    }

    void* ArenaBlock::alloc(std::size_t size, std::size_t alignment) {
        if (size == 0) {
            return nullptr;
        }
        if (!_is_power_of_two(alignment)) {
            return nullptr;
        }
        m_used = _align_up(m_used, alignment);
        if (m_used + size > m_size) {
            return nullptr;
        }
        char* res = m_data.get() + m_used;
        m_used += size;
        return res;
    }

    Arena::Arena(std::size_t block_size) :
        m_blocks(),
        m_block_size(block_size),
        m_total_allocated(0) { }

    void* Arena::alloc(std::size_t size, std::size_t alignment) {
        if (size == 0) {
            return nullptr;
        }
        if (!_is_power_of_two(alignment)) {
            return nullptr;
        }
        if (m_blocks.size() == 0) {
            m_blocks.emplace_back(m_block_size);
            m_total_allocated += m_block_size;
        }
        if (!m_blocks.back().can_fit(size, alignment)) {
            std::size_t new_block_size = m_block_size;
            while (new_block_size < _align_up(size, alignment)) {
                new_block_size *= 2;
            }
            m_blocks.emplace_back(new_block_size);
            m_total_allocated += new_block_size;
        }
        return m_blocks.back().alloc(size, alignment);
    }

    std::size_t Arena::get_total_allocated() const { return m_total_allocated; }

    std::size_t Arena::get_total_used() const {
        std::size_t used = 0;
        for (const auto& block : m_blocks) {
            used += block.used();
        }
        return used;
    }

    std::size_t Arena::get_block_count() const { return m_blocks.size(); }

}
