// Copyright 2007 Edd Dawson.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file BOOST_LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)


#include "imagexx/raster_details.hpp"

namespace imagexx
{

    raster_details::raster_details(pixel_format type, std::size_t width, std::size_t height, double width_mm, double height_mm) :
        type_(type),
        width_(width),
        height_(height),
        width_mm_(width_mm),
        height_mm_(height_mm)
    {
    }

    pixel_format raster_details::type() const
    {
        return type_;
    }

    std::size_t raster_details::channels() const
    {
        switch(type_)
        {
            case grey: return 1;
            case grey_alpha: return 2;
            case rgba: return 4;
            case rgb: default: return 3;
        }
        return 0;
    }

    std::size_t raster_details::width() const
    {
        return width_;
    }

    std::size_t raster_details::height() const
    {
        return height_;
    }

    double raster_details::width_mm() const
    {
        return width_mm_;
    }

    double raster_details::height_mm() const
    {
        return height_mm_;
    }

    bool operator== (const raster_details &lhs, const raster_details &rhs)
    {
        return lhs.type() == rhs.type() &&
               lhs.width() == rhs.width() &&
               lhs.height() == rhs.height() &&
               lhs.width_mm() == rhs.width_mm() &&
               lhs.height_mm() == rhs.height_mm();
    }

    bool operator!= (const raster_details &lhs, const raster_details &rhs)
    {
        return !(lhs == rhs);
    }

} // close namespace imagexx

