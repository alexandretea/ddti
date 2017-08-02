// C/C++ File

// Author:   Alexandre Tea <alexandre.qtea@gmail.com>
// File:     /Users/alexandretea/Work/ddti/srcs/DdtiANode.cpp
// Purpose:  TODO (a one-line explanation)
// Created:  2017-07-26 19:00:51
// Modified: 2017-08-02 15:58:11

#include <boost/date_time/posix_time/posix_time.hpp>
#include "ANode.hpp"
#include "utils/datetime.hpp"

namespace ddti {

ANode::ANode(utils::mpi::Communicator const& comm)
    : _communicator(comm), _name()
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
    return _communicator.rank();
}

size_t
ANode::nb_slaves() const
{
    // NOTE: if shadowing masters in future improvements, this will have to be
    // changed
    return _communicator.size() - 1;
}

bool
ANode::is_master() const
{
    return id() == 0;
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
