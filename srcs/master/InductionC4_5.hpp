// C/C++ File

// Author:   Alexandre Tea <alexandre.qtea@gmail.com>
// File:     /Users/alexandretea/Work/ddti/srcs/master/InductionC4_5.hpp
// Purpose:  Induction algorithm based on Quinlan's C4.5
// Created:  2017-07-28 16:14:44
// Modified: 2017-08-23 23:32:52

#ifndef INDUCTIONC4_5_H
#define INDUCTIONC4_5_H

#include <map>
#include <armadillo>
#include "MasterNode.hpp"
#include "DecisionTree.hpp"
#include "MPIDatatypeManager.hpp"
#include "TaskC4_5.hpp"
#include "Dataset.hpp"
#include "ddti.hpp"
#include "utils/matrix.hpp"

namespace ddti {
namespace induction {

class C4_5
{
    public:
        struct Parameters
        {
            bool            debug;          // logs some debug info (i.e. IGR)
            unsigned int    min_leaf_size;  // min. number of instances per leaf

            Parameters() : debug(false), min_leaf_size(2)
            {}

            std::string
            to_string() const
            {
                return std::string("debug(") + (debug ? "true" : "false") + ")"
                       ", min_leaf_size(" + std::to_string(min_leaf_size) + ")";
            }
        };

        using ull_t = unsigned long long;
        template <typename T>
        using StdVecVec = std::vector<std::vector<T>>;

    public:
        C4_5(utils::mpi::Communicator const& comm);
        virtual ~C4_5();

        C4_5(C4_5 const& other) = delete;
        C4_5&           operator=(C4_5 const& other) = delete;

    public:
        std::unique_ptr<DecisionTree>
        operator()(Dataset<double> const& dataset,
                   Parameters conf = Parameters());
        // NOTE: conf is not received as a constant reference because the param
        // will be moved in a member variable

    protected:
        DecisionTree*   rec_train_node(arma::Mat<double> const& data,
                                       std::vector<size_t> const& attrs,
                                       int split_value = -1);
        void            send_task(int task_code) const;
        void            build_children(DecisionTree* node,
                                       arma::Mat<double> const& node_data,
                                       StdVecVec<ull_t> const& split_cols,
                                       std::vector<size_t> const& node_attrs);
        DecisionTree*   create_leaf(std::pair<size_t, size_t> const& label,
                                    int split_value, size_t nb_instances) const;
        bool            check_leaf_size(StdVecVec<ull_t> const& cols) const;
        void            debug(std::string const& s) const;

        std::pair<size_t, double>   select_attribute(
            arma::Mat<double> const& data, std::vector<size_t> const& attrs,
            double entropy
        );
        std::map<size_t, ContTable> count_contingencies(
            arma::Mat<double> const& data
        );
        StdVecVec<ull_t>            get_split_indices(
            arma::Mat<double> const& data, size_t attr
        ) const;

        // scatter matrix by column or by row
        // NOTE: we don't allow the scatter of subviews because they might
        // not be continuous in memory
        template <typename T>
        arma::Mat<T>
        scatter_matrix(arma::Mat<T> const& data, bool by_column = true)
        {
            MPI_Datatype   entry_type;
            size_t         chunk_size;
            size_t         nb_elems;
            T*             aux_mem;

            if (by_column) {    // armadillo matrices are column-major
                nb_elems = data.n_rows;
                chunk_size = data.n_cols / _comm.size();
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
                        std::pair<size_t, size_t>* majority_class = nullptr,
                        bool* is_only_class = nullptr);

    protected:
        utils::mpi::Communicator const& _comm;
        task::C4_5                      _tasks;
        utils::mpi::datatype::Manager   _mpi_types;

        // only used during training:
        Dataset<double> const*          _dataset;
        Parameters                      _conf;
};

}   // end of namespace induction
}   // end of namespace ddti

#endif /* end of include guard: INDUCTIONC4_5_H */
