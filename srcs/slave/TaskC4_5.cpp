// C/C++ File

// Author:   Alexandre Tea <alexandre.qtea@gmail.com>
// File:     /Users/alexandretea/Work/ddti/srcs/slave/TaskC4_5.cpp
// Purpose:  TODO (a one-line explanation)
// Created:  2017-08-02 18:45:34
// Modified: 2017-08-09 17:33:22

#include <mlpack/core.hpp>
#include "TaskC4_5.hpp"
#include "MpiDatatype.hpp"
#include "ANode.hpp"
#include "ddti_log.hpp"

namespace ddti {
namespace task {

const int    C4_5::AttrSelectCode = 42;

C4_5::C4_5(utils::mpi::Communicator const& comm, size_t nb_slaves)
    : _comm(comm), _nb_slaves(nb_slaves)
{
    _tasks[C4_5::AttrSelectCode] = &C4_5::attribute_selection;
}

C4_5::~C4_5()
{
}

void
C4_5::operator()(int task_code)
{
    if (_tasks.find(task_code) == _tasks.end())
        throw std::runtime_error("Cannot find task: "
                                 + std::to_string(task_code));
    ddti::Logger << "Execute task: " + std::to_string(task_code);
    (this->*_tasks[task_code])();
    ddti::Logger << "Task completed: " + std::to_string(task_code);
}

std::string
C4_5::name() const
{
    return "C4_5-Tasks";
}

void
C4_5::attribute_selection()
{
    size_t              nb_elems;
    size_t              nb_entries;
    size_t              labels_dimension;
    double*             aux_mem;
    arma::mat           matrix;
    std::vector<size_t> dim_values;

    _comm.recv_broadcast(nb_elems, ANode::MasterRank);
    _comm.recv_broadcast(nb_entries, ANode::MasterRank);
    _comm.recv_broadcast(labels_dimension, ANode::MasterRank);

    aux_mem = new double[nb_elems * nb_entries];
    // expects entries to arrive contiguously
    _comm.recv_scatter(aux_mem, nb_entries * nb_elems,
                       utils::mpi::datatype::get<double>(), ANode::MasterRank);
    matrix = arma::mat(aux_mem, nb_elems, nb_entries);
    // TODO extra copy, use constructor param to force non-copy?
    delete aux_mem;
    dim_values = _comm.recv_bcast_vec<size_t>(nb_elems, ANode::MasterRank);
    attribute_selection(matrix, labels_dimension, dim_values);
    // usage of dim_values only work bc we are using an IncrementPolicy
}

void
C4_5::attribute_selection(arma::mat const& data, size_t labels_dim,
                          std::vector<size_t> const& dim_values) const
{
    std::unordered_map<size_t, arma::mat>  contingency_tables;  // <dim, ct>

    ddti::Logger << "Received matrix (" + std::to_string(data.n_cols) + "*"
        + std::to_string(data.n_rows) + ")";
    // compute contingency tables
    for (unsigned int dim = 0; dim < data.n_rows; ++dim) {
        if (dim != labels_dim) {
        }
    }
}

}   // end of namespace task
}   // end of namespace ddti

