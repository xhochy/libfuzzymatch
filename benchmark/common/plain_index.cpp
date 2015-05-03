#include <iostream>
#include <fstream>

#include <libfuzzymatch/util.h>

void indexFromFile(const char *inputFile, const char *indexFile) {
    std::ifstream file(inputFile);
    std::ofstream index(indexFile);
    for (std::string line; std::getline(file, line);) {
        std::vector<uint32_t> indexed;
        utf8to32(line.data(), indexed);
        // Length of the new string
        uint32_t length = indexed.size();
        index.write(reinterpret_cast<const char *>(&length), sizeof(uint32_t));
        index.write(reinterpret_cast<const char *>(indexed.data()), sizeof(uint32_t)*length);
    }
    file.close();
    index.close();
}

void loadIndex(const char *indexFilename, std::vector<std::vector<uint32_t>> &index) {
    std::ifstream indexFile(indexFilename);
    while (!indexFile.eof()) {
        uint32_t length;
        indexFile.read(reinterpret_cast<char *>(&length), sizeof(uint32_t));
        std::vector<uint32_t> indexed(length);
        indexFile.read(reinterpret_cast<char *>(indexed.data()), sizeof(uint32_t) * length);
        index.push_back(indexed);
    }
    indexFile.close();
}
