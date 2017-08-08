// C/C++ File

// Author:   Alexandre Tea <alexandre.qtea@gmail.com>
// File:     /Users/alexandretea/Work/ddti/srcs/utils/mlpack.hpp
// Purpose:  TODO (a one-line explanation)
// Created:  2017-08-07 12:51:30
// Modified: 2017-08-08 13:26:22

#ifndef MLPACK_H
#define MLPACK_H

namespace utils {
namespace mlpack {

#include <vector>
#include <unordered_map>
#include <mlpack/core.hpp>

typedef std::vector<std::unordered_map<size_t, std::string>>    DatasetMappings;

DatasetMappings     get_mappings(::mlpack::data::DatasetInfo /* const& */ info);
// DatasetInfo is received as a copy bc DatasetInfo::UnmapString is not const...
std::string         dump_col(arma::colvec const& col,
                             DatasetMappings const& mappings,
                             std::string const& sep = ",");
void                debug_mappings(DatasetMappings const& mappings);

}   // end of namespace mlpack
}   //.end of namespace utils

#endif /* end of include guard: MLPACK_H */
