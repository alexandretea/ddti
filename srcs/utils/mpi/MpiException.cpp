// C/C++ File

// Author:   Alexandre Tea <alexandre.qtea@gmail.com>
// File:     /Users/alexandretea/Work/ddti/srcs/utils/MpiException.cpp
// Purpose:  TODO (a one-line explanation)
// Created:  2017-08-02 16:56:05
// Modified: 2017-08-02 17:52:27

#include <unordered_map>
#include <mpi.h>
#include "MpiException.hpp"

namespace utils {
namespace mpi {

Exception::Exception(int error_code)
    : std::runtime_error(error_to_string(error_code))
{
}

Exception::~Exception() throw()
{
}

std::string const&
error_to_string(int error_code)
{
    static std::unordered_map<int, std::string> cache;

    if (cache.find(error_code) == cache.end())
    {
        char    buffer[MPI_MAX_ERROR_STRING];
        int     error_length;

        MPI_Error_string(error_code, buffer, &error_length);
        cache[error_code] = std::string(buffer, error_length);
    }
    return cache[error_code];
}

}   // end of namespace mpi
}   // end of namespace utils
