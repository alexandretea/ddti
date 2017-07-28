// C/C++ File

// Author:   Alexandre Tea <alexandre.qtea@gmail.com>
// File:     /Users/alexandretea/Work/ddti/srcs/master/InductionC4_5.hpp
// Purpose:  TODO (a one-line explanation)
// Created:  2017-07-28 16:14:44
// Modified: 2017-07-28 16:43:11

#ifndef INDUCTIONC4_5_H
#define INDUCTIONC4_5_H

#include <mlpack/core.hpp>  // TODO change with armadillo include
#include "MasterNode.hpp"
#include "DecisionTree.hpp"

namespace ddti {
namespace induction {

// TODO needs to get parameters
class C4_5
{
    public:
        C4_5(size_t nb_slaves, utils::mpi::CommProcess const& comm_process);
        virtual ~C4_5();

        C4_5(C4_5 const& other) = delete;
        C4_5&   operator=(C4_5 const& other) = delete;

    public:
        DecisionTree    operator()(arma::mat const& data);

    protected:
        size_t                          _nb_slaves;
        utils::mpi::CommProcess const&  _comm_process;
        DecisionTree                    _dt_root;
};

}   // end of namespace induction
}   // end of namespace ddti

#endif /* end of include guard: INDUCTIONC4_5_H */
