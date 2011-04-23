// Copyright 2008 Edd Dawson.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file BOOST_LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include "imagexx/exceptions/compression_failure.hpp"

namespace imagexx
{
    compression_failure::compression_failure(const char *advanced) :
        codec_failure("The codec failed to compress the raster"),
        advanced_(advanced)
    {
    }

    compression_failure::~compression_failure() throw()
    {
    }

    const char *compression_failure::advanced() const
    {
        return advanced_.c_str();
    }


} // close namespace imagexx

