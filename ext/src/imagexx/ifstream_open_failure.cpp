// Copyright 2008 Edd Dawson.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file BOOST_LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include "imagexx/exceptions/ifstream_open_failure.hpp"

namespace imagexx
{
    ifstream_open_failure::ifstream_open_failure(const std::string &filename, std::ios_base::openmode mode) :
        fstream_open_failure(filename, mode, "Failed to open a file for reading")
    {
    }

    ifstream_open_failure::~ifstream_open_failure() throw()
    {
    }


} // close namespace imagexx

