// Copyright 2009 Edd Dawson.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file BOOST_LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef EXCEPTION_STORE_HPP_1023_14072009
#define EXCEPTION_STORE_HPP_1023_14072009

#include <memory>
#include <typeinfo>
#include <new>
#include <cassert>

namespace imagexx
{
    //! A base class whose derived classes should be capable of storing and re-throwing an exception
    //! of a particular kind.
    //! Typically, an exception will be passed to the derived object's constructor to be stored.
    class caught_exception
    {
        public:
            caught_exception() { }

            virtual ~caught_exception() { }

            //! Derived classes should override this function to re-throw an exception.
            virtual void rethrow() = 0;

        private:
            //! Copying is not allowed
            caught_exception(const caught_exception &);
            
            //! Copying is not allowed
            caught_exception &operator= (const caught_exception &);
    };

    //! Implements the caught_exception interface to store and re-throw an exception of type Exception.
    template<typename Exception>
    class specific_caught_exception : public caught_exception
    {
        public:
            //! Stores a copy of \a ex internally. The dynamic type of \a ex must be Exception to
            //! prevent slicing when this copy is made.
            specific_caught_exception(const Exception &ex) : 
                ex_(ex) 
            { 
                assert(typeid(ex) == typeid(Exception)); // check that we aren't slicing
            }

            //! Throws the exception stored by the constructor
            void rethrow() { throw ex_; }

        private:
            const Exception ex_;
    };

    //! An exception_store holds a copy of an exception to be re-thrown at a later time.
    //! Derived classes should implement get_current_exception().
    class exception_store
    {
        public:
            //! Create an exception_store that treats the given exception object as a "generic exception".
            //! A copy of the given exception will be thrown when a generic exception is stored prior to
            //! a call to rethrow_stored_exception()
            template<typename Exception>
            exception_store(const Exception &generic) :
                ex_(),
                generic_ex_(new specific_caught_exception<Exception>(generic)),
                bad_alloc_(false),
                generic_(false)
            {
            }

            virtual ~exception_store() { }
            
            //! Assumes that there is an active exception i.e. this function should only get called inside a
            //! catch(...) block. Derived classes should override get_current_exception() to change the behaviour
            //! of this function. If the result of calling that function is a non-null pointer, then it is stored 
            //! internally. If the result is indeed null, then a generic exception is stored internally. If the call 
            //! to get_current_exception() throws an std::bad_alloc then an std::bad_alloc is stored internally. 
            //! Any other kind of exception is allowed to propagate.
            void store_current_exception();

            //! Stores the given caught_exception. rethrow_stored_exception() will call the rethrow() member
            //! function on the copy of \a ex that is made by this function and stored internally.
            void store(std::auto_ptr<caught_exception> &ex);

            //! Throws any exception stored during a call to store_current_exception() or store(). If no exception
            //! has been stored so far, nothing is thrown and the function returns normally.
            void rethrow_stored_exception();

            //! Returns true if rethrow_stored_exception() will throw an exception.
            bool has_exception() const;

            //! Clears any exception stored internally
            void clear();

            //! Creates a specific_caught_exception<Exception> and passes it to the other store() overload.
            template<typename Exception>
            void store(const Exception &caught)
            {
                try 
                { 
                    std::auto_ptr<caught_exception> ex(new specific_caught_exception<Exception>(caught)); 
                    store(ex);
                }
                catch (const std::bad_alloc &) 
                { 
                    clear();
                    bad_alloc_ = true; 
                }
            }

            //! An overload to avoid to store a bad_alloc more efficiently than the generic function
            void store(const std::bad_alloc &ex);

        private:
            //! Derived classes should override this function to change the behaviour of store_current_exception().
            //! A typical implementation will look something like this:
            //! \code
            //! try { throw; }
            //! catch (const X &x) { /* return caught_exception containing x */ }
            //! catch (const Y &y) { /* return caught_exception containing y */ }
            //! // ...
            //! \endcode
            //! In some cases, however, the exception may be left to propagate, or vendor-specific features may be
            //! used to store a copy of the current exception, whatever its type.
            virtual std::auto_ptr<caught_exception> get_current_exception() = 0;

            //! copying is not allowed
            exception_store(const exception_store &);

            //! copying is not allowed
            exception_store &operator= (const exception_store &);

        private:
            std::auto_ptr<caught_exception> ex_;
            std::auto_ptr<caught_exception> generic_ex_;
            bool bad_alloc_;
            bool generic_;
    };

} // close namespace imagexx

#endif // EXCEPTION_STORE_HPP_1023_14072009
