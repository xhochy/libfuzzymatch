#include <functional>
#include <iostream>
#include <fstream>
#include <queue>
#include <tuple>
#include <vector>

#include <libfuzzymatch/levenshteinLimit.h>
#include <libfuzzymatch/util.h>

#include "../common/pairwise.h"
#include "../common/plain_index.h"
#include "../common/timer.h"

int main(int argc, char *argv[]) {
    if (argc != 4) {
        std::cerr << "Usage: \n\t query-index <index> <query-file> <threshold>" << std::endl;
        exit(EXIT_FAILURE);
    }

    std::vector<std::string> queries;
    std::ifstream file(argv[2]);
    for (std::string line; std::getline(file, line);) {
        queries.push_back(line);
    }
    file.close();

    std::vector<std::vector<uint32_t>> index;
    loadIndex(argv[1], index);

    const uint32_t threshold(atoi(argv[3]));

    Timer timer;

    benchLevenshtein(queries, index, [=](const std::vector<uint32_t> &s, const std::vector<uint32_t> &t) {
        return levenshteinLimit(s, t, threshold);
    });
    auto t = timer.get();
    std::cout << "Took " << t << "s to execute the queries." << std::endl;
    std::cout << "RESULT algo=limit threshold=" << threshold << " index=" << argv[1] << " queries=" << argv[2] << " time=" << t << std::endl;
}
