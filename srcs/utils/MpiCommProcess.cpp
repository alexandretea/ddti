// C/C++ File

// Author:   Alexandre Tea <alexandre.qtea@gmail.com>
// File:     /Users/alexandretea/Work/decision-tree-distributed-learning/srcs/utils/CommProcess.cpp
// Purpose:  TODO (a one-line explanation)
// Created:  2017-07-26 18:02:13
// Modified: 2017-07-26 18:59:07

#include "MpiCommProcess.hpp"

namespace utils {
namespace mpi {

CommProcess::CommProcess(MPI_Comm const& communicator)
{
	char	processor_name[MPI_MAX_PROCESSOR_NAME];
	int		name_len;

	MPI_Init(NULL, NULL);
	MPI_Comm_size(communicator, &_comm_size);
	MPI_Comm_rank(communicator, &_rank);
	MPI_Get_processor_name(processor_name, &name_len);
    _name = std::string(processor_name, name_len);
}

CommProcess::~CommProcess()
{
    MPI_Finalize();
}

int
CommProcess::get_rank() const
{
    return _rank;
}

int
CommProcess::get_comm_size() const
{
    return _comm_size;
}

std::string const&
CommProcess::get_name() const
{
    return _name;
}

std::ostream&
operator<<(std::ostream& s, CommProcess const& o)
{
    s << o.get_name() << "["
        << o.get_rank() << "/"
        << o.get_comm_size() << "]";
    return s;
}

}   // end namespace mpi
}   // end namespace utils
