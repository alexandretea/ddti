// C/C++ File

// Author:   Alexandre Tea <alexandre.qtea@gmail.com>
// File:     /Users/alexandretea/Work/ddti/srcs/slave/TaskC4_5.cpp
// Purpose:  TODO (a one-line explanation)
// Created:  2017-08-02 18:45:34
// Modified: 2017-08-14 16:54:35

#include <mlpack/core.hpp>
#include "TaskC4_5.hpp"
#include "MpiDatatype.hpp"
#include "ANode.hpp"
#include "ddti.hpp"

namespace ddti {
namespace task {

const int    C4_5::CountContingenciesCode   = 42;
const int    C4_5::CalcCondEntropyCode      = 84;

C4_5::C4_5(utils::mpi::Communicator const& comm)
    : _comm(comm)
{
    _tasks[C4_5::CountContingenciesCode] = &C4_5::count_contingencies;
    _tasks[C4_5::CalcCondEntropyCode] = &C4_5::compute_cond_entropy;
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
C4_5::count_contingencies()
{
    size_t              labels_dimension;
    arma::mat           matrix;
    std::vector<size_t> dim_values;

    matrix = recv_scatter_mat<double>();
    _comm.recv_broadcast(labels_dimension, ANode::MasterRank);

    dim_values = _comm.recv_bcast_vec<size_t>(matrix.n_rows, ANode::MasterRank);
    count_contingencies(matrix, labels_dimension, dim_values);
    // NOTE: usage of dim_values only work bc we are using an IncrementPolicy
}

void
C4_5::count_contingencies(arma::mat const& data, size_t labels_dim,
                          std::vector<size_t> const& dim_values,
                          std::map<size_t, ContTable>* output) const
{
    ddti::Logger << "Received matrix (" + std::to_string(data.n_cols) + "*"
                    + std::to_string(data.n_rows) + ")";

    for (unsigned int dim = 0; dim < data.n_rows; ++dim) {
        if (dim != labels_dim) {

            ContTable       local_ct(dim_values[dim], dim_values[labels_dim],
                                     arma::fill::zeros);
            unsigned int*   reduced_ct(nullptr);

            // count values
            for (size_t i = 0; i < data.n_cols; ++i) {
                local_ct(data(dim, i), data(labels_dim, i)) += 1;
                //       curr dim val  label dim value
            }
            // reduce local contingency table
            reduced_ct = _comm.reduce(local_ct.memptr(), local_ct.n_elem,
                                      MPI_SUM, ANode::MasterRank);

            // only the master node should satisfy the following condition
            if (output != nullptr and reduced_ct != nullptr) {
                (*output)[dim] = ContTable(reduced_ct, dim_values[dim],
                                           dim_values[labels_dim]);
                delete reduced_ct;
            }
        }
    }
}

void
C4_5::compute_cond_entropy()
{
    ContTable   matrix = recv_scatter_mat<unsigned int>(false); // receives rows
    size_t      nb_instances;

    _comm.recv_broadcast(nb_instances, ANode::MasterRank);
    compute_cond_entropy(matrix, nb_instances);
}

void
C4_5::compute_cond_entropy(ContTable const& matrix, size_t total_instances,
                           double* output) const
{
    double  c_entropy = 0;
    double* reduced_ce;

    matrix.each_row([this, &c_entropy, total_instances](auto& row) {
        c_entropy += compute_weighted_entropy(row, total_instances);
    });
    reduced_ce = _comm.reduce(&c_entropy, 1, MPI_SUM, ANode::MasterRank);
    if (output != nullptr and reduced_ce != nullptr) {
        // should only happen on master node
        *output = *reduced_ce;
        delete reduced_ce;
    }
}

// weighted entropy of a row
double
C4_5::compute_weighted_entropy(arma::Row<unsigned int> const& row,
                               size_t total_instances) const
{
    unsigned int    total_row = arma::accu(row);
    double          w_entropy = 0;

    for (unsigned int v: row) {
        double prob = (static_cast<double>(v) / total_row);

        if (prob != 0)
            w_entropy += (prob * std::log2(prob));
    }
    w_entropy *= -1;
    w_entropy *= static_cast<double>(total_row) / total_instances;
    return w_entropy;
}

}   // end of namespace task
}   // end of namespace ddti

