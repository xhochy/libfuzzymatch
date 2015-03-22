add_library(fuzzymatch
    levenshtein.cpp
    )

add_executable(test test.cpp levenshtein.cpp)
