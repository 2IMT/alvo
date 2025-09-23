#include <catch2/catch_test_macros.hpp>

#include <cstddef>
#include <array>

#include <mem.h>

using namespace alvo::mem;

static void test_is_zeroed(void* mem, std::size_t size) {
    bool all_zeroed = true;
    for (std::size_t i = 0; i < size; i++) {
        if (static_cast<char*>(mem)[i] != 0) {
            all_zeroed = false;
        }
    }
    REQUIRE(all_zeroed);
}

static void test_write(void* mem, std::size_t size) {
    for (std::size_t i = 0; i < size; i++) {
        static_cast<char*>(mem)[i] = 42;
    }
}

static void test_mem(void* mem, std::size_t size) {
    test_is_zeroed(mem, size);
    test_write(mem, size);
}

TEST_CASE("Can construct a valid ArenaBlock", "[ArenaBlock]") {
    SECTION("Size 10") {
        ArenaBlock block(10);
        REQUIRE(block.size() == 10);
        REQUIRE(block.used() == 0);
    }

    SECTION("Size 0") {
        ArenaBlock block(0);
        REQUIRE(block.size() == 0);
        REQUIRE(block.used() == 0);
    }

    SECTION("Size 64000") {
        ArenaBlock block(64000);
        REQUIRE(block.size() == 64000);
        REQUIRE(block.used() == 0);
    }
}

TEST_CASE("Can allocate from ArenaBlock", "[ArenaBlock]") {
    static constexpr std::size_t block_size = 1024;
    ArenaBlock block(block_size);
    REQUIRE(block.size() == block_size);
    REQUIRE(block.used() == 0);

    SECTION("1 byte alignment") {
        void* ptr = block.alloc(100, 1);
        REQUIRE(block.size() == block_size);
        REQUIRE(block.used() == 100);
        REQUIRE_FALSE(ptr == nullptr);
        test_mem(ptr, 100);

        ptr = block.alloc(156, 1);
        REQUIRE(block.size() == block_size);
        REQUIRE(block.used() == 100 + 156);
        REQUIRE_FALSE(ptr == nullptr);
        test_mem(ptr, 156);

        // 0 bytes allocated
        ptr = block.alloc(0, 1);
        REQUIRE(block.size() == block_size);
        REQUIRE(block.used() == 100 + 156);
        REQUIRE(ptr == nullptr);

        ptr = block.alloc(256, 1);
        REQUIRE(block.size() == block_size);
        REQUIRE(block.used() == 512);
        REQUIRE_FALSE(ptr == nullptr);
        test_mem(ptr, 256);

        ptr = block.alloc(511, 1);
        REQUIRE(block.size() == block_size);
        REQUIRE(block.used() == 1023);
        REQUIRE_FALSE(ptr == nullptr);
        test_mem(ptr, 511);

        // Can't fit
        ptr = block.alloc(2, 1);
        REQUIRE(block.size() == block_size);
        REQUIRE(block.used() == 1023);
        REQUIRE(ptr == nullptr);

        ptr = block.alloc(1, 1);
        REQUIRE(block.size() == block_size);
        REQUIRE(block.used() == 1024);
        REQUIRE_FALSE(ptr == nullptr);
        test_mem(ptr, 1);
    }

    SECTION("Invalid alignment") {
        void* ptr;

        ptr = block.alloc(12, 3);
        REQUIRE(block.size() == block_size);
        REQUIRE(block.used() == 0);
        REQUIRE(ptr == nullptr);

        ptr = block.alloc(128, 6);
        REQUIRE(block.size() == block_size);
        REQUIRE(block.used() == 0);
        REQUIRE(ptr == nullptr);

        ptr = block.alloc(42, 7);
        REQUIRE(block.size() == block_size);
        REQUIRE(block.used() == 0);
        REQUIRE(ptr == nullptr);

        // Valid allocation
        ptr = block.alloc(100, 1);
        REQUIRE(block.size() == block_size);
        REQUIRE(block.used() == 100);
        REQUIRE_FALSE(ptr == nullptr);
        test_mem(ptr, 100);

        // Valid allocation
        ptr = block.alloc(156, 1);
        REQUIRE(block.size() == block_size);
        REQUIRE(block.used() == 100 + 156);
        REQUIRE_FALSE(ptr == nullptr);
        test_mem(ptr, 156);

        ptr = block.alloc(42, 31);
        REQUIRE(block.size() == block_size);
        REQUIRE(block.used() == 100 + 156);
        REQUIRE(ptr == nullptr);
    }

    SECTION("Valid alignment") {
        void* ptr;

        ptr = block.alloc(42, 1);
        REQUIRE(block.size() == block_size);
        REQUIRE(block.used() == 42);
        REQUIRE_FALSE(ptr == nullptr);
        test_mem(ptr, 42);

        ptr = block.alloc(11, 8);
        REQUIRE(block.size() == block_size);
        REQUIRE(block.used() == 48 + 11);
        REQUIRE_FALSE(ptr == nullptr);
        test_mem(ptr, 11);

        ptr = block.alloc(42, 2);
        REQUIRE(block.size() == block_size);
        REQUIRE(block.used() == 60 + 42);
        REQUIRE_FALSE(ptr == nullptr);
        test_mem(ptr, 42);

        ptr = block.alloc(14, 64);
        REQUIRE(block.size() == block_size);
        REQUIRE(block.used() == 128 + 14);
        REQUIRE_FALSE(ptr == nullptr);
        test_mem(ptr, 14);

        ptr = block.alloc(14, 4);
        REQUIRE(block.size() == block_size);
        REQUIRE(block.used() == 144 + 14);
        REQUIRE_FALSE(ptr == nullptr);
        test_mem(ptr, 14);

        ptr = block.alloc(14, 16);
        REQUIRE(block.size() == block_size);
        REQUIRE(block.used() == 160 + 14);
        REQUIRE_FALSE(ptr == nullptr);
        test_mem(ptr, 14);

        ptr = block.alloc(111, 32);
        REQUIRE(block.size() == block_size);
        REQUIRE(block.used() == 192 + 111);
        REQUIRE_FALSE(ptr == nullptr);
        test_mem(ptr, 111);

        ptr = block.alloc(51, 128);
        REQUIRE(block.size() == block_size);
        REQUIRE(block.used() == 384 + 51);
        REQUIRE_FALSE(ptr == nullptr);
        test_mem(ptr, 51);
    }
}

