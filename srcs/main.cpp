// C/C++ File

// Author:   Alexandre Tea <alexandre.qtea@gmail.com>
// File:     /Users/alexandretea/Work/decision-tree-distributed-learning/srcs/main_load.cpp
// Purpose:  TODO (a one-line explanation)
// Created:  2017-07-25 11:11:44
// Modified: 2017-07-26 19:26:20

#include <iostream>
#include <mlpack/core.hpp>
#include <utils/MpiCommProcess.hpp>
#include "MasterNode.hpp"
#include "SlaveNode.hpp"

PROGRAM_INFO("TODO", "TODO");
PARAM_STRING_IN("input_file", "Input file", "i", "");

int
main(int ac, char** av)
{
    utils::mpi::CommProcess process;

    mlpack::CLI::ParseCommandLine(ac, av);
    if (process.get_rank() == 0) {
        ddti::MasterNode    master(process,
                mlpack::CLI::GetParam<std::string>("input_file"));

        master.run();
    } else {
        ddti::SlaveNode     slave(process);

        slave.run();
    }
    return 0;
}
