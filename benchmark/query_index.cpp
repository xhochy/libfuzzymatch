#include <iostream>
#include <fstream>

#include <libfuzzymatch/levenshtein.h>
#include <libfuzzymatch/levenshteinLimit.h>
#include <libfuzzymatch/util.h>

#include "common/args.h"
#include "common/benchmark.h"
#include "common/index.h"
#include "common/pairwise.h"
#include "common/plain_index.h"
#include "common/timer.h"

void usage(char** argv) {
    std::cout << "Usage: " << argv[0] << " [options]" << std::endl << std::endl
        << "-a <algo>      -- algorithm to benchmark. Choices: pairwise, static, limit, lucene++" << std::endl
        << "-i <index>     -- index filename" << std::endl
        << "-q <queries>   -- query filename" << std::endl
        << std::endl
        << "Per-algorithm settings:" << std::endl
        << "limit:" << std::endl
        << "  -t <thresh>  -- distance threshold" << std::endl;
    exit(EXIT_FAILURE);
}

Index* loadIndex(const std::string &algo, const std::string &indexfn) {
    Index *index;
    if (algo == "pairwise" || algo == "static" || algo == "limit") {
        index = new PlainIndex();
    } else if (algo == "lucene++") {
        // TODO xhochy
    } else {
        return nullptr;
    }

    // Load index
    index->load(indexfn.c_str());

    return index;
}


void loadQueries(std::vector<std::vector<uint32_t>> &queries, const std::string &queryfn) {
    std::ifstream queryfile(queryfn);
    for (std::string line; std::getline(queryfile, line);) {
        std::vector<uint32_t> query;
        utf8to32(line.data(), query);
        queries.push_back(query);
    }
    queryfile.close();
}


Benchmark* createBenchmark(const std::string &algo, const uint32_t threshold) {
    Benchmark *benchmark(nullptr);
    if (algo == "pairwise") {
        std::cout << "Creating pairwise benchmark for plain pairwise Levenshtein" << std::endl;
        benchmark = new PairwiseBenchmark(levenshtein);
    } else if (algo == "static") {
        std::cout << "Creating pairwise benchmark for pairwise Levenshtein with statically allocated arrays" << std::endl;
        benchmark = new PairwiseBenchmark(levenshteinStatic);
    } else if (algo == "limit") {
        std::cout << "Creating pairwise benchmark for threshold-limited pairwise Levenshtein (threshold = " << threshold << ")" << std::endl;
        benchmark = new PairwiseBenchmark([=](const std::vector<uint32_t> &s, const std::vector<uint32_t> &t) {
            return levenshteinLimit(s, t, threshold);
        });
    } else if (algo == "lucene++" || algo == "lucenepp") {
        // TODO xhochy
    }
    return benchmark;
}

int main(int argc, char** argv) {
    ArgParser args(argc, argv);

    if (!args.isSet("i")) usage(argv);

    const std::string algo = args.get<std::string>("a", "pairwise"),
        indexfn = args.get<std::string>("i"),
        queryfn = args.get<std::string>("q", "queries.txt");
    const uint32_t threshold = args.get<uint32_t>("t", 3);

    Timer timer;

    // load queries
    std::vector<std::vector<uint32_t>> queries;
    loadQueries(queries, queryfn);
    std::cout << queries.size() << " queries loaded in " << timer.getAndReset() << "s" << std::endl;

    // load index
    Index* index = loadIndex(algo, indexfn);
    if (index == nullptr) {
        usage(argv);
    }
    std::cout << "Loaded index in " << timer.getAndReset() << "s" << std::endl;

    // create benchmark object
    Benchmark *benchmark = createBenchmark(algo, threshold);
    benchmark->setIndex(index);

    std::cout << "Querying index with " << queries.size() << " queries..." << std::endl;
    timer.reset();

    // Query!
    for (const std::vector<uint32_t> &query : queries) {
        benchmark->query(query);
    }

    double queryDuration = timer.get();
    std::cout << "Queried index in " << queryDuration << "s" << std::endl;

    std::cout << "RESULT"
        << " algo="  << algo
        << " index=" << indexfn
        << " queries=" << queryfn
        << " numQueries=" << queries.size()
        << " queryDuration=" << queryDuration
        << " msPerQuery=" << queryDuration * 1000 / queries.size()
        // TODO max/min query time
        << std::endl;
}
