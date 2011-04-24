// Copyright 2008 Edd Dawson.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file BOOST_LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef FILE_ITERATOR_HPP_1717_30032008
#define FILE_ITERATOR_HPP_1717_30032008

#include <cstdlib>
#include <cstdio>
#include <iterator>

namespace imagexx
{

//! Used to byte-wise iterate over data identified by a FILE* stream opened for reading.
//! Uses fgetc() to do its dirty work.
class file_input_iterator : public std::iterator<std::input_iterator_tag, int, std::ptrdiff_t, int *, int &>
{
    public:
        //! Constructs an iterator that represents one-past-the-end of the range
        file_input_iterator();

        //! Constructs an iterator that refers to current byte of fptr
        explicit file_input_iterator(std::FILE *fptr);

        //! Returns the element that this iterator refers to
        value_type operator* ();

        //! Returns a pointer to the element that this iterator refers to
        pointer operator-> ();

        //! Pre-increment
        file_input_iterator &operator++ ();

        //! Post-increment
        file_input_iterator operator++ (int);

        //! Returns true if this iterator is equal to other, in the appropriate sense for input iterators
        bool operator== (const file_input_iterator &other);

        //! Returns true if this iterator is not equal to other, in the apporpriate sense for input iterators
        bool operator!= (const file_input_iterator &other);

    private:
        //! Returns true if this iterator is at the one-past-the-end position
        bool is_end() const;

    private:
        std::FILE *fptr_;
        std::size_t pos_;
        int deref_;
};


//! Used to byte-wise iterate through and write data to a FILE* stream opened for writing.
//! Uses fputc() to do its dirty work.
class file_output_iterator : public std::iterator<std::output_iterator_tag, void, void, void, void>
{
    public:
        //! Constructs an iterator that refers to current byte of fptr
        explicit file_output_iterator(std::FILE *fptr);

        //! Returns an object x, such that x=i will call fputc() with i and the underlying FILE* as arguments
        file_output_iterator &operator* ();

        //! Pre-increment
        file_output_iterator &operator++ ();

        //! Post-increment
        file_output_iterator &operator++ (int);

        //! Since operator* returns *this, when we write *p = some_int where p is a file_output_iterator,
        //! we are actually fputc()-ing ch into the underlying stream.
        file_output_iterator &operator= (int ch);

    private:
        std::FILE *fptr_;
};

} //close namespace imagexx

#endif // FILE_ITERATOR_HPP_1717_30032008
