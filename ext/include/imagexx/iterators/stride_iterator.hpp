// Copyright 2008 Edd Dawson.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file BOOST_LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef STRIDE_ITERATOR_HPP_2319_11082008
#define STRIDE_ITERATOR_HPP_2319_11082008

#include <cstddef>
#include <iterator>

#include "imagexx/raster_details.hpp"
#include "imagexx/detail/optional_non_primitive.hpp"

namespace imagexx
{
    //! stride_output_iterators can be used to fill rasters that require padding on either side (or boths sides).
    //! Padding bytes are filled with zeros.
    template<typename OutputIterator>
    class stride_output_iterator : public std::iterator<std::output_iterator_tag, void, void, void, void>
    {
        public:
            //! Create a stride_output_iterator that will treat the first left and last right bytes of each line as padding.
            //! width specifies the width of the image proper IN BYTES, exluding padding. Bytes are forwarded to a copy of it, held
            //! internally.
            stride_output_iterator(const OutputIterator &it, std::size_t left, std::size_t width, std::size_t right) :
                it_(it),
                left_(left),
                right_(right),
                total_(left + width + right),
                current_(0)
            {
            }

            //! Create a stride_output_iterator that will treat the first left and last right bytes of each line as padding.
            //! The width stored in the raster_details object specifies the width of the image proper IN BYTES, exluding padding. Bytes
            //! are forwarded to a copy of it, held internally.
            stride_output_iterator(const OutputIterator &it, std::size_t left, std::size_t right, const imagexx::raster_details &d) :
                it_(it),
                left_(left),
                right_(right),
                total_(left + right + d.width() * d.channels()),
                current_(0)
            {
            }

            //! Does nothing. Exists to conform to the standard library output iterator requirements
            stride_output_iterator &operator++ () { return *this; }

            //! Does nothing. Exists to conform to the standard library output iterator requirements
            stride_output_iterator &operator++ (int) { return *this; }

            //! Does nothing. Exists to conform to the standard library output iterator requirements
            stride_output_iterator &operator* () { return *this; }

            //! Writes uc to the next location that isn't padding. If that location is on the right-hand edge of the image proper, the
            //! right hand padding for the line is zero'd out.
            stride_output_iterator &operator= (unsigned char uc)
            {
                if (current_ == 0) fill_in(left_);
                *it_++ = uc;
                current_ = (current_ + 1) % total_;
                if (current_ == total_ - right_) fill_in(right_);
                return *this;
            }

        private:
            //! Moves the internally held iterator on n places.
            void fill_in(std::size_t n)
            {
                current_ = (current_ + n) % total_;
                while (n--) *it_++ = 0;
            }

        private:
            OutputIterator it_;
            std::size_t left_;
            std::size_t right_;
            std::size_t total_;
            std::size_t current_;
    };

    //! Shorthand for stride_output_iterator<OutputIterator>(it, left, width, right). Usually it is sufficient to create a temporary
    //! stride_output_iterator at the call site of the image loading function, whence this function can be used.
    template<typename OutputIterator>
    stride_output_iterator<OutputIterator> pad(OutputIterator it, std::size_t left, std::size_t width, std::size_t right)
    {
        return stride_output_iterator<OutputIterator>(it, left, width, right);
    }

    //! Shorthand for stride_output_iterator<OutputIterator>(it, left, right, d). Usually it is sufficient to create a temporary
    //! stride_output_iterator at the call site of the image loading function, whence this function can be used
    template<typename OutputIterator>
    stride_output_iterator<OutputIterator> pad(OutputIterator it, std::size_t left, std::size_t right, const imagexx::raster_details &d)
    {
        return stride_output_iterator<OutputIterator>(it, left, right, d);
    }

    //! A pair stride_input_iterator iterators can be used to filter out padding elements in an existing iterator range. The "begin"
    //! stride_input_iterator takes the number of bytes of padding (on each side) and the width of the scanline proper. When
    //! incrementing and dereferencing, only the non-padding bytes come through.
    //!
    //! A begin/end pair of stride_input_iterators should be constructed from underlying iterators that are separated by a multiple
    //! of the number of elements in a scan-line, inclusive of padding.
    template<typename InputIterator>
    class stride_input_iterator
    {
        private:
            typedef std::iterator_traits<InputIterator> traits;

        public:
            typedef typename traits::reference reference;
            typedef typename traits::pointer pointer;
            typedef typename traits::value_type value_type;
            typedef typename traits::difference_type difference_type;
            typedef std::input_iterator_tag iterator_category;

