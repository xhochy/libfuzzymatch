#include <algorithm>
#include <cassert>
#include <cstdlib>

#include "levenshteinLimit.h"

uint32_t levenshteinLimit1(const std::vector<uint32_t> &s, const std::vector<uint32_t> &t) {
    const uint32_t n((uint32_t)s.size());
    assert(abs((int)n - (int)t.size()) <= 1);

    // Initialise with unmatched characters in t
    uint32_t curr0, curr1, last0(1), last1(2);

    for (uint32_t i = 0; i < n; ++i) {
        curr0 = std::min(last0, i+1) + 1;
        curr0 = std::min(curr0, i + (int)(s[i] != t[0]));

        curr1 = std::min(last1, curr0) + 1;
        curr1 = std::min(curr1, last0 + (int)(s[i] != t[1]));

        if (i > 0 && curr0 > 1 && curr1 > 1) {
            return -1;
        }
        std::swap(curr0, last0);
        std::swap(curr1, last1);
    }
    return last1;
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
    if (abs(n-m) > threshold) {
        return -1;
    }

    std::vector<uint32_t> current(m + 1), last(m + 1);
    switch(threshold) {
        case 1:  return levenshteinLimit1(s, t);
        //case 2:  return levenshteinLimit2(s, t);
        //case 3:  return levenshteinLimit3(s, t);
        default: return levenshteinLimitCore(s, t, current, last, threshold);
    }
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
