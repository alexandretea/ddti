// C/C++ File

// Author:   Alexandre Tea <alexandre.qtea@gmail.com>
// File:     /Users/alexandretea/Work/ddti/srcs/master/InductionC4_5.cpp
// Purpose:  TODO (a one-line explanation)
// Created:  2017-07-28 16:17:42
// Modified: 2017-07-28 16:45:03

#include "InductionC4_5.hpp"

namespace ddti {
namespace induction {

C4_5::C4_5(size_t nb_slaves, utils::mpi::CommProcess const& comm_process)
    : _nb_slaves(nb_slaves), _comm_process(comm_process), _dt_root()
{
}

C4_5::~C4_5()
{
}

DecisionTree
C4_5::operator()(arma::mat const& data)
{
    size_t          chunk_size;

    // split dataset into N chunks, where N is the number of slave nodes
    chunk_size = data.n_cols / _nb_slaves;
    for (size_t i = 0; i < _nb_slaves; ++i) {
        size_t first_row = i * chunk_size;
        size_t last_row = first_row + chunk_size - 1;

        arma::subview<double> tmp = data.cols(first_row, last_row);
        mlpack::Log::Info << "Split " << first_row << "-" << last_row
            << " size: " << tmp.n_cols << std::endl;
    }
    return _dt_root;
}

}   // end of namespace induction
}   // end of namespace ddti
