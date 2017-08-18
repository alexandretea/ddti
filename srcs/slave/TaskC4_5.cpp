// C/C++ File

// Author:   Alexandre Tea <alexandre.qtea@gmail.com>
// File:     /Users/alexandretea/Work/ddti/srcs/slave/TaskC4_5.cpp
// Purpose:  TODO (a one-line explanation)
// Created:  2017-08-02 18:45:34
// Modified: 2017-08-18 16:54:17

#include <mlpack/core.hpp>
#include "TaskC4_5.hpp"
#include "MpiDatatype.hpp"
#include "ANode.hpp"
#include "ddti.hpp"

namespace ddti {
namespace task {

const int    C4_5::CountContingenciesCode   = 42;
const int    C4_5::CompEntropiesCode        = 84;

C4_5::C4_5(utils::mpi::Communicator const& comm)
    : _comm(comm)
{
    _tasks[C4_5::CountContingenciesCode] = &C4_5::count_contingencies;
    _tasks[C4_5::CompEntropiesCode] = &C4_5::comp_condnsplit_entropies;
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
    (this->*_tasks[task_code])();
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

// Task: Compute conditional and split entropies
void
C4_5::comp_condnsplit_entropies()
{
    ContTable   matrix = recv_scatter_mat<unsigned int>(false); // receives rows
    size_t      nb_instances;

    _comm.recv_broadcast(nb_instances, ANode::MasterRank);
    comp_condnsplit_entropies(matrix, nb_instances);
}

// TODO refacto to compute cond entropy AND split entropy
void
C4_5::comp_condnsplit_entropies(ContTable const& matrix, size_t total_instances,
                                double* out_cond_e, double* out_split_e) const
{
    double  entropies[2];   // 0: conditional entropy, 1: split entropy
    double* reduced_ent;

    comp_matrix_entropies(matrix, total_instances, entropies[0], entropies[1]);

    reduced_ent = _comm.reduce(entropies, 2, MPI_SUM, ANode::MasterRank);
    if (out_cond_e != nullptr and out_split_e != nullptr
        and reduced_ent != nullptr) {
        // should only happen on master node

        *out_cond_e = reduced_ent[0];    // conditional entropy
        *out_split_e = reduced_ent[1];   // split entropy
    }
    if (reduced_ent != nullptr)
        delete reduced_ent;
}

void
C4_5::comp_matrix_entropies(ContTable const& matrix, size_t total_instances,
                            double& cond_e, double& split_e) const
{
    matrix.each_row([this, &cond_e, &split_e, total_instances](auto& row) {
        unsigned int    total_row = arma::accu(row);

        if (total_row > 0) {
            cond_e += compute_weighted_entropy(row, total_row,
                                                     total_instances);
            split_e += compute_split_entropy(total_row, total_instances);
        }
    });
}

// weighted entropy of a row
double
C4_5::compute_weighted_entropy(arma::Row<unsigned int> const& row,
                               unsigned int total_row,
                               size_t total_instances) const
{
    double          w_entropy = 0;

    if (total_row == 0)
        return 0;
    for (unsigned int v: row) {
        double prob = (static_cast<double>(v) / total_row);

        if (prob != 0)
            w_entropy += (prob * std::log2(prob));
    }
    w_entropy *= -1;
    w_entropy *= static_cast<double>(total_row) / total_instances;
    return w_entropy;
}

double
C4_5::compute_split_entropy(unsigned int total_row,
                            size_t total_instances) const
{
    double          split_e = 0;

    if (total_row == 0
        or (split_e = static_cast<double>(total_row) / total_instances) == 0)
        return 0;
    return split_e * std::log2(split_e) * -1;
}

}   // end of namespace task
}   // end of namespace ddti

