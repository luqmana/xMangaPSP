// Copyright 2008 Edd Dawson.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file BOOST_LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef COMPRESSION_FAILURE_HPP_0052_15112008
#define COMPRESSION_FAILURE_HPP_0052_15112008

#include "imagexx/detail/ref_str.hpp"
#include "imagexx/exceptions/codec_failure.hpp"

namespace imagexx
{
    //! A simple wrapper around codec_failure to indicate by type that compression failed. The interface is the same as that of
    //! codec_failure.
    class compression_failure : public codec_failure
    {
        public:
            //! Creates an exception object that contains the (advanced) details specified
            explicit compression_failure(const char *advanced);

            //! Provides polymorphic destruction
            ~compression_failure() throw();

            //! Returns a string containing advanced information about the codec failure.
            const char *advanced() const;

            IMAGEXX_DEFINE_AUTO_EXCEPTION_IMPL_MEMBERS(compression_failure)

        private:
            imagexx::detail::ref_str advanced_;
    };

} // close namespace imagexx

#endif // COMPRESSION_FAILURE_HPP_0052_15112008

