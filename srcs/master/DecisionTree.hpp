// C/C++ File

// Author:   Alexandre Tea <alexandre.qtea@gmail.com>
// File:     /Users/alexandretea/Work/ddti/srcs/DecisionTree.hpp
// Purpose:  TODO (a one-line explanation)
// Created:  2017-07-27 17:49:15
// Modified: 2017-08-23 17:13:48

#ifndef DECISIONTREE_H
#define DECISIONTREE_H

#include <map>
#include "Dataset.hpp"

namespace ddti {

// TODO better design to handle splits/leaves
class DecisionTree
{
    public:
        DecisionTree();
        DecisionTree(unsigned int index, int split_value, size_t size,
                     bool is_leaf = false, size_t misses = 0);
        virtual ~DecisionTree();

        DecisionTree(DecisionTree const& other);
        DecisionTree&   operator=(DecisionTree const& other);

    public:
        bool            is_leaf() const;
        unsigned int    index() const;
        unsigned int    attribute() const;
        unsigned int    label() const;
        int             split() const;  // returns -1 if node is root
        size_t          size() const;
        size_t          misses() const;
        DecisionTree*   child(size_t split) const;

        void            add_child(DecisionTree* node);
        void            output_txt(Dataset<double> const& dataset,
                                   std::ostream& os = std::cout,
                                   unsigned int level = 0) const;

        std::map<size_t, DecisionTree*>::const_iterator  begin() const;
        std::map<size_t, DecisionTree*>::const_iterator  end() const;

    protected:
        bool                        _is_leaf;
        unsigned int                _index;
        // index represents a class if the node is a leaf; a feature otherwise
        size_t                      _size;          // nb instances
        int                         _split_value;   // -1 if node is the root
        size_t                      _misses;
        // number of instances that doesn't match leaf class
        std::map<size_t, DecisionTree*> _children;
};

}

#endif /* end of include guard: DECISIONTREE_H */
