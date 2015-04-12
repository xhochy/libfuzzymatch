include_directories("${CMAKE_SOURCE_DIR}/src")

# Build the index
add_executable(pairwise_levenshtein_build_index build_index.cpp)
set_target_properties(pairwise_levenshtein_build_index
    PROPERTIES
    RUNTIME_OUTPUT_NAME build-index
    )

# Query the index, the exciting stuff
add_executable(pairwise_levenshtein_query_index query_index.cpp)
target_link_libraries(pairwise_levenshtein_query_index fuzzymatch)
set_target_properties(pairwise_levenshtein_query_index
    PROPERTIES
    RUNTIME_OUTPUT_NAME query-index
    )

