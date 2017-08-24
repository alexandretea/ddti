// C/C++ File

// Author:   Alexandre Tea <alexandre.qtea@gmail.com>
// File:     /Users/alexandretea/Work/ddti/srcs/slave/TaskC4_5.hpp
// Purpose:  Class that contains the logic of C4.5's tasks
// Created:  2017-08-02 18:43:30
// Modified: 2017-08-24 01:30:09

#ifndef TASKC4_5_H
#define TASKC4_5_H

#include <unordered_map>
#include <string>
#include "task.hpp"
#include "MpiCommunicator.hpp"
#include "ddti.hpp"
#include "ANode.hpp"
#include "utils/matrix.hpp"

namespace ddti {
namespace task {

class C4_5
{
    public:
        // task codes
        static const int    CountContingenciesCode;
        static const int    CompEntropiesCode;

        typedef void        (C4_5::*TaskFn)();

    public:
        C4_5(utils::mpi::Communicator const& comm);
        virtual ~C4_5();

        C4_5(C4_5 const& other) = delete;
        C4_5&   operator=(C4_5 const& other) = delete;

    public:
        void        operator()(int task_code);
        std::string name() const;

        void        count_contingencies(arma::umat const& data) const;
        void        count_contingencies(arma::umat const& data, size_t labelsdim,
                                        std::vector<size_t> const& dim_values,
                                        std::map<size_t, ContTable>* output
                                            = nullptr) const;
        void        comp_condnsplit_entropies(ContTable const& matrix,
                                              size_t total_instances,
                                              double* cond_e = nullptr,
                                              double* split_e = nullptr) const;
        void        comp_matrix_entropies(ContTable const& matrix,
                                          size_t total_instances,
                                          double& cond_e,
                                          double& split_e) const;
        double      compute_weighted_entropy(arma::Row<unsigned int> const& row,
                                             unsigned int total_row,
                                             size_t total_instances) const;
        double      compute_split_entropy(unsigned int total_row,
                                          size_t total_instances) const;

    protected:
        void        count_contingencies();
        void        comp_condnsplit_entropies();

        template <typename T>
        arma::Mat<T>
        recv_scatter_mat(bool by_column = true)
        {
            size_t          nb_elems;
            size_t          nb_entries;
            T*              aux_mem;

            _comm.recv_broadcast(nb_elems, ANode::MasterRank);
            _comm.recv_broadcast(nb_entries, ANode::MasterRank);

            aux_mem = new T[nb_elems * nb_entries];
            _comm.recv_scatter(aux_mem, nb_entries * nb_elems,
                               utils::mpi::datatype::get<T>(),
                               ANode::MasterRank);
            return utils::matrix::load_arma_matrix(aux_mem, nb_elems,
                                                   nb_entries, by_column);
        }

    protected:
        utils::mpi::Communicator const&     _comm;
        std::unordered_map<int, TaskFn>     _tasks;
};

}   // end of namespace task
}   // end of namespace ddti

#endif /* end of include guard: TASKC4_5_H */

