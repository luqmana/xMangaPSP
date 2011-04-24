// Copyright 2008 Edd Dawson.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file BOOST_LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef SIZE_FORMAT_LIMITATION_HPP_0255_15112008
#define SIZE_FORMAT_LIMITATION_HPP_0255_15112008

#include <jpegxx/exceptions/exception.hpp>
#include <imagexx/exceptions/format_limitation.hpp>

namespace jpegxx
{
    //! JPEG rasters are restricted to 65535 by 65535 in size. This exception is thrown if the library is given a raster that is bigger
    //! in either dimension.
    class size_format_limitation : public imagexx::format_limitation, public exception
    {
        public:
            //! Constructs a size_format_limitation that contains the details of the raster that could not be handled
            //! by the library.
            explicit size_format_limitation(const imagexx::raster_details &details);

            //! Provides polymorphic destruction
            ~size_format_limitation() throw() { }

            //! Returns a simple error message about a compression failure
            const char *what() const throw();

            IMAGEXX_DEFINE_AUTO_EXCEPTION_IMPL_MEMBERS(size_format_limitation)
    };

} // close namespace jpegxx

#endif // SIZE_FORMAT_LIMITATION_HPP_0255_15112008
