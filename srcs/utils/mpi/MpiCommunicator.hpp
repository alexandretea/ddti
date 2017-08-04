// C/C++ File

// Author:   Alexandre Tea <alexandre.qtea@gmail.com>
// File:     /Users/alexandretea/Work/decision-tree-distributed-learning/srcs/utils/MpiCommunicator.hpp
// Purpose:  TODO (a one-line explanation)
// Created:  2017-07-26 17:51:48
// Modified: 2017-08-05 00:23:23

#ifndef MPIPROCESS_H
#define MPIPROCESS_H

#include <string>
#include <mpi.h>
#include <ostream>
#include "MpiDatatype.hpp"

namespace utils {
namespace mpi {

class Communicator
{
    public:
        Communicator(MPI_Comm const& communicator = MPI_COMM_WORLD);
        ~Communicator();

        Communicator(Communicator const& other) = delete;
        Communicator&       operator=(Communicator const& other) = delete;
        // TODO implement copy ctr and assign op?

    public:
        int                 rank() const;
        int                 size() const;
        std::string const&  name() const;

        // communication functions for a single entry
        template <typename T>
        void
        send(int dest, T const& data, int tag = MPI_ANY_TAG) const
        { send(dest, &data, 1, tag); }

        template <typename T>
        T
        recv(int source = MPI_ANY_SOURCE,
             int tag = MPI_ANY_TAG,
             MPI_Status* status = MPI_STATUS_IGNORE) const
        {
            T   data;

            recv(&data, 1, source, tag, status);
            return data;
        }

        template <typename T>
        void
        broadcast(T& data, int root) const
        { broadcast(&data, 1, root); }

        template <typename T>
        void
        broadcast_send(T data, int root) const
        { broadcast(&data, 1, root); }
        // to handle sending of const values as MPI_Bcast() takes a non-const v
        // TODO find better solution

        // communication functions for multiple entries
        template <typename T>
        void
        send(int dest, T const* buffer, int count, int tag = MPI_ANY_TAG) const
        { send(dest, buffer, datatype::get<T>(), count, tag); }

        template <typename T>
        void
        recv(T* buffer, int count,
             int source = MPI_ANY_SOURCE,
             int tag = MPI_ANY_TAG,
             MPI_Status* status = MPI_STATUS_IGNORE) const
        { recv(buffer, datatype::get<T>(), count, source, tag, status); }

        template <typename T>
        void
        broadcast(T* buffer, int count, int root) const
        { broadcast(buffer, datatype::get<T>(), count, root); }

        // 'raw' communication functions
        // NOTE: we keep non-templated functions to handle
        // user-defined datatypes
        void    send(int dest, void const* buffer, MPI_Datatype datatype,
                     int count, int tag = MPI_ANY_TAG) const;
        void    recv(void* buffer, MPI_Datatype datatype, int count,
                     int source = MPI_ANY_SOURCE,
                     int tag = MPI_ANY_TAG,
                     MPI_Status* status = MPI_STATUS_IGNORE) const;
        void    broadcast(void* buffer, MPI_Datatype datatype, int count,
                          int root) const;
        // TODO mpi error handler, abort vs returns

    protected:
        MPI_Comm    _comm;
        int         _rank;
        int         _size;
        std::string _name;
};

std::ostream&       operator<<(std::ostream& s, Communicator const& other);

}   // end of namespace mpi
}   // end of namespace utils

#endif /* end of include guard: MPIPROCESS_H */
