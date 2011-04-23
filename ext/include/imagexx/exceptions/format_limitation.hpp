// Copyright 2008 Edd Dawson.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file BOOST_LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef FORMAT_LIMITATION_HPP_0052_15112008
#define FORMAT_LIMITATION_HPP_0052_15112008

#include "imagexx/raster_details.hpp"
#include "imagexx/exceptions/codec_failure.hpp"

namespace imagexx
{
    //! This exception may be thrown an image cannot be saved by in a particular codec due to restrictions of the format e.g. there is
    //! a raster size limitation of 64K pixels for JPEG images.
    class format_limitation : public codec_failure
    {
        public:
            //! Constructs a format_limitation that contains the details of the raster that could not be handled.
            explicit format_limitation(const raster_details &details);

            //! Provides polymorphic destruction
            ~format_limitation() throw();

            //! Returns the details of the image that could not be handled by the library
            const raster_details &details() const;

            IMAGEXX_DEFINE_AUTO_EXCEPTION_IMPL_MEMBERS(format_limitation)

        private:
            raster_details details_;
    };

} // close namespace imagexx

#endif // FORMAT_LIMITATION_HPP_0052_15112008

