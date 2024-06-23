#include "vec_uar.hh"

#include <limits>
#include <random>
#include <algorithm>

namespace pv {

u64 roll() {
    thread_local std::mt19937_64 generator(std::random_device{}());
    thread_local std::uniform_int_distribution<u64> distribution;
    return distribution(generator);
}

Span<u64> vec_uar(uz n) {
    thread_local Vec<u64> rv;
    if (n>rv.size()) {
        const auto last_size = rv.size();
        rv.resize(n);
        std::generate(rv.begin() + last_size, rv.end(), roll);
        auto x = vec_uar_sorted(n);
        std::copy(rv.begin(), rv.end(), x.begin());
        std::sort(x.begin(), x.end());
    }
    return {rv.data(), n};
}

Span<u64> vec_uar_sorted(uz n) {
    // sort logic happens in vec_uar
    thread_local Vec<u64> rv;
    if (n>rv.size()) {
        rv.resize(n);
    }
    return {rv.data(), n};
}

}
