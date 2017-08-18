// C/C++ File

// Author:   Alexandre Tea <alexandre.qtea@gmail.com>
// File:     /Users/alexandretea/Work/ddti/srcs/DdtiANode.hpp
// Purpose:  TODO (a one-line explanation)
// Created:  2017-07-26 18:53:59
// Modified: 2017-08-17 20:24:08

#ifndef DDTIANODE_H
#define DDTIANODE_H

#include <mlpack/core.hpp>
#include "mpi/MpiCommunicator.hpp"

namespace ddti {

class ANode
{
    // ANode is abstract and can only be instanciated through its child classes
    protected:
        ANode(utils::mpi::Communicator const& comm);

    public:
        virtual ~ANode();

        ANode(ANode const& other) = delete;
        ANode&          operator=(ANode const& other) = delete;

    public:
        virtual void        run() = 0;
        size_t              id() const;
        size_t              nb_slaves() const;
        bool                is_master() const;
        bool                is_slave() const;
        std::string const&  name() const;

    public:
        // NOTE: if shadowing masters in future improvements, this will have
        // to be changed
        static const int    MasterRank;

    protected:
        utils::mpi::Communicator const& _comm;
        std::string                     _name;
};

template <typename IA>
std::ostream&
operator<<(std::ostream& s, ANode const& other);

} // end namespace ddti

#endif /* end of include guard: DDTIANODE_H */
