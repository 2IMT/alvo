#pragma once

#include <cstddef>
#include <memory>
#include <vector>

namespace alvo::mem {

    class ArenaBlock {
    public:
        ArenaBlock(std::size_t size);

        std::size_t size() const;

        std::size_t used() const;

        bool can_fit(std::size_t size, std::size_t alignment) const;

        void* alloc(std::size_t size, std::size_t alignment);

    private:
        std::unique_ptr<char[]> m_data;
        std::size_t m_size;
        std::size_t m_used;
    };

    template<typename T>
    struct ArenaDeleter {
        void operator()(T* obj) const { obj->~T(); }
    };

    class Arena {
    public:
        Arena(std::size_t block_size);

        void* alloc(std::size_t size, std::size_t alignment);

        std::size_t get_total_allocated() const;

        std::size_t get_total_used() const;

        std::size_t get_block_count() const;

    private:
        std::vector<ArenaBlock> m_blocks;
        std::size_t m_block_size;
        std::size_t m_total_allocated;
    };

}
