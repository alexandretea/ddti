// C/C++ File

// Author:   Alexandre Tea <alexandre.qtea@gmail.com>
// File:     /Users/alexandretea/Work/decision-tree-distributed-learning/srcs/main_load.cpp
// Purpose:  TODO (a one-line explanation)
// Created:  2017-07-25 11:11:44
// Modified: 2017-07-28 16:31:56

#include <iostream>
#include <mlpack/core.hpp>
#include <utils/MpiCommProcess.hpp>
#include "MasterNode.hpp"
#include "InductionC4_5.hpp"
#include "SlaveNode.hpp"

static const char* PARAM_INPUT = "infile";

PROGRAM_INFO("TODO", "TODO");
PARAM_STRING_IN(PARAM_INPUT, "Input file", "i", "");

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

        // Master node
        if (is_master(process)) {
            ddti::MasterNode<ddti::induction::C4_5>    master(
                    process,
                    mlpack::CLI::GetParam<std::string>(PARAM_INPUT)
            );
            master.run();

        // Slave node
        } else {
            ddti::SlaveNode     slave(process);
            slave.run();
        }

    } catch (std::exception const& e) {
        mlpack::Log::Fatal << "Unexpected error: " << e.what() << std::endl;
    }
    return 0;
}
