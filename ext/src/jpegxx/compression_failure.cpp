// Copyright 2008 Edd Dawson.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file BOOST_LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <jpegxx/exceptions/compression_failure.hpp>

namespace jpegxx
{
    compression_failure::compression_failure(const char *advanced) :
        imagexx::compression_failure(advanced)
    {
    }

    const char *compression_failure::what() const throw()
    {
        return "The JPEG codec failed to compress the raster";
    }

} // close namespace jpegxx
