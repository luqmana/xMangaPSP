// Copyright 2009 Edd Dawson.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file BOOST_LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BEST_EXCEPTION_STORE_HPP_1144_17072009
#define BEST_EXCEPTION_STORE_HPP_1144_17072009

// Here, we pick the type of exception store that is able to correctly store and propagate the widest
// set of exceptions.

#if defined(JPEGXX_C_LIB_COMPILED_AS_CPLUSPLUS) || defined(JPEGXX_CAN_THROW_EXCEPTIONS_THROUGH_C_LIB)

    // It is safe to throw exceptions through the IJG library. It has been compiled as C++, or the client
    // of the library knows that throwing exceptions through C code on their implementation is ok.
    // We can therefore use a butter_fingers_exception_store, which lets C++ exceptions escape.
    #include <imagexx/exceptions/butter_fingers_exception_store.hpp>
    typedef imagexx::butter_fingers_exception_store best_exception_store;

#elif defined(JPEGXX_HAVE_EXCEPTION_PTR) || (__cplusplus > 199711L)

    // The C++ implementation has std::exception_ptr and friends. We can therefore use a
    // cpp0x_exception_store, which can store and propagate any exception perfectly.
    #include <imagexx/exceptions/cpp0x_exception_store.hpp>
    typedef imagexx::cpp0x_exception_store best_exception_store;

#else

    // Fall back to using a basic_exception_store, which supports all standard library
    // exceptions (defined by C++98) and anything derived from imagexx::exception. Any
    // other kind of exception will be represented by a more generic object inside the
    // store.
    #include <imagexx/exceptions/basic_exception_store.hpp>
    typedef imagexx::basic_exception_store best_exception_store;

#endif

#endif // BEST_EXCEPTION_STORE_HPP_1144_17072009
