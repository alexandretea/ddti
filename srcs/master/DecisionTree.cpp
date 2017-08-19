// C/C++ File

// Author:   Alexandre Tea <alexandre.qtea@gmail.com>
// File:     /Users/alexandretea/Work/ddti/srcs/DecisionTree.cpp
// Purpose:  TODO (a one-line explanation)
// Created:  2017-07-27 18:23:20
// Modified: 2017-08-19 17:38:16

#include "DecisionTree.hpp"

namespace ddti {

DecisionTree::DecisionTree()
    : _is_leaf(false)
{
}

DecisionTree::DecisionTree(unsigned int index, int split_value, size_t size,
                           bool is_leaf)
    : _is_leaf(is_leaf), _index(index), _size(size), _split_value(split_value)
{
}

DecisionTree::~DecisionTree()
{
    for (auto& child: _children) {
        delete child;
    }
}

DecisionTree::DecisionTree(DecisionTree const& o)
    : _is_leaf(o._is_leaf), _index(o._index), _size(o._size),
      _split_value(o._split_value), _children(o._children)
{
}

DecisionTree&
DecisionTree::operator=(DecisionTree const& o)
{
    if (this != &o) {
        _is_leaf = o._is_leaf;
        _size = o._size;
        _split_value = o._split_value;
        _index = o._index;
        _children = o._children;
    }
    return *this;
}

bool
DecisionTree::is_leaf() const
{
    return _is_leaf;
}

unsigned int
DecisionTree::index() const
{
    return _index;
}

unsigned int
DecisionTree::attribute() const
{
    return index();
}

unsigned int
DecisionTree::label() const
{
    return index();
}

int
DecisionTree::split() const
{
    return _split_value;
}

size_t
DecisionTree::size() const
{
    return _size;
}

std::vector<DecisionTree*>::const_iterator
DecisionTree::begin() const
{
    return _children.begin();
}

std::vector<DecisionTree*>::const_iterator
DecisionTree::end() const
{
    return _children.end();
}

void
DecisionTree::add_child(DecisionTree* node)
{
    _children.push_back(node);
}

// preorder print
void
DecisionTree::print(Dataset<double> const& dataset,
                    unsigned int level) const
{
    for (auto& child: _children) {
        // indent
        for (unsigned int i = 0; i < level; ++i) {
            std::cout << "|   ";
        }

        // print splits
        std::cout << dataset.attribute_name(_index)
                  << " = " << dataset.mapping(_index, child->_split_value);
        if (child->is_leaf()) {
            std::cout << ": "
                      << dataset.mapping(dataset.labelsdim(), child->label())
                      << " (" << child->_size << ")";
        }
        std::cout << std::endl;

        // print childs
        child->print(dataset, level + 1);
    }
}

}   // end of namespace ddti
