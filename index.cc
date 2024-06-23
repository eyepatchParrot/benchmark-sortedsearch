#include <algorithm>
#include <benchmark/benchmark.h>

#include "hdr.hh"
#include "Span.hh"
#include "vec_uar.hh"

using namespace pvc;

static void BM_LowerBound(benchmark::State& state) {
    const size_t n = static_cast<size_t>(state.range(0));
    const auto shuffled_span = pv::vec_uar(n);
    const auto sorted_span = pv::vec_uar_sorted(n);
    uz target_i = 0;

    for (auto _ : state) {
        if (target_i >= n) target_i -= n;
        const auto target = shuffled_span.p_[target_i];
        benchmark::DoNotOptimize(std::lower_bound(sorted_span.begin(), sorted_span.end(), target));
        ++target_i;
    }
}
BENCHMARK(BM_LowerBound)->Range(1 << 10, 1 << 20); // Vary data size

BENCHMARK_MAIN();
