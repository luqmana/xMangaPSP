// Copyright 2008 Edd Dawson.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file BOOST_LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include "imagexx/exceptions/fstream_depleted.hpp"

namespace imagexx
{
    fstream_depleted::fstream_depleted(std::ios_base::iostate state, const std::string &filename, const char *what) :
        stream_io_failure(state, what),
        fstream_io_failure(state, filename, what),
        stream_depleted(state, what)
    {
    }

    fstream_depleted::~fstream_depleted() throw()
    {
    }

} // close namespace imagexx

