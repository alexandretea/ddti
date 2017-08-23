// C/C++ File

// Author:   Alexandre Tea <alexandre.qtea@gmail.com>
// File:     /Users/alexandretea/Work/ddti/srcs/master/Classifier.hpp
// Purpose:  TODO (a one-line explanation)
// Created:  2017-08-23 00:25:45
// Modified: 2017-08-23 01:04:11

#ifndef CLASSIFIER_H
#define CLASSIFIER_H

#include <armadillo>
#include "DecisionTree.hpp"

namespace ddti {

class Classifier
{
    public:
        Classifier(DecisionTree* dt);
        ~Classifier();

        Classifier(Classifier const& o) = delete;
        Classifier& operator=(Classifier const& o) = delete;

    public:
        double      test(Dataset<double> const& dataset) const;
        // returns the predictive accuracy of the model
        size_t      classify(arma::Col<double> const& instance) const;

    private:
        DecisionTree*   _root;
};

}   // end of namespace ddti

#endif /* end of include guard: CLASSIFIER_H */
