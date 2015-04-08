#include <iostream>
#include <fstream>

int main(int argc, char *argv[]) {
    if (argc != 2) {
        std::cerr << "You must specify exactly a single input filename." << std::endl;
        exit(EXIT_FAILURE);
    }

    std::ifstream file(argv[argc - 1]);
    std::ofstream index("index.plain");
    index << file.rdbuf();
    file.close();
    index.close();

    // Only I/O which does not count towards measured time.
    std::cout << "Took 0s to build an index." << std::endl;
}
