// Copyright 2008 Edd Dawson.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file BOOST_LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include "imagexx/exceptions/stream_io_failure.hpp"

namespace imagexx
{
    stream_io_failure::stream_io_failure(std::ios_base::iostate state, const char *what) :
        std::ios_base::failure(what),
        state_(state)
    {
    }

    stream_io_failure::~stream_io_failure() throw()
    {
    }

    std::ios_base::iostate stream_io_failure::state() const
    {
        return state_;
    }


} // close namespace imagexx

