#pragma once

#include <cstdint>
#include <vector>

/**
 * Levenshtein distance for UTF-32 "strings" with maximum distance parameter.
 * @param s First string input for levenshtein distance
 * @param t Second string input for levenshtein distance
 * @param threshold Maximum Levenshtein distance between s and t before aborting
 * @return The Levenshtein distance between the two inputs, or -1 if it exceeds the threshold.
 */
uint32_t levenshteinLimit(const std::vector<uint32_t> &, const std::vector<uint32_t> &, const uint32_t);

/**
 * Limited Levenshtein distance for UTF-32 "strings" using static storage for less allocations.
 *
 * The two temporary used arrays are statically allocated internally, i.e. are
 * always expanded to the maximum size of the second input and never scaled down.
 * Although this offers better performance, you need to be aware that large
 * inputs will increase the memory usage for the whole lifespan of the procress.
 *
 * @param s First string input for levenshtein distance
 * @param t Second string input for levenshtein distance
 * @param threshold Maximum Levenshtein distance between s and t before aborting
 * @return The Levenshtein distance between the two inputs, or -1 if it exceeds the threshold.
 */
uint32_t levenshteinLimitStatic(const std::vector<uint32_t> &, const std::vector<uint32_t> &, const uint32_t);
