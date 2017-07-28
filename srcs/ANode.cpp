// C/C++ File

// Author:   Alexandre Tea <alexandre.qtea@gmail.com>
// File:     /Users/alexandretea/Work/ddti/srcs/DdtiANode.cpp
// Purpose:  TODO (a one-line explanation)
// Created:  2017-07-26 19:00:51
// Modified: 2017-07-28 15:33:24

#include "ANode.hpp"

namespace ddti {

ANode::ANode(utils::mpi::CommProcess const& process)
    : _comm_process(process)
{
}

ANode::~ANode()
{
}

size_t
ANode::get_id() const
{
    return _comm_process.get_rank();
}

size_t
ANode::get_nb_slaves() const
{
    return _comm_process.get_comm_size() - 1;
}

}   // end of namespace ddti
