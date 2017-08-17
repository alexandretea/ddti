// C/C++ File

// Author:   Alexandre Tea <alexandre.qtea@gmail.com>
// File:     /Users/alexandretea/Work/ddti/srcs/DecisionTree.hpp
// Purpose:  TODO (a one-line explanation)
// Created:  2017-07-27 17:49:15
// Modified: 2017-08-17 17:13:44

#ifndef DECISIONTREE_H
#define DECISIONTREE_H

#include <vector>
#include "Dataset.hpp"

namespace ddti {

// TODO better design to handle splits/leaves
class DecisionTree
{
    public:
        DecisionTree();
        DecisionTree(unsigned int index, int split_value, bool is_leaf = false);
        virtual ~DecisionTree();

        DecisionTree(DecisionTree const& other);
        DecisionTree&   operator=(DecisionTree const& other);
        // TODO need to keep track of subset of data

    public:
        bool            is_leaf() const;
        unsigned int    index() const;
        unsigned int    feature() const;
        unsigned int    label() const;
        int             split() const;  // returns -1 if node is root

        void            add_child(DecisionTree* node);
        void            print(Dataset<double> const& mappings,
                              unsigned int level = 0) const;

        std::vector<DecisionTree*>::const_iterator  begin() const;
        std::vector<DecisionTree*>::const_iterator  end() const;

    protected:
        bool                        _is_leaf;
        unsigned int                _index;
        // index represents a class if the node is a leaf; a feature otherwise
        int                         _split_value;   // -1 if node is the root
        std::vector<DecisionTree*>  _children;
};

}

#endif /* end of include guard: DECISIONTREE_H */
