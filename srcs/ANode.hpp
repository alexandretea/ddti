// C/C++ File

// Author:   Alexandre Tea <alexandre.qtea@gmail.com>
// File:     /Users/alexandretea/Work/ddti/srcs/DdtiANode.hpp
// Purpose:  TODO (a one-line explanation)
// Created:  2017-07-26 18:53:59
// Modified: 2017-07-30 15:49:13

#ifndef DDTIANODE_H
#define DDTIANODE_H

#include <mlpack/core.hpp>
#include "utils/MpiCommProcess.hpp"

namespace ddti {

class ANode
{
    // ANode is abstract and can only be instanciated through its child classes
    protected:
        ANode(utils::mpi::CommProcess const& process);

    public:
        virtual ~ANode();

        ANode(ANode const& other) = delete;
        ANode&          operator=(ANode const& other) = delete;

    public:
        virtual void        run() = 0;
        size_t              get_id() const;
        size_t              get_nb_slaves() const;
        bool                is_master() const;
        bool                is_slave() const;
        std::string const&  get_name() const;

    protected:
        utils::mpi::CommProcess const&  _comm_process;
        std::string                     _name;
};

template <typename IA>
std::ostream&
operator<<(std::ostream& s, ANode const& other);

} // end namespace ddti

#endif /* end of include guard: DDTIANODE_H */
