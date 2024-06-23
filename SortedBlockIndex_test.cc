#include <vector>
#include <iostream>
#include "SortedBlockIndex.hh"

using namespace pvc;

void test(bool condition, const char* message) {
    if (!condition) {
        throw std::logic_error(message);
    }
}

void test_construct_level1() {
    // Test case 1: Evenly divisible
    {
        std::vector<u64> level0_data = {0, 2, 4, 6, 8, 10, 12, 14};
        Span<u64> level0{level0_data.data(), level0_data.size()};
        uz block_bytes = 2 * sizeof(u64);
        SortedBlockIndex level1(level0, block_bytes);
        test((level1.index == std::vector<u64>{2, 6, 10}), "Test case 1 failed");
    }

    // Test case 2: Not evenly divisible
    {
        std::vector<u64> level0_data = {0, 1, 2, 3, 4, 5, 6, 7, 8};
        Span<u64> level0{level0_data.data(), level0_data.size()};
        uz block_bytes = 3 * sizeof(u64);
        SortedBlockIndex level1(level0, block_bytes);
        test((level1.index == std::vector<u64>{2, 5}), "Test case 2 failed");
    }

    // Test case 3: Single block
    {
        std::vector<u64> level0_data = {0, 1, 2};
        Span<u64> level0{level0_data.data(), level0_data.size()};
        uz block_bytes = 4 * sizeof(u64);
        SortedBlockIndex level1(level0, block_bytes);
        test((level1.index == std::vector<u64>{}), "Test case 3 failed");
    }

    // Test case 4: Large block size
    {
        std::vector<u64> level0_data = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
        Span<u64> level0{level0_data.data(), level0_data.size()};
        uz block_bytes = 5 * sizeof(u64);
        SortedBlockIndex level1(level0, block_bytes);
        test((level1.index == std::vector<u64>{4, 9}), "Test case 4 failed");
    }
}

void test_search_sorted_block_index() {
    std::vector<u64> level0_data = {0, 2, 4, 6, 8, 10, 12, 14};
    Span<u64> level0{level0_data.data(), level0_data.size()};
    uz block_bytes = 2 * sizeof(u64);
    SortedBlockIndex level1(level0, block_bytes);

    for (size_t i = 0; i < level0.size(); ++i) {
        test(pv::search_sorted_block_index(level0, level0[i], level1) == i, "search_sorted_block_index test failed");
    }
}

int main() {
    test_construct_level1();
    test_search_sorted_block_index();
    std::cout << "All tests passed!" << std::endl;
    return 0;
}

