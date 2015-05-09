#include "pairwise.h"

#include <queue>

#include <libfuzzymatch/util.h>

void benchLevenshtein(std::vector<std::string> queries,
                      std::vector<std::vector<uint32_t>> index,
                      std::function<uint32_t(const std::vector<uint32_t> &, const std::vector<uint32_t> &)> lv) {
    for (const std::string &_query: queries) {
        std::vector<uint32_t> query;
        utf8to32(_query.data(), query);
        std::priority_queue<std::tuple<uint32_t, size_t>> pq;
        for (size_t idx = 0; idx < index.size(); idx++) {
            uint32_t distance = lv(query, index[idx]);
            if (pq.size() < 20) {
                // Less than 20 elements, we can just push it into the queue.
                pq.push(std::tuple<uint32_t, size_t>(distance, idx));
            } else {
                // There are 20 element in the PriorityQueue. Check if the
                // element should go into the queue but we should first remove
                // the weakest element so that the queue is a bit smaller.
                if (std::get<0>(pq.top()) > distance) {
                    pq.pop();
                    pq.push(std::tuple<uint32_t, size_t>(distance, idx));
                }
            }
        }
    }
}


void PairwiseBenchmark::setIndex(const Index *const indexptr) {
    index = (PlainIndex*) indexptr;
}

void PairwiseBenchmark::query(const std::vector<uint32_t> &q, const size_t num_results = 20) {
    std::priority_queue<std::tuple<uint32_t, size_t>> pq;
    const size_t indexSize = index->size();
    for (size_t idx = 0; idx < indexSize; idx++) {
        uint32_t distance = lv(q, index->get(idx));
        if (pq.size() < num_results) {
            // Less than num_results elements, we can just push it into the queue.
            pq.push(std::tuple<uint32_t, size_t>(distance, idx));
        } else {
            // There are num_results element in the PriorityQueue. Check if the
            // element should go into the queue but we should first remove
            // the weakest element so that the queue is a bit smaller.
            if (std::get<0>(pq.top()) > distance) {
                pq.pop();
                pq.push(std::tuple<uint32_t, size_t>(distance, idx));
            }
        }
    }
}
