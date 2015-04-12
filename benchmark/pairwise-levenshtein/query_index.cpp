#include <functional>
#include <iostream>
#include <fstream>
#include <queue>
#include <tuple>
#include <vector>

#include <libfuzzymatch/levenshtein.h>
#include <libfuzzymatch/util.h>

#include "../common/timing.h"

void benchLevenshtein(std::vector<std::string> queries,
                      std::vector<std::vector<uint32_t>> index,
                      std::function<uint32_t(const std::vector<uint32_t> &, const std::vector<uint32_t> &)> lv) {
    for (const std::string &_query: queries) {
        std::vector<uint32_t> query;
        utf8to32(_query.data(), query);
        std::priority_queue<std::tuple<uint32_t, size_t>> pq;
        for (size_t idx = 0; idx < index.size(); idx++) {
            uint32_t distance = lv(query, index[idx]);
            pq.push(std::tuple<uint32_t, size_t>(distance, idx));
            if (pq.size() > 20) {
                pq.pop();
            }
        }
    }
}

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
    benchLevenshtein(queries, index, levenshtein);
    double endTime = getCurrentTime();
    std::cout << "Took " << (endTime - startTime) << "s to execute the queries." << std::endl;

    startTime = getCurrentTime();
    benchLevenshtein(queries, index, levenshteinStatic);
    endTime = getCurrentTime();
    std::cout << "Took " << (endTime - startTime) << "s to execute the queries (static allocated buffers)." << std::endl;
}
