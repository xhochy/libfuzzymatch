/**
 * Build a (fuzzy) index with Lucene++ give an input file containing a value on each line
 */

#include <cstdlib>
#include <iostream>
#include <fstream>

#include <lucene++/Lucene.h>
#include <lucene++/Document.h>
#include <lucene++/Field.h>
#include <lucene++/FSDirectory.h>
#include <lucene++/IndexWriter.h>
#include <lucene++/SimpleAnalyzer.h>

#include "../../src/utf8/utf8.h"
#include "config.h"

int main(int argc, char *argv[]) {
    if (argc != 2) {
        std::cerr << "You must specify exactly a single input filename." << std::endl;
        exit(EXIT_FAILURE);
    }

    // Initialise classes for index creation.
    auto analyzer = Lucene::newLucene<Lucene::SimpleAnalyzer>();
    auto directory = Lucene::FSDirectory::open(L"index.lucene");
    auto writer = Lucene::newLucene<Lucene::IndexWriter>(directory, analyzer, true, Lucene::IndexWriter::MaxFieldLengthUNLIMITED);

    std::ifstream file(argv[argc - 1]);
    for (std::string line; std::getline(file, line);) {
        std::vector<wchar_t> str(utf8::distance(line.begin(), line.end()));
        // By just looking at this code, you may realise that wchar_t is not the best option possibly.
#if SIZEOF_WCHAR_T == 2
            utf8::utf8to16(line.begin(), line.end(), str.data());
#elif SIZEOF_WCHAR_T == 4
            utf8::utf8to32(line.begin(), line.end(), str.data());
#else
#error Unsupported size of wchar_t
#endif
        Lucene::DocumentPtr doc = Lucene::newLucene<Lucene::Document>();
        doc->add(Lucene::newLucene<Lucene::Field>(L"text", std::wstring(str.begin(), str.end()), Lucene::Field::STORE_YES, Lucene::Field::INDEX_NOT_ANALYZED_NO_NORMS));
        writer->addDocument(doc);
    }

    // Close the index again.
    writer->optimize();
    writer->close();
    writer.reset();
}
