// Copyright 2008 Edd Dawson.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file BOOST_LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef DECOMPRESSION_FAILURE_HPP_0255_15112008
#define DECOMPRESSION_FAILURE_HPP_0255_15112008

#include <jpegxx/exceptions/exception.hpp>
#include <imagexx/exceptions/decompression_failure.hpp>

namespace jpegxx
{
    //! A simple wrapper around imagexx::decompression_failure to indicate by type that decompression failed within the
    //! jpegxx library.
    class decompression_failure : public imagexx::decompression_failure, public exception
    {
        public:
            //! Creates an exception object that contains the (advanced) details specified
            explicit decompression_failure(const char *advanced);

            //! Provides polymorphic destruction
            ~decompression_failure() throw() { }

            //! Returns a simple error message about a decompression failure
            const char *what() const throw();

            IMAGEXX_DEFINE_AUTO_EXCEPTION_IMPL_MEMBERS(decompression_failure)
    };

} // close namespace jpegxx

#endif // DECOMPRESSION_FAILURE_HPP_0255_15112008
