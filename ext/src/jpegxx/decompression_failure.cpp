// Copyright 2008 Edd Dawson.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file BOOST_LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <jpegxx/exceptions/decompression_failure.hpp>

namespace jpegxx
{
    decompression_failure::decompression_failure(const char *advanced) :
        imagexx::decompression_failure(advanced)
    {
    }

    const char *decompression_failure::what() const throw()
    {
        return "The JPEG codec failed to decompress the encoded raster data";
    }



} // close namespace jpegxx
