// C/C++ File

// Author:   Alexandre Tea <alexandre.qtea@gmail.com>
// File:     /Users/alexandretea/Work/ddti/srcs/DecisionTree.cpp
// Purpose:  TODO (a one-line explanation)
// Created:  2017-07-27 18:23:20
// Modified: 2017-08-23 17:13:50

#include <iomanip>
#include "DecisionTree.hpp"

namespace ddti {

DecisionTree::DecisionTree()
    : _is_leaf(false)
{
}

DecisionTree::DecisionTree(unsigned int index, int split_value, size_t size,
                           bool is_leaf, size_t misses)
    : _is_leaf(is_leaf), _index(index), _size(size), _split_value(split_value),
      _misses(misses)
{
}

DecisionTree::~DecisionTree()
{
    for (auto& child: _children) {
        delete child.second;
    }
}

DecisionTree::DecisionTree(DecisionTree const& o)
    : _is_leaf(o._is_leaf), _index(o._index), _size(o._size),
      _split_value(o._split_value), _misses(o._misses), _children(o._children)
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
        _misses = o._misses;
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

size_t
DecisionTree::misses() const
{
    return _misses;
}

DecisionTree*
DecisionTree::child(size_t split) const
{
    return _children.at(split);
}

std::map<size_t, DecisionTree*>::const_iterator
DecisionTree::begin() const
{
    return _children.begin();
}

std::map<size_t, DecisionTree*>::const_iterator
DecisionTree::end() const
{
    return _children.end();
}

void
DecisionTree::add_child(DecisionTree* node)
{
    _children[node->split()] = node;
}

// preorder print
void
DecisionTree::output_txt(Dataset<double> const& dataset,
                         std::ostream& os, unsigned int level) const
{
    for (auto& pair: _children) {
        DecisionTree*   child = pair.second;
        // indent
        for (unsigned int i = 0; i < level; ++i) {
            os << "|   ";
        }
        // print splits
        os << dataset.attribute_name(_index)
           << " = " << dataset.mapping(_index, child->_split_value);
        if (child->is_leaf()) {
            os << ": " << dataset.mapping(dataset.labelsdim(), child->label())
               << " (" << child->_size << ".0";
            if (child->_misses > 0)
                os << "/" << child->_misses << ".0";
            os << ")";
            // NOTE: the appending of ".0" after the integers is to keep the
            // same output style of Weka's J48
        }
        os << std::endl;

        // print childs
        child->output_txt(dataset, os, level + 1);
    }
}

}   // end of namespace ddti
