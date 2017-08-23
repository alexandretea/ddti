// C/C++ File

// Author:   Alexandre Tea <alexandre.qtea@gmail.com>
// File:     /Users/alexandretea/Work/ddti/srcs/master/Classifier.hpp
// Purpose:  TODO (a one-line explanation)
// Created:  2017-08-23 00:25:45
// Modified: 2017-08-23 21:32:07

#ifndef CLASSIFIER_H
#define CLASSIFIER_H

#include <memory>
#include <armadillo>
#include "DecisionTree.hpp"

namespace ddti {

// Note: The Classifier object takes ownership of the decision tree
class Classifier
{
    public:
        Classifier(std::unique_ptr<DecisionTree> dt);
        ~Classifier();

        Classifier(Classifier const& o) = delete;
        Classifier& operator=(Classifier const& o) = delete;

    public:
        double      test(Dataset<double> const& dataset) const;
        size_t      classify(arma::Col<double> const& instance) const;
        // returns the predictive accuracy of the model
        void        dump_model(Dataset<double> const& dataset,
                               std::ostream& os = std::cout) const;

    private:
        std::unique_ptr<DecisionTree>   _root;
};

}   // end of namespace ddti

#endif /* end of include guard: CLASSIFIER_H */
