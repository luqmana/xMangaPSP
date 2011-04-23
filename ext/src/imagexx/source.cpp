// Copyright 2008 Edd Dawson.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file BOOST_LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include "imagexx/source.hpp"

namespace imagexx
{
    source::source()
    {
    }

    source::~source()
    {
    }

    std::size_t source::read(unsigned char *buff, std::size_t at_most)
    {
        return read_impl(buff, at_most);
    }

    std::size_t source::skip(std::size_t at_most)
    {
        return skip_impl(at_most);
    }

} // close namespace imagexx
