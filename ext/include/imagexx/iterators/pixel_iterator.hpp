// Copyright 2008 Edd Dawson.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file BOOST_LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef PIXEL_ITERATOR_HPP_1716_30032008
#define PIXEL_ITERATOR_HPP_1716_30032008

#include <cstring>
#include <cassert>
#include <iterator>

#include "imagexx/raster_details.hpp"
#include "imagexx/detail/optional_non_primitive.hpp"

#if defined(__BORLANDC__) || defined(__DMC__)
#define IMAGEXX_ARRAY_REF_DECL(element_type, name, num_elements) element_type *name
#else
// Borland and Digital Mars don't seem to like this, which is a shame since we don't get the nice
// compile time check for the number of elements in the array
#define IMAGEXX_ARRAY_REF_DECL(element_type, name, num_elements) element_type (&name)[num_elements]
#endif

namespace imagexx
{
    /*
    concept Composer
    {
        public:
            void set_rgb(unsigned char, unsigned char, unsigned char);
            void set_grey(unsigned char);
            void set_rgba(unsigned char, unsigned char, unsigned char, unsigned char);
            void set_grey_alpha(unsigned char, unsigned char);
            void next_pixel();

            // copy construction and assignment must be supported
    };

    concept Decomposer
    {
        public:
            unsigned char get_red();
            unsigned char get_green();
            unsigned char get_blue();
            unsigned char get_alpha();
            unsigned char get_grey();
            void next_pixel();

            bool operator== (const Decomposer &other) const;

            // copy construction and assignment must be supported
    };
    */

    //! The set and get static template functions within this class manipulate a (de)composer to read and write
    //! an RGB pixel from and to an array of unsigned chars.
    struct rgb_forwarder
    {
        //! This typedef must exist so that pixel_compose_iterator and pixel_decompose_iterator know how big their
        //! internal byte arrays should be when this class is given as their 2nd template parameter.
        enum { channels = 3 };

        //! Calls c.set_rgb() with the 3 bytes in chans as the arguments
        template<typename Composer>
        static void set(Composer &c, IMAGEXX_ARRAY_REF_DECL(const unsigned char, chans, channels))
        {
            c.set_rgb(chans[0], chans[1], chans[2]);
            c.next_pixel();
        }

        //! Assigns d.get_red(), d.get_green() and d.get_blue() to the 3 bytes in chans
        template<typename Decomposer>
        static void get(Decomposer &d, IMAGEXX_ARRAY_REF_DECL(unsigned char, chans, channels))
        {
            chans[0] = d.get_red();
            chans[1] = d.get_green();
            chans[2] = d.get_blue();
            d.next_pixel();
        }
    };

    //! The set and get static template functions within this class manipulate a (de)composer to read and write
    //! an RGB pixel from and to an array of unsigned chars.
    struct rgba_forwarder
    {
        //! This typedef must exist so that pixel_compose_iterator and pixel_decompose_iterator know how big their
        //! internal byte arrays should be when this class is given as their 2nd template parameter.
        enum { channels = 4 };

        //! Calls c.set_rgb() with the 3 bytes in chans as the arguments
        template<typename Composer>
        static void set(Composer &c, IMAGEXX_ARRAY_REF_DECL(const unsigned char, chans, channels))
        {
            c.set_rgba(chans[0], chans[1], chans[2], chans[3]);
            c.next_pixel();
        }

        //! Assigns d.get_red(), d.get_green() and d.get_blue() to the 3 bytes in chans
        template<typename Decomposer>
        static void get(Decomposer &d, IMAGEXX_ARRAY_REF_DECL(unsigned char, chans, channels))
        {
            chans[0] = d.get_red();
            chans[1] = d.get_green();
            chans[2] = d.get_blue();
            chans[3] = d.get_alpha();
            d.next_pixel();
        }
    };

    //! The set and get static template functions within this class manipulate a (de)composer to read and write
    //! a greyscale pixel from and to an array of unsigned chars.
    struct grey_forwarder
    {
        //! This typedef must exist so that pixel_compose_iterator and pixel_decompose_iterator know how big their
        //! internal byte arrays should be when this class is given as their 2nd template parameter.
        enum { channels = 1 };

