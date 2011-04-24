// Copyright 2008 Edd Dawson.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file BOOST_LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <jpegxx/exceptions/alpha_format_limitation.hpp>

namespace jpegxx
{
    alpha_format_limitation::alpha_format_limitation(const imagexx::raster_details &details) :
        imagexx::format_limitation(details)
    {
    }

    const char *alpha_format_limitation::what() const throw()
    {
        return "JPEG-compressed images do not support alpha";
    }


} // close namespace jpegxx
