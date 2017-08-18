// C/C++ File

// Author:   Alexandre Tea <alexandre.qtea@gmail.com>
// File:     /Users/alexandretea/Work/ddti/srcs/master/InductionC4_5.cpp
// Purpose:  TODO (a one-line explanation)
// Created:  2017-07-28 16:17:42
// Modified: 2017-08-18 13:57:50

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

    _dataset = dataset; // TODO fix: avoid copy of matrix

    dt_root = rec_train_node(dataset.subview(0, 0, dataset.n_rows() - 1,
                                             dataset.n_cols() - 1),
                             attributes);
    send_task(task::End);
    dt_root->print(dataset); // TODO remove debug
    // TODO end of induction? although pruning
    return dt_root;
}

void
C4_5::send_task(int task_code) const
{
    _comm.broadcast(task_code);
}

DecisionTree*
C4_5::rec_train_node(arma::Mat<double> const& data,
                     std::vector<size_t> const& attrs,
                     int split_value)
{
    double          entropy;
    size_t          majority_class;
    bool            is_only_class;
    DecisionTree*   node;

    if (data.n_cols == 0)
        throw std::runtime_error("Not enough data");
    entropy = C4_5::compute_entropy(data.row(_dataset.labelsdim()),
                                    &majority_class, &is_only_class);
    if (is_only_class or attrs.empty()) {   // TODO min nb instances?
        return create_leaf(majority_class, split_value, data.n_cols);
    }
    // TODO bufferised scatter + bufferised load of matrix
    std::pair<size_t, double>   attr = select_attribute(data, attrs, entropy);
    if (attr.second < 0.05) {   // base case: no information gain
        return create_leaf(majority_class, split_value, data.n_cols);
    }

    ddti::Logger << "Create node with attribute `"
                    + _dataset.attribute_name(attr.first) + "` ("
                    + std::to_string(data.n_cols) +  "; IGR: "
                    + std::to_string(attr.second) + ")";

    node = new DecisionTree(attr.first, split_value);
    build_children_nodes(node, data, attrs, attr.first);
    return node;
}

DecisionTree*
C4_5::create_leaf(size_t label, int split_value, size_t nb_instances) const
{
    ddti::Logger << "Create leaf with class `"
        + _dataset.mapping(_dataset.labelsdim(), label)
        + "` (" + std::to_string(nb_instances) + ")";
    return new DecisionTree(label, split_value, true);
}

void
C4_5::build_children_nodes(DecisionTree* node,
                          arma::Mat<double> const& node_data,
                          std::vector<size_t> const& node_attrs,
                          size_t attr_split)
{
    std::vector<size_t>                 split_attrs = node_attrs;
    std::vector<std::vector<size_t>>    cols_by_vals(
            _dataset.mapping_size(attr_split));

    // remove the selected attribute from the attribute list
    split_attrs.erase(std::remove(split_attrs.begin(), split_attrs.end(),
                attr_split), split_attrs.end());

    // sort entries by value; each batch of entries will be given to a child
    // for training
    for (size_t col_i = 0; col_i < node_data.n_cols; ++col_i) {
        cols_by_vals[node_data(attr_split, col_i)].push_back(col_i);
    }

    // create and train child node
    for (unsigned int split_value = 0; split_value < cols_by_vals.size();
         ++split_value) {
        std::vector<size_t> const& instances = cols_by_vals[split_value];

        if (not instances.empty()) {
            node->add_child(rec_train_node(_dataset.submat(instances),
                                           split_attrs, split_value));
        }
    }
}

std::pair<size_t, double>
C4_5::select_attribute(arma::Mat<double> const& data,
                       std::vector<size_t> const& attrs, double entropy)
{
    std::map<size_t, ContTable> conts = count_contingencies(data);
    // TODO refactor count_contingenceis to handle non-divisible scatter
    // + data < nb node
    std::pair<size_t, double>   selected_attr = std::make_pair(0, 0);
    //        dim     info gain

    // compute information gain ratio
    for (auto& dim: attrs) {
        double  info_gain_ratio;
        double  cond_e = 0;
        double  split_e = 0;
        size_t  remainings = 0;
        size_t  nb_instances = arma::accu(conts[dim]);

        // compute conditional entropy of dimension dim
        if (conts[dim].n_rows >= static_cast<size_t>(_comm.size())) {
            remainings = conts[dim].n_rows % _comm.size();

            send_task(task::C4_5::CompEntropiesCode);

            // scatter by row
            ContTable to_process = scatter_matrix<unsigned int>(
                (remainings > 0
                    ? conts[dim].head_rows(conts[dim].n_rows - remainings)
                    : conts[dim]),
                false
            );  // we specify the template argument arma::Mat because
                // head_rows() would return a subview with non-continuous data

            _comm.broadcast(nb_instances);
            _tasks.comp_condnsplit_entropies(to_process, nb_instances, &cond_e,
                                             &split_e);
        } else {    // can't scatter if nb of processors > data
            remainings = conts[dim].n_rows;
        }
        if (remainings > 0) {
            // compute entropies for the remaining data
            // (that is, the data that hasn't been scattered)
            arma::subview<unsigned int> to_process =
                conts[dim].tail_rows(remainings);

            _tasks.comp_matrix_entropies(to_process, nb_instances,
                                         cond_e, split_e);
        }
        info_gain_ratio = entropy - cond_e; // information gain
        if (split_e > 0)
            info_gain_ratio /= split_e;     // information gain ratio
        ddti::Logger << "IGR(" + _dataset.attribute_name(dim) + ") = "
                        + std::to_string(info_gain_ratio);
        if (selected_attr.second < info_gain_ratio) {
            // keep track of largest information gain
            selected_attr.first = dim;
            selected_attr.second = info_gain_ratio;
        }
    }
    return selected_attr;
}

// TODO refactor to only compute contingencies of given list of features/attrs
std::map<size_t, ContTable>
C4_5::count_contingencies(arma::Mat<double> const& data)
{
    static std::vector<size_t>  mapping_sizes = _dataset.mapping_sizes();

    arma::mat                   to_process;
    std::map<size_t, ContTable> contingencies;

    send_task(task::C4_5::CountContingenciesCode);

    to_process = scatter_matrix<double>(data);
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
    dim.for_each([&counts](double label) {
        if (counts.find(label) == counts.end())
            counts[label] = 1;
        else
            ++counts[label];
    });

    // output majority_class and is_only_class
    if (majority_class != nullptr) {
        auto it = std::max_element(counts.begin(), counts.end(),
                [](auto& p1, auto& p2)
                { return p1.second < p2.second; });

        *majority_class = it->first;
    }
    if (is_only_class != nullptr) {
        *is_only_class = (counts.size() <= 1);
    }

    // compute entropy
    for (auto& label_cnt: counts) {
        double prob = static_cast<double>(label_cnt.second) / dim.n_elem;

        if (prob != 0)
            entropy += prob * std::log2(prob);
    }
    return -entropy;
}

}   // end of namespace induction
}   // end of namespace ddti
