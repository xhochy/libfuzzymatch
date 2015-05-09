include_directories("${CMAKE_SOURCE_DIR}/src")

# Query the index, the exciting stuff
add_executable(pairwise_levenshtein_query_index query_index.cpp)
target_link_libraries(pairwise_levenshtein_query_index fuzzymatch
    fuzzymatch_benchmark_common
    )
set_target_properties(pairwise_levenshtein_query_index
    PROPERTIES
    RUNTIME_OUTPUT_NAME query-index
    )

