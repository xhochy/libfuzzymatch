#include <iostream>
#include <fstream>
#include <vector>

#include <libfuzzymatch/bktree.h>
#include <libfuzzymatch/levenshtein.h>
#include <libfuzzymatch/util.h>

#include "../common/timing.h"

int main(int argc, char *argv[]) {
    if (argc != 2) {
        std::cerr << "You must specify exactly a single input filename." << std::endl;
        exit(EXIT_FAILURE);
    }

    std::ifstream file(argv[argc - 1]);
    std::vector<std::string> strings;
    for (std::string line; std::getline(file, line);) {
        strings.push_back(line);
    }
    file.close();

    double startTime = getCurrentTime();
    BKTree<uint32_t, uint32_t> tree(strings.size(), levenshteinStatic);
    for (const std::string &str : strings) {
        std::vector<uint32_t> data;
        utf8to32(str.c_str(), data);
        tree.insert(data);
    }
    double endTime = getCurrentTime();
    std::cout << "Took " << (endTime - startTime) << "s to build an index." << std::endl;

    std::ofstream index("index.bktree");
    // TODO
    index.close();

}
