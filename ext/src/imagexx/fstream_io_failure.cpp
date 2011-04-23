// Copyright 2008 Edd Dawson.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file BOOST_LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include "imagexx/exceptions/fstream_io_failure.hpp"

namespace imagexx
{
    fstream_io_failure::fstream_io_failure(std::ios_base::iostate state, const std::string &filename, const char *what) :
        stream_io_failure(state, what),
        filename_(filename)
    {
    }

    fstream_io_failure::~fstream_io_failure() throw()
    {
    }

    const char *fstream_io_failure::filename() const
    {
        return filename_.c_str();
    }


} // close namespace imagexx

