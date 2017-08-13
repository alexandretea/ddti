// C/C++ File

// Author:   Alexandre Tea <alexandre.qtea@gmail.com>
// File:     /Users/alexandretea/Work/decision-tree-distributed-learning/srcs/utils/Communicator.cpp
// Purpose:  TODO (a one-line explanation)
// Created:  2017-07-26 18:02:13
// Modified: 2017-08-13 14:46:22

#include <iostream>
#include "MpiCommunicator.hpp"
#include "MpiException.hpp"

#define MPI_THROW_ON_ERR(x)                     \
    int error_code;                             \
    if ((error_code = (x)) != MPI_SUCCESS)      \
        throw mpi::Exception(error_code);

namespace utils {
namespace mpi {

Communicator::Communicator(MPI_Comm const& comm)
    : _comm(comm)
{
    char    processor_name[MPI_MAX_PROCESSOR_NAME];
    int     name_len;

    MPI_Init(NULL, NULL);
    MPI_Comm_size(comm, &_size);
    MPI_Comm_rank(comm, &_rank);
    MPI_Get_processor_name(processor_name, &name_len);
    _name = std::string(processor_name, name_len);
}

Communicator::~Communicator()
{
    MPI_Finalize();
}

int
Communicator::rank() const
{
    return _rank;
}

int
Communicator::size() const
{
    return _size;
}

std::string const&
Communicator::name() const
{
    return _name;
}

void
Communicator::barrier() const
{
    MPI_THROW_ON_ERR(MPI_Barrier(_comm));
}

void
Communicator::send(int dest, void const* buffer, MPI_Datatype const& datatype,
                   int count, int tag) const
{
    MPI_THROW_ON_ERR(MPI_Send(buffer, count, datatype, dest, tag, _comm));
}

void
Communicator::recv(void* buffer, MPI_Datatype const& datatype, int count,
                   int source, int tag, MPI_Status* status) const
{
    MPI_THROW_ON_ERR(MPI_Recv(buffer, count, datatype, source, tag, _comm,
                              status));
}

void
Communicator::broadcast(void* buffer, MPI_Datatype const& datatype, int count,
                        int root) const
{
    MPI_THROW_ON_ERR(MPI_Bcast(buffer, count, datatype, root, _comm));
}

void
Communicator::scatter(void const* sbuf, int scount, MPI_Datatype const& stype,
                      void* rbuf, int rcount, MPI_Datatype const& rtype) const
{
    MPI_THROW_ON_ERR(MPI_Scatter(sbuf, scount, stype, rbuf, rcount, rtype,
                                  _rank, _comm));
}

void
Communicator::recv_scatter(void* buffer, int count, MPI_Datatype const& type,
                           int root) const
{
    MPI_THROW_ON_ERR(MPI_Scatter(nullptr, 0, type, buffer, count, type, root,
                                  _comm));
}

void
Communicator::reduce(void const* sdata, int count, MPI_Datatype const& datatype,
                     MPI_Op const& op, void* rbuffer, int root) const
{
    MPI_THROW_ON_ERR(MPI_Reduce(sdata, rbuffer, count, datatype, op,
                                (root == -1 ? _rank : root), _comm));
}

std::ostream&
operator<<(std::ostream& s, Communicator const& o)
{
    s << o.name() << "["
        << o.rank() << "/"
        << o.size() << "]";
    return s;
}

}   // end namespace mpi
}   // end namespace utils
