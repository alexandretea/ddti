// C/C++ File

// Author:   Alexandre Tea <alexandre.qtea@gmail.com>
// File:     /Users/alexandretea/Work/ddti/srcs/master/SlaveNode.hpp
// Purpose:  TODO (a one-line explanation)
// Created:  2017-07-26 18:51:03
// Modified: 2017-07-29 18:21:04

#ifndef SLAVENODE_H
#define SLAVENODE_H

#include <mlpack/core.hpp>
#include "ANode.hpp"

namespace ddti {

class SlaveNode : public ANode
{
    public:
        SlaveNode(utils::mpi::CommProcess const& process);
        virtual ~SlaveNode();

        SlaveNode(SlaveNode const& other) = delete;
        SlaveNode&      operator=(SlaveNode const& other) = delete;

    public:
        virtual void    run();
};

}


#endif /* end of include guard: SLAVENODE_H */
