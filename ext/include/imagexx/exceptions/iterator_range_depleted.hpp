// Copyright 2008 Edd Dawson.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file BOOST_LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef ITERATOR_RANGE_DEPLETED_HPP_0052_15112008
#define ITERATOR_RANGE_DEPLETED_HPP_0052_15112008

#include <stdexcept>

#include "imagexx/exceptions/source_depleted.hpp"

namespace imagexx
{
    //! An exception of this type is thrown when an iterator range contains less than the expected amount of data
    class iterator_range_depleted : public source_depleted, public std::runtime_error
    {
        public:
            //! Creates an iterator_range_depleted object. what will be returned by what().
            explicit iterator_range_depleted(const char *what = "The input data range ended prematurely");

            //! Provides polymorphic destruction
            ~iterator_range_depleted() throw();

            IMAGEXX_DEFINE_AUTO_EXCEPTION_IMPL_MEMBERS(iterator_range_depleted)
    };

} // close namespace imagexx

#endif // ITERATOR_RANGE_DEPLETED_HPP_0052_15112008

