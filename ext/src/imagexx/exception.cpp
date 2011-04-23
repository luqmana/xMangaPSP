// Copyright 2008 Edd Dawson.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file BOOST_LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include "imagexx/exceptions/exception.hpp"

#include <cassert>
#include <typeinfo>

namespace imagexx
{
    exception::~exception()
    {
    }

    std::auto_ptr<exception> exception::clone() const
    {
        std::auto_ptr<exception> c = clone_impl();
        assert(c.get());
        assert(typeid(*c) == typeid(*this));
        return c;
    }

    void exception::throw_copy() const
    {
        throw_copy_impl();
        assert("implementation failed to throw" == 0);
    }



} // close namespace imagexx

