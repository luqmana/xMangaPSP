// Copyright 2009 Edd Dawson.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file BOOST_LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include "imagexx/exceptions/exception_store.hpp"
#include <new>
#include <exception>
#include <stdexcept>
#include <cassert>

namespace imagexx
{
    void exception_store::store_current_exception()
    {
        clear();

        try { ex_ = get_current_exception(); }
        catch (const std::bad_alloc &) { bad_alloc_ = true; }

        if (!ex_.get() && !bad_alloc_) generic_ = true;
    }

    void exception_store::store(std::auto_ptr<caught_exception> &ex)
    {
        clear();
        ex_ = ex;

        if (!ex_.get())
            generic_ = true;
    }

    void exception_store::rethrow_stored_exception()
    {
        if (ex_.get())
        {
            std::auto_ptr<caught_exception> ex = ex_;
            ex->rethrow();
            generic_ = true; 
        }

        if (bad_alloc_)
        {
            bad_alloc_ = false;
            throw std::bad_alloc();
        }

        if (generic_)
        {
            generic_ = false;
            generic_ex_->rethrow();
        }
    }

    bool exception_store::has_exception() const
    {
        return ex_.get() || bad_alloc_ || generic_;
    }

    void exception_store::clear()
    {
        ex_.reset(0);
        bad_alloc_ = false;
        generic_ = false;
    }

    void exception_store::store(const std::bad_alloc &)
    {
        clear();
        bad_alloc_ = true;
    }

} // close namespace imagexx
