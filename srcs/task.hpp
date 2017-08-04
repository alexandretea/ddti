// C/C++ File

// Author:   Alexandre Tea <alexandre.qtea@gmail.com>
// File:     /Users/alexandretea/Work/ddti/srcs/task.hpp
// Purpose:  TODO (a one-line explanation)
// Created:  2017-08-02 18:31:24
// Modified: 2017-08-03 16:03:17

#ifndef TASK_H
#define TASK_H

#include <unordered_map>
#include <string>

namespace ddti {
namespace task {

extern const int End;

class ATask
{
    public:
        typedef void    (ATask::*TaskFn)();

    protected:
        ATask();
        virtual ~ATask();

    public:
        ATask(ATask const& other) = delete;
        ATask&  operator=(ATask const& other) = delete;

        virtual void        operator()(int task_code);
        virtual std::string name() const = 0;

    protected:
        std::unordered_map<int, TaskFn>   _tasks;
};

}   // end of namespace task
}   // end of namespace ddti

#endif /* end of include guard: TASK_H */
