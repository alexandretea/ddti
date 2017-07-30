// C/C++ File

// Author:   Alexandre Tea <alexandre.qtea@gmail.com>
// File:     /Users/alexandretea/Work/decision-tree-distributed-learning/srcs/main_load.cpp
// Purpose:  TODO (a one-line explanation)
// Created:  2017-07-25 11:11:44
// Modified: 2017-07-30 17:45:48

#include <iostream>
#include <mlpack/core.hpp>
#include <utils/MpiCommProcess.hpp>
#include "MasterNode.hpp"
#include "InductionC4_5.hpp"
#include "SlaveNode.hpp"

PROGRAM_INFO("TODO", "TODO");   // TODO

// Program parameter names
static const char* PARAM_TRAINING_SET       = "training_set";
static const char* PARAM_TEST_SET           = "test_set";
static const char* PARAM_LABELS_DIMENSION   = "labels_column";

// Program parameters
PARAM_STRING_IN_REQ(PARAM_TRAINING_SET, "Path to the training dataset.", "i");
// TODO specify allowed formats for datasets, in param help or program info
PARAM_STRING_IN(PARAM_TEST_SET, "Test dataset file", "t", "");
PARAM_INT_IN(PARAM_LABELS_DIMENSION,
             "Index of the column containing the labels to predict "
             "(must be between 0 and N-1). If unspecified, the algorithm will "
             "use the last column of the dataset.", "l", -1);

static bool
is_master(utils::mpi::CommProcess const& p)
{
    return p.get_rank() == 0;
}

int
main(int ac, char** av)
{
    utils::mpi::CommProcess process;

    try {
        mlpack::CLI::ParseCommandLine(ac, av);
        ddti::Logger = ddti::MlpackLogger(&mlpack::Log::Info);

        // Master node
        if (is_master(process)) {
            ddti::MasterNode<ddti::induction::C4_5>    master(
                    process,
                    mlpack::CLI::GetParam<std::string>(PARAM_TRAINING_SET),
                    mlpack::CLI::GetParam<int>(PARAM_LABELS_DIMENSION),
                    mlpack::CLI::GetParam<std::string>(PARAM_TEST_SET)
            );

            ddti::Logger.set_id(master.get_name());
            master.run();

        // Slave node
        } else {
            ddti::SlaveNode     slave(process);

            ddti::Logger.set_id(slave.get_name());
            slave.run();
        }

    } catch (std::exception const& e) {
        mlpack::Log::Fatal << "Unexpected error: " << e.what() << std::endl;
    }
    return 0;
}
