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

	std::cout << "Levenshtein distance is " << levenshtein(s2, t2) << std::endl;
}
