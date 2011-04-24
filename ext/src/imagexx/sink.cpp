// Copyright 2008 Edd Dawson.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file BOOST_LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include "imagexx/sink.hpp"

namespace imagexx
{
    sink::sink()
    {
    }

    sink::~sink()
    {
    }

    void sink::write(const unsigned char *buff, std::size_t num_bytes)
    {
        write_impl(buff, num_bytes);
    }

} // close namespace imagexx
