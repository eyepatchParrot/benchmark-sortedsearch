#ifndef SortedBlockIndex_hh_INCLUDED
#define SortedBlockIndex_hh_INCLUDED

#include <vector>
#include <numeric>
#include <algorithm>
#include <cassert>

#include "hdr.hh"
#include "Span.hh"

namespace pvc {
struct SortedBlockIndex {
    using T = u64;

    std::vector<T> index;
    std::vector<uz> offsets;
    uz order;

    SortedBlockIndex(Span<T> level0, uz block_bytes);
};
}

namespace pv {
uz search_sorted_block_index(Span<u64> level0, int value, const SortedBlockIndex& h_index);
}

#endif // SortedBlockIndex_hh_INCLUDED

