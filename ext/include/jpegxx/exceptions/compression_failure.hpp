// Copyright 2008 Edd Dawson.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file BOOST_LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef COMPRESSION_FAILURE_HPP_0255_15112008
#define COMPRESSION_FAILURE_HPP_0255_15112008

#include <jpegxx/exceptions/exception.hpp>
#include <imagexx/exceptions/compression_failure.hpp>

namespace jpegxx
{
    //! A simple wrapper around imagexx::compression_failure to indicate by type that compression failed within the
    //! jpegxx library.
    class compression_failure : public imagexx::compression_failure, public exception
    {
        public:
            //! Creates an exception object that contains the (advanced) details specified
            explicit compression_failure(const char *advanced);

            //! Provides polymorphic destruction
            ~compression_failure() throw() { }

            //! Returns a simple error message about a compression failure
            const char *what() const throw();

            IMAGEXX_DEFINE_AUTO_EXCEPTION_IMPL_MEMBERS(compression_failure)
    };

} // close namespace jpegxx

#endif // COMPRESSION_FAILURE_HPP_0255_15112008