        //! Calls c.set_grey() with the single byte in chans as the arguments
        template<typename Composer>
        static void set(Composer &c, IMAGEXX_ARRAY_REF_DECL(const unsigned char, chans, channels))
        {
            c.set_grey(chans[0]);
            c.next_pixel();
        }

        //! Assigns d.get_grey() to the single byte in chans
        template<typename Decomposer>
        static void get(Decomposer &d, IMAGEXX_ARRAY_REF_DECL(unsigned char, chans, channels))
        {
            chans[0] = d.get_grey();
            d.next_pixel();
        }
    };


    //! The set and get static template functions within this class manipulate a (de)composer to read and write
    //! a greyscale pixel from and to an array of unsigned chars.
    struct grey_alpha_forwarder
    {
        //! This typedef must exist so that pixel_compose_iterator and pixel_decompose_iterator know how big their
        //! internal byte arrays should be when this class is given as their 2nd template parameter.
        enum { channels = 2 };

        //! Calls c.set_grey() with the single byte in chans as the arguments
        template<typename Composer>
        static void set(Composer &c, IMAGEXX_ARRAY_REF_DECL(const unsigned char, chans, channels))
        {
            c.set_grey_alpha(chans[0], chans[1]);
            c.next_pixel();
        }

        //! Assigns d.get_grey() to the single byte in chans
        template<typename Decomposer>
        static void get(Decomposer &d, IMAGEXX_ARRAY_REF_DECL(unsigned char, chans, channels))
        {
            chans[0] = d.get_grey();
            chans[1] = d.get_alpha();
            d.next_pixel();
        }
    };

    //! Models the output iterator concept defined by the C++ standard library. Designed for use in conjunction with
    //! the image reading functions.
    //!
    //! A pixel_compose_iterator object collects bytes in an array as they are assigned to it when dereferenced. Once
    //! the array is full it's contents are flushed to an external image class via the composer given to the
    //! constructor.
    //!
    //! The Forwarder type given as the second template parameter should be either rgb_forwarder, rgba_forwarder,
    //! grey_forwarder or grey_alpha_forwarder depending on the kind of image being decoded.
    template<typename Composer, typename Forwarder>
    class pixel_compose_iterator : public std::iterator<std::output_iterator_tag, void, void, void, void>
    {
        public:
            //! Constructs an iterator that writes pixels via the given composer
            explicit pixel_compose_iterator(const Composer &impl) : impl_(impl), p_(chans_) { }

            //! Copy constructor
            pixel_compose_iterator(const pixel_compose_iterator &other) :
                impl_(other.impl_),
                p_(chans_ + (other.p_ - other.chans_))
            {
                std::memcpy(chans_, other.chans_, Forwarder::channels);
            }

            //! Copy assignment operator
            pixel_compose_iterator &operator= (const pixel_compose_iterator &other)
            {
                impl_ = other.impl_;
                p_ = chans_ + (other.p_ - other.chans_);
                std::memcpy(chans_, other.chans_, Forwarder::channels);
                return *this;
            }

            //! When a pixel_compose_iterator is dereferenced, it yields a reference to itself. Thus assigning a char to
            //! a dereferenced pixel_compose_iterator invokes this operator, which adds c to the internal array and
            //! flushes the array via the composer if necessary.
            pixel_compose_iterator &operator= (unsigned char c)
            {
                *p_ = c;
                if (++p_ == chans_ + Forwarder::channels)
                {
                    p_ = chans_;
                    Forwarder::set(impl_, chans_);
                }
                return *this;
            }

            //! Returns a reference to this object
            pixel_compose_iterator &operator* () { return *this; }

            //! Pre-increments this iterator with the usual semantics
            pixel_compose_iterator &operator++ () { return *this; }

            //! Post-increments this iterator with the usual semantics
            pixel_compose_iterator &operator++ (int) { return *this; }

        private:
            Composer impl_;
            unsigned char chans_[Forwarder::channels];
            unsigned char *p_;
    };


