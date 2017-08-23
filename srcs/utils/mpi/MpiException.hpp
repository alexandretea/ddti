// C/C++ File

// Author:   Alexandre Tea <alexandre.qtea@gmail.com>
// File:     /Users/alexandretea/Work/ddti/srcs/utils/MpiException.hpp
// Purpose:  MPI exception
// Created:  2017-08-02 16:54:27
// Modified: 2017-08-23 23:18:43

#ifndef MPIEXCEPTION_H
#define MPIEXCEPTION_H

#include <string>
#include <stdexcept>

namespace utils {
namespace mpi {

class Exception : public std::runtime_error
{
    public:
        Exception(int error_code);
        virtual ~Exception() throw();
};

std::string const&  error_to_string(int error_code);

}   // end of namespace mpi
}   // end of namespace utils

#endif /* end of include guard: MPIEXCEPTION_H */
