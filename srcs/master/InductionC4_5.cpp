// C/C++ File

// Author:   Alexandre Tea <alexandre.qtea@gmail.com>
// File:     /Users/alexandretea/Work/ddti/srcs/master/InductionC4_5.cpp
// Purpose:  TODO (a one-line explanation)
// Created:  2017-07-28 16:17:42
// Modified: 2017-08-02 18:51:12

#include "InductionC4_5.hpp"
#include "task.hpp"

namespace ddti {
namespace induction {

C4_5::C4_5(size_t nb_slaves, utils::mpi::Communicator const& comm_process)
    : _stop(false), _nb_slaves(nb_slaves), _communicator(comm_process),
      _dt_root()
{
}

C4_5::~C4_5()
{
}

DecisionTree
C4_5::operator()(arma::mat const& data,
                 mlpack::data::DatasetInfo const& data_info,
                 size_t labels_dim)
{
    int task_code;

    _stop = true;
    while (not _stop) {
    }
    ddti::Logger << "Send broadcast";
    _communicator.broadcast_send(task::End, ANode::MasterRank);
    // end of induction? although pruning

    //size_t          chunk_size;

    // split dataset into N chunks, where N is the number of slave nodes
    //chunk_size = data.n_cols / _nb_slaves;
    //for (size_t i = 0; i < _nb_slaves; ++i) {
        //size_t first_row = i * chunk_size;
        //size_t last_row = first_row + chunk_size - 1;

        //arma::subview<double> tmp = data.cols(first_row, last_row);
        //mlpack::Log::Info << "Split " << first_row << "-" << last_row
            //<< " size: " << tmp.n_cols << std::endl;
    //}

    //MPI_Datatype    double_column;

    //MPI_Type_vector(data.n_rows, 1, data.n_rows, MPI_DOUBLE, &double_column);
    //MPI_Scatter(data.begin(), data.n_cols, double_column, );
    // TODO master: broadcast flag split/stop
    // TODO slave task container ddti::tasks::C4_5()
    return _dt_root;
}

}   // end of namespace induction
}   // end of namespace ddti
