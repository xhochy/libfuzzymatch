include_directories("${CMAKE_SOURCE_DIR}/src")

add_subdirectory(common)

add_subdirectory(bktree)
add_subdirectory(pairwise-levenshtein)
add_subdirectory(pairwise-levenshtein-static)
add_subdirectory(limited-levenshtein)

# Build the index
add_executable(build_index build_index.cpp)
target_link_libraries(build_index
    fuzzymatch_benchmark_common
    )
set_target_properties(build_index
    PROPERTIES
    RUNTIME_OUTPUT_NAME build-index
    )

# Query the index
add_executable(query_index query_index.cpp)
target_link_libraries(query_index
    fuzzymatch_benchmark_common
    fuzzymatch
    )
set_target_properties(query_index
    PROPERTIES
    RUNTIME_OUTPUT_NAME query-index
    )
