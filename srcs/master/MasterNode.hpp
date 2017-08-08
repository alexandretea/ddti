// C/C++ File

// Author:   Alexandre Tea <alexandre.qtea@gmail.com>
// File:     /Users/alexandretea/Work/ddti/srcs/master/MasterNode.hpp
// Purpose:  TODO (a one-line explanation)
// Created:  2017-07-26 18:51:03
// Modified: 2017-08-07 14:55:54

#ifndef MASTERNODE_H
#define MASTERNODE_H

#include <mlpack/core.hpp>
#include "ANode.hpp"
#include "DecisionTree.hpp"
#include "ddti_log.hpp"
#include "utils/mlpack.hpp"

namespace ddti {

// TODO network terminal to give instructions to node? could be in ANode
// so SlaveNode has it too; inherits from a TerminalCapabilities class?
template <typename InductionAlgo>
class MasterNode : public ANode
{
    public:
        MasterNode(utils::mpi::Communicator const& process,
                   std::string const& training_set, int labels_dim,
                   std::string const& test_set = "")
            : ANode(process), _training_set(training_set),
              _labels_dim(labels_dim), _test_set(test_set), _data(),
              _mappings()
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
                InductionAlgo               induction_algorithm(nb_slaves(),
                                                                _communicator);
                DecisionTree*               dt_root;
                mlpack::data::DatasetInfo   data_info;

                mlpack::data::Load(_training_set, _data, data_info, true);
                _mappings = utils::mlpack::get_mappings(data_info);
                if (_labels_dim == -1)  // labels dimension is not set
                    _labels_dim = _data.n_rows - 1;
                ddti::Logger << "Decision tree induction started"; // TODO log config

                dt_root = induction_algorithm(_data, _mappings, _labels_dim);
                ddti::Logger << "Induction complete";
            } catch (std::exception const& e) {
                ddti::Logger.log(e.what(), mlpack::Log::Fatal);
            }
        }

    protected:
        std::string                     _training_set;
        int                             _labels_dim;
        std::string                     _test_set;
        arma::mat                       _data;
        utils::mlpack::DatasetMappings  _mappings;
};

}   // end of namespace ddti


#endif /* end of include guard: MASTERNODE_H */
