// Copyright 2007 Edd Dawson.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file BOOST_LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include "imagexx/iterators/file_iterator.hpp"

#include <cassert>

namespace imagexx
{

    file_input_iterator::file_input_iterator() :
        fptr_(0),
        pos_(0),
        deref_(EOF)
    {
    }

    file_input_iterator::file_input_iterator(std::FILE *fptr) :
        fptr_(fptr),
        pos_(0),
        deref_(0)
    {
        assert(fptr_);
        deref_ = fgetc(fptr_);
    }

    file_input_iterator::value_type file_input_iterator::operator* ()
    {
        return deref_;
    }

    file_input_iterator::pointer file_input_iterator::operator-> ()
    {
        return &deref_;
    }

    file_input_iterator &file_input_iterator::operator++ ()
    {
        deref_ = fgetc(fptr_);
        ++pos_;

        return *this;
    }

    file_input_iterator file_input_iterator::operator++ (int)
    {
        file_input_iterator temp(*this);
        ++(*this);

        return temp;
    }

    bool file_input_iterator::operator== (const file_input_iterator &other)
    {
        if (is_end() && other.is_end())
            return true;

        return fptr_ == other.fptr_ && pos_ == other.pos_;
    }

    bool file_input_iterator::operator!= (const file_input_iterator &other)
    {
        return !(*this == other);
    }

    bool file_input_iterator::is_end() const
    {
        return deref_ == EOF;
    }


    file_output_iterator::file_output_iterator(std::FILE *fptr) :
        fptr_(fptr)
    {
        assert(fptr_);
    }

    file_output_iterator &file_output_iterator::operator* ()
    {
        return *this;
    }

    file_output_iterator &file_output_iterator::operator++ ()
    {
        return *this;
    }

    file_output_iterator &file_output_iterator::operator++ (int)
    {
        return *this;
    }

    file_output_iterator &file_output_iterator::operator= (int ch)
    {
        std::fputc(ch, fptr_); return *this;
    }

} // close namespace imagexx
