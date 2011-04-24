// Copyright 2007 Edd Dawson.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file BOOST_LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef READ_HPP_0004_30062006
#define READ_HPP_0004_30062006

#include <algorithm>
#include <cassert>
#include <cstring>
#include <fstream>
#include <iterator>
#include <istream>
#include <memory>
#include <string>
#include <vector>

#include <imagexx/raster_details.hpp>
#include <imagexx/source.hpp>
#include <imagexx/stream_exception_state.hpp>
#include <imagexx/exceptions.hpp>

//! This namespace contains all the entities that comprise the library
namespace jpegxx
{
    //! This class provides the necessary machinery to implement jpegxx::read_image. It also provides an opporunity for
    //! pre-allocating a buffer of the appropriate size for the image raster once the image details are known and
    //! before the raster is read.
    //!
    //! To use the class one would do something like the following:
    //! \code
    //! my_iter_type begin = my_datastream.begin(); // Use any iterator type that dereferences to an integral byte value
    //! my_iter_type end = my_datastream.end();
    //!
    //! // jpeg code begin
    //! loader ldr;
    //! begin = ldr.read_header(begin, end); // Return value is kept for read_raster() (see below)
    //!
    //! size_t raster_bytes = ldr.details().width() * ldr.details().height() * ldr.details().channels();
    //!
    //! std::vector<unsigned char> raster(raster_bytes, 0); // pre-allocate space
    //! begin = ldr.read_raster(begin, end, raster.begin()); // raster data copied in to raster.begin()
    //! \endcode
    //!
    //! \section exception_safety Exception Safety
    //!
    //! An object of type jpegxx::loader has 2 well defined states: "header-ready" and "raster-ready".
    //!
    //! When a jpegxx::loader is header-ready, it means that it is ready to begin reading data from an iterator range
    //! that contains JPEG data (e.g. from a JPEG image file). A jpegxx::loader object is in this state after it is
    //! constructed and will remain in this state until a successful call to read_header() is made (i.e. a call is made
    //! that doesn't throw an exception). After such a successful call, the object will move in to the raster-ready
    //! state.
    //!
    //! When in the raster-ready state, details of the JPEG header that were retrieved through the previous call to
    //! read_header() are available through the details() member function. A jpegxx::loader will stay in the
    //! raster-ready state until a successful call to read_raster() is made (i.e. a call that returns before throwing).
    //! After calling read_raster(), the state of the jpegxx::loader will revert back to header-ready, regardless of
    //! whether the function returned normally or failed via a thrown exception.
    //!
    //! Calling details() or read_raster() on a jpegxx::loader that is not raster-ready will result in a
    //! imagexx::load_order_error being thrown. Calling read_header() on a jpegxx::loader is ok regardless of it's
    //! state.
    //!
    //! Calling details() on a jpegxx::loader does not change the state of the object under any circumstances.
    class loader
    {
        public:
            //! Create a loader object. buff_size is the amount of data that will be read from a stream at once. The default
            //! value is fine in all cases except where you only need to buffer up the minimum amount of data from a stream.
            loader(std::size_t buff_size = 8192);

            //! Destroys the object
            ~loader();

            //! Reads the header segment of a JPEG-formatted block of data delimted by [begin, end).
            //! The return value should be stored and passed as the first argument to read_raster (see the example
            //! code in the jpegxx::loader description).
            //!
            //! \throw jpegxx::decompression_failure
            //! \throw imagexx::load_order_error
            //! \throw imagexx::iterator_range_depleted
            template<typename InputIterator>
            InputIterator read_header(InputIterator begin, InputIterator end)
            {
                try
                {
                    std::auto_ptr<imagexx::source> src(new imagexx::iterator_source<InputIterator>(begin, end));
                    set_source(src);
                    read_details();
                }
                catch (...)
                {
                    clear();
                    throw;
                }

                return begin;
            }

            //! Returns the raster_details structure which represents the information that was obtained from the image
            //! header read using read_header.
            //!
            //! An object of type imagexx::load_order_error is thrown if this jpegxx::loader is not in a raster-ready
            //! state.
            //!
            //! \throw imagexx::load_order_error
            const imagexx::raster_details &details() const;

