// Copyright 2008 Edd Dawson.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file BOOST_LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef RASTER_DETAILS_HPP_1715_30032008
#define RASTER_DETAILS_HPP_1715_30032008

#include <cstdlib>

namespace imagexx
{
    //! Used to indicate the interpretation of the channels in an image.
    enum pixel_format { grey, grey_alpha, rgb, rgba };


    //! A non-behavioural data object that is used to describe the image that is being loaded or has been loaded
    class raster_details
    {
        public:
            //! Constructs an raster_details object from the data provided
            raster_details(pixel_format type, std::size_t width, std::size_t height, double width_mm = 0, double height_mm = 0);

            //! The format of the image data
            pixel_format type() const;

            //! Returns the number of channels-per-pixel in the corresponding image
            std::size_t channels() const;

            //! The width of the raster (pixels)
            std::size_t width() const;

            //! The height of the raster (pixels)
            std::size_t height() const;

            //! The width of the raster (millimeters). 0 is returned if the physical width is unknown.
            double width_mm() const;

            //! The height of the raster (millimeters). 0 is returned if the physical height is unknown.
            double height_mm() const;

        private:
            pixel_format type_;
            std::size_t width_;
            std::size_t height_;
            double width_mm_;
            double height_mm_;
    };

    //! Returns true if the two lhs and rhs have equal type(), width(), height(), width_mm() and height_mm()
    bool operator== (const raster_details &lhs, const raster_details &rhs);

    //! Returns the negation of operator==
    bool operator!= (const raster_details &lhs, const raster_details &rhs);

} // close namespace imagexx

#endif // RASTER_DETAILS_HPP_1715_30032008

