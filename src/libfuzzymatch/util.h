#include <cstring>
#include <string>
#include <vector>

#include "../utf8/utf8.h"

/*
 * Convert UTF-8 string to std::vector<uint32_t>
 */
void utf8to32(const std::string &s, std::vector<uint32_t> &vec) {
	vec.assign(utf8::distance(s.cbegin(), s.cend()), 0);
	utf8::utf8to32(s.cbegin(), s.cend(), vec.data());
}

/*
 * Convert UTF-8 C-string to std::vector<uint32_t>
 */
void utf8to32(char* const s, std::vector<uint32_t> &vec) {
	const size_t len(strlen(s));
	vec.assign(utf8::distance(s, s+len), 0);
	utf8::utf8to32(s, s+len, vec.data());
}
