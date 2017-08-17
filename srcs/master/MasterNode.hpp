// C/C++ File

// Author:   Alexandre Tea <alexandre.qtea@gmail.com>
// File:     /Users/alexandretea/Work/ddti/srcs/master/MasterNode.hpp
// Purpose:  TODO (a one-line explanation)
// Created:  2017-07-26 18:51:03
// Modified: 2017-08-17 16:52:53

#ifndef MASTERNODE_H
#define MASTERNODE_H

#include <mlpack/core.hpp>
#include "ANode.hpp"
#include "DecisionTree.hpp"
#include "ddti.hpp"
#include "Dataset.hpp"

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
              _attr_names(attr_names), _data()
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
                DecisionTree*               dt_root;
                mlpack::data::DatasetInfo   data_info;
                InductionAlgo               induction_algorithm(_communicator);

                mlpack::data::Load(_training_set, _data, data_info, true);
                if (_labels_dim == -1)  // labels dimension is not set
                    _labels_dim = _data.n_rows - 1;
                ddti::Logger << "Decision tree induction started"; // TODO log config
                dt_root = induction_algorithm(
                    Dataset<double>(_data, _labels_dim, _attr_names, &data_info)
                );
                ddti::Logger << "Induction complete";
            } catch (std::exception const& e) {
                ddti::Logger.log(e.what(), mlpack::Log::Fatal);
            }
        }

    protected:
        std::string                 _training_set;
        int                         _labels_dim;
        std::string                 _test_set;
        std::vector<std::string>    _attr_names;
        arma::mat                   _data;
};

}   // end of namespace ddti


#endif /* end of include guard: MASTERNODE_H */
