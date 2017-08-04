// C/C++ File

// Author:   Alexandre Tea <alexandre.qtea@gmail.com>
// File:     /Users/alexandretea/Work/ddti/srcs/utils/mpi/MpiDatatypeManager.cpp
// Purpose:  TODO (a one-line explanation)
// Created:  2017-08-05 00:07:16
// Modified: 2017-08-05 00:43:14

#include <mpi.h>
#include "MpiDatatypeManager.hpp"
#include "MpiDatatype.hpp"

namespace utils {
namespace mpi {
namespace datatype {

Manager::Manager()
{}

Manager::~Manager()
{
    clear();
}

std::string
Manager::commit(MPI_Datatype type)
{
    std::string name;
    int         error_code;

    if ((error_code = MPI_Type_commit(&type)) != MPI_SUCCESS)
        throw mpi::Exception(error_code);
    name = datatype::get_name(type);
    _datatypes[name] = type;
    return name;
}

bool
Manager::free(std::string const& id)
{
    auto it = _datatypes.find(id);
    int  error_code;

    if (it == _datatypes.end())
        return false;
    if ((error_code = MPI_Type_free(&(it->second))) != MPI_SUCCESS)
        throw mpi::Exception(error_code);
    _datatypes.erase(it);
    return true;
}

void
Manager::clear()
{
    int  error_code;

    for (auto& type: _datatypes) {
        if ((error_code = MPI_Type_free(&(type.second))) != MPI_SUCCESS)
            throw mpi::Exception(error_code);
    }
    _datatypes.clear();
}

MPI_Datatype const&
Manager::find(std::string const& name) const
{
    return _datatypes.at(name);
}

}   // end namespace datatype
}   // end namespace mpi
}   // end namespace utils

