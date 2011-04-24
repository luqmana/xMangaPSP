// Copyright 2008 Edd Dawson.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file BOOST_LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef SOURCE_DEPLETED_HPP_0052_15112008
#define SOURCE_DEPLETED_HPP_0052_15112008

#include "imagexx/exceptions/exception.hpp"

namespace imagexx
{
    //! An exception of this type is thrown when a data source does not contain the expected amount of data.
    class source_depleted : public auto_exception_impl<source_depleted>
    {
        public:
            //! Provides polymorphic destruction
            ~source_depleted() throw();

            IMAGEXX_DEFINE_AUTO_EXCEPTION_IMPL_MEMBERS(source_depleted)
    };

} // close namespace imagexx

#endif // SOURCE_DEPLETED_HPP_0052_15112008

