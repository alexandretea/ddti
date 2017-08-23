// C/C++ File

// Author:   Alexandre Tea <alexandre.qtea@gmail.com>
// File:     /Users/alexandretea/Work/ddti/srcs/DdtiANode.cpp
// Purpose:  Abstract class representing a node in ddti's distributed design
// Created:  2017-07-26 19:00:51
// Modified: 2017-08-23 23:10:34

#include <boost/date_time/posix_time/posix_time.hpp>
#include "ANode.hpp"
#include "utils/datetime.hpp"

namespace ddti {

// NOTE: if shadowing masters in future improvements, this will have to be
// changed
const int    ANode::MasterRank = 0;

ANode::ANode(utils::mpi::Communicator const& comm)
    : _comm(comm), _name()
{
	std::stringstream	ss;

    ss << (is_master() ? "M" : "S") << id()
		<< "(" << comm.name() << ")";
	_name = ss.str();
}

ANode::~ANode()
{
}

size_t
ANode::id() const
{
    return _comm.rank();
}

size_t
ANode::nb_slaves() const
{
    // NOTE: if shadowing masters in future improvements, this will have to be
    // changed
    return _comm.size() - 1;
}

bool
ANode::is_master() const
{
    // NOTE: if shadowing masters in future improvements, this will have to be
    // changed
    return id() == MasterRank;
}

bool
ANode::is_slave() const
{
    return not is_master();
}

std::string const&
ANode::name() const
{
    return _name;
}

template <typename IA>
std::ostream&
operator<<(std::ostream& s, ANode const& other)
{
    s << other.name();
    return s;
}

}   // end of namespace ddti
