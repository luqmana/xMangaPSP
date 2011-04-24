// Copyright 2008 Edd Dawson.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file BOOST_LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include "imagexx/exceptions/load_order_error.hpp"

namespace imagexx
{
    load_order_error::load_order_error() :
        std::runtime_error("An attempt was made to query the details of raster before "
                           "the image's header block was read. "
                           "You should always call loader::read_header() before either "
                           "loader::read_raster() or loader::details().")
    {
    }

    load_order_error::~load_order_error() throw()
    {
    }



} // close namespace imagexx

