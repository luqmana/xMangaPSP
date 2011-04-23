// Copyright 2008 Edd Dawson.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file BOOST_LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef ALPHA_FORMAT_LIMITATION_HPP_0255_15112008
#define ALPHA_FORMAT_LIMITATION_HPP_0255_15112008

#include <jpegxx/exceptions/exception.hpp>
#include <imagexx/exceptions/format_limitation.hpp>

namespace jpegxx
{
    //! JPEG rasters cannot contain alpha. This exception is thrown if the library is given a raster to save that contains an alpha
    //! channel.
    class alpha_format_limitation : public imagexx::format_limitation, public exception
    {
        public:
            //! Constructs an alpha_format_limitation that contains the details of the raster that could not be handled
            //! by the library.
            explicit alpha_format_limitation(const imagexx::raster_details &details);

            //! Provides polymorphic destruction
            ~alpha_format_limitation() throw() { }

            //! Returns a simple error message about a compression failure
            const char *what() const throw();

            IMAGEXX_DEFINE_AUTO_EXCEPTION_IMPL_MEMBERS(alpha_format_limitation)
    };

} // close namespace jpegxx

#endif // ALPHA_FORMAT_LIMITATION_HPP_0255_15112008