            //! Create a stride_input_iterator that will treat the first left and last right bytes of each line as padding.
            //! width specifies the width of the image proper IN BYTES, exluding padding. Bytes are extracted from a copy of begin,
            //! held internally.
            //! You should use this constructor to create the iterator referring to the beginning of a range.
            stride_input_iterator(const InputIterator &begin, std::size_t left, std::size_t width, std::size_t right) :
                it_(begin),
                left_(left),
                right_(right),
                total_(left + width + right),
                current_(0)
            {
            }

            //! Create a stride_input_iterator that will treat the first left and last right bytes of each line as padding.
            //! d.width() specifies the width of the image proper IN BYTES, exluding padding. Bytes are extracted from a copy of begin,
            //! held internally.
            //! You should use this constructor to create the iterator referring to the beginning of a range.
            stride_input_iterator(const InputIterator &begin, std::size_t left, std::size_t right, const imagexx::raster_details &d) :
                it_(begin),
                left_(left),
                right_(right),
                total_(left + right + d.width() * d.channels()),
                current_(0)
            {
            }

            //! Create a stride_input_iterator that refers to the end of the range. The distance between end and the underlying iterator
            //! passed to the constructor to create the "begin" stride_input_iterator should be a multiple of the total number of BYTES
            //! in a scanline, including padding.
            stride_input_iterator(const InputIterator &end) :
                it_(end),
                left_(0),
                right_(0),
                total_(0),
                current_(0)
            {
            }

            //! Pre-increment with the usual semantics. Will skip over padding bytes as it encounters them.
            stride_input_iterator &operator++ ()
            {
                if (current_ == 0)
                {
                    advance(left_ + 1);
                    if (current_ == total_ - right_) advance(right_);
                }
                else if (current_ == total_ - right_ - 1) advance(right_ + 1);
                else advance(1);
                return *this;
            }

            //! Post-increment with the usual semantics. Will skip over padding bytes as it encounters them.
            stride_input_iterator operator++ (int)
            {
                stride_input_iterator ret(*this);
                ++*this;
                return ret;
            }

            //! Returns the object that this iterator refers to
            reference operator* ()
            {
                if (current_ == 0) advance(left_);
                return *it_;
            }

            //! Returns a pointer to the object that this iterator refers to
            pointer operator-> ()
            {
                if (current_ == 0) advance(left_);
                return it_.operator->();
            }

            //! Returns true if this iterator refers to the same position as rhs
            bool operator== (const stride_input_iterator &rhs) const { return it_ == rhs.it_; }

            //! Returns true if this iterator refers to a different position as rhs
            bool operator!= (const stride_input_iterator &rhs) const { return !(*this == rhs); }

        private:
            //! Moves the underlying iterator fwd places forward and keeps track of the number of bytes moved through so far.
            void advance(std::size_t fwd)
            {
                std::advance(it_, fwd);
                current_ = (current_ + fwd) % total_;
            }

        private:
            InputIterator it_;
            std::size_t left_;
            std::size_t right_;
            std::size_t total_;
            std::size_t current_;
    };

    //! Shorthand for stride_input_iterator<InputIterator>(begin, left, width, right). Usually it is sufficient to create a temporary
    //! stride_input_iterator at the call site of the image loading function, whence this function can be used.
    template<typename InputIterator>
    stride_input_iterator<InputIterator> unpad(InputIterator begin, std::size_t left, std::size_t width, std::size_t right)
    {
        return stride_input_iterator<InputIterator>(begin, left, width, right);
    }

    //! Shorthand for stride_input_iterator<InputIterator>(begin, left, right, d). Usually it is sufficient to create a temporary
    //! stride_input_iterator at the call site of the image loading function, whence this function can be used.
    template<typename InputIterator>
    stride_input_iterator<InputIterator>
    unpad(InputIterator begin, std::size_t left, std::size_t right, const imagexx::raster_details &d)
    {
        return stride_input_iterator<InputIterator>(begin, left, right, d);
    }

    //! Shorthand for stride_input_iterator<InputIterator>(end). Usually it is sufficient to create a temporary stride_input_iterator
    //! at the call site of the image loading function, whence this function can be used.
    template<typename InputIterator>
    stride_input_iterator<InputIterator>
    unpad(InputIterator end)
    {
        return stride_input_iterator<InputIterator>(end);
    }

} //close namespace imagexx

#endif // STRIDE_ITERATOR_HPP_2319_11082008

