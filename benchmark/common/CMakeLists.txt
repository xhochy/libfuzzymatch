include_directories("${CMAKE_SOURCE_DIR}/src")

add_library(fuzzymatch_benchmark_common
    pairwise.cpp
    plain_index.cpp
    )

