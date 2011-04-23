// Copyright 2008 Edd Dawson.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file BOOST_LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef LOAD_ORDER_ERROR_HPP_0052_15112008
#define LOAD_ORDER_ERROR_HPP_0052_15112008

#include <stdexcept>

#include "imagexx/exceptions/exception.hpp"

namespace imagexx
{
    //! An exception of this type is thrown when a client attempts to read the raster before the header in a data stream.
    class load_order_error : public std::runtime_error, public auto_exception_impl<load_order_error>
    {
        public:
            //! Creates a load_order_error object
            load_order_error();

            //! Provides polymorphic destruction
            ~load_order_error() throw();
    };

} // close namespace imagexx

#endif // LOAD_ORDER_ERROR_HPP_0052_15112008

