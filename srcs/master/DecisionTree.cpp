// C/C++ File

// Author:   Alexandre Tea <alexandre.qtea@gmail.com>
// File:     /Users/alexandretea/Work/ddti/srcs/DecisionTree.cpp
// Purpose:  TODO (a one-line explanation)
// Created:  2017-07-27 18:23:20
// Modified: 2017-08-15 13:55:07

#include "DecisionTree.hpp"

namespace ddti {

DecisionTree::DecisionTree()
    : _is_leaf(false), _index(-1), _children()
{
}

DecisionTree::DecisionTree(size_t index, bool is_leaf)
    : _is_leaf(is_leaf), _index(index), _children()
{
}

DecisionTree::~DecisionTree()
{
    for (auto& child: _children) {
        delete child;
    }
}

DecisionTree::DecisionTree(DecisionTree const& o)
    : _is_leaf(o._is_leaf), _index(o._index), _children(o._children)
{
}

DecisionTree&
DecisionTree::operator=(DecisionTree const& o)
{
    if (this != &o) {
        _is_leaf = o._is_leaf;
        _index = o._index;
        _children = o._children;
    }
    return *this;
}

void
DecisionTree::add_child(DecisionTree* node)
{
    _children.push_back(node);
}

}   // end of namespace ddti
