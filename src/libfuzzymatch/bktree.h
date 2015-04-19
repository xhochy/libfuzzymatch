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

    /**
     * Insert a new element in the tree.
     *
     * This will compute the scoring function several times to find the correct
     * place in the tree. A copy of the input vector will be stored in the tree.
     *
     * @param input The new element.
     */
    void insert(const std::vector<input_type> &input) {
        // At the moment we only support adding as much elements as were
        // specified on construction. We might easily lift this restriction
        // but this will come at the moment with the drawback of copying the
        // entire tree which is a very expensive operation.
        assert(m_next_free < m_tree.size());

        if (m_next_free == 0) {
            m_tree[0].first = input;
            m_next_free++;
        } else {
            recursive_insert(input, 0);
        }
    }

protected:
    /**
     * Recurse through the tree to find the correct position to insert the new element.
     *
     * To insert an element into the tree, we compute the distance of the new
     * element to the currently traversed node. If they have a distance of 0
     * the new element is considered as equal and will not be inserted into the tree.
     * For a distance >0, we will look at the outgoing edge labelled with the given
     * score. If there is no such edge (either the list of edges is smaller
     * than the score or at the position we have a value of -1) then we will insert
     * a new edge and connect it to the new node. In the other case where the
     * edge matching the distance score already exists, we will recursively call
     * the `recursive_insert` function with this edge and continue the algorithm.
     *
     * @param input The new element.
     * @param current_node The current node which is checked as a candidate for being the direct parent.
     */
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
    /**
     * The index of the next available position of a node in the internal tree
     * representation `m_tree`.
     */
    size_t m_next_free = 0;

    /**
     * Function that is called to compute the distance between two elements. As
     * a requirement for the BK-Tree's correct function, this needs to be a
     * metric distance function.
     */
    score_func_type m_score_func;

    /**
     * Internal tree representation.
     *
     * Each entry is the pair of the value at this node (e.g. a UTF-32 string)
     * and the list of outgoing edges. The list of edges is implemented as a
     * not fully "used" array. If an edges exists for a score D, the entry at
     * position [D - 1] points to the index in m_tree where the target node
     * resides. Empty entries in this array are marked with a value of -1
     * (as this is not a valid value for an array index in C++).
     *
     * For the sake of performance, the outer std::vector should be initialised
     * on the instantiation of the BKTree class once and not resized during
     * building as this would invoke a very costly copy operation of the whole
     * tree.
     */
    std::vector<std::pair<std::vector<input_type>, std::vector<int64_t>>> m_tree;
};

#endif // BKTREE_H