    //! Models the input iterator concept defined by the C++ standard library. Designed for use in conjunction with
    //! image writing functions.
    //!
    //! A pixel_decompose_iterator object collects bytes in an array from an external image via a decomposer and yields
    //! them in order when dereferenced. Once each byte in the array has been returned by operator* it's contents are
    //! replenished.
    //!
    //! The Forwarder type given as the second template parameter should be rgb_forwarder, rgba_forwarder,
    //! grey_forwarder or grey_alpha_forwarder depending on the kind of image being encoded.
    template<typename Decomposer, typename Forwarder>
    class pixel_decompose_iterator : public std::iterator<std::input_iterator_tag, const unsigned char>
    {
        public:
            //! Constructs an iterator that may read bytes through the given decomposer. details should be the same
            //! object that is passed as the first argument of write_image().
            pixel_decompose_iterator(const Decomposer &impl, const raster_details &details) :
                impl_(impl),
                p_(chans_ + Forwarder::channels),
                remaining_(details.width() * details.height())
            {
            }

            //! Constructs an iterator that represents the end of any sequence
            pixel_decompose_iterator() : impl_(), p_(chans_ + Forwarder::channels), remaining_(0) { }

            //! Copy constructor
            pixel_decompose_iterator(const pixel_decompose_iterator &other) :
                impl_(other.impl_),
                p_(chans_ + (other.p_ - other.chans_)),
                remaining_(other.remaining_)
            {
                std::memcpy(chans_, other.chans_, Forwarder::channels);
            }

            //! Copy assigment operator
            pixel_decompose_iterator &operator= (const pixel_decompose_iterator &other)
            {
                impl_ = other.impl_;
                p_ = chans_ + (other.p_ - other.chans_);
                std::memcpy(chans_, other.chans_, Forwarder::channels);
                remaining_ = other.remaining_;
                return *this;
            }

            //! Returns the current element in the internal array of bytes
            const value_type &operator* ()
            {
                check_refill();
                assert(p_ != chans_ + Forwarder::channels);
                return *p_;
            }

            //! Returns a pointer to the current element in the array of bytes. This function doesn't make sense, but
            //! it's included for completeness of implementation w.r.t the input iterator concept
            pointer operator-> ()
            {
                check_refill();
                return p_;
            }

            //! Pre-increments this iterator with the usual semantics
            pixel_decompose_iterator &operator++ ()
            {
                check_refill();
                ++p_;
                return *this;
            }

            //! Post-increments this iterator with the usual semantics
            pixel_decompose_iterator operator++ (int)
            {
                pixel_decompose_iterator ret(*this);
                ++*this;
                return ret;
            }

            //! Compares this iterator for equality with other.
            bool operator== (const pixel_decompose_iterator &other) const
            {
                pixel_decompose_iterator lhs(*this);
                pixel_decompose_iterator rhs(other);
                lhs.check_refill();
                rhs.check_refill();
                const Decomposer *d1 = lhs.impl_.get();
                const Decomposer *d2 = rhs.impl_.get();

                if (d1 && d2) return *d1 == *d2 && (lhs.p_ - lhs.chans_ == rhs.p_ - rhs.chans_);
                if (d1 || d2) return false; // one is the end, the other isn't
                return true; // both are the end
            }

            //! Returns the negation of operator==
            bool operator!= (const pixel_decompose_iterator &other) const
            {
                return !(*this == other);
            }

        private:
            //! Replenishes the internal array of bytes from the decomposer if needed
            void check_refill()
            {
                if (impl_.get() && p_ == chans_ + Forwarder::channels)
                {
                    if (remaining_)
                    {
                        --remaining_;
                        p_ = chans_;
                        Forwarder::get(*impl_.get(), chans_);
                    }
                    else
                        impl_ = detail::optional_non_primitive<Decomposer>();
                }
            }

        private:
            detail::optional_non_primitive<Decomposer> impl_;

            // INVARIANT:
            // p_ always points to a valid byte that was read from the source image, or one-past-the-end of chans_
            unsigned char chans_[Forwarder::channels];
            unsigned char *p_;
            std::size_t remaining_;
    };

} // close namespace imagexx

#endif // PIXEL_ITERATOR_HPP_1716_30032008
