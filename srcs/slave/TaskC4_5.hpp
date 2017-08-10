// C/C++ File

// Author:   Alexandre Tea <alexandre.qtea@gmail.com>
// File:     /Users/alexandretea/Work/ddti/srcs/slave/TaskC4_5.hpp
// Purpose:  TODO (a one-line explanation)
// Created:  2017-08-02 18:43:30
// Modified: 2017-08-10 22:00:53

#ifndef TASKC4_5_H
#define TASKC4_5_H

#include <unordered_map>
#include <string>
#include "task.hpp"
#include "MpiCommunicator.hpp"
#include "ddti.hpp"

namespace ddti {
namespace task {

class C4_5
{
    public:
        static const int    AttrSelectCode;
        typedef void        (C4_5::*TaskFn)();

    public:
        C4_5(utils::mpi::Communicator const& comm);
        virtual ~C4_5();

        C4_5(C4_5 const& other) = delete;
        C4_5&   operator=(C4_5 const& other) = delete;

    public:
        void        operator()(int task_code);
        std::string name() const;

        void        count_contingencies(arma::mat const& data) const;
        void        count_contingencies(arma::mat const& data, size_t labelsdim,
                                        std::vector<size_t> const& dim_values,
                                        std::map<size_t, ContTable>* output
                                            = nullptr) const;

    protected:
        void        count_contingencies();

    protected:
        utils::mpi::Communicator const&     _comm;
        std::unordered_map<int, TaskFn>     _tasks;
};

}   // end of namespace task
}   // end of namespace ddti

#endif /* end of include guard: TASKC4_5_H */

