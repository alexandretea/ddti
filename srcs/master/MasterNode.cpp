// C/C++ File

// Author:   Alexandre Tea <alexandre.qtea@gmail.com>
// File:     /Users/alexandretea/Work/ddti/srcs/master/MasterNode.cpp
// Purpose:  TODO (a one-line explanation)
// Created:  2017-07-26 19:13:07
// Modified: 2017-07-26 19:36:12

#include "MasterNode.hpp"

namespace ddti {

MasterNode::MasterNode(utils::mpi::CommProcess const& process,
                       std::string const& infile)
    : ANode(process), _infile(infile)
{
}

MasterNode::~MasterNode()
{
}

void
MasterNode::run()
{
    mlpack::Log::Info << *this << std::endl;

    arma::mat                   data;
    mlpack::data::DatasetInfo   info;
    std::string                 input_file;

    try {
        mlpack::data::Load(input_file, data, info, true);
        //data.print("zobb: ");
        std::cout << "Dimensionality: " << info.Dimensionality() << std::endl;
    } catch (std::exception const& e) {
        std::cerr << e.what() << std::endl;
    }
}

std::ostream&
operator<<(std::ostream& s, MasterNode const& other)
{
    s << "Master(" << other.get_id() << ")";
    return s;
}

}   // end of namespace ddti
