// C/C++ File

// Author:   Alexandre Tea <alexandre.qtea@gmail.com>
// File:     /Users/alexandretea/Work/decision-tree-distributed-learning/srcs/main_load.cpp
// Purpose:  TODO (a one-line explanation)
// Created:  2017-07-25 11:11:44
// Modified: 2017-07-26 18:31:19

#include <iostream>
#include <mlpack/core.hpp>
#include <utils/MpiProcess.hpp>

PROGRAM_INFO("TODO", "TODO");
//PARAM_STRING_IN_REQ("input_file", "Input file", "i");

int
main(int ac, char** av)
{
    utils::mpi::Process         comm_process;

    mlpack::CLI::ParseCommandLine(ac, av);
    if (comm_process.get_rank() == 0) {
        mlpack::Log::Info << "Master node woop woop" << std::endl;

        //arma::mat                   data;
        //mlpack::data::DatasetInfo   info;
        //std::string                 input_file;

        //try {
            //mlpack::CLI::ParseCommandLine(ac, av);
            //input_file = mlpack::CLI::GetParam<std::string>("input_file");
            //mlpack::data::Load(input_file, data, info, true);
            //data.print("zobb: ");
            //std::cout << "Dimensionality: " << info.Dimensionality() << std::endl;
        //} catch (std::exception const& e) {
            //std::cerr << e.what() << std::endl;
        //}
    } else {
        mlpack::Log::Info << "Fk it, slave node" << std::endl;
    }
    return 0;
}
