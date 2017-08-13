// C/C++ File

// Author:   Alexandre Tea <alexandre.qtea@gmail.com>
// File:     /Users/alexandretea/Work/ddti/srcs/slave/TaskC4_5.hpp
// Purpose:  TODO (a one-line explanation)
// Created:  2017-08-02 18:43:30
// Modified: 2017-08-13 16:16:22

#ifndef TASKC4_5_H
#define TASKC4_5_H

#include <unordered_map>
#include <string>
#include "task.hpp"
#include "MpiCommunicator.hpp"
#include "ddti.hpp"
#include "ANode.hpp"

namespace ddti {
namespace task {

class C4_5
{
    public:
        // task codes
        static const int    CountContingenciesCode;
        static const int    CalcCondEntropyCode;

        typedef void        (C4_5::*TaskFn)();

    public:
        C4_5(utils::mpi::Communicator const& comm);
        virtual ~C4_5();

        C4_5(C4_5 const& other) = delete;
        C4_5&   operator=(C4_5 const& other) = delete;

    public:
        void        operator()(int task_code);
        std::string name() const;

        void        count_contingencies(arma::mat const& data) const;
        void        count_contingencies(arma::mat const& data, size_t labelsdim,
                                        std::vector<size_t> const& dim_values,
                                        std::map<size_t, ContTable>* output
                                            = nullptr) const;
        void        compute_cond_entropy(ContTable const& matrix,
                                         double* centropy = nullptr) const;

    protected:
        void        count_contingencies();
        void        compute_cond_entropy();

        template <typename T>
        arma::Mat<T>
        recv_scatter_mat(bool by_column = true)
        {
            size_t          nb_elems;
            size_t          nb_entries;
            T*              aux_mem;
            arma::Mat<T>    matrix;

            _comm.recv_broadcast(nb_elems, ANode::MasterRank);
            _comm.recv_broadcast(nb_entries, ANode::MasterRank);

            aux_mem = new T[nb_elems * nb_entries];
            _comm.recv_scatter(aux_mem, nb_entries * nb_elems,
                               utils::mpi::datatype::get<T>(),
                               ANode::MasterRank);
            if (by_column)
                matrix = arma::Mat<T>(aux_mem, nb_elems, nb_entries);
            else
                matrix = arma::Mat<T>(aux_mem, nb_entries, nb_elems);
            delete aux_mem;
            return matrix;
        }

    protected:
        utils::mpi::Communicator const&     _comm;
        std::unordered_map<int, TaskFn>     _tasks;
};

}   // end of namespace task
}   // end of namespace ddti

#endif /* end of include guard: TASKC4_5_H */

