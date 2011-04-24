// Copyright 2008 Edd Dawson.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file BOOST_LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef IJG_HEADERS_HPP_1634_21072009
#define IJG_HEADERS_HPP_1634_21072009

#include <stdio.h> // jpeglib.h forgets to #include this

#ifndef JPEGXX_C_LIB_COMPILED_AS_CPLUSPLUS
    extern "C" {
#else
    // DONT_USE_EXTERN_C introduced in v7 of the IJG library.
    // By default the v7 IJG headers check for __cplusplus being defined and
    // wrap the content in an 'extern "C"' block if it's present.
    // When DONT_USE_EXTERN_C is defined, this wrapping is not performed.
    #ifndef DONT_USE_EXTERN_C
        #define DONT_USE_EXTERN_C 1
    #endif
#endif

#include "jpeglib.h"
#include "jerror.h"

#ifndef JPEGXX_C_LIB_COMPILED_AS_CPLUSPLUS
    }
#endif

#endif // IJG_HEADERS_HPP_1634_21072009
