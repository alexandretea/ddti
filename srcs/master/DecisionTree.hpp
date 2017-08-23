// C/C++ File

// Author:   Alexandre Tea <alexandre.qtea@gmail.com>
// File:     /Users/alexandretea/Work/ddti/srcs/DecisionTree.hpp
// Purpose:  DecisionTree is a class that represents a node/subtree of a DT
// Created:  2017-07-27 17:49:15
// Modified: 2017-08-23 23:14:33

#ifndef DECISIONTREE_H
#define DECISIONTREE_H

#include <memory>
#include <map>
#include "Dataset.hpp"

namespace ddti {

// Note: A decision tree has ownership of his children (via unique_ptrs)
class DecisionTree
{
    public:
        using UniqueDtMap = std::map<size_t, std::unique_ptr<DecisionTree>>;
        using UIntPair = std::pair<unsigned int, unsigned int>;

    public:
        DecisionTree();
        DecisionTree(unsigned int index, int split_value, size_t nb_insts,
                     bool is_leaf = false, size_t misses = 0);
        virtual ~DecisionTree();

        DecisionTree(DecisionTree const& other) = delete;
        DecisionTree&   operator=(DecisionTree const& other) = delete;

    public:
        bool            is_leaf() const;
        unsigned int    index() const;
        unsigned int    attribute() const;
        unsigned int    label() const;
        int             split() const;  // returns -1 if node is root
        size_t          nb_instances() const;
        size_t          misses() const;
        DecisionTree*   child(size_t split) const;
        UIntPair        size() const;
        // returns <nb nodes, nb leaves>
        // NOTE: traverse tree and count nodes without caching, bare in mind
        // that using it will alter the performance of your code

        void            add_child(std::unique_ptr<DecisionTree> node);
        void            output_txt(Dataset<double> const& dataset,
                                   std::ostream& os = std::cout,
                                   unsigned int level = 0) const;

        UniqueDtMap::const_iterator  begin() const;
        UniqueDtMap::const_iterator  end() const;

    protected:
        bool            _is_leaf;
        unsigned int    _index;
        // index represents a class if the node is a leaf; a feature otherwise
        size_t          _nb_instances;  // nb instances
        int             _split_value;   // -1 if node is the root
        size_t          _misses;
        // number of instances that doesn't match leaf class
        UniqueDtMap     _children;
};

}

#endif /* end of include guard: DECISIONTREE_H */
