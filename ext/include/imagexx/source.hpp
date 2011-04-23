// Copyright 2008 Edd Dawson.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file BOOST_LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef SOURCE_HPP_2323_31032008
#define SOURCE_HPP_2323_31032008

#include <cstddef>
#include "imagexx/raster_details.hpp"

namespace imagexx
{
    //! A source object is used to read data from a synchronous data source. iterator_source<> derives from this class an implements
    //! the virtual read function to read data from an arbitrary iterator pair.
    class source
    {
        public:
            //! Destroys the object mercilessly
            virtual ~source();

            //! Reads at most num_bytes bytes from the source in to the array whose first element is pointed to by target. The
            //! number of bytes successfully read from the source should be retured. 0 indicates that the source is depleted or
            //! inaccessible.
            //! The behaviour of this function can be customised in derived classes by overriding read_impl()
            std::size_t read(unsigned char *target, std::size_t num_bytes);

            //! Attempts to skip over num_bytes bytes from the source. Returns the number of bytes skipped over or 0 if the
            //! associated source is depleted or inaccessible.
            //! The behaviour of this function can be customised in derived classes by overriding skip_impl()
            std::size_t skip(std::size_t num_bytes);

        protected:
            source();

        private:
            //! Reads at most num_bytes bytes from the source in to the array whose first element is pointed to by target. The
            //! number of bytes successfully read from the source should be retured. 0 indicates that the source is depleted or
            //! inaccessible.
            virtual std::size_t read_impl(unsigned char *target, std::size_t num_bytes) = 0;

            //! Attempts to skip over num_bytes bytes from the source. Returns the number of bytes skipped over or 0 if the
            //! associated source is depleted or inaccessible.
            virtual std::size_t skip_impl(std::size_t num_bytes) = 0;

            //! Remains undefined. source objects aren't copyable
            source(const source &other);

            //! Remains undefined. source objects aren't copyable
            source &operator= (const source &other);

    };

    //! Implements the pngxx::detail::source interface for an arbitrary input iterator type
    template<typename InputIterator>
    class iterator_source : public source
    {
        public:
            //! Creates a source object that reads from the iterators given. begin_ref is held by reference internally and is
            //! updated as data is read to refer to the point at which the next read will occur.
            iterator_source(InputIterator &begin_ref, InputIterator end) :
                begin_(begin_ref),
                end_(end)
            {
            }

        private:
            //! Overrides source::read_impl() to read from the iterator pair given at construction time.
            std::size_t read_impl(unsigned char *target, std::size_t num_bytes)
            {
                std::size_t read = 0;
                if (begin_ == end_) return 0;

                while (num_bytes-- && begin_ != end_)
                {
                    *target++ = *begin_++;
                    ++read;
                }
                return read;
            }

            //! Overrides source::skip_impl() to skip over bytes in the iterator range delimted by the pair given at construction time.
            std::size_t skip_impl(std::size_t num_bytes)
            {
                std::size_t skipped = 0;
                if (begin_ == end_) return 0;

                while (num_bytes-- && begin_ != end_)
                {
                    *begin_++;
                    ++skipped;
                }
                return skipped;
            }

        private:
            InputIterator &begin_;
            InputIterator end_;
    };

} //close namespace imagexx

#endif // SOURCE_HPP_2323_31032008
