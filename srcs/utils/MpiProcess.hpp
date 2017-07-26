// C/C++ File

// Author:   Alexandre Tea <alexandre.qtea@gmail.com>
// File:     /Users/alexandretea/Work/decision-tree-distributed-learning/srcs/utils/MpiProcess.hpp
// Purpose:  TODO (a one-line explanation)
// Created:  2017-07-26 17:51:48
// Modified: 2017-07-26 18:26:39

#ifndef MPIPROCESS_H
#define MPIPROCESS_H

#include <string>
#include <mpi.h>
#include <ostream>

namespace utils {
namespace mpi {

class Process
{
    public:
        Process(MPI_Comm const& communicator = MPI_COMM_WORLD);
        ~Process();

        Process(Process const& other) = delete;
        Process&    operator=(Process const& other) = delete;

    public:
        int                 get_rank() const;
        int                 get_comm_size() const;
        std::string const&  get_name() const;

    protected:
        int         _rank;
        int         _comm_size;
        std::string _name;
};

std::ostream&
operator<<(std::ostream& s, Process const& other);

}
}

#endif /* end of include guard: MPIPROCESS_H */
