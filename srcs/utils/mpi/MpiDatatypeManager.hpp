// C/C++ File

// Author:   Alexandre Tea <alexandre.qtea@gmail.com>
// File:     /Users/alexandretea/Work/ddti/srcs/utils/mpi/MpiDatatypeManager.hpp
// Purpose:  TODO (a one-line explanation)
// Created:  2017-08-04 23:38:51
// Modified: 2017-08-05 00:42:52

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

        // create and commit type
        template <typename T>
        MPI_Datatype const&
        matrix_column(size_t n_rows, size_t n_cols)
        {
            MPI_Datatype    column;
            int             error_code;

            if ((error_code = MPI_Type_vector(n_rows, 1, n_cols,
                                              datatype::get<T>(), &column))
                    != MPI_SUCCESS)
                throw mpi::Exception(error_code);
            return _datatypes[commit(column)];
        }

    protected:
        std::unordered_map<std::string, MPI_Datatype>   _datatypes;
};

}   // end namespace datatype
}   // end namespace mpi
}   // end namespace utils

#endif /* end of include guard: MPIDATATYPEMANAGER_H */
