// Copyright 2008 Edd Dawson.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file BOOST_LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef STREAM_IO_FAILURE_HPP_0052_15112008
#define STREAM_IO_FAILURE_HPP_0052_15112008

#include <stdexcept>
#include <ios>

#include "imagexx/exceptions/exception.hpp"

namespace imagexx
{
    //! An exception of this type is thrown if reading from or writing to a stream fails.
    class stream_io_failure : public std::ios_base::failure, public auto_exception_impl<stream_io_failure>
    {
        public:
            //! Creates an exception
            stream_io_failure(std::ios_base::iostate state, const char *what);

            //! Provides polymorphic destruction
            ~stream_io_failure() throw();

            //! Returns the failure state of the stream associated with the operation
            std::ios_base::iostate state() const;

        private:
            std::ios_base::iostate state_;
    };

} // close namespace imagexx

#endif // STREAM_IO_FAILURE_HPP_0052_15112008

