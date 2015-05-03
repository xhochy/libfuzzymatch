#pragma once
#include <vector>

void indexFromFile(const char *inputFile, const char *indexFile);
void loadIndex(const char *indexFile, std::vector<std::vector<uint32_t>> &index);

