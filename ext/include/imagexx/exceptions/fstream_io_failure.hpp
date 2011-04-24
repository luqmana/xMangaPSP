// Copyright 2008 Edd Dawson.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file BOOST_LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef FSTREAM_IO_FAILURE_HPP_0052_15112008
#define FSTREAM_IO_FAILURE_HPP_0052_15112008

#include <string>

#include "imagexx/detail/ref_str.hpp"
#include "imagexx/exceptions/stream_io_failure.hpp"

namespace imagexx
{
    //! An exception of this type is thrown if reading from or writing to an fstream with a known file name fails.
    class fstream_io_failure : public virtual stream_io_failure
    {
        public:
            //! Creates an fstream_io_failure
            fstream_io_failure(std::ios_base::iostate state, const std::string &filename, const char *what);

            //! Provides polymorphic destruction
            ~fstream_io_failure() throw();

            //! Returns the name of the associated file
            const char *filename() const;

            IMAGEXX_DEFINE_AUTO_EXCEPTION_IMPL_MEMBERS(fstream_io_failure)

        private:
            imagexx::detail::ref_str filename_;
    };

} // close namespace imagexx

#endif // FSTREAM_IO_FAILURE_HPP_0052_15112008

