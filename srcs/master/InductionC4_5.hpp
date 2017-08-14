// C/C++ File

// Author:   Alexandre Tea <alexandre.qtea@gmail.com>
// File:     /Users/alexandretea/Work/ddti/srcs/master/InductionC4_5.hpp
// Purpose:  TODO (a one-line explanation)
// Created:  2017-07-28 16:14:44
// Modified: 2017-08-14 16:51:57

#ifndef INDUCTIONC4_5_H
#define INDUCTIONC4_5_H

#include <map>
#include <armadillo>  // TODO change with armadillo include
#include "MasterNode.hpp"
#include "DecisionTree.hpp"
#include "MPIDatatypeManager.hpp"
#include "TaskC4_5.hpp"
#include "Dataset.hpp"
#include "ddti.hpp"
#include "utils/matrix.hpp"

namespace ddti {
namespace induction {

// TODO needs to get parameters
class C4_5
{
    public:
        C4_5(utils::mpi::Communicator const& comm);
        virtual ~C4_5();

        C4_5(C4_5 const& other) = delete;
        C4_5&           operator=(C4_5 const& other) = delete;

    public:
        DecisionTree*   operator()(Dataset<double> const& dataset);

    protected:
        DecisionTree*   rec_train_node(arma::subview<double> const& data,
                                       std::vector<size_t> const& attrs);
        void            send_task(int task_code) const;
        size_t          select_attribute(arma::subview<double> const& data,
                                         std::vector<size_t> const& attrs,
                                         double entropy);

        std::map<size_t, ContTable>
        count_contingencies(arma::subview<double> const& data);

        // scatter matrix by column or by row
        template <typename T, template<typename> class MatType = arma::Mat>
        arma::Mat<T>
        scatter_matrix(MatType<T> const& data, bool by_column = true)
        {
            MPI_Datatype   entry_type;
            size_t         chunk_size;
            size_t         nb_elems;
            T*             aux_mem;

            if (by_column) {    // armadillo matrices are column-major
                nb_elems = data.n_rows;
                chunk_size = data.n_cols / _comm.size();
                // TODO check case odd number
                entry_type = _mpi_types.matrix_contiguous_entry<T>(data.n_rows);
            } else {
                nb_elems = data.n_cols;
                chunk_size = data.n_rows / _comm.size();
                entry_type =
                    _mpi_types.matrix_noncontiguous_entry<T>(nb_elems,
                                                             data.n_rows);
            }
            _comm.broadcast(nb_elems);   // nb elems
            _comm.broadcast(chunk_size); // nb entries
            aux_mem = _comm.scatter<T>(data.colptr(0), chunk_size, entry_type,
                                       chunk_size * nb_elems);
            return utils::matrix::load_arma_matrix(aux_mem, nb_elems,
                                                   chunk_size, by_column);
        }

        static double
        compute_entropy(arma::subview_row<double> const& dim,
                        size_t* majority_class = nullptr,
                        bool* is_only_class = nullptr);

    protected:
        utils::mpi::Communicator const& _comm;
        task::C4_5                      _tasks;
        utils::mpi::datatype::Manager   _mpi_types;

        // used during training:
        Dataset<double>                 _dataset;
};

}   // end of namespace induction
}   // end of namespace ddti

#endif /* end of include guard: INDUCTIONC4_5_H */
