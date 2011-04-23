// Copyright 2008 Edd Dawson.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file BOOST_LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <jpegxx/exceptions/size_format_limitation.hpp>

namespace jpegxx
{
    size_format_limitation::size_format_limitation(const imagexx::raster_details &details) :
        imagexx::format_limitation(details)
    {
    }

    const char *size_format_limitation::what() const throw()
    {
        return "JPEG-compressed images are restricted to 65535 by 65535 pixels";
    }


} // close namespace jpegxx
