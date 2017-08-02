// C/C++ File

// Author:   Alexandre Tea <alexandre.qtea@gmail.com>
// File:     /Users/alexandretea/Work/ddti/srcs/DecisionTree.hpp
// Purpose:  TODO (a one-line explanation)
// Created:  2017-07-27 17:49:15
// Modified: 2017-07-27 18:23:14

#ifndef DECISIONTREE_H
#define DECISIONTREE_H

#include <vector>

namespace ddti {

class DecisionTree
{
    public:
        DecisionTree();
        virtual ~DecisionTree();

        DecisionTree(DecisionTree const& other);
        DecisionTree&   operator=(DecisionTree const& other);
        // TODO need to keep track of subset of data

    protected:
        size_t                      _split_dimension;
        std::vector<DecisionTree*>  _children;
        // pointers to avoid copies when pruning
};

}


#endif /* end of include guard: DECISIONTREE_H */
