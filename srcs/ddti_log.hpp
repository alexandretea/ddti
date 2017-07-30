// C/C++ File

// Author:   Alexandre Tea <alexandre.qtea@gmail.com>
// File:     /Users/alexandretea/Work/ddti/srcs/ddti_log.hpp
// Purpose:  TODO (a one-line explanation)
// Created:  2017-07-30 16:43:19
// Modified: 2017-07-30 17:31:21

#ifndef DDTI_LOG_H
#define DDTI_LOG_H

#include <mlpack/core.hpp>
#include "utils/log.hpp"

namespace ddti {

typedef utils::log::Logger<mlpack::util::PrefixedOutStream> MlpackLogger;

extern MlpackLogger   Logger;

}

#endif /* end of include guard: DDTI_LOG_H */
