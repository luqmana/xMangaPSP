// Copyright 2008 Edd Dawson.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file BOOST_LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef EXCEPTION_HPP_0255_15112008
#define EXCEPTION_HPP_0255_15112008

namespace jpegxx
{
    //! Every exception defined by jpegxx inherits this base class
    class exception
    {
        protected:
            //! Constructs a jpegxx::exception
            exception();

        public:
            //! Destroys the exception mercilessly
            virtual ~exception();
    };

} // close namespace jpegxx

#endif // EXCEPTION_HPP_0255_15112008
