#include <algorithm>
#include <cassert>
#include <iostream>

#include "levenshtein.h"


uint32_t levenshteinCore(const std::vector<uint32_t> &s, const std::vector<uint32_t> &t,
                         std::vector<uint32_t> &current, std::vector<uint32_t> &last) {
    const size_t n(s.size()), m(t.size());

    // Initialise with unmatched characters in t
    for (size_t j = 0; j <= m; ++j) last[j] = j;

    std::cout << "LEVENSHTEIN" << std::endl;
    for (size_t i = 0; i < n; ++i) {
        current[0] = i+1; // = last[0]+1, corresponds to unmatched character in s
        std::cout << "current[0] = " << i+1 << std::endl;
        for (size_t j  = 1; j <= m; ++j) {
            std::cout << "current[" << j << "] = min(" << last[j] + 1 << ", "
                << current[j-1] + 1 << ", " << last[j-1] + (int)(s[i] != t[j-1]) << " // s[" << i << "] != t[" << j-1 << "])" << std::endl;
            current[j] = std::min(last[j] + 1, current[j-1] + 1); // insertion / deletion
            current[j] = std::min(current[j], last[j-1] + (int)(s[i] != t[j-1])); // modification
        }
        current.swap(last);
        std::cout << std::endl;
    }
    return last.back();
}

/**
 * Levenshtein distance for UTF-32 "strings".
 */
uint32_t levenshtein(const std::vector<uint32_t> &s, const std::vector<uint32_t> &t) {
	const size_t n(s.size()), m(t.size());
	std::vector<uint32_t> current(m + 1, 0), last(m + 1);
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
    assert(n >= m);
    const size_t width(std::min((size_t)2*threshold+1, m));
    assert(current.size() >= width && last.size() >= width);

    // Initialise with unmatched characters in t
    for (size_t j = 0; j <= width; ++j) last[j] = j;

    std::cout << "LEVENSHTEIN WITH LIMIT" << std::endl;

    for (size_t i = 0; i <= threshold && i < n; ++i) {
        current[0] = i+1; // = last[0]+1, corresponds to unmatched character in s
        const size_t stop(std::min(m, size_t{i+threshold}));
        for (size_t j = 1; j < stop; ++j) {
            std::cout << "current[" << j << "] = min(" << last[j] + 1 << ", "
                << current[j-1] + 1 << ", " << last[j-1] + (int)(s[i] != t[j-1]) << "=" << last[j-1] << "+" << (int)(s[i] != t[j-1]) << ")" << std::endl;
            current[j] = std::min(last[j] + 1, current[j-1] + 1); // insertion / deletion
            current[j] = std::min(current[j], last[j-1] + (int)(s[i] != t[j-1])); // modification
        }
        std::cout << "current[" << stop << "] = min(<optimized out>, " << current[stop-1]+1 << ", " << last[stop-1] + (int)(s[i] != t[stop-1]) << "=" << last[stop-1] << "+" << (int)(s[i] != t[stop-1]) << ")" << std::endl;
        current[stop] = std::min(current[stop-1]+1, last[stop-1] + (int)(s[i] != t[stop-1]));

        current.swap(last);
        std::cout << std::endl;    
    }


    uint32_t minDistance;
    for (size_t i = threshold + 1; i < n; ++i) {
        // Determine area to look at
        const size_t start(i-threshold), stop(std::min(m, size_t{i + threshold}));

        std::cout << "i=" << i << " start=" << start << " stop=" << stop << std::endl; std::cout << "current[0 (" << start << ")] = min(" << last[1]+1 << ", <optimized out>, " << last[0] + (int)(s[i] != t[start-1]) << " // s[" << i << "] != t[" << start-1 << "])" << std::endl;
        current[0] = std::min(last[1]+1, last[0] + (int)(s[i] != t[start-1]));
        minDistance = current[0];

        for (size_t j = 1; j < stop-start; ++j) {
            std::cout << "current[" << j << " (" << j+start << ")] = min("<< last[j+1] + 1 << ", " << current[j-1] + 1 << ", " << last[j] + (int)(s[i] != t[j+start-1]) << " = " << last[j] << " // s[" << i << "]!=t[" << j+start-1 << "])" << std::endl;

            current[j] = std::min(last[j+1] + 1, current[j-1] + 1); // insertion / deletion
            current[j] = std::min(current[j], last[j] + (int)(s[i] != t[j+start-1])); // modification
            minDistance = std::min(minDistance, current[j]);
        }

        current[stop-start] = std::min(current[stop-start-1] + 1, last[stop-start] + (int)(s[i] != t[stop-1]));
        minDistance = std::min(minDistance, current[stop-start]);
        std::cout << "current[" << stop-start << " (" << stop << ")] = min(<optimized out>, " << current[stop-start-1] +1 << ", " << last[stop-start] + (int)(s[i] != t[stop-1]) << " // s[" << i << "] != t[" << stop-1 << "])" << std::endl << "minDistance: " << minDistance << std::endl;
        if (minDistance > threshold) {
            return -1;
        }
        current.swap(last);
    }

    //if (n < m + threshold) {
        std::cout << "n = " << n << " < " << m+threshold << " = (m=" << m << " + t=" << threshold << ")" << std::endl;
        size_t pos = std::min(m, size_t{n-1 + threshold}) - (n-1-threshold);
        std::cout << "last[" << pos << " = min(" << m <<", " << size_t{n-1 + threshold} << ") + " << (n-1-threshold) << "]" << std::endl;
        return last[pos];
    /*} else {
        std::cout << "last[t=" << threshold << "]" << std::endl;
        return last[threshold];
    }*/
}

/**
 * Levenshtein distance for UTF-32 "strings".
 */
uint32_t levenshteinLimit(const std::vector<uint32_t> &s, const std::vector<uint32_t> &t, const uint32_t threshold) {
    if (s.size() < t.size()) {
        std::cout << "FLIPPING so that n >= m" << std::endl;
        return levenshteinLimit(t, s, threshold);
    }
    const size_t width(std::min(t.size(), (size_t)2*threshold + 1));
    std::vector<uint32_t> current(width + 1), last(width + 1);
    return levenshteinLimitCore(s, t, current, last, threshold);
}
