// Copyright 2008 Edd Dawson.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file BOOST_LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef FSTREAM_DEPLETED_HPP_0052_15112008
#define FSTREAM_DEPLETED_HPP_0052_15112008

#include <string>

#include "imagexx/exceptions/stream_depleted.hpp"
#include "imagexx/exceptions/fstream_io_failure.hpp"

namespace imagexx
{
    //! An exception of this type is thrown when an ifstream contains less than the expected amount of data
    class fstream_depleted : public fstream_io_failure, public stream_depleted
    {
        public:
            //! Creates an fstream_depleted object. filename should be the name of the file that the problematic stream represents.
            fstream_depleted(std::ios_base::iostate state, const std::string &filename, const char *what = "file stream depleted");

            //! Provides polymorphic destruction
            ~fstream_depleted() throw();

            IMAGEXX_DEFINE_AUTO_EXCEPTION_IMPL_MEMBERS(fstream_depleted)
    };

} // close namespace imagexx

#endif // FSTREAM_DEPLETED_HPP_0052_15112008

