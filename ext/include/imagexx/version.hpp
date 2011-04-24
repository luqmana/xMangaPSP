// Copyright 2008 Edd Dawson.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file BOOST_LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef VERSION_HPP_1715_30032008
#define VERSION_HPP_1715_30032008

#define IMAGEXX_MAJOR 0
#define IMAGEXX_MINOR 3
#define IMAGEXX_FIX 0

namespace imagexx
{
    struct version
    {
        enum
        {
            major = IMAGEXX_MAJOR,
            minor = IMAGEXX_MINOR,
            fix = IMAGEXX_FIX
        };
    };

} //close namespace imagexx

#endif // VERSION_HPP_1715_30032008

