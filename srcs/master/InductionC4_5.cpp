// C/C++ File

// Author:   Alexandre Tea <alexandre.qtea@gmail.com>
// File:     /Users/alexandretea/Work/ddti/srcs/master/InductionC4_5.cpp
// Purpose:  TODO (a one-line explanation)
// Created:  2017-07-28 16:17:42
// Modified: 2017-08-09 17:58:24

#include <algorithm>
#include <vector>
#include <unordered_map>
#include "InductionC4_5.hpp"
#include "task.hpp"
#include "TaskC4_5.hpp"
#include "MpiDatatype.hpp"

namespace ddti {
namespace induction {

C4_5::C4_5(size_t nb_slaves, utils::mpi::Communicator const& comm_process)
    : _nb_slaves(nb_slaves), _communicator(comm_process),
      _tasks(_communicator, _nb_slaves), _mpi_types()
{
}

C4_5::~C4_5()
{
}

DecisionTree*
C4_5::operator()(Dataset<double> const& dataset) // TODO change to <uint> ?
{
    std::vector<size_t> attributes(dataset.n_rows());
    DecisionTree*       dt_root;
    int                 task_code;

    std::iota(attributes.begin(), attributes.end(), 0); // build attributes vec
    _dataset = dataset;
    dt_root = rec_train_node(dataset.subview(0, 0, dataset.n_rows() - 1,
                                             dataset.n_cols() - 1),
                             attributes);
    _communicator.broadcast(task::End);
    // TODO end of induction? although pruning
    return dt_root;
}

DecisionTree*
C4_5::rec_train_node(arma::subview<double> const& data,
                     std::vector<size_t> const& attrs)
{
    size_t          majority_class;
    bool            is_only_class;
    std::vector<size_t> mapping_sizes;

    if (data.n_cols == 0)
        throw std::runtime_error("Not enough data");
    majority_class = C4_5::find_majority_class(data.row(_dataset.labelsdim()),
                                               &is_only_class);
    if (is_only_class or attrs.empty()) {   // TODO min nb instances?
        return new DecisionTree(majority_class, true);
    }
    // TODO bufferised scatter + bufferised load of matrix

    // attribute selection
    arma::mat   to_process;

    mapping_sizes = _dataset.mapping_sizes();
    _communicator.broadcast(task::C4_5::AttrSelectCode);
    to_process = scatter_matrix(data);
    _communicator.bcast_vec(mapping_sizes);     // nb of values by dimension
    _tasks.attribute_selection(to_process, _dataset.labelsdim(), mapping_sizes);
    // End of attribute selection
    return nullptr;
}

arma::mat
C4_5::scatter_matrix(arma::subview<double> const& data)
{
    MPI_Datatype        column_type;
    size_t              chunk_size;
    double*             aux_mem;
    arma::mat           matrix;

    column_type = _mpi_types.matrix_contiguous_entry<double>(data.n_rows);
    // armadillo matrices are column-major
    chunk_size = data.n_cols / _nb_slaves + 1;
    // TODO what if odd number? and fix nb_slaes +1
    _communicator.broadcast(data.n_rows);           // nb elems
    _communicator.broadcast(chunk_size);            // nb entries
    _communicator.broadcast(_dataset.labelsdim());  // labels dimension
    aux_mem = _communicator.scatter<double>(data.colptr(0), chunk_size,
                                            column_type,
                                            chunk_size * data.n_rows);
    matrix = arma::mat(aux_mem, data.n_rows, chunk_size);
    delete aux_mem; // TODO extra copy; use param of mat ctor to avoid that?
    return matrix;
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
