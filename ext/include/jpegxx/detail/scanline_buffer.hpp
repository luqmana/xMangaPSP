// Copyright 2008 Edd Dawson.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file BOOST_LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef SCANLINE_BUFFER_HPP_0102_25082008
#define SCANLINE_BUFFER_HPP_0102_25082008

#include <algorithm>
#include <cstddef>
#include <vector>
#include <cstring>

namespace jpegxx
{
    namespace detail
    {
        //! A scanline buffer prepares an array of unsigned chars for consumption by a jpegxx::writer object. If the source iterators
        //! are const unsigned char pointers, the "begin" iterator is itself returned as the buffer, eliding a copy.
        template<typename InputIterator>
        class scanline_buffer
        {
            public:
                //! Construct a scanline buffer of the given size
                scanline_buffer(std::size_t sz) : buff_(sz) { }

                //! Read data from [resume, end) in to the buffer and return an iterator representing the point reached in the input
                //! stream.
                InputIterator buffer_next_scanline(InputIterator resume, InputIterator end)
                {
                    std::vector<unsigned char>::iterator b = buff_.begin();
                    std::size_t n = buff_.size();
                    while (n != 0 && resume != end)
                    {
                        *b++ = *resume++;
                        --n;
                    }

                    if (n) std::memset(&*b, 0, n);

                    return resume;
                }

                //! Returns a pointer to the beginning of the buffer
                const unsigned char *raw() const { return &buff_.front(); }

            private:
                std::vector<unsigned char> buff_;
        };

        //! This specialisation reduces the amount of copying performed in the (common) case where the iterators are const unsigned char
        //! pointers.
        template<>
        class scanline_buffer<const unsigned char *>
        {
            public:
                //! Construct a scanline buffer of the given size
                scanline_buffer(std::size_t sz) : p_(0), buff_(sz) { }

                //! Read data from [resume, end) in to the buffer and return an iterator representing the point reached in the input
                //! stream. If distance(resume, end) is greater than or equal to the buffer size, no data is copied and raw() will
                //! return begin.
                const unsigned char *buffer_next_scanline(const unsigned char *resume, const unsigned char *end)
                {
                    std::ptrdiff_t diff = end - resume;
                    if (diff >= static_cast<std::ptrdiff_t>(buff_.size()))
                    {
                        p_ = resume;
                        resume += buff_.size();
                    }
                    else
                    {
                        // Shouldn't really get here, means that the whole raster hasn't been supplied
                        std::copy(resume, end, buff_.begin());
                        std::memset(&buff_[diff], 0, buff_.size() - diff);
                        p_ = 0;
                    }

                    return resume;
                }

                //! Returns a pointer to the beginning of the buffer
                const unsigned char *raw() const { return p_ ? p_ : &buff_.front(); }

            private:
                const unsigned char *p_;
                std::vector<unsigned char> buff_;
        };

    } // close namespace detail

} // close namespace jpegxx

#endif // SCANLINE_BUFFER_HPP_0102_25082008
