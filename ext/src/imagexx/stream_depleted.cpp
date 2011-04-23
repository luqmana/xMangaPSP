// Copyright 2008 Edd Dawson.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file BOOST_LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include "imagexx/exceptions/stream_depleted.hpp"

namespace imagexx
{
    stream_depleted::stream_depleted(std::ios_base::iostate state, const char *what) :
        stream_io_failure(state, what)
    {
    }

    stream_depleted::~stream_depleted() throw()
    {
    }


} // close namespace imagexx

