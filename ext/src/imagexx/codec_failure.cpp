// Copyright 2008 Edd Dawson.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file BOOST_LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include "imagexx/exceptions/codec_failure.hpp"

namespace imagexx
{
    codec_failure::codec_failure(const char *what) :
        std::runtime_error(what)
    {
    }

    codec_failure::~codec_failure() throw()
    {
    }


} // close namespace imagexx

