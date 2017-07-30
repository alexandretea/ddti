// C/C++ File

// Author:   Alexandre Tea <alexandre.qtea@gmail.com>
// File:     /Users/alexandretea/Work/ddti/srcs/master/SlaveNode.cpp
// Purpose:  TODO (a one-line explanation)
// Created:  2017-07-26 19:13:07
// Modified: 2017-07-30 17:48:10

#include "SlaveNode.hpp"
#include "ddti_log.hpp"

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
    ddti::Logger << "Running";
}

}   // end of namespace ddti
