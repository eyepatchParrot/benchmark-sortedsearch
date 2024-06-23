#include "SortedBlockIndex.hh"

using namespace pvc;
using T = SortedBlockIndex::T;

namespace pvc {
namespace {
std::vector<T> construct_next_level(Span<T> prev_level, uz block_size) {
    uz num_blocks = (prev_level.size() + block_size - 1) / block_size;
    std::vector<T> next_level;
    for (uz i = block_size - 1; i < prev_level.size(); i += block_size) {
        next_level.push_back(prev_level[i]);
    }
    return next_level;
}
}

SortedBlockIndex::SortedBlockIndex(Span<T> level0, uz block_bytes) : order(block_bytes / sizeof(T)) {
    std::vector<uz> lengths = {0};
    auto current_length = level0.size();

    while (current_length > order) {
        current_length = (current_length + order - 1) / order;
        lengths.push_back(current_length);
    }
    std::vector<uz> offsets(lengths.size());
    std::partial_sum(lengths.begin(), lengths.end(), offsets.begin());

    std::vector<u64> master_index(offsets.back());
    auto current_level = level0;

    uz start_idx = 0;
    for (uz i = 0; i < offsets.size() - 1; ++i) {
        uz end_idx = offsets[i + 1];
        std::vector<T> next_level = construct_next_level(current_level, order);
        std::copy(next_level.begin(), next_level.end(), master_index.begin() + start_idx);
        current_level = Span<T>{master_index.data() + start_idx, next_level.size()};
        start_idx = end_idx;
    }
}
} // namespace pvc

namespace pv {
uz search_sorted_block_index(Span<T> level0, int value, const SortedBlockIndex& h_index)
{
    const auto& index = h_index.index;
    const auto& offsets = h_index.offsets;
    auto block_size = h_index.order;

    int start_idx = 0;

    for (int i = offsets.size() - 1; i > 0; --i) {
        int level_start = offsets[i - 1];
        int level_end = offsets[i];
        int end_idx = start_idx + block_size;

        auto pos = std::lower_bound(index.begin() + level_start + start_idx, index.begin() + level_start + end_idx, value);
        start_idx = (pos - (index.begin() + level_start + start_idx) + start_idx) * block_size;
    }

    int end_idx = start_idx + block_size;
    auto pos = std::lower_bound(level0.begin() + start_idx, level0.begin() + end_idx, value);
    return start_idx + (pos - (level0.begin() + start_idx));
}
}