            //! Once a jpegxx::loader is in a raster-ready state (i.e. after a successful call to read_header()), this
            //! function may be used to read raster data from the additional data delimited by [resume, end).
            //! The value passed for resume should be that returned from read_header.
            //!
            //! An object of type imagexx::load_order_error is thrown if this jpegxx::loader is not in a raster-ready
            //! state.
            //!
            //! The iterator returned refers to the output location immediately following the raster.
            //!
            //! \throw jpegxx::decompression_failure
            //! \throw imagexx::iterator_range_depleted
            template<typename InputIterator, typename OutputIterator>
            OutputIterator read_raster(InputIterator resume, InputIterator end, OutputIterator target)
            {
                if (!pimpl_.get()) throw imagexx::load_order_error();

                try
                {
                    std::auto_ptr<imagexx::source> src(new imagexx::iterator_source<InputIterator>(resume, end));
                    set_source(src);

                    begin_raster_read();
                    std::vector<unsigned char> scanline(details_.channels() * details_.width());
                    for (std::size_t i = 0, n = details_.height(); i != n; ++i)
                    {
                        read_row(&scanline.front());
                        target = std::copy(scanline.begin(), scanline.end(), target);
                    }
                    end_raster_read();
                    clear();
                }
                catch (...)
                {
                    clear();
                    throw;
                }
                return target;
            }

        private:
            void set_source(std::auto_ptr<imagexx::source> &src);
            void read_details();
            void begin_raster_read();
            void end_raster_read();
            void read_row(unsigned char *scanline);
            void clear();

            //! Remains undefined. loader objects aren't copyable
            loader(const loader &);

            //! Remains undefined. loader objects aren't copyable
            loader &operator= (const loader &rhs); // remains undefined

        private:
            class impl;
#ifdef __BORLANDC__
            friend class impl;
#endif
            std::auto_ptr<impl> pimpl_;
            std::auto_ptr<imagexx::source> src_;
            imagexx::raster_details details_;
            std::size_t buff_size_;

    };

    //! Arguably the most important function of the library. Loads an image from the jpeg-data in the range [begin,
    //! end) and copies the data as unsigned chars in to target.
    //!
    //! target must contain enough room for the entire loaded raster. The number of bytes is not known in advance,
    //! so you should only use something like an std::back_insert_iterator for target. If you would like to allocate
    //! all the needed memory at once, you will need to use the jpegxx::loader class (which is still extremely simple
    //! to use).
    //!
    //! \throw jpegxx::decompression_failure
    //! \throw imagexx::iterator_range_depleted
    template<typename InputIterator, typename OutputIterator>
    imagexx::raster_details read_image(InputIterator begin, InputIterator end,
                                       OutputIterator target, std::size_t buff_size = 8192)
    {
        loader ldr(buff_size);

        begin = ldr.read_header(begin, end);
        imagexx::raster_details ret = ldr.details();
        ldr.read_raster(begin, end, target);

        return ret;
    }

    //! Convenience function. Provided to place a raster loaded from in in to the area that may be iterated over by
    //! target. target must 'point' to enough space for the raster, the size of which is unknown before hand. For this
    //! reason it is recommended that you use something like an std::back_insert_iterator.
    //!
    //! Details of the image are returned, unless an exception is thrown.
    //!
    //! \throw jpegxx::decompression_failure
    //! \throw imagexx::stream_depleted
    //! \throw imagexx::stream_io_failure
    template<typename OutputIterator>
    imagexx::raster_details read_image(std::istream &in, OutputIterator target, std::size_t buff_size = 8192)
    {
        in.clear();

        imagexx::stream_exception_state ses(in, std::ios_base::failbit | std::ios_base::badbit);

        try
        {
            std::istreambuf_iterator<char> begin(in), end;
            return read_image(begin, end, target, buff_size);
        }
        catch (const std::ios_base::failure &f)
        {
            if (in.good()) throw; // target is probably writing to a stream, nothing to do with us

            if (in.rdstate() & std::ios_base::eofbit) throw imagexx::stream_depleted(in.rdstate());
            else throw imagexx::stream_io_failure(in.rdstate(), f.what());
        }
        catch (const imagexx::iterator_range_depleted &) { throw imagexx::stream_depleted(in.rdstate() | std::ios_base::eofbit); }
    }


    //! Convenience function to place the raster data loaded from the file given by filename in to the output iterator
    //! given by target.
    //!
    //! An std::ifstream is created using filename and read_image(std::istream &, OutputIterator) is called passing it
    //! as the first argument.
    //!
    //! \throw jpegxx::decompression_failure
    //! \throw imagexx::ifstream_open_failure
    //! \throw imagexx::fstream_io_failure
    //! \throw imagexx::fstream_depleted
    template<typename OutputIterator>
    imagexx::raster_details read_image(const std::string &filename, OutputIterator target, std::size_t buff_size = 8192)
    {
        std::ifstream in(filename.c_str(), std::ios::binary);
        if (!in) throw imagexx::ifstream_open_failure(filename, std::ios::binary | std::ios::in);

        try { return read_image<OutputIterator>(in, target, buff_size); }
        catch (const imagexx::stream_depleted &sd) { throw imagexx::fstream_depleted(sd.state(), filename); }
        catch (const imagexx::stream_io_failure &f) { throw imagexx::fstream_io_failure(f.state(), filename, f.what()); }
    }

} // close namespace jpegxx

#endif // READ_HPP_0004_30062006

