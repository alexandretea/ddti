// C/C++ File

// Author:   Alexandre Tea <alexandre.qtea@gmail.com>
// File:     /Users/alexandretea/Work/ddti/srcs/utils/MpiDatatype.hpp
// Purpose:  TODO (a one-line explanation)
// Created:  2017-08-02 17:15:48
// Modified: 2017-08-06 00:47:38

#ifndef MPIDATATYPE_H
#define MPIDATATYPE_H

#include <string>
#include <mpi.h>
#include "MpiException.hpp"

namespace utils {
namespace mpi {
namespace datatype {

// Helper functions to translate C++ types into MPI datatypes
// Simpler and lighter version of BoostMPI datatype conversion
template <typename T>
MPI_Datatype    get();

#define MPI_IMPL_GET_DATATYPE(cxxtype, mpitype) \
template <>                                 	\
inline MPI_Datatype                        		\
get<cxxtype>() { return mpitype; }

MPI_IMPL_GET_DATATYPE(char, MPI_CHAR);
MPI_IMPL_GET_DATATYPE(short, MPI_SHORT);
MPI_IMPL_GET_DATATYPE(int, MPI_INT);
MPI_IMPL_GET_DATATYPE(long, MPI_LONG);
MPI_IMPL_GET_DATATYPE(float, MPI_FLOAT);
MPI_IMPL_GET_DATATYPE(double, MPI_DOUBLE);
MPI_IMPL_GET_DATATYPE(long double, MPI_LONG_DOUBLE);
MPI_IMPL_GET_DATATYPE(unsigned char, MPI_UNSIGNED_CHAR);
MPI_IMPL_GET_DATATYPE(unsigned short, MPI_UNSIGNED_SHORT);
MPI_IMPL_GET_DATATYPE(unsigned, MPI_UNSIGNED);
MPI_IMPL_GET_DATATYPE(unsigned long, MPI_UNSIGNED_LONG);
MPI_IMPL_GET_DATATYPE(unsigned long long, MPI_UNSIGNED_LONG_LONG);

std::string  get_name(MPI_Datatype const& datatype);

}   // end namespace datatype
}   // end namespace mpi
}   // end namespace utils

#endif /* end of include guard: MPIDATATYPE_H */
