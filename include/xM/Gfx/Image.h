/**
 * This file is part of the xMangaPSP application.
 *
 * Copyright (C) Luqman Aden <www.luqmanrocks.co.cc>.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * Full License: GNU-LICENSE
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
 
/**
 * Image loading stuff and.
 * Header file.
 * 
 * @package xMangaPSP
 */

#ifndef _Image_H
#define _Image_H

// BEGIN Includes
#include <string>
// END Includes

// BEGIN Defines

// END Defines

namespace xM {

	namespace Gfx {
	
	    typedef struct {
	    
	        unsigned int width, height;
	        int bitDepth;
	        int colourType;
	        unsigned char* pixels;
	    
	    } Image;
	    
	    /**
	     * Load an image from a file.
	     * 
	     * @param const std::string& file File to load from.
	     * 
	     * @return Image Struct with info about image.
	     */
	    Image imageFromFile(const std::string& file); 
	    
	    /**
	     * Render an image onto the screen.
	     * 
	     * @param Image& img The image to render.
	     * @param int x X position to render to.
	     * @param int y Y position to render to.
	     */
	    void renderImage(Image& img, int x, int y);
	    
	    /**
	     * Render part of an image onto the screen.
	     * 
	     * @param Image& img The image to render.
	     * @param int x X position to render to.
	     * @param int y Y position to render to.
	     * @param int width Width to render.
	     * @param int height Height to render.
	     */
	    void renderImage(Image& img, int x, int y, int width, int height);
	    
	    /**
	     * Render part of an image onto the screen.
	     * 
	     * @param Image& img The image to render.
	     * @param int x X position to render to.
	     * @param int y Y position to render to.
	     * @param int width Width to render.
	     * @param int height Height to render.
	     * @param int xOffset Source x offset.
	     * @param int yOffset Source y offset.
	     */
	    void renderImage(Image& img, int x, int y, int width, int height, int xOffset, int yOffset);
	
	}

}

#endif /* _Image_H */
