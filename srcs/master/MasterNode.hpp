// C/C++ File

// Author:   Alexandre Tea <alexandre.qtea@gmail.com>
// File:     /Users/alexandretea/Work/ddti/srcs/master/MasterNode.hpp
// Purpose:  TODO (a one-line explanation)
// Created:  2017-07-26 18:51:03
// Modified: 2017-07-28 16:44:40

#ifndef MASTERNODE_H
#define MASTERNODE_H

#include <mlpack/core.hpp>
#include "ANode.hpp"
#include "DecisionTree.hpp"

namespace ddti {

template <typename InductionAlgo>
class MasterNode : public ANode
{
    public:
        MasterNode(utils::mpi::CommProcess const& process,
                   std::string const& infile)
            : ANode(process), _infile(infile), _data(), _data_info()
        {}

        virtual ~MasterNode() {}

        MasterNode(MasterNode const& other) = delete;
        MasterNode&     operator=(MasterNode const& other) = delete;

    public:
        virtual void
        run()
        {
            mlpack::Log::Info << *this << std::endl;    // TODO proper log format

            try {
                InductionAlgo   induction_algorithm(get_nb_slaves(),
                                                    _comm_process);
                DecisionTree    dt_root;

                mlpack::data::Load(_infile, _data, _data_info, true);
                mlpack::Log::Info << "Induction of decision tree started" << std::endl;

                dt_root = induction_algorithm(_data);
                mlpack::Log::Info << "Induction done" << std::endl;
            } catch (std::exception const& e) {
                mlpack::Log::Fatal << e.what() << std::endl;
            }
        }

    protected:
        std::string                 _infile;
        arma::mat                   _data;
        mlpack::data::DatasetInfo   _data_info;
};

template <typename IA>
std::ostream&
operator<<(std::ostream& s, MasterNode<IA> const& other)
{
    s << "Master(" << other.get_id() << ")";
    return s;
}

}


#endif /* end of include guard: MASTERNODE_H */
