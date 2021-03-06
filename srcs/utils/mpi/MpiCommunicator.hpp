// C/C++ File

// Author:   Alexandre Tea <alexandre.qtea@gmail.com>
// File:     /Users/alexandretea/Work/decision-tree-distributed-learning/srcs/utils/MpiCommunicator.hpp
// Purpose:  Abstraction of an MPI communicator
// Created:  2017-07-26 17:51:48
// Modified: 2017-08-23 23:36:35

#ifndef MPIPROCESS_H
#define MPIPROCESS_H

#include <string>
#include <mpi.h>
#include <ostream>
#include <vector>
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

    public:
        int                 rank() const;
        int                 size() const;
        std::string const&  name() const;
        void                barrier() const;

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
        recv_broadcast(T& data, int root) const
        { broadcast(&data, 1, root); }

        template <typename T>
        void
        broadcast(T data) const
        { broadcast(&data, 1, _rank); }
        // take data as a copy
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
        bcast_vec(std::vector<T> /* const */& container) const
        { broadcast(&container.front(), container.size(), _rank); }
        // container is not const bc MPI_Bcast() don't take consts

        template <typename T>
        std::vector<T>
        recv_bcast_vec(size_t count, int root) const
        {
            std::vector<T>  data(count);

            broadcast(&data.front(), count, root);
            return data;
        }
        // container is not const bc MPI_Bcast() don't take consts

        template <typename T>
        void
        broadcast(T* buffer, int count, int root) const
        { broadcast(buffer, datatype::get<T>(), count, root); }

        template <typename RecvType>
        RecvType*
        scatter(void const* sbuf, int scount, MPI_Datatype const& stype,
                int rcount) const
        {
            RecvType*  rbuf;

            rbuf = new RecvType[rcount];
            scatter(sbuf, scount, stype, rbuf, rcount,
                    datatype::get<RecvType>());
            return rbuf;
        }

        template <typename T>
        T*
        reduce(T const* sdata, int count, MPI_Op const& op, int root = -1) const
        {
            T*  rbuf = nullptr;

            if (root == -1 or root == _rank)
                rbuf = new T[count];
            reduce(sdata, count, datatype::get<T>(), op, rbuf, root);
            return rbuf;
        }

        // 'raw' communication functions
        // NOTE: we keep non-templated functions to handle
        // user-defined datatypes
        void    send(int dest, void const* buffer, MPI_Datatype const& datatype,
                     int count, int tag = MPI_ANY_TAG) const;
        void    recv(void* buffer, MPI_Datatype const& datatype, int count,
                     int source = MPI_ANY_SOURCE,
                     int tag = MPI_ANY_TAG,
                     MPI_Status* status = MPI_STATUS_IGNORE) const;
        void    broadcast(void* buffer, MPI_Datatype const& datatype, int count,
                          int root) const;
        void    scatter(void const* sbuf, int scount, MPI_Datatype const& stype,
                        void* rbuf, int rcount,
                        MPI_Datatype const& rtype) const;
        void    recv_scatter(void* buffer, int count, MPI_Datatype const& type,
                             int root) const;
        void    reduce(void const* sdata, int count, MPI_Datatype const& type,
                       MPI_Op const& op, void* rbuffer = nullptr,
                       int root = -1) const;

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
