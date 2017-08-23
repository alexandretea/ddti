// C/C++ File

// Author:   Alexandre Tea <alexandre.qtea@gmail.com>
// File:     /Users/alexandretea/Work/ddti/srcs/utils/mpi/MpiDatatype.cpp
// Purpose:  utility functions for MPI datatypes
// Created:  2017-08-05 00:04:02
// Modified: 2017-08-23 23:17:52

#include <mpi.h>
#include "MpiDatatype.hpp"

namespace utils {
namespace mpi {
namespace datatype {

std::string
get_name(MPI_Datatype const& datatype)
{
    char    name[MPI_MAX_OBJECT_NAME];
    int     len;
    int     error_code;

    if ((error_code = MPI_Type_get_name(datatype, name, &len)) != MPI_SUCCESS)
        throw mpi::Exception(error_code);
    return std::string(name, len);
}

}   // end namespace datatype
}   // end namespace mpi
}   // end namespace utils

