// C/C++ File

// Author:   Alexandre Tea <alexandre.qtea@gmail.com>
// File:     /Users/alexandretea/Work/ddti/srcs/master/InductionC4_5.hpp
// Purpose:  TODO (a one-line explanation)
// Created:  2017-07-28 16:14:44
// Modified: 2017-08-05 00:38:48

#ifndef INDUCTIONC4_5_H
#define INDUCTIONC4_5_H

#include <mlpack/core.hpp>  // TODO change with armadillo include
#include "MasterNode.hpp"
#include "DecisionTree.hpp"
#include "mpi/MPIDatatypeManager.hpp"

namespace ddti {
namespace induction {

// TODO needs to get parameters
class C4_5
{
    public:
        C4_5(size_t nb_slaves, utils::mpi::Communicator const& comm_process);
        virtual ~C4_5();

        C4_5(C4_5 const& other) = delete;
        C4_5&           operator=(C4_5 const& other) = delete;

    public:
        DecisionTree*   operator()(arma::mat const& data,
                                   mlpack::data::DatasetInfo const& info,
                                   size_t labels_dim);

    protected:
        // TODO keep track of attributes? datasetinfo?
        // TODO check signatures of the functions below
        DecisionTree*   rec_train_node(arma::subview<double> const& data,
                                       std::vector<size_t> const& attrs);
        //void    compute_contingency_tables();

        static size_t
        find_majority_class(arma::subview_row<double> const& data,
                            bool* is_only_class = nullptr);

    protected:
        bool                            _stop;  // TODO useless?
        size_t                          _nb_slaves;
        utils::mpi::Communicator const& _communicator;
        utils::mpi::datatype::Manager   _mpi_types;
        ssize_t                         _labels_dim;
};

}   // end of namespace induction
}   // end of namespace ddti

#endif /* end of include guard: INDUCTIONC4_5_H */
