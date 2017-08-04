// C/C++ File

// Author:   Alexandre Tea <alexandre.qtea@gmail.com>
// File:     /Users/alexandretea/Work/ddti/srcs/master/InductionC4_5.cpp
// Purpose:  TODO (a one-line explanation)
// Created:  2017-07-28 16:17:42
// Modified: 2017-08-05 00:38:38

#include <algorithm>
#include <vector>
#include <unordered_map>
#include "InductionC4_5.hpp"
#include "task.hpp"
#include "MpiDatatype.hpp"

namespace ddti {
namespace induction {

C4_5::C4_5(size_t nb_slaves, utils::mpi::Communicator const& comm_process)
    : _stop(false), _nb_slaves(nb_slaves), _communicator(comm_process),
      _mpi_types(), _labels_dim(-1)
{
}

C4_5::~C4_5()
{
}

DecisionTree*
C4_5::operator()(arma::mat const& data, // TODO change to arma::Mat<uint> ?
                 mlpack::data::DatasetInfo const& data_info,
                 size_t labels_dim)
{
    std::vector<size_t> attributes(data.n_rows);
    DecisionTree*       dt_root;
    int                 task_code;

    std::iota(attributes.begin(), attributes.end(), 0); // build attributes vec
    _labels_dim = labels_dim;
    dt_root = rec_train_node(data.submat(0, 0, data.n_rows - 1,
                                         data.n_cols - 1),
                             attributes);
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

    // TODO master: broadcast flag split/stop
    // TODO slave task container ddti::tasks::C4_5()
    return dt_root;
}

DecisionTree*
C4_5::rec_train_node(arma::subview<double> const& data,
                     std::vector<size_t> const& attrs)
{
    size_t          majority_class;
    bool            is_only_class;
    MPI_Datatype    column_type;

    if (data.n_cols == 0)
        throw std::runtime_error("Not enough data");
    majority_class = C4_5::find_majority_class(data.row(_labels_dim),
                                               &is_only_class);
    if (is_only_class or attrs.empty()) {   // TODO min nb instances?
        return new DecisionTree(majority_class, true);
    }
    // TODO bufferised scatter + bufferised load of matrix
    column_type = _mpi_types.matrix_column<double>(data.n_rows, data.n_cols);
    // TODO scatter
    // use mempter() or colptr() instead
    return nullptr;
}

// static functions
size_t
C4_5::find_majority_class(arma::subview_row<double> const& data,
                          bool* is_only_class)
{
    std::unordered_map<size_t, size_t>  counts; // <class index, count>

    if (data.n_elem == 0)
        throw std::runtime_error("Empty input");
    for (unsigned int i = 0; i < data.n_elem; ++i) {
        if (counts.find(data(i)) == counts.end())
            counts[data(i)] = 1;
        else
            ++counts[data(i)];
    }
    if (is_only_class != nullptr)
        *is_only_class = (counts.size() == 1);

    auto it = std::max_element(counts.begin(), counts.end(),
                               [](auto& p1, auto& p2)
                               { return p1.second < p2.second; });
    return it->first;
}

}   // end of namespace induction
}   // end of namespace ddti
