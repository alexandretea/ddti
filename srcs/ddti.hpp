// C/C++ File

// Author:   Alexandre Tea <alexandre.qtea@gmail.com>
// File:     /Users/alexandretea/Work/ddti/srcs/ddti_log.hpp
// Purpose:  ddti's variable definitions and typedefs
// Created:  2017-07-30 16:43:19
// Modified: 2017-08-23 23:12:04

#ifndef DDTI_H
#define DDTI_H

#include <mlpack/core.hpp>
#include "utils/log.hpp"

namespace ddti {

using MlpackLogger  = utils::log::Logger<mlpack::util::PrefixedOutStream>;
using ContTable     = arma::Mat<unsigned int>;
// Contingency table
// NOTE: columns always represent labels and rows represent an attribute
// values

extern MlpackLogger   Logger;

}

#endif /* end of include guard: DDTI_H */
