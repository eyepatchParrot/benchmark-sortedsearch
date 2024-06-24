#define CATCH_CONFIG_MAIN
#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_approx.hpp>
#include <catch2/catch_template_test_macros.hpp>
#include <catch2/catch_tostring.hpp>
#include "SortedBlockIndex.hh"
#include "Span.hh"

using namespace pvc;

TEST_CASE("Test construct level1") {
    SECTION("Evenly divisible")
    {
        std::vector<u64> level0_data = {0, 2, 4, 6, 8, 10, 12, 14};
        Span<u64> level0{level0_data.data(), level0_data.size()};
        uz block_bytes = 2 * sizeof(u64);
        SortedBlockIndex level1(level0, block_bytes);
        REQUIRE(level1.index == std::vector<u64>{4, 8, 12, 12});
        for (size_t i = 0; i < level0.size(); ++i) {
            REQUIRE(pv::search_sorted_block_index(level0, level0[i], level1) == i);
        }
    }

    SECTION("Not evenly divisible")
    {
        std::vector<u64> level0_data = {0, 1, 2, 3, 4, 5, 6, 7, 8};
        Span<u64> level0{level0_data.data(), level0_data.size()};
        uz block_bytes = 3 * sizeof(u64);
        SortedBlockIndex level1(level0, block_bytes);
        REQUIRE(level1.index == std::vector<u64>{3, 6});
        for (size_t i = 0; i < level0.size(); ++i) {
            REQUIRE(pv::search_sorted_block_index(level0, level0[i], level1) == i);
        }
    }

    SECTION("Single block")
    {
        std::vector<u64> level0_data = {0, 1, 2};
        Span<u64> level0{level0_data.data(), level0_data.size()};
        uz block_bytes = 4 * sizeof(u64);
        SortedBlockIndex level1(level0, block_bytes);
        REQUIRE(level1.index == std::vector<u64>{});
        for (size_t i = 0; i < level0.size(); ++i) {
            REQUIRE(pv::search_sorted_block_index(level0, level0[i], level1) == i);
        }
    }

    SECTION("Large block size")
    {
        std::vector<u64> level0_data = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
        Span<u64> level0{level0_data.data(), level0_data.size()};
        uz block_bytes = 5 * sizeof(u64);
        SortedBlockIndex level1(level0, block_bytes);
        REQUIRE(level1.index == std::vector<u64>{5, 10});
        for (size_t i = 0; i < level0.size(); ++i) {
            REQUIRE(pv::search_sorted_block_index(level0, level0[i], level1) == i);
        }
    }
}
