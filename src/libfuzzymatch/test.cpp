#include <cstdint>
#include <iostream>
#include <string>

#include "levenshtein.h"
#include "../utf8/utf8.h"

int main(int argc, char** argv) {
	if (argc < 3) {
		std::cout << "Usage: " << argv[0] << " string1 string2" << std::endl;
		return 1;
	}

	std::string s(argv[1]);
	std::string t(argv[2]);
	std::vector<uint32_t> s2(utf8::distance(s.cbegin(), s.cend())), t2(utf8::distance(t.cbegin(), t.cend()));

	std::cout << "Matching " << s << " and " << t << std::endl;

	utf8::utf8to32(s.cbegin(), s.cend(), s2.data());
	utf8::utf8to32(t.cbegin(), t.cend(), t2.data());

	std::cout << levenshtein(s2, t2) << std::endl;
}
