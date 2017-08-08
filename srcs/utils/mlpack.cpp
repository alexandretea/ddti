// C/C++ File

// Author:   Alexandre Tea <alexandre.qtea@gmail.com>
// File:     /Users/alexandretea/Work/ddti/srcs/utils/mlpack.cpp
// Purpose:  TODO (a one-line explanation)
// Created:  2017-08-07 13:01:00
// Modified: 2017-08-08 13:25:05

#include <vector>
#include <unordered_map>
#include <mlpack/core.hpp>
#include "mlpack.hpp"

namespace utils {
namespace mlpack {

DatasetMappings
get_mappings(::mlpack::data::DatasetInfo /* const& */ info)
{
    DatasetMappings mappings(info.Dimensionality());

    for (size_t dim = 0; dim < info.Dimensionality(); ++dim) {
        for (size_t value = 0; value < info.NumMappings(dim); ++value) {
            mappings[dim][value] = info.UnmapString(value, dim);
        }
    }
    return mappings;
}

void
debug_mappings(DatasetMappings const& mappings)
{
    for (unsigned int dim = 0; dim < mappings.size(); ++dim) {
        std::cout << dim << ": ";
        for (auto& mapping: mappings[dim]) {
            std::cout << mapping.second << "(" << mapping.first << ") ";
        }
        std::cout << std::endl;
    }
}

std::string
dump_col(arma::colvec const& col,
         DatasetMappings const& mappings,
         std::string const& sep)
{
    std::string res;

    for (unsigned int dim = 0; dim < col.n_elem; ++dim) {
        try {
            res += mappings.at(dim).at(col(dim));
        } catch (...) {
            res += "err(" + std::to_string(dim) + ","
                + std::to_string(col(dim)) + ")";
        }
        if (dim < col.n_elem - 1)
            res += sep;
    }
    return res;
}

}   // end of namespace mlpack
}   //.end of namespace utils
