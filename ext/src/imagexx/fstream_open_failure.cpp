// Copyright 2008 Edd Dawson.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file BOOST_LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include "imagexx/exceptions/fstream_open_failure.hpp"

namespace imagexx
{
    fstream_open_failure::fstream_open_failure(const std::string &filename, std::ios_base::openmode mode, const char *what) :
        std::runtime_error(what),
        mode_(mode),
        filename_(filename)
    {
    }

    fstream_open_failure::~fstream_open_failure() throw()
    {
    }

    std::ios_base::openmode fstream_open_failure::mode() const
    {
        return mode_;
    }

    const char *fstream_open_failure::filename() const
    {
        return filename_.c_str();
    }


} // close namespace imagexx

