// C/C++ File

// Author:   Alexandre Tea <alexandre.qtea@gmail.com>
// File:     /Users/alexandretea/Work/ddti/srcs/master/MasterNode.hpp
// Purpose:  TODO (a one-line explanation)
// Created:  2017-07-26 18:51:03
// Modified: 2017-07-26 19:30:26

#ifndef MASTERNODE_H
#define MASTERNODE_H

#include <mlpack/core.hpp>
#include "ANode.hpp"

namespace ddti {

class MasterNode : public ANode
{
    public:
        MasterNode(utils::mpi::CommProcess const& process,
                   std::string const& infile);
        virtual ~MasterNode();

        MasterNode(MasterNode const& other) = delete;
        MasterNode&     operator=(MasterNode const& other) = delete;

    public:
        virtual void    run();

    protected:
        std::string     _infile;
};

std::ostream&
operator<<(std::ostream& s, MasterNode const& other);

}


#endif /* end of include guard: MASTERNODE_H */
