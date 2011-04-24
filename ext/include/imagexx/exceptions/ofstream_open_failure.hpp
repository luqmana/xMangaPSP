// Copyright 2008 Edd Dawson.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file BOOST_LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef OFSTREAM_OPEN_FAILURE_HPP_0052_15112008
#define OFSTREAM_OPEN_FAILURE_HPP_0052_15112008

#include "imagexx/exceptions/fstream_open_failure.hpp"

namespace imagexx
{
    //! A simple wrapper around fstream_open_failure to indicate by type that a file could not be opened for writing. The interface is
    //! the same as that of fstream_open_failure.
    class ofstream_open_failure : public fstream_open_failure
    {
        public:
            //! Creates an exception object that stores the given filename and mode of opening for retrieval at a catch-site.
            ofstream_open_failure(const std::string &filename, std::ios_base::openmode mode);

            //! Provides polymorphic destruction
            ~ofstream_open_failure() throw();

            IMAGEXX_DEFINE_AUTO_EXCEPTION_IMPL_MEMBERS(ofstream_open_failure)
    };

} // close namespace imagexx

#endif // OFSTREAM_OPEN_FAILURE_HPP_0052_15112008

