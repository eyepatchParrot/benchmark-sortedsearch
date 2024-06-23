#ifndef hdr_hh_INCLUDED
#define hdr_hh_INCLUDED

#include <cstddef>
#include <cstdint>
#include <vector>
#include <unordered_map>

namespace pvc {
using uz = size_t;
using u64 = uint64_t;
template<class T> using Vec = std::vector<T>;
template<class K, class V> using Map = std::unordered_map<K, V>;
}

namespace pv {
using namespace pvc;
}

#endif // hdr_hh_INCLUDED

