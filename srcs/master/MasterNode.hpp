// C/C++ File

// Author:   Alexandre Tea <alexandre.qtea@gmail.com>
// File:     /Users/alexandretea/Work/ddti/srcs/master/MasterNode.hpp
// Purpose:  TODO (a one-line explanation)
// Created:  2017-07-26 18:51:03
// Modified: 2017-08-23 01:03:37

#ifndef MASTERNODE_H
#define MASTERNODE_H

#include <mlpack/core.hpp>
#include "ANode.hpp"
#include "DecisionTree.hpp"
#include "ddti.hpp"
#include "Dataset.hpp"
#include "Classifier.hpp"

namespace ddti {

// TODO network terminal to give instructions to node? could be in ANode
// so SlaveNode has it too; inherits from a TerminalCapabilities class?
template <typename InductionAlgo>
class MasterNode : public ANode
{
    public:
        MasterNode(utils::mpi::Communicator const& process,
                   std::string const& training_set, int labels_dim,
                   std::vector<std::string> const& attr_names = {},
                   std::string const& test_set = "")
            : ANode(process), _training_set(training_set),
              _labels_dim(labels_dim), _test_set(test_set),
              _attr_names(attr_names)
        {}

        virtual ~MasterNode() {}

        MasterNode(MasterNode const& other) = delete;
        MasterNode&     operator=(MasterNode const& other) = delete;

    public:
        virtual void
        run()
        {
            ddti::Logger << "Running";
            try {
                Dataset<double> dataset = load_data();
                Classifier      classifier(train(dataset));

                ddti::Logger << "Test model using "
                                + (_test_set.empty() ? "training set"
                                                     : _test_set);
                std::cout << classifier.test(dataset) << std::endl;
            } catch (std::exception const& e) {
                ddti::Logger.log(e.what(), mlpack::Log::Fatal);
            }
            _comm.barrier();
        }

    protected:
        Dataset<double>
        load_data()
        {
            mlpack::data::DatasetInfo   data_info;
            arma::mat                   data;

            mlpack::data::Load(_training_set, data, data_info, true);
            if (_labels_dim == -1)  // labels dimension is not set
                _labels_dim = data.n_rows - 1;
            return Dataset<double>(data, _labels_dim, _attr_names, &data_info);
        }

        DecisionTree*
        train(Dataset<double> const& dataset) const
        {
            DecisionTree*   dt_root;
            InductionAlgo   induction_algorithm(_comm);

            ddti::Logger << "Decision tree induction started"; // TODO log config
            dt_root = induction_algorithm(dataset);
            ddti::Logger << "Induction complete";
            return dt_root;
        }

    protected:
        std::string                 _training_set;
        int                         _labels_dim;
        std::string                 _test_set;
        std::vector<std::string>    _attr_names;
};

}   // end of namespace ddti


#endif /* end of include guard: MASTERNODE_H */
