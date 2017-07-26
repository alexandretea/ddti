// C/C++ File

// Author:   Alexandre Tea <alexandre.qtea@gmail.com>
// File:     /Users/alexandretea/Work/decision-tree-distributed-learning/srcs/utils/Process.cpp
// Purpose:  TODO (a one-line explanation)
// Created:  2017-07-26 18:02:13
// Modified: 2017-07-26 18:26:28

#include "MpiProcess.hpp"

namespace utils {
namespace mpi {

Process::Process(MPI_Comm const& communicator)
{
	char	processor_name[MPI_MAX_PROCESSOR_NAME];
	int		name_len;

	MPI_Init(NULL, NULL);
	MPI_Comm_size(communicator, &_comm_size);
	MPI_Comm_rank(communicator, &_rank);
	MPI_Get_processor_name(processor_name, &name_len);
    _name = std::string(processor_name, name_len);
}

Process::~Process()
{
    MPI_Finalize();
}

int
Process::get_rank() const
{
    return _rank;
}

int
Process::get_comm_size() const
{
    return _comm_size;
}

std::string const&
Process::get_name() const
{
    return _name;
}

std::ostream&
operator<<(std::ostream& s, Process const& o)
{
    s << o.get_name() << "["
        << o.get_rank() << "/"
        << o.get_comm_size() << "]";
    return s;
}

}   // end namespace mpi
}   // end namespace utils
