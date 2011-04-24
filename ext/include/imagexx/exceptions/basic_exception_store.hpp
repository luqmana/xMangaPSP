// Copyright 2009 Edd Dawson.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file BOOST_LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BASIC_EXCEPTION_STORE_HPP_1101_14072009
#define BASIC_EXCEPTION_STORE_HPP_1101_14072009

#include <imagexx/exceptions/exception_store.hpp>

namespace imagexx
{
    //! The basic exception store recognises and correctly stores all exceptions derived from
    //! imagexx::exception.
    //! Any other kind of exception is stored as a generic exception.
    class basic_exception_store : public exception_store
    {
        public:
            template<typename Exception>
            basic_exception_store(const Exception &generic) : exception_store(generic) { }

        private:
            std::auto_ptr<caught_exception> get_current_exception();
    };

} // close namespace imagexx

#endif // BASIC_EXCEPTION_STORE_HPP_1101_14072009
