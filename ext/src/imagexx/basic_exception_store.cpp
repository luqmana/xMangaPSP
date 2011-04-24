// Copyright 2009 Edd Dawson.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file BOOST_LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <imagexx/exceptions/basic_exception_store.hpp>
#include <imagexx/exceptions/exception.hpp>

#include <stdexcept>
#include <ios> // ios_base::failure
#include <new> // bad_alloc
#include <typeinfo>

#define CATCH_AND_STORE_IF_STATIC_TYPE_IS(extype) \
    catch (const extype &ex) \
    { \
        if (typeid(ex) == typeid(extype)) \
            ret.reset(new specific_caught_exception<extype>(ex)); \
    }

namespace imagexx
{
    namespace
    {
        class caught_imagexx_exception : public caught_exception
        {
            public:
                caught_imagexx_exception(const imagexx::exception &ex) : ex_(ex.clone()) { }

                void rethrow() { ex_->throw_copy(); }

            private:
                std::auto_ptr<imagexx::exception> ex_;
        };
    }

    std::auto_ptr<caught_exception> basic_exception_store::get_current_exception()
    {
        std::auto_ptr<caught_exception> ret;
        try { throw; }
        catch (const imagexx::exception &ex) { ret.reset(new caught_imagexx_exception(ex)); }
        CATCH_AND_STORE_IF_STATIC_TYPE_IS(std::length_error)
        CATCH_AND_STORE_IF_STATIC_TYPE_IS(std::domain_error)
        CATCH_AND_STORE_IF_STATIC_TYPE_IS(std::out_of_range)
        CATCH_AND_STORE_IF_STATIC_TYPE_IS(std::invalid_argument)
        CATCH_AND_STORE_IF_STATIC_TYPE_IS(std::logic_error)
        CATCH_AND_STORE_IF_STATIC_TYPE_IS(std::bad_exception)
        CATCH_AND_STORE_IF_STATIC_TYPE_IS(std::bad_cast)
        CATCH_AND_STORE_IF_STATIC_TYPE_IS(std::bad_typeid)
        CATCH_AND_STORE_IF_STATIC_TYPE_IS(std::ios_base::failure)
        CATCH_AND_STORE_IF_STATIC_TYPE_IS(std::range_error)
        CATCH_AND_STORE_IF_STATIC_TYPE_IS(std::overflow_error)
        CATCH_AND_STORE_IF_STATIC_TYPE_IS(std::underflow_error)
        CATCH_AND_STORE_IF_STATIC_TYPE_IS(std::runtime_error)
        catch (std::bad_alloc &) { throw; }
        CATCH_AND_STORE_IF_STATIC_TYPE_IS(std::exception)
        catch (...) { }
        return ret;
    }

} // close namespace imagexx

