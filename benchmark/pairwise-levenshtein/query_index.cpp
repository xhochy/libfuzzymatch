#include <iostream>
#include <fstream>
#include <vector>

#include <libfuzzymatch/levenshtein.h>
#include <libfuzzymatch/util.h>

#include "../common/timing.h"

int main(int argc, char *argv[]) {
    if (argc != 3) {
        std::cerr << "Usage: \n\t query-index <index> <query-file>" << std::endl;
        exit(EXIT_FAILURE);
    }

    std::vector<std::string> queries;
    std::ifstream file(argv[argc - 1]);
    for (std::string line; std::getline(file, line);) {
        queries.push_back(line);
    }
    file.close();

    std::vector<std::vector<uint32_t>> index;
    std::ifstream indexFile(argv[argc - 2]);
    for (std::string line; std::getline(indexFile, line);) {
        std::vector<uint32_t> indexed;
        utf8to32(line.data(), indexed);
        index.push_back(indexed);
    }
    file.close();

    double startTime = getCurrentTime();
    for (const std::string &_query: queries) {
        std::vector<uint32_t> query;
        utf8to32(_query.data(), query);
        for (const std::vector<uint32_t> &indexed: index) {
            uint32_t distance = levenshtein(query, indexed);
            // TODO: Add to Top20 PQ
        }
    }
    double endTime = getCurrentTime();
    std::cout << "Took " << (endTime - startTime) << "s to execute the queries." << std::endl;
}
