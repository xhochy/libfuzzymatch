#include <cstdint>
#include <vector>

/**
 * Levenshtein distance for UTF-32 "strings".
 * @param s First string input for levenshtein distance
 * @param t Second string input for levenshtein distance
 * @return The levenshtein distance between the two inputs.
 */
uint32_t levenshtein(const std::vector<uint32_t> &, const std::vector<uint32_t> &);

/**
 * Levenshtein distance for UTF-32 "strings" using static storage for less allocations.
 *
 * The two temporary used arrays are statically allocated internally, i.e. are
 * always expanded to the maximum size of the second input and never scaled down.
 * Although this offers better performance, you need to be aware that large
 * inputs will increase the memory usage for the whole lifespan of the procress.
 *
 * @param s First string input for levenshtein distance
 * @param t Second string input for levenshtein distance
 * @return The levenshtein distance between the two inputs.
 */
uint32_t levenshteinStatic(const std::vector<uint32_t> &s, const std::vector<uint32_t> &t);

uint32_t levenshteinLimit(const std::vector<uint32_t> &, const std::vector<uint32_t> &, const uint32_t);
