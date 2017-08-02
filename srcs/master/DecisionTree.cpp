// C/C++ File

// Author:   Alexandre Tea <alexandre.qtea@gmail.com>
// File:     /Users/alexandretea/Work/ddti/srcs/DecisionTree.cpp
// Purpose:  TODO (a one-line explanation)
// Created:  2017-07-27 18:23:20
// Modified: 2017-07-28 15:18:08

#include "DecisionTree.hpp"

namespace ddti {

DecisionTree::DecisionTree()
    : _split_dimension(-1), _children()
{
}

DecisionTree::~DecisionTree()
{
    for (auto& child: _children) {
        delete child;
    }
}

DecisionTree::DecisionTree(DecisionTree const& other)
    : _split_dimension(other._split_dimension), _children(other._children)
{
}

DecisionTree&
DecisionTree::operator=(DecisionTree const& other)
{
    if (this != &other) {
        _split_dimension = other._split_dimension;
        _children = other._children;
    }
    return *this;
}

}   // end of namespace ddti
