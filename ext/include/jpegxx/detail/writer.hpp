// Copyright 2008 Edd Dawson.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file BOOST_LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef WRITER_HPP_2221_25082008
#define WRITER_HPP_2221_25082008

#include <memory>
#include <cstddef>

namespace imagexx
{
    class sink;
    class raster_details;
}

namespace jpegxx
{
    namespace detail
    {
        //! This class does the work of encoding JPEG rasters.
        class writer
        {
            public:
                //! Create a writer object that is ready to encode an image whose details are given by d to the given sink. Use a
                //! buffer size of buff_size to pass data to the underlying IJG library
                writer(const imagexx::raster_details &d, double quality, std::auto_ptr<imagexx::sink> &snk, std::size_t buff_size);

                //! Destroys the object mercilessly
                ~writer();

                //! Writes a scanline of data to the sink via the intermediate buffer
                void write_scanline(const unsigned char *scanline);

                //! Once all scanlines have been written, this should be called to flush any remaining buffered data.
                void finish();

            private:
                //! Remains unimplemented. Copying is not allowed
                writer(const writer &);

                //! Remains unimplemented. Copying is not allowed
                writer &operator= (const writer &);

            private:
                class impl;
                std::auto_ptr<impl> pimpl_;
        };

    } // close namespace detail

} // close anonymous namespace

#endif // WRITER_HPP_2221_25082008
