// Copyright 2009 Edd Dawson.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file BOOST_LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef CPP0X_EXCEPTION_STORE_HPP_1154_17072009
#define CPP0X_EXCEPTION_STORE_HPP_1154_17072009

#include <imagexx/exceptions/exception_store.hpp>
#include <exception>

namespace imagexx
{
    //! Implements the imagexx::caught_exception interface by wrapping an std::exception_ptr object,
    //! new to C++0x.
    class caught_exception_ptr : public caught_exception
    {
        public:
            caught_exception_ptr() { ex_ = std::current_exception(); }

            virtual void rethrow() { std::rethrow_exception(ex_); }

        private:
            std::exception_ptr ex_;
    };

    //! Implements the imagexx::exception_store interface using std::exception_ptr and related
    //! facilities from C++0x.
    class cpp0x_exception_store : public exception_store
    {
        public:
            template<typename Exception>
            cpp0x_exception_store(const Exception &generic) : exception_store(generic) { }

        private:
            virtual std::auto_ptr<caught_exception> get_current_exception()
            {
                std::auto_ptr<caught_exception> ret(new caught_exception_ptr);
                return ret;
            }
    };

} // close namespace imagexx

#endif // CPP0X_EXCEPTION_STORE_HPP_1154_17072009
