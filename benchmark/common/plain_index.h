#pragma once
#include <cstdint>
#include <vector>

#include "index.h"

void indexFromFile(const char *inputFile, const char *indexFile);
void loadIndex(const char *indexFile, std::vector<std::vector<uint32_t>> &index);

class PlainIndex : public Index {
public:
	~PlainIndex() {}

	void load(const char* fn);
	size_t size() const;
	const std::vector<uint32_t> &get(const size_t) const;
protected:
	std::vector<std::vector<uint32_t>> index;
};
