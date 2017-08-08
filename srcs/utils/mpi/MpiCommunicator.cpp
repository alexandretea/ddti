// C/C++ File

// Author:   Alexandre Tea <alexandre.qtea@gmail.com>
// File:     /Users/alexandretea/Work/decision-tree-distributed-learning/srcs/utils/Communicator.cpp
// Purpose:  TODO (a one-line explanation)
// Created:  2017-07-26 18:02:13
// Modified: 2017-08-06 18:21:32

#include "MpiCommunicator.hpp"
#include "MpiException.hpp"

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
    int error_code;

    if ((error_code = MPI_Barrier(_comm)) != MPI_SUCCESS)
        throw mpi::Exception(error_code);
}

void
Communicator::send(int dest, void const* buffer, MPI_Datatype const& datatype,
                   int count, int tag) const
{
    int error_code;

    if ((error_code = MPI_Send(buffer, count, datatype, dest, tag, _comm))
            != MPI_SUCCESS)
        throw mpi::Exception(error_code);
}

void
Communicator::recv(void* buffer, MPI_Datatype const& datatype, int count,
                   int source, int tag, MPI_Status* status) const
{
    int error_code;

    if ((error_code = MPI_Recv(buffer, count, datatype, source, tag, _comm,
                    status)) != MPI_SUCCESS)
        throw mpi::Exception(error_code);
}

void
Communicator::broadcast(void* buffer, MPI_Datatype const& datatype, int count,
                        int root) const
{
    int error_code;

    if ((error_code = MPI_Bcast(buffer, count, datatype, root, _comm))
            != MPI_SUCCESS)
        throw mpi::Exception(error_code);
}

void
Communicator::scatter(void const* sbuf, int scount, MPI_Datatype const& stype,
                      void* rbuf, int rcount, MPI_Datatype const& rtype) const
{
    int error_code;

    if ((error_code = MPI_Scatter(sbuf, scount, stype, rbuf, rcount, rtype,
                                  _rank, _comm)) != MPI_SUCCESS)
        throw mpi::Exception(error_code);
}

void
Communicator::recv_scatter(void* buffer, int count, MPI_Datatype const& type,
                           int root) const
{
    int error_code;

    //int MPI_Scatter(const void *sendbuf, int sendcount, MPI_Datatype sendtype,
    //               void *recvbuf, int recvcount, MPI_Datatype recvtype, int root,
    //                              MPI_Comm comm)
    //
    if ((error_code = MPI_Scatter(nullptr, 0, type, buffer, count, type, root,
                                  _comm)) != MPI_SUCCESS)
        throw mpi::Exception(error_code);
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
