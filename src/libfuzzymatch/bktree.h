#pragma once
#ifndef BKTREE_H
#define BKTREE_H

#include <cassert>
#include <cstdint>
#include <cstdlib>
#include <functional>
#include <vector>

template<typename score_type = uint32_t, typename input_type = uint32_t>
class BKTree
{
public:
    using score_func_type = std::function<score_type(const std::vector<input_type> &, const std::vector<input_type> &)>;

    BKTree(size_t n, score_func_type score_func) :
        m_tree(n), m_score_func(score_func) { }

    void insert(const std::vector<input_type> &input) {
        assert(m_next_free < m_tree.size());
        if (m_next_free == 0) {
            m_tree[0].first = input;
            m_next_free++;
        } else {
            recursive_insert(input, 0);
        }
    }

protected:
    void recursive_insert(const std::vector<input_type> &input, size_t current_node) {
        score_type score = m_score_func(m_tree[current_node].first, input);
        if (score == 0) {
            // Duplicate addition.
            // no-op
        } else if (m_tree[current_node].second.size() < score) {
            m_tree[current_node].second.resize(score, -1);
            // Found a fitting location. Insert here.
            m_tree[current_node].second[score - 1] = m_next_free;
            m_tree[m_next_free].first = input;
            m_next_free++;
        } else if (m_tree[current_node].second[score - 1] == -1) {
            // Found a fitting location. Insert here.
            m_tree[current_node].second[score - 1] = m_next_free;
            m_tree[m_next_free].first = input;
            m_next_free++;
        } else {
            // Position (score - 1) is already occupied, recurse!
            recursive_insert(input, m_tree[current_node].second[score - 1]);
        }
    }

private:
    size_t m_next_free = 0;
    score_func_type m_score_func;
    std::vector<std::pair<std::vector<input_type>, std::vector<int64_t>>> m_tree;
};

#endif // BKTREE_H
