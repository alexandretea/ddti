// C/C++ File

// Author:   Alexandre Tea <alexandre.qtea@gmail.com>
// File:     /Users/alexandretea/Work/ddti/srcs/DdtiANode.cpp
// Purpose:  TODO (a one-line explanation)
// Created:  2017-07-26 19:00:51
// Modified: 2017-07-30 15:48:29

#include <boost/date_time/posix_time/posix_time.hpp>
#include "ANode.hpp"
#include "utils/datetime.hpp"

namespace ddti {

ANode::ANode(utils::mpi::CommProcess const& process)
    : _comm_process(process), _name()
{
	std::stringstream	ss;

    ss << (is_master() ? "M" : "S") << get_id()
		<< "(" << process.get_name() << ")";
	_name = ss.str();
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

bool
ANode::is_master() const
{
    return get_id() == 0;
}

bool
ANode::is_slave() const
{
    return not is_master();
}

std::string const&
ANode::get_name() const
{
    return _name;
}

template <typename IA>
std::ostream&
operator<<(std::ostream& s, ANode const& other)
{
    s << other.get_name();
    return s;
}

}   // end of namespace ddti
