// C/C++ File

// Author:   Alexandre Tea <alexandre.qtea@gmail.com>
// File:     /Users/alexandretea/Work/ddti/srcs/slave/TaskC4_5.hpp
// Purpose:  TODO (a one-line explanation)
// Created:  2017-08-02 18:43:30
// Modified: 2017-08-02 18:49:36

#ifndef TASKC4_5_H
#define TASKC4_5_H

#include <string>

namespace ddti {
namespace task {

class C4_5
{
    public:
        C4_5();
        virtual ~C4_5();

        C4_5(C4_5 const& other) = delete;
        C4_5&   operator=(C4_5 const& other) = delete;

    public:
        std::string name() const;
        void operator()(int task_code);
};

}   // end of namespace task
}   // end of namespace ddti

#endif /* end of include guard: TASKC4_5_H */

