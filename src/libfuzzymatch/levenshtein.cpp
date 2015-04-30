#include <algorithm>

#include "levenshtein.h"


uint32_t levenshteinCore(const std::vector<uint32_t> &s, const std::vector<uint32_t> &t,
                         std::vector<uint32_t> &current, std::vector<uint32_t> &last) {
    const size_t n(s.size()), m(t.size());

    // Initialise with unmatched characters in t
    for (size_t j = 0; j <= m; ++j) last[j] = j;

    for (size_t i = 0; i < n; ++i) {
        current[0] = i+1; // = last[0]+1, corresponds to unmatched character in s
        for (size_t j  = 1; j <= m; ++j) {
            current[j] = std::min(last[j], current[j-1]) + 1; // insertion / deletion
            current[j] = std::min(current[j], last[j-1] + (int)(s[i] != t[j-1])); // modification
        }
        current.swap(last);
    }
    return last.back();
}

/**
 * Levenshtein distance for UTF-32 "strings".
 */
uint32_t levenshtein(const std::vector<uint32_t> &s, const std::vector<uint32_t> &t) {
    const size_t n(s.size()), m(t.size());
    std::vector<uint32_t> current(m + 1), last(m + 1);
    return levenshteinCore(s, t, current, last);
}

uint32_t levenshteinStatic(const std::vector<uint32_t> &s, const std::vector<uint32_t> &t) {
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

uint32_t levenshteinLimitCore(const std::vector<uint32_t> &s, const std::vector<uint32_t> &t,
                         std::vector<uint32_t> &current, std::vector<uint32_t> &last,
                         const uint32_t threshold) {
    const size_t n(s.size()), m(t.size());

    // Initialise with unmatched characters in t
    for (size_t j = 0; j <= m; ++j) last[j] = j;

    for (size_t i = 0; i < n; ++i) {
        current[0] = i+1;
        // min(start) = 1 as position 0 is handled above
        const uint32_t start(std::max(1, (int)i - (int)threshold)), stop(std::min(m, i + threshold));
        for (size_t j = start; j <= stop; ++j) {
            current[j] = std::min(last[j], current[j-1]) + 1; // insertion / deletion
            current[j] = std::min(current[j], last[j-1] + (int)(s[i] != t[j-1])); // modification
        }

        // start needs to include position 0 and current[stop]
        const auto it_start(current.cbegin() + std::max(0, (int)i - (int)threshold)),
                   it_stop(current.cbegin() + stop + 1);
        if (*std::min_element(it_start, it_stop) > threshold) {
            return -1;
        }
        current.swap(last);
    }
    return last.back();
}

/**
 * Levenshtein distance for UTF-32 "strings".
 */
uint32_t levenshteinLimit(const std::vector<uint32_t> &s, const std::vector<uint32_t> &t, const uint32_t threshold) {
    const size_t n(s.size()), m(t.size());

    // If the difference in word length exceeds the threshold, don't even bother
    if ((m < n && n-m > threshold) || (m > n && m-n > threshold)) {
        return -1;
    }

    std::vector<uint32_t> current(m + 1), last(m + 1);
    return levenshteinLimitCore(s, t, current, last, threshold);
}

uint32_t levenshteinLimitStatic(const std::vector<uint32_t> &s, const std::vector<uint32_t> &t, const uint32_t threshold) {
    const size_t n(s.size()), m(t.size());
    static std::vector<uint32_t> current;
    static std::vector<uint32_t> last;

    // If the difference in word length exceeds the threshold, don't even bother
    if ((m < n && n-m > threshold) || (m > n && m-n > threshold)) {
        return -1;
    }

    if (current.size() < m + 1) {
        // Also: last.size < m + 1
        current.resize(m + 1);
        last.resize(m + 1);
    }

    return levenshteinLimitCore(s, t, current, last, threshold);
}
