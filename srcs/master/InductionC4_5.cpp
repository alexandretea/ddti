// C/C++ File

// Author:   Alexandre Tea <alexandre.qtea@gmail.com>
// File:     /Users/alexandretea/Work/ddti/srcs/master/InductionC4_5.cpp
// Purpose:  Induction algorithm based on Quinlan's C4.5
// Created:  2017-07-28 16:17:42
// Modified: 2017-08-23 23:30:14

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

std::unique_ptr<DecisionTree>
C4_5::operator()(Dataset<double> const& dataset, Parameters conf)
{
    std::vector<size_t>             attributes;
    std::unique_ptr<DecisionTree>   dt_root;

    // build attributes vector
    attributes.reserve(dataset.n_rows());
    for (unsigned int i = 0; i < dataset.n_rows(); ++i) {
        if (i != dataset.labelsdim())
            attributes.push_back(i);
    }

    _dataset = &dataset;
    _conf = std::move(conf);

    dt_root = std::unique_ptr<DecisionTree>(
        rec_train_node(dataset.subview(0, 0, dataset.n_rows() - 1,
                                       dataset.n_cols() - 1),
                       attributes)
    );
    send_task(task::End);
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
    double                      entropy;
    std::pair<size_t, size_t>   maj_class; // <class, count>
    bool                        is_only_class;
    DecisionTree*               node;

    if (data.n_cols == 0)
        throw std::runtime_error("Not enough data");
    entropy = C4_5::compute_entropy(data.row(_dataset->labelsdim()),
                                    &maj_class, &is_only_class);
    if (is_only_class or attrs.empty()) {
        return create_leaf(maj_class, split_value, data.n_cols);
    }

    std::pair<size_t, double>   attr = select_attribute(data, attrs, entropy);
    if (attr.second < 0.05) {
        // base case: no information gain
        return create_leaf(maj_class, split_value, data.n_cols);
    }

    StdVecVec<ull_t>    split_cols = get_split_indices(data, attr.first);
    if (not check_leaf_size(split_cols)) {
        // check minimum number of instances per leaf
        return create_leaf(maj_class, split_value, data.n_cols);
    }

    debug("Split with attribute `"
          + _dataset->attribute_name(attr.first) + "` ("
          + std::to_string(data.n_cols) + ")");

    node = new DecisionTree(attr.first, split_value, data.n_cols);
    build_children(node, data, split_cols, attrs);
    return node;
}

bool
C4_5::check_leaf_size(StdVecVec<ull_t> const& split_cols) const
{
    return std::min_element(split_cols.begin(), split_cols.end(),
            [](auto& a, auto& b) { return a.size() < b.size(); })
            ->size() >= _conf.min_leaf_size;  // check minimum leaf size
}

DecisionTree*
C4_5::create_leaf(std::pair<size_t, size_t> const& label, int split_value,
                  size_t nb_instances) const
{
    debug("Create leaf with class `"
        + _dataset->mapping(_dataset->labelsdim(), label.first)
        + "` (" + std::to_string(label.second) + ")");
    return new DecisionTree(label.first, split_value, nb_instances, true,
                            nb_instances - label.second);
}

void
C4_5::build_children(DecisionTree* node, arma::Mat<double> const& node_data,
                     StdVecVec<ull_t> const& split_cols,
                     std::vector<size_t> const& node_attrs)
{
    std::vector<size_t> split_attrs = node_attrs;

    // remove the selected attribute from the attribute list
    split_attrs.erase(std::remove(split_attrs.begin(), split_attrs.end(),
                node->attribute()), split_attrs.end());

    // create and train child node
    for (unsigned int split_value = 0; split_value < split_cols.size();
         ++split_value) {
        std::vector<ull_t> const& instances = split_cols[split_value];

        if (not instances.empty()) {
            node->add_child(std::unique_ptr<DecisionTree>(
                rec_train_node(node_data.cols(arma::uvec(instances)),
                                split_attrs, split_value)
            ));
        }
    }
}

std::pair<size_t, double>
C4_5::select_attribute(arma::Mat<double> const& data,
                       std::vector<size_t> const& attrs, double entropy)
{
    std::map<size_t, ContTable> conts = count_contingencies(data);
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
        debug("IGR(" + _dataset->attribute_name(dim) + ") = "
              + std::to_string(info_gain_ratio));
        if (selected_attr.second < info_gain_ratio) {
            // keep track of largest information gain
            selected_attr.first = dim;
            selected_attr.second = info_gain_ratio;
        }
    }
    return selected_attr;
}

std::map<size_t, ContTable>
C4_5::count_contingencies(arma::Mat<double> const& data)
{
    static std::vector<size_t>  mapping_sizes = _dataset->mapping_sizes();

    arma::mat                   to_process;
    std::map<size_t, ContTable> contingencies;

    send_task(task::C4_5::CountContingenciesCode);

    to_process = scatter_matrix<double>(data);
    _comm.broadcast(_dataset->labelsdim());  // labels dimension
    _comm.bcast_vec(mapping_sizes);         // nb of values by dimension
    _tasks.count_contingencies(to_process, _dataset->labelsdim(), mapping_sizes,
                               &contingencies);
    return contingencies;
}

C4_5::StdVecVec<C4_5::ull_t>
C4_5::get_split_indices(arma::Mat<double> const& node_data,
                        size_t attr_split) const
{
    StdVecVec<ull_t> cols_by_vals(_dataset->mapping_size(attr_split));

    // sort instances by value; each batch of instances will be given to a child
    // for training
    for (size_t col_i = 0; col_i < node_data.n_cols; ++col_i) {
        cols_by_vals[node_data(attr_split, col_i)].push_back(col_i);
    }
    return cols_by_vals;
}

void
C4_5::debug(std::string const& s) const
{
    if (_conf.debug)
        ddti::Logger << s;
}

// static functions
double
C4_5::compute_entropy(arma::subview_row<double> const& dim,
                      std::pair<size_t, size_t>* majority_class,
                      bool* is_only_class)
{
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

        *majority_class = *it;
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
