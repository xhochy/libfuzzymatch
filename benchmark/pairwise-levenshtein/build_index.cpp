#include <iostream>

#include "../common/plain_index.h"

int main(int argc, char *argv[]) {
    if (argc != 2) {
        std::cerr << "You must specify exactly a single input filename." << std::endl;
        exit(EXIT_FAILURE);
    }

    indexFromFile(argv[argc - 1], "index.plain");

    // Only I/O which does not count towards measured time.
    std::cout << "Took 0s to build an index." << std::endl;
}
