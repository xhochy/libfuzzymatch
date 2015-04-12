/**
 * Query a (fuzzy) index with Lucene++ given an input file containing a value
 * on each line and a pre-built index.
 */

#include <cstdlib>
#include <iostream>

#include <lucene++/Lucene.h>
#include <lucene++/FSDirectory.h>
#include <lucene++/FuzzyQuery.h>
#include <lucene++/IndexReader.h>
#include <lucene++/IndexSearcher.h>
#include <lucene++/Term.h>
#include <lucene++/TopDocs.h>
#include <lucene++/TopScoreDocCollector.h>

#include "../../src/utf8/utf8.h"
#include "../common/timing.h"
#include "config.h"

std::wstring towstring(const std::string &str) {
    std::vector<wchar_t> wstr(utf8::distance(str.begin(), str.end()));
    // By just looking at this code, you may realise that wchar_t is not the best option possibly.
#if SIZEOF_WCHAR_T == 2
        utf8::utf8to16(str.begin(), str.end(), wstr.data());
#elif SIZEOF_WCHAR_T == 4
        utf8::utf8to32(str.begin(), str.end(), wstr.data());
#else
#error Unsupported size of wchar_t
#endif
    return std::wstring(wstr.begin(), wstr.end());
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        std::cerr << "Usage: \n\t query-index <index> <query-file>" << std::endl;
        exit(EXIT_FAILURE);
    }

    auto directory = Lucene::FSDirectory::open(towstring(std::string(argv[argc - 2])));
    auto reader = Lucene::IndexReader::open(directory);
    auto searcher = Lucene::newLucene<Lucene::IndexSearcher>(reader);

    std::vector<std::string> queries;
    std::ifstream file(argv[argc - 1]);
    for (std::string line; std::getline(file, line);) {
        queries.push_back(line);
    }
    file.close();

    double startTime = getCurrentTime();
    for (const std::string &_query: queries) {
        std::wstring queryString = towstring(_query);
        auto term = Lucene::newLucene<Lucene::Term>(L"text", queryString);
        auto query = Lucene::newLucene<Lucene::FuzzyQuery>(term);
        auto collector = Lucene::TopScoreDocCollector::create(20, true);
        searcher->search(query, collector);
        Lucene::Collection<Lucene::ScoreDocPtr> hits = collector->topDocs()->scoreDocs;
    }
    double endTime = getCurrentTime();
    std::cout << "Took " << (endTime - startTime) << "s to execute the queries." << std::endl;
}
