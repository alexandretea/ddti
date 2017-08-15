// C/C++ File

// Author:   Alexandre Tea <alexandre.qtea@gmail.com>
// File:     /Users/alexandretea/Work/ddti/srcs/master/Dataset.hpp
// Purpose:  TODO (a one-line explanation)
// Created:  2017-08-09 14:19:43
// Modified: 2017-08-15 19:09:17

#ifndef DATASET_H
#define DATASET_H

#include <vector>
#include <unordered_map>
#include <mlpack/core.hpp>

namespace ddti {

template <typename T>
class Dataset
{
    public:
        using Mappings = std::vector<std::unordered_map<size_t, std::string>>;
        using SubviewNC = arma::subview_elem2<T, arma::Mat<unsigned long long>,
                                              arma::Mat<unsigned long long>>;

    public:
        Dataset() : _matrix(), _mappings() {}
        ~Dataset() {}

        Dataset(arma::Mat<T> const& mat, ssize_t labelsdim = -1,
                ::mlpack::data::DatasetInfo const* info = nullptr)
            : _matrix(mat), _mappings()
        {
            _labels_dimension = (labelsdim == -1 ? mat.n_rows - 1 : labelsdim);
            if (info != nullptr)
                init_mappings(*info);
        }

        Dataset(Dataset const& o)
            : _matrix(o._matrix), _mappings(o._mappings),
              _labels_dimension(o._labels_dimension) {}

        Dataset&
        operator=(Dataset const& o)
        {
            if (this != &o) {
                _matrix = o._matrix;
                _mappings = o._mappings;
                _labels_dimension = o._labels_dimension;
            }
            return *this;
        }

    public:
        size_t
        n_rows() const
        {
            return _matrix.n_rows;
        }

        size_t
        n_cols() const
        {
            return _matrix.n_cols;
        }

        arma::subview<T>
        subview(size_t begin_row, size_t begin_col,
                size_t end_row, size_t end_col) const
        {
            return _matrix.submat(begin_row, begin_col, end_row, end_col);
        }

        Dataset<T>::SubviewNC
        subview(std::vector<unsigned long long> const& cols)
        {
            return _matrix.cols(arma::uvec(cols));
        }

        arma::Row<T> const&
        row(size_t row) const
        {
            return _matrix.row(row);
        }

        arma::Col<T> const&
        col(size_t col) const
        {
            return _matrix.col(col);
        }

        T const&
        get(size_t row, size_t col) const
        {
            return _matrix(row, col);
        }

        size_t
        labelsdim() const
        {
            return _labels_dimension;
        }

        arma::Mat<T> const&
        matrix() const
        {
            return _matrix;
        }

        void
        init_mappings(::mlpack::data::DatasetInfo const& info)
        {
            _mappings = Dataset::get_mappings(info);
        }

        size_t
        mapping_size(size_t dim) const
        {
            return _mappings.at(dim).size();
        }

        std::vector<size_t>
        mapping_sizes() const
        {
            std::vector<size_t>     mapping_sizes(_mappings.size());

            for (unsigned int dim = 0; dim < _mappings.size(); ++dim) {
                mapping_sizes[dim] = _mappings.at(dim).size();
            }
            return mapping_sizes;
        }

        void
        debug_mappings(std::ostream& os = std::cout) const
        {
            for (unsigned int dim = 0; dim < _mappings.size(); ++dim) {
                os << dim << ": ";
                for (auto& mapping: _mappings[dim]) {
                    os << mapping.second << "(" << mapping.first << ") ";
                }
                os << std::endl;
            }
        }

        // NOTE: Strongly affect performance, should only be used for debug
        // Verify integrity of data with the dataset mappings
        bool
        validate(arma::Mat<T> const& view)
        {
            // loop on entries
            for (unsigned int i = 0; i < view.n_cols; ++i) {
                arma::Col<T>    entry(view.col(i));

                // loop on dimensions
                for (unsigned int dim = 0; dim < entry.n_elem; ++dim) {

                    if (dim >= _mappings.size())
                        return false;

                    std::unordered_map<size_t, std::string> const& m =
                        _mappings.at(dim);

                    if (m.find(entry(dim)) == m.end())
                        return false;
                }
            }
            return true;
        }

        std::string
        dump_col(size_t n, std::string const& sep = ",") const
        {
            return Dataset::dump_entry(col(n), _mappings, sep);
        }

        std::string
        dump_row(size_t n, std::string const& sep = ",") const
        {
            return Dataset::dump_entry(row(n), _mappings, sep);
        }

        template <typename EntryType>   // to use with Col<T> or Row<T>
        static std::string
        dump_entry(EntryType const& entry, Mappings const& mappings,
                   std::string const& sep = ",")
        {
            std::string res;

            for (unsigned int dim = 0; dim < entry.n_elem; ++dim) {
                try {
                    res += mappings.at(dim).at(entry(dim));
                } catch (...) {
                    res += "err(" + std::to_string(dim) + ","
                        + std::to_string(entry(dim)) + ")";
                }
                if (dim < entry.n_elem - 1)
                    res += sep;
            }
            return res;
        }

        // DatasetInfo is received as a copy bc DatasetInfo::UnmapString
        // is not const...
        static Dataset::Mappings
        get_mappings(::mlpack::data::DatasetInfo /* const& */ info)
        {
            Mappings mappings(info.Dimensionality());

            for (size_t dim = 0; dim < info.Dimensionality(); ++dim) {
                for (size_t value = 0; value < info.NumMappings(dim); ++value) {
                    mappings[dim][value] = info.UnmapString(value, dim);
                }
            }
            return mappings;
        }

    protected:
        arma::Mat<T>    _matrix;
        Mappings        _mappings;
        size_t          _labels_dimension;
};

}

#endif /* end of include guard: DATASET_H */