TEST_CASE("ArenaBlock can correctly calculate whether or not it can fit an "
          "allocation",
    "[ArenaBlock]") {
    static constexpr std::size_t block_size = 1024;
    ArenaBlock block(block_size);
    REQUIRE(block.size() == block_size);
    REQUIRE(block.used() == 0);

    SECTION("Edge cases") {
        for (std::size_t alignment = 1; alignment <= 128; alignment *= 2) {
            REQUIRE(block.can_fit(block_size, alignment));
            REQUIRE_FALSE(block.can_fit(block_size + 1, alignment));
        }
    }

    SECTION("1 alignment") {
        REQUIRE(block.can_fit(127, 1));
        block.alloc(127, 1);
        REQUIRE(block.can_fit(256, 1));
        block.alloc(256, 1);
        REQUIRE(block.can_fit(512, 1));
        block.alloc(512, 1);
        REQUIRE(block.can_fit(block_size - 127 - 256 - 512, 1));
        REQUIRE_FALSE(block.can_fit(block_size, 1));
        block.alloc(block_size - 127 - 256 - 512, 1);
        REQUIRE_FALSE(block.can_fit(1, 1));
    }

    SECTION("Valid alignment") {
        block.alloc(1, 1);
        REQUIRE(block.can_fit(block_size - 1, 1));
        for (std::size_t i = 2; i <= 128; i *= 2) {
            REQUIRE_FALSE(block.can_fit(block_size - 1, i));
        }
        block.alloc(128, 1);
        REQUIRE(block.can_fit(block_size - 128 - 1, 1));
        for (std::size_t i = 2; i <= 128; i *= 2) {
            REQUIRE_FALSE(block.can_fit(block_size - 128 - 1, i));
        }
        block.alloc(315, 1);
        REQUIRE(block.can_fit(block_size - 315 - 128 - 1, 1));
        for (std::size_t i = 8; i <= 128; i *= 2) {
            REQUIRE_FALSE(block.can_fit(block_size - 315 - 128 - 1, i));
        }
    }

    SECTION("Invalid alignment") {
        static constexpr std::array<std::size_t, 11> alignments = { 3, 5, 6, 7,
            9, 10, 11, 17, 28, 29, 111 };

        block.alloc(1, 1);
        REQUIRE(block.can_fit(block_size - 1, 1));
        for (auto i : alignments) {
            REQUIRE_FALSE(block.can_fit(13, i));
        }
        block.alloc(128, 1);
        REQUIRE(block.can_fit(block_size - 128 - 1, 1));
        for (auto i : alignments) {
            REQUIRE_FALSE(block.can_fit(10, i));
        }
        block.alloc(315, 1);
        REQUIRE(block.can_fit(block_size - 315 - 128 - 1, 1));
        for (auto i : alignments) {
            REQUIRE_FALSE(block.can_fit(12, i));
        }
    }
}

