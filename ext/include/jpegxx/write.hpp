// Copyright 2008 Edd Dawson.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file BOOST_LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef WRITE_HPP_0436_20082008
#define WRITE_HPP_0436_20082008

#include <iterator>
#include <memory>
#include <fstream>

#include <imagexx/raster_details.hpp>
#include <imagexx/sink.hpp>
#include <imagexx/stream_exception_state.hpp>
#include <imagexx/exceptions.hpp>
#include <jpegxx/detail/scanline_buffer.hpp>
#include <jpegxx/detail/writer.hpp>

namespace jpegxx
{
    namespace detail
    {
        //! This meta-function returns const unsigned char * if T is unsigned char *. Else T is returned.
        template<typename T>
        struct add_const_if_uchar_ptr { typedef T type; };

        template<>
        struct add_const_if_uchar_ptr<unsigned char *> { typedef const unsigned char *type;  };

    } // close namespace detail

    //! Writes the image with the dimensions and pixel format given by details and whose raster is delimited by [begin, end) to out.
    //! A jpegxx::compression_failure may be thrown if the image cannot be saved for some reason. The quality argument is clamped
    //! to values in [0, 1]. Higher values yield better looking results at the expense of a larger amount of output.
    //!
    //! \throw jpegxx::compression_failure
    //! \throw jpegxx::size_format_limitation
    //! \throw jpegxx::alpha_format_limitation
    template<typename InputIterator, typename OutputIterator>
    OutputIterator write_image(const imagexx::raster_details &details, InputIterator begin, InputIterator end, OutputIterator out,
                               double quality = 0.8, std::size_t buff_size = 8192)
    {
        typedef typename detail::add_const_if_uchar_ptr<InputIterator>::type it_t;
        const std::size_t scanline_bytes = details.width() * details.channels();
        detail::scanline_buffer<it_t> buff(scanline_bytes);
        it_t resume = begin;

        std::auto_ptr<imagexx::sink> snk(new imagexx::iterator_sink<OutputIterator>(out));
        detail::writer w(details, quality, snk, buff_size);

        for (std::size_t i = 0, n = details.height(); i != n; ++i)
        {
            resume = buff.buffer_next_scanline(resume, end);
            w.write_scanline(buff.raw());
        }
        w.finish();

        return out;
    }

    //! Writes the image whose raster is described by details and whose bytes are represented by the iterator range [begin, end) to the
    //! target represented by the given std::ostream. The quality argument should be in the range [0, 1] (and is clamped to that
    //! range). Higher values produce better quality images at the expense of a larger amount of data.
    //!
    //! An exception of type jpegxx::compression_failure will be thrown if compression fails.
    //!
    //! out is returned on success.
    //!
    //! \throw jpegxx::compression_failure
    //! \throw jpegxx::size_format_limitation
    //! \throw jpegxx::alpha_format_limitation
    //! \throw imagexx::stream_io_failure
    template<typename InputIter>
    std::ostream &write_image(const imagexx::raster_details &details, std::ostream &out,
                              InputIter begin, InputIter end,
                              double quality = 0.8, std::size_t buff_size = 8192)
    {
        imagexx::stream_exception_state ses(out, std::ios_base::failbit | std::ios_base::badbit | std::ios_base::eofbit);

        try { write_image(details, begin, end, std::ostreambuf_iterator<char>(out), quality, buff_size); }
        catch (const std::ios_base::failure &f) { throw imagexx::stream_io_failure(out.rdstate(), f.what()); }
        return out.flush();
    }

    //! Writes the image whose raster is described by details and whose bytes are represented by the iterator range
    //! [begin, end) to the file with the given name. The quality argument should be in the range [0, 1] (and is
    //! clamped to that range). Higher values produce better quality images at the expense of a large data size.
    //!
    //! An object of type jpegxx::compression_failure will be thrown if compression fails. If a the file could not be
    //! opened for writing, an object of type imagexx::ofstream_open_failure is thrown.
    //!
    //! \throw jpegxx::compression_failure
    //! \throw jpegxx::size_format_limitation
    //! \throw jpegxx::alpha_format_limitation
    //! \throw imagexx::fstream_io_failure
    //! \throw imagexx::ofstream_open_failure
    template<typename InputIter>
    void write_image(const imagexx::raster_details &details, const std::string &filename,
                     InputIter begin, InputIter end,
                     double quality = 0.8, std::size_t buff_size = 8192)
    {
        std::ofstream out(filename.c_str(), std::ios::binary);
        if (!out)
            throw imagexx::ofstream_open_failure(filename, std::ios::binary | std::ios::out);

        try { write_image(details, out, begin, end, quality, buff_size); }
        catch (const imagexx::stream_io_failure &f) { throw imagexx::fstream_io_failure(f.state(), filename, f.what()); }
    }

} // close namespace jpegxx

#endif // WRITE_HPP_0436_20082008
