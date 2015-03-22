#include <algorithm>

#include "levenshtein.h"

/**
 * Levenshtein distance for UTF-32 "strings".
 */
uint32_t levenshtein(const std::vector<uint32_t> &s, const std::vector<uint32_t> &t) {
	const size_t n(s.size()), m(t.size());
	std::vector<uint32_t> current(m + 1, 0), last(m + 1);

	// Initialise with unmatched characters in t
	for (size_t j = 0; j <= m; ++j) last[j] = j;

	for (size_t i = 0; i < n; ++i) {
		current[0] = i+1; // = last[0]+1, corresponds to unmatched character in s
		for (size_t j  = 1; j <= m; ++j) {
			current[j] = std::min(last[j] + 1, current[j-1] + 1); // insertion / deletion
			current[j] = std::min(current[j], last[j-1] + (int)(s[i] != t[j-1])); // modification
		}
		current.swap(last);
	}
    return last.back();
}
