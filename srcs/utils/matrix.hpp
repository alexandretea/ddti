// C/C++ File

// Author:   Alexandre Tea <alexandre.qtea@gmail.com>
// File:     /Users/alexandretea/Work/ddti/srcs/utils/matrix.hpp
// Purpose:  matrix utilities
// Created:  2017-08-14 16:44:51
// Modified: 2017-08-23 23:19:40

#ifndef MATRIX_H
#define MATRIX_H

#include <armadillo>

namespace utils {
namespace matrix {

template <typename T>
arma::Mat<T>
load_arma_matrix(T* buffer, size_t nb_elems, size_t nb_entries,
                 bool column_major = true, bool free_buffer = true)
{
    arma::Mat<T>    matrix(buffer, nb_elems, nb_entries);

    if (not column_major) {
        // armadillo matrices are column-major and the matrix we
        // received is row-major, so we transpose it
        matrix = matrix.t();
    }
    if (free_buffer)
        delete buffer;
    return matrix;
}

}   // end of namespace matrix
}   // end of namespace utils

#endif /* end of include guard: MATRIX_H */
