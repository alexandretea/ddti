// C/C++ File

// Author:   Alexandre Tea <alexandre.qtea@gmail.com>
// File:     /Users/alexandretea/Work/ddti/srcs/DdtiANode.cpp
// Purpose:  TODO (a one-line explanation)
// Created:  2017-07-26 19:00:51
// Modified: 2017-07-26 19:33:27

#include "ANode.hpp"

namespace ddti {

ANode::ANode(utils::mpi::CommProcess const& process)
    : _process(process)
{
}

ANode::~ANode()
{
}

int
ANode::get_id() const
{
    return _process.get_rank();
}

}   // end of namespace ddti
