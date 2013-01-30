/**
 * This file is part of the xMangaPSP application.
 *
 * Copyright (C) Luqman Aden <www.luqman.ca>.
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
#include <vector>
// END Includes

// BEGIN Defines

// END Defines

namespace xM {

    namespace Gfx {

        typedef struct {
            float x, y;
            unsigned int width, height;
        } ImageClip;

        typedef struct {
            int x, y;
            int sX, sY;
            unsigned int width, height;
            unsigned int sWidth, sHeight;
            unsigned int p2Width, p2Height;
            unsigned char* pixels;
        } ImageSegment;

        class Image {
        private:
            std::vector<ImageSegment*> segments;
            bool swizzled, scaled;
            
            /**
             * Determine if this is a PNG image.
             * 
             * @param const std::string& imgBuffer Reference to the image buffer.
             * 
             * @return bool Yes or no.
             */
            bool isPNG(const std::string& imgBuffer);
            
            /**
             * Determine if this is a JPEG image.
             * 
             * @param const std::string& imgBuffer Reference to the image buffer.
             * 
             * @return bool Yes or no.
             */
            bool isJPEG(const std::string& imgBuffer);
            
            /**
             * Attempts to figure out what type of image is in the buffer
             * and load it accordingly. Only PNG & JPEG supported currently.
             * 
             * @param const std::string& imgBuffer Reference to the image buffer.
             * @param ImageSegment* destImg Where to store the decoded result.
             * 
             * @return bool Whether it all worked out.
             */
            bool loadImage(const std::string& imgBuffer, ImageSegment* destImg);
            
        public:
        
            unsigned int width, height;
            unsigned int sWidth, sHeight;
            unsigned int p2Width, p2Height;

            /**
             * Default constructor.
             */
            Image() {
                this->reset();
            };
            
            /**
             * Destructor
             */
            ~Image() {
                this->reset();
            };

            /**
             * Return whether the image is swizzled or not.
             *
             * @return bool Swizzle status.
             */
            bool isSwizzled();
            
            /**
             * Whether rendered image is scaled or not.
             * 
             * @return bool Scale status.
             */
            bool isScaled();
            
            /**
             * Toggle scale status.
             */
            void toggleScale();

            /**
             * Load an image from a file.
             *
             * @param const std::string& file File to load from.
             *
             * @return bool Success or not?
             */
            bool loadFile(const std::string& file);

            /**
             * Load an image from memory.
             *
             * @param const std::string& imageBuffer The image in memory.
             * 
             * @return bool Success or not?
             */
            bool loadData(const std::string& imageBuffer);
            
            /**
             * Whether an image is already loaded.
             * 
             * @return bool
             */
            bool isLoaded();

            /**
             * Rearrange the pixels to optimize speed.
             */
            void swizzle();

            /**
             * Reset
             */
            void reset();

            /**
             * This function does the actual rendering and works on a per-segment basis.
             * 
             * @param float x X position to render to.
             * @param float y Y position to render to.
             * @param const ImageClip* clip Src blitting region.
             * @param const unsigned int seg Which segment to render.
             */
            void render(float x, float y, const ImageClip* clip, const unsigned int seg);

            /**
             * Render an image onto the screen.
             *
             * @param float x X position to render to.
             * @param float y Y position to render to.
             * @param const ImageClip* clip Src blitting region.
             */
            void draw(float x, float y, const ImageClip* clip = NULL);

        };

    }

}

#endif /* _Image_H */
