// Copyright 2008 Edd Dawson.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file BOOST_LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <jpegxx/read.hpp>
#include <jpegxx/exceptions.hpp>

#include <imagexx/source.hpp>

#include "ijg_headers.hpp"
#include "best_exception_store.hpp"

#include <new>
#include <cassert>
#include <csetjmp>

#define TRY(expr) \
    do { \
    if (setjmp(jmpbuf_) == 0) expr; \
    ex_store_.rethrow_stored_exception(); \
    } while (false)

namespace jpegxx
{
    class loader::impl
    {
        public:
            impl(loader *ldr, std::size_t buff_size) :
                ex_store_(decompression_failure("An unknown decompression error occurred")),
                buff_(std::max(buff_size, std::size_t(1))),
                ldr_(ldr)
            {
                decomp_.err = jpeg_std_error(&error_mgr_);
                error_mgr_.error_exit = error;
                error_mgr_.output_message = output;
                decomp_.client_data = this;

                TRY(jpeg_create_decompress(&decomp_));

                // Set up buffering callbacks
                source_mgr_.init_source = init_source;
                source_mgr_.fill_input_buffer = fill_input_buffer;
                source_mgr_.skip_input_data = skip_input_data;
                source_mgr_.resync_to_restart = jpeg_resync_to_restart;
                source_mgr_.term_source = term_source;
                decomp_.src = &source_mgr_;
            }

            ~impl()
            {
                jpeg_destroy_decompress(&decomp_);
            }

            void read_details(imagexx::raster_details &d)
            {
                TRY(jpeg_read_header(&decomp_, TRUE));

                jpeg_calc_output_dimensions(&decomp_);
                imagexx::pixel_format fmt = (decomp_.output_components == 3) ?  imagexx::rgb : imagexx::grey;

                double width_mm = 0;
                double height_mm = 0;

                double units_conversion = 0;
                if (decomp_.density_unit == 1) // dots per inch
                    units_conversion = 25.4; // millimeters in an inch
                else if (decomp_.density_unit == 2) // dots per cm
                    units_conversion = 10; // millimeters in a centimeter

                width_mm = decomp_.X_density ? (decomp_.output_width / double(decomp_.X_density)) * units_conversion : 0;
                height_mm = decomp_.Y_density ? (decomp_.output_height / double(decomp_.Y_density)) * units_conversion : 0;
                
                d = imagexx::raster_details(fmt, decomp_.output_width, decomp_.output_height, width_mm, height_mm);
            }

            void start_raster_read()
            {
                TRY(jpeg_start_decompress(&decomp_));
            }

            void end_raster_read()
            {
                assert(decomp_.output_scanline == decomp_.output_height);
                TRY(jpeg_finish_decompress(&decomp_));
            }

            void read_row(unsigned char *scanline)
            {
                assert(decomp_.output_scanline < decomp_.output_height);
                JSAMPROW rows[1] = { scanline };

                TRY(jpeg_read_scanlines(&decomp_, rows, 1));
            }

        private:
            static void error(j_common_ptr cinfo)
            {
                // !!!!!! IMPORTANT !!!!!!!
                // Please don't touch this function unless you understand the interaction of longjmp with
                // C++ code. At the very least, please read the notes on "Error Handling" in docs/notebook.html

                // Stash an exception to be thrown later.
                // Note that jpgexx::decompression_failure's constructor may throw bad_alloc!
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
                        self.ex_store_.store(decompression_failure(buffer));
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
                // TODO: perhaps store error messages somewhere?
            }

            static void init_source(j_decompress_ptr cinfo)
            {
                cinfo->src->next_input_byte = 0;
                cinfo->src->bytes_in_buffer = 0; // data will be buffered on first read
            }

            static void skip_input_data(j_decompress_ptr cinfo, long num_bytes)
            {
                impl &self = *static_cast<impl *>(cinfo->client_data);

                if (num_bytes <= 0) return;

                if (self.ex_store_.has_exception())
                {
                    self.decomp_.src->bytes_in_buffer = 0;
                    return;
                }

                if (static_cast<long>(self.decomp_.src->bytes_in_buffer) > num_bytes)
                {
                    self.decomp_.src->bytes_in_buffer -= num_bytes;
                    self.decomp_.src->next_input_byte += num_bytes;
                }
                else
                {
                    num_bytes -= static_cast<long>(self.decomp_.src->bytes_in_buffer);
                    self.decomp_.src->bytes_in_buffer = 0;
                    assert(self.ldr_->src_.get());
                    try { self.ldr_->src_->skip(num_bytes); }
                    catch (...) { self.ex_store_.store_current_exception(); }
                }
            }

            static void term_source(j_decompress_ptr) { }

            static boolean fill_input_buffer(j_decompress_ptr cinfo)
            {
                impl &self = *static_cast<impl *>(cinfo->client_data);
                assert(self.ldr_->src_.get());

                // Returning FALSE is typically used to help interleave other operations with
                // the loading of a JPEG image. The IJG library calls this "I/O suspension mode".
                // We hijack this mode to get out of IJG's C calls quickly and back in to C++ land
                // where we can re-throw a stored exception

                if (self.ex_store_.has_exception())
                    return FALSE;

                std::size_t n = 0;
                try 
                {
                    n = self.ldr_->src_->read(&self.buff_.front(), self.buff_.size()); 
                }
                catch (...)
                {
                    self.ex_store_.store_current_exception();
                    return FALSE;
                }

                if (n == 0) 
                {
                    self.ex_store_.store(imagexx::iterator_range_depleted());
                    return FALSE;
                }

                self.decomp_.src->next_input_byte = &self.buff_.front();
                self.decomp_.src->bytes_in_buffer = n;

                return TRUE;
            }

        private:
            best_exception_store ex_store_;
            std::jmp_buf jmpbuf_;
            std::vector<JOCTET> buff_;
            loader *ldr_;
            jpeg_decompress_struct decomp_;
            jpeg_error_mgr error_mgr_;
            jpeg_source_mgr source_mgr_;
    };


    loader::loader(std::size_t buff) :
        pimpl_(),
        src_(),
        details_(imagexx::rgb, 0, 0),
        buff_size_(buff)
    {
    }

    loader::~loader()
    {
    }

    const imagexx::raster_details &loader::details() const
    {
        if (!pimpl_.get()) throw imagexx::load_order_error();
        return details_;
    }

    void loader::read_details()
    {
        if (pimpl_.get()) throw imagexx::load_order_error();
        assert(src_.get());

        pimpl_.reset(new impl(this, buff_size_));
        pimpl_->read_details(details_);
    }

    void loader::begin_raster_read()
    {
        pimpl_->start_raster_read();
    }

    void loader::end_raster_read()
    {
        pimpl_->end_raster_read();
    }

    void loader::read_row(unsigned char *scanline)
    {
        pimpl_->read_row(scanline);
    }

    void loader::clear()
    {
        pimpl_.reset(0);
        src_.reset(0);
    }

    void loader::set_source(std::auto_ptr<imagexx::source> &src)
    {
        src_ = src;
    }

} // close namespace jpegxx

