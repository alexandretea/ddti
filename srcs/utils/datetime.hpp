// C/C++ File

// Author:   Alexandre Tea <alexandre.qtea@gmail.com>
// File:     /Users/alexandretea/Work/ddti/srcs/utils/datetime.hpp
// Purpose:  TODO (a one-line explanation)
// Created:  2017-07-29 22:50:21
// Modified: 2017-08-23 19:17:02

#ifndef DATETIME_H
#define DATETIME_H

#include <string>
#include <chrono>

namespace utils {
namespace datetime {

namespace chrono = std::chrono;

std::string	now_str(char const* format = "%c");

class Timer
{
    protected:

    public:
        Timer();
        ~Timer();

        Timer(Timer const& o) = delete;
        Timer&  operator=(Timer const& o) = delete;

    public:
        void    start();
        void    stop();
        void    reset();

        // get elapsed time in seconds
        template <typename T>
        T
        get() const
        {
            chrono::duration<T> d = _end - _start;

            return d.count();
        }

    protected:
        chrono::time_point<chrono::system_clock>    _start;
        chrono::time_point<chrono::system_clock>    _end;
};
//start = std::chrono::system_clock::now();
//std::cout << "f(42) = " << fibonacci(42) << '\n';
//end = std::chrono::system_clock::now();

//std::chrono::duration<double> elapsed_seconds = end-start;
//std::time_t end_time = std::chrono::system_clock::to_time_t(end);

//std::cout << "finished computation at " << std::ctime(&end_time)
//<< "elapsed time: " << elapsed_seconds.count() << "s\n";

}   // end of namespace datetime
}   // end of namespace utils

#endif /* end of include guard: DATETIME_H */
