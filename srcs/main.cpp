// C/C++ File

// Author:   Alexandre Tea <alexandre.qtea@gmail.com>
// File:     /Users/alexandretea/Work/decision-tree-distributed-learning/srcs/main_load.cpp
// Purpose:  TODO (a one-line explanation)
// Created:  2017-07-25 11:11:44
// Modified: 2017-08-22 23:16:09

#include <iostream>
#include <mlpack/core.hpp>
#include "MpiCommunicator.hpp"
#include "MasterNode.hpp"
#include "InductionC4_5.hpp"
#include "TaskC4_5.hpp"
#include "SlaveNode.hpp"

PROGRAM_INFO("TODO", "TODO");   // TODO

// Program parameter names
static const char* PARAM_TRAINING_SET       = "training_set";
static const char* PARAM_TEST_SET           = "test_set";
static const char* PARAM_LABELS_DIMENSION   = "labels_column";
static const char* PARAM_ATTRIBUTES         = "attributes";

// Program parameters
PARAM_STRING_IN_REQ(PARAM_TRAINING_SET, "Path to the training dataset.", "i");
// TODO specify allowed formats for datasets, in param help or program info
PARAM_STRING_IN(PARAM_TEST_SET,
                "The dataset used to test the predictive accuracy of the "
                "generated model. If none is provided, the training set will be"
                "used.", "t", "");
PARAM_INT_IN(PARAM_LABELS_DIMENSION,
             "Index of the column containing the labels to predict "
             "(must be between 0 and N-1). If unspecified, the algorithm will "
             "use the last column of the dataset.", "l", -1);
PARAM_VECTOR_IN(std::string, PARAM_ATTRIBUTES,
                "List of attribute names, separated by spaces "
                "(e.g. -a name lastname age).", "a");

int
main(int ac, char** av)
{
    utils::mpi::Communicator comm;

    try {
        mlpack::CLI::ParseCommandLine(ac, av);
        ddti::Logger = ddti::MlpackLogger(&mlpack::Log::Info);

        // Master node
        if (comm.rank() == ddti::ANode::MasterRank) {
            using namespace mlpack;

            ddti::MasterNode<ddti::induction::C4_5>    master(
                    comm,
                    CLI::GetParam<std::string>(PARAM_TRAINING_SET),
                    CLI::GetParam<int>(PARAM_LABELS_DIMENSION),
                    CLI::GetParam<std::vector<std::string>>(PARAM_ATTRIBUTES),
                    CLI::GetParam<std::string>(PARAM_TEST_SET)
            );

            ddti::Logger.set_id(master.name());
            master.run();

        // Slave node
        } else {
            ddti::SlaveNode<ddti::task::C4_5>   slave(comm);

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
