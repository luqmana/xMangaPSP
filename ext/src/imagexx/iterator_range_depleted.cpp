// Copyright 2008 Edd Dawson.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file BOOST_LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include "imagexx/exceptions/iterator_range_depleted.hpp"

namespace imagexx
{
    iterator_range_depleted::iterator_range_depleted(const char *what) :
        source_depleted(),
        std::runtime_error(what)
    {
    }

    iterator_range_depleted::~iterator_range_depleted() throw()
    {
    }


} // close namespace imagexx

