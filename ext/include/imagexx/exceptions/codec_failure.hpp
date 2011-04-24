// Copyright 2008 Edd Dawson.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file BOOST_LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef CODEC_FAILURE_HPP_0052_15112008
#define CODEC_FAILURE_HPP_0052_15112008

#include <stdexcept>

#include "imagexx/exceptions/exception.hpp"

namespace imagexx
{
    //! An object of this type is thrown whenever raster compression or decompression fails. This is a base class for
    //! imagexx::decompression_failure and imagexx::compression_failure.
    class codec_failure : public std::runtime_error, public auto_exception_impl<codec_failure>
    {
        public:
            //! Creates a codec_failure object. what will be returned by what()
            explicit codec_failure(const char *what = "The codec failed to encode/decode the raster data");

            //! Provides polymorphic destruction
            ~codec_failure() throw();
    };

} // close namespace imagexx

#endif // CODEC_FAILURE_HPP_0052_15112008

