// C/C++ File

// Author:   Alexandre Tea <alexandre.qtea@gmail.com>
// File:     /Users/alexandretea/Work/ddti/srcs/master/Dataset.hpp
// Purpose:  Class that represents a dataset
// Created:  2017-08-09 14:19:43
// Modified: 2017-08-23 23:13:21

#ifndef DATASET_H
#define DATASET_H

#include <vector>
#include <unordered_map>
#include <mlpack/core.hpp>

namespace ddti {

using DatasetMappings = std::vector<std::unordered_map<size_t, std::string>>;

template <typename T = unsigned int>
class Dataset
{
    public:
        using SubviewNC = arma::subview_elem2<T, arma::Mat<unsigned long long>,
                                              arma::Mat<unsigned long long>>;

    public:
        Dataset() {}
        ~Dataset() {}

        Dataset(arma::Mat<T> const& mat, ssize_t labelsdim = -1,
                std::vector<std::string> const& attr_names = {},
                ::mlpack::data::DatasetInfo const* info = nullptr)
            : _matrix(mat), _attr_names(attr_names), _mappings()
        {
            // set label dimension to the last dimension if none provided
            _labels_dimension = (labelsdim == -1 ? mat.n_rows - 1 : labelsdim);

            // load mappings from DatasetInfo if provided
            if (info != nullptr)
                init_mappings(*info);

            // set attribute names if none provided
            if (_attr_names.empty()) {
                _attr_names.reserve(_matrix.n_rows);
                for (unsigned int i = 0; i < _matrix.n_rows; ++i) {
                    _attr_names.push_back(std::to_string(i));
                }
            }
        }

        Dataset(Dataset const& o)
            : _matrix(o._matrix),
              _attr_names(o._attr_names),
              _mappings(o._mappings),
              _labels_dimension(o._labels_dimension) {}

        Dataset&
        operator=(Dataset const& o)
        {
            if (this != &o) {
                _matrix = o._matrix;
                _mappings = o._mappings;
                _labels_dimension = o._labels_dimension;
                _attr_names = o._attr_names;
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

        size_t
        size() const
        {
            return n_cols();
        }

        arma::subview<T>
        subview(size_t begin_row, size_t begin_col,
                size_t end_row, size_t end_col) const
        {
            return _matrix.submat(begin_row, begin_col, end_row, end_col);
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

        DatasetMappings const&
        mappings() const
        {
            return _mappings;
        }

        std::string const&
        mapping(size_t attribute, size_t value) const
        {
            return _mappings.at(attribute).at(value);
        }

        std::string const&
        attribute_name(size_t dim) const
        {
            return _attr_names.at(dim);
        }

        void
        debug_mappings(std::ostream& os = std::cout) const
        {
            for (unsigned int dim = 0; dim < _mappings.size(); ++dim) {
                os << _attr_names[dim] << ": ";
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
        dump_entry(EntryType const& entry, DatasetMappings const& mappings,
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
        static DatasetMappings
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

    protected:
        arma::Mat<T>                _matrix;
        std::vector<std::string>    _attr_names;
        DatasetMappings             _mappings;
        size_t                      _labels_dimension;
};

}

#endif /* end of include guard: DATASET_H */
