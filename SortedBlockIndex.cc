#include "SortedBlockIndex.hh"
#include <immintrin.h>

using namespace pvc;
using T = SortedBlockIndex::T;

namespace pvc {
namespace {
std::vector<T> construct_next_level(Span<T> prev_level, uz block_size) {
    uz num_blocks = (prev_level.size() - 1) / block_size;
    std::vector<T> next_level;
    for (uz i = block_size; i < prev_level.size(); i += block_size) {
        next_level.push_back(prev_level[i]);
    }
    return next_level;
}
// Each element in the parent is the first element of a child array. We search with lower_bound, and skip the first child,
// So we know that the first target to get sent to the second child is the first one to match the first element of that child.
}

SortedBlockIndex::SortedBlockIndex(Span<T> level0, uz block_bytes) : order(block_bytes / sizeof(T)) {
    std::vector<uz> lengths = {0};
    auto current_length = level0.size();

    while (current_length > order) {
        // This needs to be sync'd with construct_next_level
        current_length = (current_length - 1) / order;
        lengths.push_back(current_length);
    }
    offsets.resize(lengths.size());
    std::partial_sum(lengths.begin(), lengths.end(), offsets.begin());

    index.resize(offsets.back());
    auto current_level = level0;

    uz start_idx = 0;
    for (uz i = 0; i < offsets.size() - 1; ++i) {
        uz end_idx = offsets[i + 1];
        std::vector<T> next_level = construct_next_level(current_level, order);
        std::copy(next_level.begin(), next_level.end(), index.begin() + start_idx);
        current_level = Span<T>{index.data() + start_idx, next_level.size()};
        start_idx = end_idx;
    }
}
} // namespace pvc

namespace pv {
namespace {
uz lb8_cnt(u64 const * X, u64 y) {
    auto vy = _mm256_set1_epi64x(y);
    auto vx0 = _mm256_loadu_si256((__m256i const *)X);
    auto vx4 = _mm256_loadu_si256((__m256i const *)(X+4));
    auto cmp0 = _mm256_cmpgt_epi64(vx0, vy);
    auto cmp4 = _mm256_cmpgt_epi64(vx4, vy);
    auto blend = _mm256_blend_epi32(cmp0, cmp4, 0x55);
    auto mask = _mm256_movemask_ps(blend);
    auto sum = _mm_popcnt_u32(mask);
    return sum;
}
}

uz search_sorted_block_index(Span<T> level0, T value, const SortedBlockIndex& h_index)
{
    const auto& index = h_index.index;
    const auto& offsets = h_index.offsets;
    auto block_size = h_index.order;

    uz start_idx = 0;
    for (int i = offsets.size() - 1; i > 0; --i) {
        auto level_start = offsets[i - 1];
        auto level_end = offsets[i];
        auto end_idx = std::min(level_start + start_idx + block_size, level_end);
        auto search_begin = index.begin() + level_start + start_idx;
        auto search_end = index.begin() + end_idx;
        if (search_end - search_begin == 8) {
#if 0
            auto pos = std::distance(index.begin() + level_start, std::lower_bound(search_begin, search_end, value));
#else
            auto pos = lb8_cnt(index.data() + level_start + start_idx, value);
#endif
            start_idx = pos * block_size;
        } else {
            auto pos = std::distance(index.begin() + level_start, std::lower_bound(search_begin, search_end, value));
            start_idx = pos * block_size;
        }
    }

    auto end_idx = std::min(start_idx + block_size, level0.size());
    auto search_begin = level0.begin() + start_idx;
    auto search_end = level0.begin() + end_idx;
    if (search_end - search_begin == 8) {
#if 0
        auto pos = std::distance(index.begin() + level_start, std::lower_bound(search_begin, search_end, value));
#else
        auto pos = lb8_cnt(level0.p_ + start_idx, value);
#endif
        return start_idx + pos;
    } else {
        return std::distance(level0.begin(), std::lower_bound(search_begin, search_end, value));
    }
}
}
