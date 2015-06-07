#pragma once

#include <cstdint>
#include <vector>

/**
 * INTERNAL implementation of levenshtein distance for UTF-32 strings
 */
template<typename C1, typename C2>
uint32_t levenshteinCore(const C1 &s, const C2 &t,
                         std::vector<uint32_t> &current, std::vector<uint32_t> &last) {
    static_assert(std::is_same<typename C1::value_type, uint32_t>::value,
        "First container must be a 32bit unsigned int");
    static_assert(std::is_same<typename C2::value_type, uint32_t>::value,
        "Second container must be a 32bit unsigned int");

    const size_t n(s.size()), m(t.size());

    // Initialise with unmatched characters in t
    for (size_t j = 0; j <= m; ++j) last[j] = j;

    for (size_t i = 0; i < n; ++i) {
        current[0] = i+1; // = last[0]+1, corresponds to unmatched character in s
        for (size_t j  = 1; j <= m; ++j) {
            uint32_t cur = std::min(last[j], current[j-1]) + 1; // insertion / deletion
            current[j] = std::min(cur, last[j-1] + (uint32_t)(s[i] != t[j-1])); // modification
        }
        current.swap(last);
    }
    return last.back();
}


/**
 * Levenshtein distance for UTF-32 "strings".
 * @param s First string input for levenshtein distance
 * @param t Second string input for levenshtein distance
 * @return The levenshtein distance between the two inputs.
 */
template<typename C1, typename C2>
uint32_t levenshtein(const C1 &s, const C2 &t) {
    const size_t n(s.size()), m(t.size());
    std::vector<uint32_t> current(m + 1), last(m + 1);
    return levenshteinCore(s, t, current, last);
}


/**
 * Levenshtein distance for UTF-32 "strings" using static storage for less allocations.
 *
 * The two temporary used arrays are statically allocated internally, i.e. are
 * always expanded to the maximum size of the second input and never scaled down.
 * Although this offers better performance, you need to be aware that large
 * inputs will increase the memory usage for the whole lifespan of the procress.
 *
 * @param s First string input for levenshtein distance
 * @param t Second string input for levenshtein distance
 * @return The levenshtein distance between the two inputs.
 */
template<typename C1, typename C2>
uint32_t levenshteinStatic(const C1 &s, const C2 &t) {
    const size_t m(t.size());
    static std::vector<uint32_t> current;
    static std::vector<uint32_t> last;

    if (current.size() < m + 1) {
        // Also: g_last.size < m + 1
        current.resize(m + 1);
        last.resize(m + 1);
    }

    return levenshteinCore(s, t, current, last);
}
