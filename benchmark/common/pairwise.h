#include <cstdint>
#include <functional>
#include <vector>

void benchLevenshtein(std::vector<std::string> queries,
                      std::vector<std::vector<uint32_t>> index,
                      std::function<uint32_t(const std::vector<uint32_t> &, const std::vector<uint32_t> &)> lv);

