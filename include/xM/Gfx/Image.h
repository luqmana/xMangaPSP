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
 * Image loading and stuff.
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
	
	    class Image {
	    
	        private:
	        
	            unsigned int width, height;
	            int bitDepth;
	            int colourType;
	            int channels;
	            char* pixels;
	            bool swizzled;
	        
	        public:
	        
                /**
                 * Default constructor.
                 */
                Image() { this->reset(); };
                
                /**
                 * Return whether the image is swizzled or not.
                 * 
                 * @return bool Swizzle status.
                 */
                bool isSwizzled();
                
                /**
                 * Load an image from a file.
                 * 
                 * @param const std::string& file File to load from.
                 * 
                 * @return bool Success or not?
                 */
                bool loadFile(const std::string& file);
                
                /**
                 * Rearrange the pixels to optimize speed.
                 */
                void swizzle();
                
                /**
                 * Reset
                 */
                void reset();
                
                /**
	             * Render an image onto the screen.
	             * 
	             * @param float x X position to render to.
	             * @param float y Y position to render to.
	             */
	            void draw(float x, float y);
	            
	            /**
	             * Render part of an image onto the screen.
	             * 
	             * @param float x X position to render to.
	             * @param float y Y position to render to.
	             * @param int rWidth Width to render.
	             * @param int rHeight Height to render.
	             */
	            void draw(float x, float y, int width, int height);
	            
	            /**
	             * Render part of an image onto the screen.
	             * 
	             * @param float x X position to render to.
	             * @param float y Y position to render to.
	             * @param int rWidth Width to render.
	             * @param int rHeight Height to render.
	             * @param float xOffset Source x offset.
	             * @param float yOffset Source y offset.
	             */
	            void draw(float x, float y, int width, int height, float xOffset, float yOffset);
	    
	    };
	    	
	}

}

#endif /* _Image_H */
