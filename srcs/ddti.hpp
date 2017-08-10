// C/C++ File

// Author:   Alexandre Tea <alexandre.qtea@gmail.com>
// File:     /Users/alexandretea/Work/ddti/srcs/ddti_log.hpp
// Purpose:  TODO (a one-line explanation)
// Created:  2017-07-30 16:43:19
// Modified: 2017-08-10 20:05:44

#ifndef DDTI_H
#define DDTI_H

#include <mlpack/core.hpp>
#include "utils/log.hpp"

namespace ddti {

using MlpackLogger  = utils::log::Logger<mlpack::util::PrefixedOutStream>;
using ContTable     = arma::Mat<unsigned int>;  // Contingency table

extern MlpackLogger   Logger;

}

#endif /* end of include guard: DDTI_H */
