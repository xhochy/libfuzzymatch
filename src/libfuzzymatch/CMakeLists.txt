add_library(fuzzymatch
    levenshtein.cpp
    levenshteinLimit.cpp
    )

add_executable(test test.cpp levenshtein.cpp)

install(FILES levenshtein.h DESTINATION include/libfuzzymatch)
