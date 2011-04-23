// Copyright 2008 Edd Dawson.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file BOOST_LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef FSTREAM_OPEN_FAILURE_HPP_0052_15112008
#define FSTREAM_OPEN_FAILURE_HPP_0052_15112008

#include <string>
#include <stdexcept>
#include <ios>

#include "imagexx/exceptions/exception.hpp"
#include "imagexx/detail/ref_str.hpp"

namespace imagexx
{
    //! An exception of this type is thrown when an fstream could not be opened for some reason. The details of the stream can be
    //! polled using the various member functions. Used as a base for ofstream_open_failure and ifstream_open_failure.
    class fstream_open_failure : public std::runtime_error, public auto_exception_impl<fstream_open_failure>
    {
        public:
            //! Creates an exception object that stores the given filename and mode of opening for retrieval at a catch-site.
            //! what will be returned by what()
            fstream_open_failure(const std::string &filename, std::ios_base::openmode mode, const char *what = "Failed to open a file");

            //! Provides polymorphic destruction
            ~fstream_open_failure() throw();

            //! Returns the mode that the stream opened with
            std::ios_base::openmode mode() const;

            //! Returns the nam of the file that could not be opened
            const char *filename() const;

        private:
            std::ios_base::openmode mode_;
            imagexx::detail::ref_str filename_;
    };

} // close namespace imagexx

#endif // FSTREAM_OPEN_FAILURE_HPP_0052_15112008

