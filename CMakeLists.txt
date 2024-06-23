cmake_minimum_required(VERSION 3.12)
project(Index)

set (CMAKE_CXX_STANDARD 17)

# Find Google Benchmark
find_package(benchmark REQUIRED)

# Add the executable
add_executable(index
    index.cc
    vec_uar.hh
    vec_uar.cc
    Span.hh
    hdr.hh
    SortedBlockIndex.cc
    SortedBlockIndex.hh
)

# Link with Google Benchmark
target_link_libraries(index PRIVATE benchmark::benchmark)
