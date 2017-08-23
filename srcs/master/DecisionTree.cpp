// C/C++ File

// Author:   Alexandre Tea <alexandre.qtea@gmail.com>
// File:     /Users/alexandretea/Work/ddti/srcs/DecisionTree.cpp
// Purpose:  DecisionTree implementation
// Created:  2017-07-27 18:23:20
// Modified: 2017-08-23 23:14:06

#include <iomanip>
#include "DecisionTree.hpp"

namespace ddti {

DecisionTree::DecisionTree()
    : _is_leaf(false)
{
}

DecisionTree::DecisionTree(unsigned int index, int split_value, size_t nb_insts,
                           bool is_leaf, size_t misses)
    : _is_leaf(is_leaf), _index(index), _nb_instances(nb_insts),
      _split_value(split_value), _misses(misses)
{
}

DecisionTree::~DecisionTree()
{
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
DecisionTree::nb_instances() const
{
    return _nb_instances;
}

size_t
DecisionTree::misses() const
{
    return _misses;
}

DecisionTree::UIntPair
DecisionTree::size() const
{
    UIntPair    size = std::make_pair(1, 0);

    if (_is_leaf)
        return std::make_pair(1, 1);
    for (auto& p: _children) {
        UIntPair child_size = p.second->size();

        size.first += child_size.first;
        size.second += child_size.second;
    }
    return size;
}

DecisionTree*
DecisionTree::child(size_t split) const
{
    return _children.at(split).get();
}

DecisionTree::UniqueDtMap::const_iterator
DecisionTree::begin() const
{
    return _children.begin();
}

DecisionTree::UniqueDtMap::const_iterator
DecisionTree::end() const
{
    return _children.end();
}

void
DecisionTree::add_child(std::unique_ptr<DecisionTree> node)
{
    _children[node->split()] = std::move(node);
}

// preorder print
void
DecisionTree::output_txt(Dataset<double> const& dataset,
                         std::ostream& os, unsigned int level) const
{
    for (auto& pair: _children) {
        DecisionTree*   child = pair.second.get();
        // indent
        for (unsigned int i = 0; i < level; ++i) {
            os << "|   ";
        }
        // print splits
        os << dataset.attribute_name(_index)
           << " = " << dataset.mapping(_index, child->_split_value);
        if (child->is_leaf()) {
            os << ": " << dataset.mapping(dataset.labelsdim(), child->label())
               << " (" << child->_nb_instances << ".0";
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
