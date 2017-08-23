// C/C++ File

// Author:   Alexandre Tea <alexandre.qtea@gmail.com>
// File:     /Users/alexandretea/Work/ddti/srcs/master/Classifier.cpp
// Purpose:  TODO (a one-line explanation)
// Created:  2017-08-23 00:33:01
// Modified: 2017-08-23 01:03:09

#include "Classifier.hpp"

namespace ddti {

Classifier::Classifier(DecisionTree* dt)
    : _root(dt)
{
}
Classifier::~Classifier()
{
}

double
Classifier::test(Dataset<double> const& dataset) const
{
    size_t  nb_success = 0;

    // entries as columns
    dataset.matrix().each_col(
        [this, &nb_success, &dataset](arma::Col<double> const& col) {
            if (classify(col) == col(dataset.labelsdim()))
                ++nb_success;
        });
    return static_cast<double>(nb_success) / dataset.size();
}

size_t
Classifier::classify(arma::Col<double> const& instance) const
{
    DecisionTree*   node = _root;

    while (not node->is_leaf()) {
        node = node->child(instance(node->attribute()));
    }
    return node->label();
}

}   // end of namespace ddti
