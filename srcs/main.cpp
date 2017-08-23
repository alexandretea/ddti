// C/C++ File

// Author:   Alexandre Tea <alexandre.qtea@gmail.com>
// File:     /Users/alexandretea/Work/decision-tree-distributed-learning/srcs/main_load.cpp
// Purpose:  TODO (a one-line explanation)
// Created:  2017-07-25 11:11:44
// Modified: 2017-08-23 19:50:29

#include <iostream>
#include <mlpack/core.hpp>
#include "MpiCommunicator.hpp"
#include "MasterNode.hpp"
#include "InductionC4_5.hpp"
#include "TaskC4_5.hpp"
#include "SlaveNode.hpp"
#include "ddti.hpp"

PROGRAM_INFO(
    "(DDTI) Distributed Decision Tree Induction",
    "Train a model with a decision tree, then measures its predictive "
    "accuracy. Ddti does this in a distributed manner, using OpenMPI. "
    "The master node will induct the decision tree, using slave nodes "
    "to execute tasks such as computing entropies.\n"
    "The algorithm used to build the decision tree is based on Quilan's C4.5."
);

int
main(int ac, char** av)
{
    utils::mpi::Communicator comm;

    try {
        ddti::Logger = ddti::MlpackLogger(&mlpack::Log::Info);

        // Master node
        if (comm.rank() == ddti::ANode::MasterRank) {
            ddti::MasterNode<ddti::induction::C4_5>    master(comm);

            master.init_cli(ac, av);
            ddti::Logger.set_id(master.name());
            master.run();

        // Slave node
        } else {
            ddti::SlaveNode<ddti::task::C4_5>   slave(comm);

            slave.init_cli(ac, av);
            ddti::Logger.set_id(slave.name());
            slave.run();
        }
    } catch (std::exception const& e) {
        mlpack::Log::Fatal << "Unexpected error: " << e.what() << std::endl;
        mlpack::CLI::Destroy();
        return EXIT_FAILURE;
    }
    mlpack::CLI::Destroy();
    return EXIT_SUCCESS;;
}
