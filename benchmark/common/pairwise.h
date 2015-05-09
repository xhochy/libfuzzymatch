#pragma once

#include <cstdint>
#include <functional>
#include <vector>

#include "benchmark.h"
#include "index.h"
#include "plain_index.h"

void benchLevenshtein(std::vector<std::string> queries,
                      std::vector<std::vector<uint32_t>> index,
                      std::function<uint32_t(const std::vector<uint32_t> &, const std::vector<uint32_t> &)> lv);


class PairwiseBenchmark : public Benchmark {
public:
    PairwiseBenchmark(std::function<uint32_t(const std::vector<uint32_t> &, const std::vector<uint32_t> &)> lv) : Benchmark(), lv(lv) {}
    ~PairwiseBenchmark() {}

    void setIndex(const Index *const);
    void query(const std::vector<uint32_t> &, const size_t);

protected:
    PlainIndex *index;
    std::function<uint32_t(const std::vector<uint32_t> &, const std::vector<uint32_t> &)> lv;
};
