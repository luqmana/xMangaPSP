// Copyright 2007 Edd Dawson.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file BOOST_LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef VERSION_HPP_1123_14052007
#define VERSION_HPP_1123_14052007

#define JPEGXX_MAJOR 0
#define JPEGXX_MINOR 3
#define JPEGXX_FIX 0

namespace jpegxx
{
    struct version
    {
        enum
        {
            major = JPEGXX_MAJOR,
            minor = JPEGXX_MINOR,
            fix = JPEGXX_FIX
        };
    };

} //close namespace jpegxx

#endif // VERSION_HPP_1123_14052007
