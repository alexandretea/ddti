// C/C++ File

// Author:   Alexandre Tea <alexandre.qtea@gmail.com>
// File:     /Users/alexandretea/Work/ddti/srcs/utils/log.hpp
// Purpose:  TODO (a one-line explanation)
// Created:  2017-07-30 15:42:02
// Modified: 2017-08-06 16:04:44

#ifndef LOG_H
#define LOG_H

#include <iostream>
#include <string>
#include "utils/datetime.hpp"

namespace utils {
namespace log {

template <typename StreamType>
class Logger
{
    public:
        Logger()
            : _id(""), _outstream(nullptr) {}

        Logger(std::string const& id, StreamType* outstream = nullptr)
            : _id(id), _outstream(outstream)
        {}

        Logger(StreamType* outstream, std::string const& id = "")
            : _id(id), _outstream(outstream)
        {}

        virtual ~Logger() {}

        Logger(Logger const& other)
            : _id(other._id), _outstream(other._outstream) {}

        Logger<StreamType>&
        operator=(Logger const& other)
        {
            if (this != &other) {
                _id = other._id;
                _outstream = other._outstream;
            }
            return *this;
        }

    public:
        Logger<StreamType>&
        operator<<(std::string const& msg)
        {
            log(msg);
            return *this;
        }

        bool
        log(std::string const& msg)
        {
            if (_outstream == nullptr)
                return false;
            log(msg, *_outstream);
            return true;
        }

        void
        log(std::string const& msg, StreamType& s)
        {
            s << format(msg) << std::endl;
        }

        void
        set_id(std::string const& id)
        {
            _id = id;
        }

        void
        set_stream(StreamType& s)
        {
            _outstream = &s;
        }

        std::string const&
        get_id() const
        {
            return _id;
        }

        StreamType const&
        get_stream() const
        {
            return _outstream;
        }

        virtual std::string
        format(std::string const& msg)
        {
            std::string formatted_msg("[");

            formatted_msg += utils::datetime::now_str("%d.%m.%Y %T") + "]";
            if (not _id.empty())
                formatted_msg += " " + _id + ": ";
            return formatted_msg + " " + msg;
        }

    protected:
        std::string _id;
        StreamType* _outstream;
};

typedef Logger<std::ostream>    StdLogger;

}   // end of namespace log
}   // end of namespace utils

#endif /* end of include guard: LOG_H */
