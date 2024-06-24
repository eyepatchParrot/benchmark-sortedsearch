cmake_minimum_required(VERSION 3.12)
project(Index)

set (CMAKE_CXX_STANDARD 17)
set (CMAKE_EXPORT_COMPILE_COMMANDS ON)
set (CMAKE_CXX_FLAGS_DEBUG "-g -O0 -fsanitize=address")
string(APPEND CMAKE_CXX_FLAGS " -march=skylake -mtune=skylake")
# string(APPEND CMAKE_CXX_FLAGS " -march=skylake")

find_package(benchmark REQUIRED)
find_package(Catch2 REQUIRED)

add_library(sortedsearch
    STATIC
    vec_uar.hh
    vec_uar.cc
    Span.hh
    hdr.hh
    SortedBlockIndex.cc
    SortedBlockIndex.hh
)

# Add the executable
add_executable(index index.cc)
target_link_libraries(index PRIVATE benchmark::benchmark sortedsearch)

add_executable(SortedBlockIndexExe SortedBlockIndex_test.cc)
target_link_libraries(SortedBlockIndexExe sortedsearch Catch2::Catch2WithMain)

enable_testing()
add_test(NAME SortedBlockIndexTest COMMAND SortedBlockIndexExe)