TEST_CASE("Can construct a valid Arena", "[Arena]") {
    SECTION("Size 10") {
        Arena arena(10);
        REQUIRE(arena.get_block_count() == 0);
        REQUIRE(arena.get_total_allocated() == 0);
        REQUIRE(arena.get_total_used() == 0);
    }

    SECTION("Size 1024") {
        Arena arena(1024);
        REQUIRE(arena.get_block_count() == 0);
        REQUIRE(arena.get_total_allocated() == 0);
        REQUIRE(arena.get_total_used() == 0);
    }

    SECTION("Size 65000") {
        Arena arena(65000);
        REQUIRE(arena.get_block_count() == 0);
        REQUIRE(arena.get_total_allocated() == 0);
        REQUIRE(arena.get_total_used() == 0);
    }

    SECTION("Size 0") {
        Arena arena(0);
        REQUIRE(arena.get_block_count() == 0);
        REQUIRE(arena.get_total_allocated() == 0);
        REQUIRE(arena.get_total_used() == 0);
    }
}

TEST_CASE("Can allocate from Arena", "[Arena]") {
    static constexpr std::size_t block_size = 1024;
    Arena arena(block_size);
    REQUIRE(arena.get_block_count() == 0);
    REQUIRE(arena.get_total_allocated() == 0);
    REQUIRE(arena.get_total_used() == 0);

    SECTION("Regular allocations") {
        void* ptr;

        ptr = arena.alloc(10, 1);
        REQUIRE_FALSE(ptr == nullptr);
        REQUIRE(arena.get_block_count() > 0);
        REQUIRE(arena.get_total_allocated() > 0);
        REQUIRE(arena.get_total_used() > 0);
        test_mem(ptr, 10);

        ptr = arena.alloc(128, 16);
        REQUIRE_FALSE(ptr == nullptr);
        REQUIRE(arena.get_block_count() > 0);
        REQUIRE(arena.get_total_allocated() > 0);
        REQUIRE(arena.get_total_used() > 0);
        test_mem(ptr, 128);

        ptr = arena.alloc(313, 16);
        REQUIRE_FALSE(ptr == nullptr);
        REQUIRE(arena.get_block_count() > 0);
        REQUIRE(arena.get_total_allocated() > 0);
        REQUIRE(arena.get_total_used() > 0);
        test_mem(ptr, 313);

        ptr = arena.alloc(618, 16);
        REQUIRE_FALSE(ptr == nullptr);
        REQUIRE(arena.get_block_count() > 0);
        REQUIRE(arena.get_total_allocated() > 0);
        REQUIRE(arena.get_total_used() > 0);
        test_mem(ptr, 618);

        ptr = arena.alloc(1, 16);
        REQUIRE_FALSE(ptr == nullptr);
        REQUIRE(arena.get_block_count() > 0);
        REQUIRE(arena.get_total_allocated() > 0);
        REQUIRE(arena.get_total_used() > 0);
        test_mem(ptr, 1);

        ptr = arena.alloc(998, 16);
        REQUIRE_FALSE(ptr == nullptr);
        REQUIRE(arena.get_block_count() > 0);
        REQUIRE(arena.get_total_allocated() > 0);
        REQUIRE(arena.get_total_used() > 0);
        test_mem(ptr, 998);
    }

    SECTION("Invalid allocations") {
        void* ptr;

        for (std::size_t i = 1; i <= 128; i *= 2) {
            ptr = arena.alloc(0, i);
            REQUIRE(ptr == nullptr);
            REQUIRE(arena.get_block_count() == 0);
            REQUIRE(arena.get_total_allocated() == 0);
            REQUIRE(arena.get_total_used() == 0);
        }

        static constexpr std::array<std::size_t, 11> alignments = { 3, 5, 6, 7,
            9, 10, 11, 17, 28, 29, 111 };

        for (auto i : alignments) {
            ptr = arena.alloc(256, i);
            REQUIRE(ptr == nullptr);
            REQUIRE(arena.get_block_count() == 0);
            REQUIRE(arena.get_total_allocated() == 0);
            REQUIRE(arena.get_total_used() == 0);
        }

        // Valid allocation
        ptr = arena.alloc(128, 4);
        REQUIRE_FALSE(ptr == nullptr);
        REQUIRE(arena.get_block_count() > 0);
        REQUIRE(arena.get_total_allocated() > 0);
        REQUIRE(arena.get_total_used() > 0);
        test_mem(ptr, 128);

        // Valid allocation
        ptr = arena.alloc(394, 4);
        REQUIRE_FALSE(ptr == nullptr);
        REQUIRE(arena.get_block_count() > 0);
        REQUIRE(arena.get_total_allocated() > 0);
        REQUIRE(arena.get_total_used() > 0);
        test_mem(ptr, 394);


        for (auto i : alignments) {
            ptr = arena.alloc(3000, i);
            REQUIRE(ptr == nullptr);
            REQUIRE(arena.get_block_count() > 0);
            REQUIRE(arena.get_total_allocated() > 0);
            REQUIRE(arena.get_total_used() > 0);
        }

        for (auto i : alignments) {
            ptr = arena.alloc(0, i);
            REQUIRE(ptr == nullptr);
            REQUIRE(arena.get_block_count() > 0);
            REQUIRE(arena.get_total_allocated() > 0);
            REQUIRE(arena.get_total_used() > 0);
        }
    }

    SECTION("Allocations bigger than block size") {
        void* ptr;

        for (std::size_t i = 1; i <= 128; i *= 2) {
            ptr = arena.alloc(block_size + 128, i *= 2);
            REQUIRE_FALSE(ptr == nullptr);
            REQUIRE(arena.get_block_count() > 0);
            REQUIRE(arena.get_total_allocated() > 0);
            REQUIRE(arena.get_total_used() > 0);
            test_mem(ptr, block_size + 128);

            ptr = arena.alloc(block_size * 2, i *= 2);
            REQUIRE_FALSE(ptr == nullptr);
            REQUIRE(arena.get_block_count() > 0);
            REQUIRE(arena.get_total_allocated() > 0);
            REQUIRE(arena.get_total_used() > 0);
            test_mem(ptr, block_size * 2);
        }
    }

    SECTION("Behavior with zero block size") {
        Arena arena(0);
        void* ptr;

        ptr = arena.alloc(10, 1);
        REQUIRE_FALSE(ptr == nullptr);
        REQUIRE(arena.get_block_count() > 0);
        REQUIRE(arena.get_total_allocated() > 0);
        REQUIRE(arena.get_total_used() > 0);
        test_mem(ptr, 10);

        ptr = arena.alloc(128, 16);
        REQUIRE_FALSE(ptr == nullptr);
        REQUIRE(arena.get_block_count() > 0);
        REQUIRE(arena.get_total_allocated() > 0);
        REQUIRE(arena.get_total_used() > 0);
        test_mem(ptr, 128);

        ptr = arena.alloc(313, 16);
        REQUIRE_FALSE(ptr == nullptr);
        REQUIRE(arena.get_block_count() > 0);
        REQUIRE(arena.get_total_allocated() > 0);
        REQUIRE(arena.get_total_used() > 0);
        test_mem(ptr, 313);

        ptr = arena.alloc(618, 16);
        REQUIRE_FALSE(ptr == nullptr);
        REQUIRE(arena.get_block_count() > 0);
        REQUIRE(arena.get_total_allocated() > 0);
        REQUIRE(arena.get_total_used() > 0);
        test_mem(ptr, 618);

        ptr = arena.alloc(1, 16);
        REQUIRE_FALSE(ptr == nullptr);
        REQUIRE(arena.get_block_count() > 0);
        REQUIRE(arena.get_total_allocated() > 0);
        REQUIRE(arena.get_total_used() > 0);
        test_mem(ptr, 1);

        ptr = arena.alloc(998, 16);
        REQUIRE_FALSE(ptr == nullptr);
        REQUIRE(arena.get_block_count() > 0);
        REQUIRE(arena.get_total_allocated() > 0);
        REQUIRE(arena.get_total_used() > 0);
        test_mem(ptr, 998);
    }
}
