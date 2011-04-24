// Copyright 2008 Edd Dawson.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file BOOST_LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef OPTIONAL_NON_PRIMITIVE_HPP_0006_14042008
#define OPTIONAL_NON_PRIMITIVE_HPP_0006_14042008

namespace imagexx
{

    namespace detail
    {

        //! Used as a dummy class in aligned_bytes
        class undefined;

        //! This union has an array of bytes, data, that is of sufficient size and alignment to hold an object of type T, provided T is
        //! of non-primitive type
        template<typename T>
        union aligned_bytes
        {
            char data[sizeof(T)];

            // These ensure alignment is correct for any T object stored in data, as long as it's not a primitive type.
            struct { char a; } x1;
            struct { char* a; } x2;
            struct { int a; } x3;
            struct { int* a; } x4;
            struct { long a; } x5;
            struct { long* a; } x6;
            struct { short a; } x7;
            struct { short* a; } x8;
            struct { float a; } x9;
            struct { float* a; } x10;
            struct { double a; } x11;
            struct { double* a; } x12;
            struct { long double a; } x13;
            struct { long double* a; } x14;
            struct { void* a; } x15;
            struct { undefined (*a)(undefined); } x16;
            struct { undefined* undefined::*a; } x17;
            struct { undefined (undefined::*a)(undefined); } x18;
        };

        //! Returns the address of the base element in the data array of ab
        template<typename T> void *address(aligned_bytes<T> &ab) { return ab.data; }

        //! Returns the address of the base element in the data array of ab
        template<typename T> const void *address(const aligned_bytes<T> &ab) { return ab.data; }

        //! This class is used to hold an object of type T that may or may not exist. T must be of non-primitive type.  If an
        //! optional_non_primitive holds an object, its get() member function will return a non-null-valued pointer to that object.
        //!
        //! By using this in the implementation of pixel_decompose_iterator, it means that a Decomposer need not had a default
        //! constructor or any other special way of representing an 'end' state.
        //!
        //! No dynamic allocation is involved in the implementation of this class.
        //!
        //! Refer to boost::variant, boost::optional or http://www.ddj.com/dept/cpp/184403828 for details of the implementation.
        template<typename T>
        class optional_non_primitive
        {
            public:
                //! Constructs an object that does not contain a T
                optional_non_primitive() : data_(0) { }

                //! Creates an object that holds a copy of the T given as an argument
                optional_non_primitive(const T &obj) :
                    data_(new (address(bytes_)) T(obj))
                {
                }

                //! Copy constructor. Creates a value copy of other's internal T, if it has one
                optional_non_primitive(const optional_non_primitive &other) :
                    data_(other.data_ ? new (address(bytes_)) T(*other.data_) : 0)
                {
                }

                //! Copys the internal T object of other in to this optional_non_primitive if other contains such a T, else this object
                //! is emptied of any T that it may have held.
                //!
                //! This function only has the WEAK exception safety guarantee. If an exception is thrown in T's copy constructor, this
                //! object is left without any internal T.
                optional_non_primitive &operator= (const optional_non_primitive &other)
                {
                    if (&other != this)
                    {
                        if (data_) data_->~T();
                        data_ = 0;
                        if (other.data_) data_ = new (address(bytes_)) T(*other.data_);
                    }
                    return *this;
                }

                //! Destroys the object along with the internal T object, if it exists
                ~optional_non_primitive() { if (data_) data_->~T();   }

                //! Returns 0 if no internal T exists otherwise the address of that T is returned.
                T *get() const { return data_; }

            private:
                aligned_bytes<T> bytes_;
                T *data_;
        };

    } // close namespace detail

} // close namespace imagexx

#endif // OPTIONAL_NON_PRIMITIVE_HPP_0006_14042008

