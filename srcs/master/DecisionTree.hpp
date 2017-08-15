// C/C++ File

// Author:   Alexandre Tea <alexandre.qtea@gmail.com>
// File:     /Users/alexandretea/Work/ddti/srcs/DecisionTree.hpp
// Purpose:  TODO (a one-line explanation)
// Created:  2017-07-27 17:49:15
// Modified: 2017-08-15 13:54:38

#ifndef DECISIONTREE_H
#define DECISIONTREE_H

#include <vector>

namespace ddti {

// TODO better design to handle splits/leaves
class DecisionTree
{
    public:
        DecisionTree();
        DecisionTree(size_t index, bool is_leaf = false);
        virtual ~DecisionTree();

        DecisionTree(DecisionTree const& other);
        DecisionTree&   operator=(DecisionTree const& other);
        // TODO need to keep track of subset of data

        void            add_child(DecisionTree* node);

    protected:
        bool                        _is_leaf;
        ssize_t                     _index;
        // -1 if not initialised, class index if leaf else split dimension
        std::vector<DecisionTree*>  _children;
        // pointers to avoid copies when pruning
};

}


#endif /* end of include guard: DECISIONTREE_H */
