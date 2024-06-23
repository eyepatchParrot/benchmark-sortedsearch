#include <algorithm>
#include <stdexcept>
#include <benchmark/benchmark.h>

#include "hdr.hh"
#include "Span.hh"
#include "vec_uar.hh"
#include "SortedBlockIndex.hh"

using namespace pvc;

static void BM_LowerBound(benchmark::State& state) {
    const size_t bytes = static_cast<size_t>(state.range(0));
    const size_t n = bytes / sizeof(u64);
    const auto shuffled_span = pv::vec_uar(n);
    const auto sorted_span = pv::vec_uar_sorted(n);
    uz target_i = 0;

    for (auto _ : state) {
        if (target_i >= n) target_i -= n;
        const auto target = shuffled_span.p_[target_i];
        benchmark::DoNotOptimize(std::lower_bound(sorted_span.begin(), sorted_span.end(), target));
        ++target_i;
    }
    state.counters["Bytes"] = bytes;
}

static void BM_SortedBlockIndex(benchmark::State& state) {
    const size_t bytes = static_cast<size_t>(state.range(0));
    const size_t n = bytes / sizeof(u64);
    const auto shuffled_span = pv::vec_uar(n);
    const auto sorted_span = pv::vec_uar_sorted(n);
    SortedBlockIndex index(sorted_span, 64);

    uz target_i = 0;
    for (auto _ : state) {
        if (target_i >= n) target_i -= n;
        const auto target = shuffled_span.p_[target_i];
        benchmark::DoNotOptimize(pv::search_sorted_block_index(sorted_span, target, index));
        ++target_i;
    }
    state.counters["Bytes"] = bytes;
}


static void BM_Both(benchmark::State& state) {
    const size_t bytes = static_cast<size_t>(state.range(0));
    const size_t n = bytes / sizeof(u64);
    const auto shuffled_span = pv::vec_uar(n);
    const auto sorted_span = pv::vec_uar_sorted(n);
    SortedBlockIndex index(sorted_span, 64);

    uz target_i = 0;
    for (auto _ : state) {
        if (target_i >= n) target_i -= n;
        const auto target = shuffled_span.p_[target_i];
        const auto index_i = pv::search_sorted_block_index(sorted_span, target, index);
        const auto lb_i = std::distance(sorted_span.begin(), std::lower_bound(sorted_span.begin(), sorted_span.end(), target));
        if (index_i != lb_i) {
            throw std::logic_error(__PRETTY_FUNCTION__);
        }
        ++target_i;
    }
    state.counters["Bytes"] = bytes;
}

// BENCHMARK(BM_LowerBound)->Range(64 * 1024, 1 << 20); // Vary data size
// BENCHMARK(BM_SortedBlockIndex)->Range(64 * 1024, 1 << 20); // Vary data size
BENCHMARK(BM_Both)->Range(64 * 1024, 1 << 20); // Vary data size

BENCHMARK_MAIN();
