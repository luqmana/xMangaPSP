// Copyright 2009 Edd Dawson.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file BOOST_LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BUTTER_FINGERS_EXCEPTION_STORE_HPP_1118_14072009
#define BUTTER_FINGERS_EXCEPTION_STORE_HPP_1118_14072009

#include <imagexx/exceptions/exception_store.hpp>

namespace imagexx
{
    //! This exception_store will not catch any kind of exception. Calling store_current_exception()
    //! will result in an exception escaping the function.
    class butter_fingers_exception_store : public exception_store
    {
        public:
            template<typename Exception>
            butter_fingers_exception_store(const Exception &generic) : exception_store(generic) { }

        private:
            std::auto_ptr<caught_exception> get_current_exception() { throw; }
    };

} // close namespace imagexx

#endif // BUTTER_FINGERS_EXCEPTION_STORE_HPP_1118_14072009
