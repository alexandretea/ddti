// C/C++ File

// Author:   Alexandre Tea <alexandre.qtea@gmail.com>
// File:     /Users/alexandretea/Work/ddti/srcs/utils/datetime.cpp
// Purpose:  TODO (a one-line explanation)
// Created:  2017-07-29 22:52:26
// Modified: 2017-08-23 19:05:22

#include <ctime>
#include "datetime.hpp"

namespace utils {
namespace datetime {

std::string
now_str(char const* format)
{
    std::time_t     t = std::time(0);
    char            cstr[128];

    std::strftime(cstr, sizeof(cstr), format, std::localtime(&t));
    return cstr;
}

// Timer class
Timer::Timer()
    : _start(chrono::system_clock::now()),
      _end(chrono::system_clock::now())
{
}

Timer::~Timer()
{
}

void
Timer::start()
{
    _start = chrono::system_clock::now();
    _end = chrono::system_clock::now();
}

void
Timer::stop()
{
    _end = chrono::system_clock::now();
}

void
Timer::reset()
{
    _start = chrono::system_clock::now();
    _end = chrono::system_clock::now();
}

}   // end of namespace datetime
}   // end of namespace utils
