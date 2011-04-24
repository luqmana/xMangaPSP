// Copyright 2008 Edd Dawson.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file BOOST_LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include "imagexx/exceptions/decompression_failure.hpp"

namespace imagexx
{
    decompression_failure::decompression_failure(const char *advanced) :
        codec_failure("The codec failed to decompress the encoded raster data"),
        advanced_(advanced)
    {
    }

    decompression_failure::~decompression_failure() throw()
    {
    }

    const char *decompression_failure::advanced() const
    {
        return advanced_.c_str();
    }


} // close namespace imagexx

