// Copyright 2008 Edd Dawson.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file BOOST_LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include "imagexx/exceptions/format_limitation.hpp"

namespace imagexx
{
    format_limitation::format_limitation(const raster_details &details) :
        codec_failure("A codec limit or restriction was encountered"),
        details_(details)
    {
    }

    format_limitation::~format_limitation() throw()
    {
    }

    const raster_details &format_limitation::details() const
    {
        return details_;
    }


} // close namespace imagexx

