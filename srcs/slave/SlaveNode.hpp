// C/C++ File

// Author:   Alexandre Tea <alexandre.qtea@gmail.com>
// File:     /Users/alexandretea/Work/ddti/srcs/master/SlaveNode.hpp
// Purpose:  TODO (a one-line explanation)
// Created:  2017-07-26 18:51:03
// Modified: 2017-08-02 15:57:39

#ifndef SLAVENODE_H
#define SLAVENODE_H

#include <mlpack/core.hpp>
#include "ANode.hpp"
#include "ddti_log.hpp"

namespace ddti {

template <typename TaskManager>
class SlaveNode : public ANode
{
    public:
        SlaveNode(utils::mpi::Communicator const& comm)
            : ANode(comm) {}

        virtual ~SlaveNode() {}

        SlaveNode(SlaveNode const& other) = delete;
        SlaveNode&      operator=(SlaveNode const& other) = delete;

    public:
        virtual void    run()
        {
            bool    running(true);

            ddti::Logger << "Running";  // TODO with taskmanager name
            while (running) {
                // TODO loop logic (recv flag split/stop)
            }
        }

    protected:
        TaskManager _tasks;
};

}


#endif /* end of include guard: SLAVENODE_H */
