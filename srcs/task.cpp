// C/C++ File

// Author:   Alexandre Tea <alexandre.qtea@gmail.com>
// File:     /Users/alexandretea/Work/ddti/srcs/task.cpp
// Purpose:  TODO (a one-line explanation)
// Created:  2017-08-02 18:33:06
// Modified: 2017-08-04 16:01:20

#include "task.hpp"

namespace ddti {
namespace task {

const int End = 0;

ATask::ATask()
{
}

ATask::~ATask()
{
}

void
ATask::operator()(int task_code)
{
    if (_tasks.find(task_code) == _tasks.end())
        throw std::runtime_error("Cannot find task: "
                                 + std::to_string(task_code));
    (this->*_tasks[task_code])();
}


}   // end of namespace task
}   // end of namespace ddti

