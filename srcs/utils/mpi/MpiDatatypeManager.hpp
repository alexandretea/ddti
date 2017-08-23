// C/C++ File

// Author:   Alexandre Tea <alexandre.qtea@gmail.com>
// File:     /Users/alexandretea/Work/ddti/srcs/utils/mpi/MpiDatatypeManager.hpp
// Purpose:  Class that manages MPI datatypes
// Created:  2017-08-04 23:38:51
// Modified: 2017-08-23 23:37:02

#ifndef MPIDATATYPEMANAGER_H
#define MPIDATATYPEMANAGER_H

#include <unordered_map>
#include <string>
#include <mpi.h>
#include "MpiException.hpp"
#include "MpiDatatype.hpp"

namespace utils {
namespace mpi {
namespace datatype {

class Manager
{
    public:
        Manager();
        ~Manager();

        Manager(Manager const& o) = delete;
        Manager&            operator=(Manager const& o) = delete;

    public:
        std::string         commit(MPI_Datatype type);
        // returns name of type (can be used to query it)
        bool                free(std::string const& id);
        MPI_Datatype const& find(std::string const& name) const;
        void                clear();

        // create and commit a contiguous entry of a matrix
        // if the matrix is column-major, then the type represents a column
        // if it is row-major, the type represents a row
        template <typename T>
        MPI_Datatype const&
        matrix_contiguous_entry(size_t nb_elems)
        {
            MPI_Datatype    entry;
            int             error_code;

            if ((error_code = MPI_Type_vector(nb_elems, 1, 1,
                                              datatype::get<T>(), &entry))
                    != MPI_SUCCESS)
                throw mpi::Exception(error_code);
            return _datatypes[commit(entry)];
        }

        // create and commit a non-contiguous entry of a matrix
        // if the matrix is column-major, then the type represents a row
        // if it is row-major, the type represents a column
        template <typename T>
        MPI_Datatype const&
        matrix_noncontiguous_entry(size_t nb_elems, size_t contiguous_size)
        {
            MPI_Datatype    entry;
            int             err;

            if ((err = MPI_Type_vector(nb_elems, 1, contiguous_size,
                            datatype::get<T>(), &entry)) != MPI_SUCCESS or
                (err = MPI_Type_create_resized(entry, 0, sizeof(T),
                            &entry)) != MPI_SUCCESS)
                throw mpi::Exception(err);
            // NOTE: The call to MPI_Type_create_resized is necessary to
            // restrict the extent of the type, in such a way that when used
            // with a count the extent of the whole entry is just one element
            return _datatypes[commit(entry)];
        }

    protected:
        std::unordered_map<std::string, MPI_Datatype>   _datatypes;
};

}   // end namespace datatype
}   // end namespace mpi
}   // end namespace utils

#endif /* end of include guard: MPIDATATYPEMANAGER_H */
