// C/C++ File

// Author:   Alexandre Tea <alexandre.qtea@gmail.com>
// File:     /Users/alexandretea/Work/ddti/srcs/master/SlaveNode.hpp
// Purpose:  TODO (a one-line explanation)
// Created:  2017-07-26 18:51:03
// Modified: 2017-08-02 18:52:02

#ifndef SLAVENODE_H
#define SLAVENODE_H

#include <mlpack/core.hpp>
#include "ANode.hpp"
#include "ddti_log.hpp"
#include "task.hpp"

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
        virtual void
        run()
        {
            int    task_code;

            ddti::Logger << "Running with tasks: " + _task_manager.name();
            _communicator.broadcast(task_code, ANode::MasterRank);
            ddti::Logger << "Receive broadcast: " + std::to_string(task_code);
            while (task_code != task::End) {
                _task_manager(task_code);   // TODO handle tasks exceptions?
                _communicator.broadcast(task_code, ANode::MasterRank);
            }
        }

    protected:
        TaskManager _task_manager;
};

}


#endif /* end of include guard: SLAVENODE_H */
