// C/C++ File

// Author:   Alexandre Tea <alexandre.qtea@gmail.com>
// File:     /Users/alexandretea/Work/ddti/srcs/master/InductionC4_5.cpp
// Purpose:  TODO (a one-line explanation)
// Created:  2017-07-28 16:17:42
// Modified: 2017-08-13 17:40:27

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
    std::vector<size_t> attributes(dataset.n_rows());
    DecisionTree*       dt_root;
    int                 task_code;

    std::iota(attributes.begin(), attributes.end(), 0); // build attributes vec
    // TODO remove label dimension
    _dataset = dataset;
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
    // TODO task compute information gain
    return nullptr;
}

size_t
C4_5::select_attribute(arma::subview<double> const& data,
                       std::vector<size_t> const& attrs, double entropy)
{
    std::map<size_t, ContTable> conts = count_contingencies(data);

    // debug  conts
    for (auto& dim: attrs) {
        if (dim == _dataset.labelsdim())
            continue ;  // TODO remove labelsdims from vec attrs
        std::cout << "DIM: " << dim << std::endl;
        conts[dim].print();
    }
    // end debug

    // compute information gain
    for (auto& dim: attrs) {
        if (dim == _dataset.labelsdim())
            continue ;  // TODO remove labelsdims from vec attrs

        if (conts[dim].n_rows < _comm.size()) {
            // TODO
        } else {
            send_task(task::C4_5::CalcCondEntropyCode);

            // scatter by row
            arma::Mat<unsigned int> to_process = scatter_matrix(conts[dim], false);
            _tasks.compute_cond_entropy(to_process);
        }
    }
    return 0;
}

// TODO refactor to only compute contingencies of given list of features/attrs
std::map<size_t, ContTable>
C4_5::count_contingencies(arma::subview<double> const& data)
{
    static std::vector<size_t>  mapping_sizes = _dataset.mapping_sizes();

    arma::mat                   to_process;
    std::map<size_t, ContTable> contingencies;

    send_task(task::C4_5::CountContingenciesCode);

    to_process = scatter_matrix(data);  // TODO refacto scatter based on reduce design?
    _comm.broadcast(_dataset.labelsdim());  // labels dimension
    _comm.bcast_vec(mapping_sizes);     // nb of values by dimension
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
        double prob = static_cast<double>(count.second)
                      / static_cast<double>(dim.n_elem);

        if (prob != 0)
            entropy += prob * std::log2(prob);
    }
    return -entropy;
}

}   // end of namespace induction
}   // end of namespace ddti
