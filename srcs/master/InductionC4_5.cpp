// C/C++ File

// Author:   Alexandre Tea <alexandre.qtea@gmail.com>
// File:     /Users/alexandretea/Work/ddti/srcs/master/InductionC4_5.cpp
// Purpose:  TODO (a one-line explanation)
// Created:  2017-07-28 16:17:42
// Modified: 2017-08-14 17:16:57

#include <algorithm>
#include <vector>
#include <unordered_map>
#include "InductionC4_5.hpp"
#include "task.hpp"
#include "TaskC4_5.hpp"
#include "MpiDatatype.hpp"

namespace ddti {
namespace induction {

C4_5::C4_5(utils::mpi::Communicator const& comm_process)
    : _comm(comm_process), _tasks(_comm), _mpi_types()
{
}

C4_5::~C4_5()
{
}

DecisionTree*
C4_5::operator()(Dataset<double> const& dataset) // TODO change to <uint> ?
{
    std::vector<size_t> attributes;
    DecisionTree*       dt_root;
    int                 task_code;

    // build attributes vector
    attributes.reserve(dataset.n_rows());
    for (unsigned int i = 0; i < dataset.n_rows(); ++i) {
        if (i != dataset.labelsdim())
            attributes.push_back(i);
    }

    _dataset = dataset; // TODO fix copy of matrix.

    // TODO template rec_train node to handle subviews and matrices, like in scatter_matrix()
    dt_root = rec_train_node(dataset.subview(0, 0, dataset.n_rows() - 1,
                                             dataset.n_cols() - 1),
                             attributes);
    send_task(task::End);
    // TODO end of induction? although pruning
    return dt_root;
}

void
C4_5::send_task(int task_code) const
{
    _comm.broadcast(task_code);
}

DecisionTree*
C4_5::rec_train_node(arma::subview<double> const& data,
                     std::vector<size_t> const& attrs)
{
    double  entropy;
    size_t  majority_class;
    bool    is_only_class;

    if (data.n_cols == 0)
        throw std::runtime_error("Not enough data");
    entropy = C4_5::compute_entropy(data.row(_dataset.labelsdim()),
                                    &majority_class, &is_only_class);
    ddti::Logger << "Entropy: " + std::to_string(entropy);
    if (is_only_class or attrs.empty()) {   // TODO min nb instances?
        return new DecisionTree(majority_class, true);
    }
    // TODO bufferised scatter + bufferised load of matrix

    size_t selected_attr = select_attribute(data, attrs, entropy);
    ddti::Logger << "Split on dimension: " + std::to_string(selected_attr);
    return nullptr;
}

size_t
C4_5::select_attribute(arma::subview<double> const& data,
                       std::vector<size_t> const& attrs, double entropy)
{
    std::map<size_t, ContTable> conts = count_contingencies(data);
    // TODO refactor count_contingenceis to handle non-divisible scatter + data < nb node
    std::pair<size_t, double>   selected_attr = std::make_pair(0, 0);
    //        dim     info gain

    // compute information gain
    for (auto& dim: attrs) {
        double  info_gain;
        double  c_entropy = 0;
        size_t  remainings = 0;
        size_t  nb_instances = arma::accu(conts[dim]);

        // compute conditional entropy of dimension dim
        if (conts[dim].n_rows >= static_cast<size_t>(_comm.size())) {
            remainings = conts[dim].n_rows % _comm.size();

            send_task(task::C4_5::CalcCondEntropyCode);

            // scatter by row
            ContTable to_process = scatter_matrix<unsigned int, arma::Mat>(
                (remainings > 0
                    ? conts[dim].head_rows(conts[dim].n_rows - remainings)
                    : conts[dim]),
                false
            );  // we specify the template argument arma::Mat because
                // head_rows() would return a subview with non-continuous data

            _comm.broadcast(nb_instances);
            _tasks.compute_cond_entropy(to_process, nb_instances, &c_entropy);
        } else {    // can't scatter if nb of processors > data
            remainings = conts[dim].n_rows;
        }
        if (remainings > 0) {
            // compute conditional entropy for the remaining data (that is,
            // the data that hasn't been scattered)
            arma::subview<unsigned int> to_process =
                conts[dim].tail_rows(remainings);

            to_process.each_row([this, &c_entropy, nb_instances](auto& row) {
                c_entropy += _tasks.compute_weighted_entropy(row, nb_instances);
            });
        }
        info_gain = entropy - c_entropy;
        if (selected_attr.second < info_gain) {
            // keep track of largest information gain
            selected_attr.first = dim;
            selected_attr.second = info_gain;
        }
    }
    return selected_attr.first;
}

// TODO refactor to only compute contingencies of given list of features/attrs
std::map<size_t, ContTable>
C4_5::count_contingencies(arma::subview<double> const& data)
{
    static std::vector<size_t>  mapping_sizes = _dataset.mapping_sizes();

    arma::mat                   to_process;
    std::map<size_t, ContTable> contingencies;

    send_task(task::C4_5::CountContingenciesCode);

    to_process = scatter_matrix(data);
    _comm.broadcast(_dataset.labelsdim());  // labels dimension
    _comm.bcast_vec(mapping_sizes);         // nb of values by dimension
    _tasks.count_contingencies(to_process, _dataset.labelsdim(), mapping_sizes,
                               &contingencies);
    return contingencies;
}

// static functions
double
C4_5::compute_entropy(arma::subview_row<double> const& dim,
                      size_t* majority_class, bool* is_only_class)
{
    // TODO distribute data?
    std::map<size_t, size_t>    counts; // <class value, count>
    double                      entropy = 0.0;

    if (dim.n_elem == 0)
        throw std::runtime_error("Empty input");

    // loop and count
    for (unsigned int i = 0; i < dim.n_elem; ++i) {
        if (counts.find(dim(i)) == counts.end())
            counts[dim(i)] = 1;
        else
            ++counts[dim(i)];
    }

    // output majority_class and is_only_class
    if (majority_class != nullptr) {
        auto it = std::max_element(counts.begin(), counts.end(),
                [](auto& p1, auto& p2)
                { return p1.second < p2.second; });

        *majority_class = it->first;
    }
    if (is_only_class != nullptr)
        *is_only_class = (counts.size() == 1);

    // compute entropy
    for (auto& count: counts) {
        double prob = static_cast<double>(count.second) / dim.n_elem;

        if (prob != 0)
            entropy += prob * std::log2(prob);
    }
    return -entropy;
}

}   // end of namespace induction
}   // end of namespace ddti
