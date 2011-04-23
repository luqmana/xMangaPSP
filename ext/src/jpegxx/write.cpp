// Copyright 2008 Edd Dawson.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file BOOST_LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <jpegxx/write.hpp>
#include <jpegxx/exceptions.hpp>

#include <stdio.h> // jpeglib.h forgets to #include this
//#include "jpeglib.h"
//#include "jerror.h"
#include "ijg_headers.hpp"
#include "best_exception_store.hpp"

#include <algorithm>
#include <cassert>
#include <csetjmp>

#define TRY(expr) \
    do { \
    if (setjmp(jmpbuf_) == 0) expr; \
    ex_store_.rethrow_stored_exception(); \
    } while (false)

namespace jpegxx
{
    namespace
    {
        // only for non-negative numbers
        UINT16 round(double d)
        {
            return static_cast<UINT16>(d + 0.5);
        }
    }

    namespace detail
    {
        class writer::impl
        {
            public:
                impl(const imagexx::raster_details &d, std::auto_ptr<imagexx::sink> &snk, std::size_t buff_size, double quality) :
                    ex_store_(compression_failure("An unknown compression error occurred")),
                    buffer_(std::max(buff_size, std::size_t(1))),
                    snk_(snk)
                {
                    if (d.width() > 65535 || d.height() > 65535)
                        throw size_format_limitation(d);

                    if (d.type() != imagexx::rgb && d.type() != imagexx::grey)
                        throw alpha_format_limitation(d);

                    compressor_.err = jpeg_std_error(&error_mgr_);
                    error_mgr_.error_exit = error;
                    error_mgr_.output_message = output;

                    // Set user-defined data to be this object
                    compressor_.client_data = this;

                    TRY(jpeg_create_compress(&compressor_));

                    // Set up destination manager callbacks
                    dest_mgr_.init_destination = init_destination;
                    dest_mgr_.empty_output_buffer = empty_output_buffer;
                    dest_mgr_.term_destination = term_destination;

                    // Finish intialising the compressor
                    compressor_.dest = &dest_mgr_;
                    compressor_.image_width = d.width();
                    compressor_.image_height = d.height();
                    compressor_.input_components = d.channels();
                    compressor_.in_color_space = (d.type() == imagexx::rgb) ? JCS_RGB : JCS_GRAYSCALE;

                    jpeg_set_defaults(&compressor_);

                    if (d.width_mm() && d.height_mm())
                    {
                        compressor_.density_unit = 2; // dots per cm
                        compressor_.X_density = jpegxx::round(d.width() / (d.width_mm() / 10));
                        compressor_.Y_density = jpegxx::round(d.height() / (d.height_mm() / 10));
                    }

                    int q = static_cast<int>(100.0 * quality); // quality 0...100 (should be)
                    jpeg_set_quality(&compressor_, std::min(100, std::max(0, q)), TRUE);

                    // Start the compression
                    if (setjmp(jmpbuf_) == 0) 
                        jpeg_start_compress(&compressor_, TRUE);

                    if (ex_store_.has_exception()) jpeg_destroy_compress(&compressor_);
                    ex_store_.rethrow_stored_exception();
                }

                ~impl()
                {
                    jpeg_destroy_compress(&compressor_);
                }

                void write_scanline(const unsigned char *scanline)
                {
                    assert(compressor_.next_scanline < compressor_.image_height);
                    JSAMPROW rows[1] = { const_cast<unsigned char *>(scanline) };

                    TRY(jpeg_write_scanlines(&compressor_, rows, 1));
                }

                void finish()
                {
                    assert(compressor_.next_scanline == compressor_.image_height);
                    TRY(jpeg_finish_compress(&compressor_));
                }

            private:
                static void error(j_common_ptr cinfo)
                {
                    // !!!!!! IMPORTANT !!!!!!!
                    // Please don't touch this function unless you understand the interaction of longjmp with
                    // C++ code. At the very least, please read the notes on "Error Handling" in docs/notebook.html

                    // Stash an exception to be thrown later.
                    // Note that jpgexx::compression_failure's constructor may throw bad_alloc!
                    try
                    {
                        impl &self = *static_cast<impl *>(cinfo->client_data);

                        // We can translate an OOM error in to something more appropriate than a general 
                        // decompression_failure exception
                        if (cinfo->err->msg_code == JERR_OUT_OF_MEMORY) 
                            self.ex_store_.store(std::bad_alloc());
                        else
                        {
                            char buffer[JMSG_LENGTH_MAX];
                            (*cinfo->err->format_message)(cinfo, buffer);
                            self.ex_store_.store(compression_failure(buffer));
                        }
                    }
                    catch (const std::bad_alloc &ba)
                    {
                        impl &self = *static_cast<impl *>(cinfo->client_data);
                        self.ex_store_.store(ba);
                    }

                    // Note:
                    // There are no variables with automatic storage in C++ between here and the corresponding setbuf() 
                    // calls.
                    std::longjmp(static_cast<impl *>(cinfo->client_data)->jmpbuf_, 42);
                }

                static void output(j_common_ptr)
                {
                    // TODO: perhaps store the warning somewhere
                }

                static void init_destination(j_compress_ptr cinfo)
                {
                    impl &self = *static_cast<impl *>(cinfo->client_data);
                    self.dest_mgr_.next_output_byte = &(self.buffer_.front());
                    self.dest_mgr_.free_in_buffer = self.buffer_.size();
                }

                static boolean empty_output_buffer(j_compress_ptr cinfo)
                {
                    impl &self = *static_cast<impl *>(cinfo->client_data);

                    // flush entire buffer
                    try { self.snk_->write(&self.buffer_.front(), self.buffer_.size()); }
                    catch (...)
                    {
                        self.ex_store_.store_current_exception();

                        // Returning FALSE is typically used to help interleave other operations with
                        // the saving of a JPEG image. The IJG library calls this "I/O suspension mode".
                        // We hijack this mode to get out of IJG's C calls quickly and back in to C++ land
                        // where we can re-throw a stored exception
                        return FALSE;
                    }

                    self.dest_mgr_.next_output_byte = &(self.buffer_.front());
                    self.dest_mgr_.free_in_buffer = self.buffer_.size();

                    return TRUE;
                }

                static void term_destination(j_compress_ptr cinfo)
                {
                    impl &self = *static_cast<impl *>(cinfo->client_data);

                    // flush partially filled buffer
                    std::size_t n = self.buffer_.size() - self.dest_mgr_.free_in_buffer;
                    try { self.snk_->write(&self.buffer_.front(), n); }
                    catch (...) { self.ex_store_.store_current_exception(); }
                }

            private:
                best_exception_store ex_store_;
                std::jmp_buf jmpbuf_;
                jpeg_compress_struct compressor_;
                jpeg_error_mgr error_mgr_;
                jpeg_destination_mgr dest_mgr_;
                std::vector<JOCTET> buffer_;
                std::auto_ptr<imagexx::sink> snk_;
        };

        writer::writer(const imagexx::raster_details &d, double quality, std::auto_ptr<imagexx::sink> &snk, std::size_t buff_size) :
            pimpl_(new impl(d, snk, buff_size, quality))
        {
        }

        writer::~writer()
        {
        }

        void writer::write_scanline(const unsigned char *scanline)
        {
            pimpl_->write_scanline(scanline);
        }

        void writer::finish()
        {
            pimpl_->finish();
        }

    } // close namespace detail

} // close namespace jpegxx

