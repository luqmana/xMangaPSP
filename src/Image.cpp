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
 * Image loading adn stuff.
 * Header file.
 * 
 * @package xMangaPSP
 */

#ifndef _Image_CPP
#define _Image_CPP

// BEGIN Includes
#include "xM/Gfx/Image.h"

#include <stdio.h>
#include <png.h>
#include <malloc.h>
// END Includes

// BEGIN Defines

// END Defines

namespace xM {

	namespace Gfx {
	
        /**
	     * Load an image from a file.
	     * 
	     * @param const std::string& file File to load from.
	     * 
	     * @return Image Struct with info about image.
	     */
	    Image imageFromFile(const std::string& file) {
	    
	        // Initialize to defaults
	        Image img;
	        img.width  = 0;
	        img.height = 0;
	        img.pixels = NULL;
	        
	        // File pointer
	        FILE* fp;
	        
	        // Variables to read/parse png
	        unsigned char sig[8];
	        png_structp png_ptr;
	        png_infop info_ptr;
	        
	        if ((fp = fopen(file.c_str(), "rb")) == NULL)
	            return img;
	            
	        // Read in the sig
	        fread(sig, sizeof(*sig), sizeof(sig), fp);
	        
	        if (!png_check_sig(sig, sizeof(*sig))) {
	        
	            fclose(fp);
	            return img;
	            
	        }
	        
	        if ((png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL)) == NULL) {
	        
	            fclose(fp);
	            return img;
	            
	        }
	        
	        if ((info_ptr = png_create_info_struct(png_ptr)) == NULL) {
	        
	            png_destroy_read_struct(&png_ptr, NULL, NULL);
	            fclose(fp);
	            return img;
	            
	        }
	        
	        // libpng-style error handling
	        if (setjmp(png_jmpbuf(png_ptr))) {
	        
	            png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
	            fclose(fp);
	            return img;
	            
	        }
	        
	        // Pass the file pointer on to libpng
	        png_ptr->io_ptr = (png_voidp) fp;
	        
	        // Tell it we did the header check so it should skip the first 8 bytes
	        png_set_sig_bytes(png_ptr, 8);
	        
	        // Read in the info
	        png_read_info(png_ptr, info_ptr);
	        
	        // Get the info
	        png_get_IHDR(png_ptr, info_ptr, (png_uint_32*)&img.width, (png_uint_32*)&img.height, &img.bitDepth, &img.colourType, NULL, NULL, NULL);
	        
	        // BEGIN normalization
	        // We want all our images to be RGBA
	       
	        // Extract multiple pixels with bit depths of 1, 2, and 4 	  
	        png_set_packing(png_ptr);
	        	            
	        // Strip 16 bit/color files down to 8 bits/co
	        if (img.bitDepth == 16)
	            png_set_strip_16(png_ptr);
	        
	        // Palette to RGB
	        if (img.colourType == PNG_COLOR_TYPE_PALETTE)
	            png_set_palette_to_rgb(png_ptr);
	        
	        // Grayscale to RGB
	        if (img.colourType == PNG_COLOR_TYPE_GRAY || img.colourType == PNG_COLOR_TYPE_GRAY_ALPHA)
	            png_set_gray_to_rgb(png_ptr);
	            
	        // tRNS chunk to alpha
	        if (png_get_valid(png_ptr, info_ptr, PNG_INFO_tRNS))
	            png_set_tRNS_to_alpha(png_ptr);
	            	        
	        // END Normalization
	        
	        png_read_update_info(png_ptr, info_ptr);
	        
	        // Allocate the image data buffers
	        if ((img.pixels = (unsigned char*) malloc(sizeof(png_bytep) * img.height)) == NULL) {
	        
	            png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
	            fclose(fp);
	            return img;
	        
	        }
	        
	        // Read the image now!
	        png_read_image(png_ptr, (png_bytep*)img.pixels);
	        
	        // Cleanup
	        png_read_end(png_ptr, info_ptr);
	        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
	        
	        // Close the file
	        fclose(fp);
	            
	        return img;
	    
	    }
	    
	    /**
	     * Render an image onto the screen.
	     * 
	     * @param Image& img The image to render.
	     * @param int x X position to render to.
	     * @param int y Y position to render to.
	     */
	    void renderImage(Image& img, int x, int y) {
	    
	        renderImage(img, x, y, img.width, img.height, 0, 0);
	    
	    }
	    
	    /**
	     * Render part of an image onto the screen.
	     * 
	     * @param Image& img The image to render.
	     * @param int x X position to render to.
	     * @param int y Y position to render to.
	     * @param int width Width to render.
	     * @param int height Height to render.
	     */
	    void renderImage(Image& img, int x, int y, int width, int height) {
	    
	        renderImage(img, x, y, width, height, 0, 0);
	    
	    }
	    
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
	    void renderImage(Image& img, int x, int y, int width, int height, int xOffset, int yOffset) {
	    
	        // ToDo: Render code.
	    
	    }
    	
	}

}

#endif /* _Image_CPP */
