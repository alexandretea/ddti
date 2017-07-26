// C/C++ File

// Author:   Alexandre Tea <alexandre.qtea@gmail.com>
// File:     /Users/alexandretea/Work/ddti/srcs/master/SlaveNode.cpp
// Purpose:  TODO (a one-line explanation)
// Created:  2017-07-26 19:13:07
// Modified: 2017-07-26 19:36:29

#include "SlaveNode.hpp"

namespace ddti {

SlaveNode::SlaveNode(utils::mpi::CommProcess const& process)
    : ANode(process)
{
}

SlaveNode::~SlaveNode()
{
}

void
SlaveNode::run()
{
    mlpack::Log::Info << *this << std::endl;
}

std::ostream&
operator<<(std::ostream& s, SlaveNode const& other)
{
    s << "Slave(" << other.get_id() << ")";
    return s;
}

}   // end of namespace ddti
