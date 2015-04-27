#include <cassert>
#include <cstdint>
#include <iostream>

#include "levenshtein.h"
#include "util.h"

int main(int argc, char** argv) {
	if (argc < 3) {
		std::cout << "Usage: " << argv[0] << " string1 string2" << std::endl;
		return 1;
	}

	std::cout << "Matching " << argv[1] << " and " << argv[2] << "; " << std::flush;

	// Convert input to UTF32 int-arrays
	std::vector<uint32_t> s2, t2;
	utf8to32(argv[1], s2);
	utf8to32(argv[2], t2);

	const uint32_t threshold(3);

	uint32_t dist(levenshtein(s2, t2));
	uint32_t threshDist(levenshteinLimit(s2, t2, threshold));

	std::cout << "Levenshtein distance is " << dist << " = " << threshDist << std::endl;

	assert(threshDist == (uint32_t)(-1) && (dist > threshold) || dist == threshDist);
}
