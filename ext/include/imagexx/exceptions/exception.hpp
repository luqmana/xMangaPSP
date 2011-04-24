// Copyright 2008 Edd Dawson.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file BOOST_LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef EXCEPTION_HPP_0052_15112008
#define EXCEPTION_HPP_0052_15112008

#include <memory>

//! This macro defines the machinery necessary to implement the imagexx::exception interface inside a class definition
#define IMAGEXX_DEFINE_AUTO_EXCEPTION_IMPL_MEMBERS(type) \
    private: \
        std::auto_ptr<imagexx::exception> clone_impl() const \
        { \
            return std::auto_ptr<imagexx::exception>(new type(*this)); \
        } \
        \
        void throw_copy_impl() const \
        { \
            throw type(*this); \
        }


namespace imagexx
{

    //! All exceptions explicitly thrown by the library inherit this class so that they may be caught polymorphically by the client.
    //! This class also provides an interface that provides threaded code a means to save and re-throw exceptions so that they may
    //! be propagated in to a parent thread
    class exception
    {
        public:
            //! Enables polymorphic destruction
            virtual ~exception();

            //! Returns a smart-pointer to a copy of *this. Each derived class needs to implement clone_impl() to modify the behaviour
            //! of this function accordingly.
            //! This exists to aid threaded code in propagating exceptions across join boundaries.
            std::auto_ptr<exception> clone() const;

            //! Throws a copy of *this. Each derived class needs to implement throw_copy_impl() to modify the behaviour of this
            //! function accordingly.
            void throw_copy() const;

        private:
            //! Derived classes should implement this to return a smart-pointer to an object of the derived type. This can be done
            //! automatically if the DerivedType inherits auto_exception_impl<DerivedType>.
            //! This exists to aid threaded code in propagating exceptions across join boundaries.
            virtual std::auto_ptr<exception> clone_impl() const = 0;

            //! Derived classes should implement this to throw a copy of *this (without slicing). This can be done automatically if
            //! the DerivedType inherits auto_exception_impl<DerivedType>.
            virtual void throw_copy_impl() const = 0;
    };

    //! This class may be used to fill in implementations for exception::clone_impl() and exception::throw_copy_impl(). It uses the
    //! Curiously Recurring Template Pattern, so a class D that wants to inherit imagexx::exception might instead inherit
    //! auto_exception_impl<D>!
    template<typename CRTPType>
    class auto_exception_impl : public virtual exception
    {
        private:
            //! Provides an implementation of exception::clone_impl() suitable for CRTPType
            std::auto_ptr<exception> clone_impl() const
            {
                std::auto_ptr<exception> p(new CRTPType(*static_cast<const CRTPType *>(this)));
                return p;
            }

            //! Provides an implementation of exception::throw_copy_impl() suitable for CRTPType
            void throw_copy_impl() const
            {
                throw CRTPType(*static_cast<const CRTPType *>(this));
            }
    };

} // close namespace imagexx

#endif // EXCEPTION_HPP_0052_15112008

