// C/C++ File

// Author:   Alexandre Tea <alexandre.qtea@gmail.com>
// File:     /Users/alexandretea/Work/ddti/srcs/master/InductionC4_5.hpp
// Purpose:  TODO (a one-line explanation)
// Created:  2017-07-28 16:14:44
// Modified: 2017-08-07 14:46:59

#ifndef INDUCTIONC4_5_H
#define INDUCTIONC4_5_H

#include <mlpack/core.hpp>  // TODO change with armadillo include
#include "MasterNode.hpp"
#include "DecisionTree.hpp"
#include "MPIDatatypeManager.hpp"
#include "TaskC4_5.hpp"

namespace ddti {
namespace induction {

// TODO needs to get parameters
class C4_5
{
    public:
        C4_5(size_t nb_slaves, utils::mpi::Communicator const& comm);
        virtual ~C4_5();

        C4_5(C4_5 const& other) = delete;
        C4_5&           operator=(C4_5 const& other) = delete;

    public:
        DecisionTree*   operator()(arma::mat const& data,
                                   utils::mlpack::DatasetMappings const& maps,
                                   size_t labels_dim);

    protected:
        // TODO keep track of attributes? datasetinfo?
        // TODO check signatures of the functions below
        DecisionTree*   rec_train_node(arma::subview<double> const& data,
                                       std::vector<size_t> const& attrs);
        arma::mat       scatter_matrix(arma::subview<double> const& data);

        static size_t
        find_majority_class(arma::subview_row<double> const& data,
                            bool* is_only_class = nullptr);

    protected:
        size_t                          _nb_slaves;
        utils::mpi::Communicator const& _communicator;
        task::C4_5                      _tasks;
        utils::mpi::datatype::Manager   _mpi_types;

        // used during training:
        ssize_t                         _labels_dim;
        utils::mlpack::DatasetMappings  _mappings;
};

}   // end of namespace induction
}   // end of namespace ddti

#endif /* end of include guard: INDUCTIONC4_5_H */
