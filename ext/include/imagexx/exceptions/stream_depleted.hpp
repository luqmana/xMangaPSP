// Copyright 2008 Edd Dawson.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file BOOST_LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef STREAM_DEPLETED_HPP_0052_15112008
#define STREAM_DEPLETED_HPP_0052_15112008

#include "imagexx/exceptions/source_depleted.hpp"
#include "imagexx/exceptions/stream_io_failure.hpp"

namespace imagexx
{
    //! An exception of this type is thrown when an istream contains less than the expected amount of data
    class stream_depleted : public source_depleted, public virtual stream_io_failure
    {
        public:
            //! Creates a stream_depleted object.
            explicit stream_depleted(const std::ios_base::iostate state, const char *what = "input stream depleted");

            //! Provides polymorphic destruction
            ~stream_depleted() throw();

            IMAGEXX_DEFINE_AUTO_EXCEPTION_IMPL_MEMBERS(stream_depleted)
    };

} // close namespace imagexx

#endif // STREAM_DEPLETED_HPP_0052_15112008

