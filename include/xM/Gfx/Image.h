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
            unsigned int width, height;
            unsigned int p2Width, p2Height;
            std::vector<unsigned char> pixels;

        } ImageSegment;

        class Image {
        private:

            unsigned int width, height;
            unsigned int p2Width, p2Height;
            std::vector<ImageSegment> segments;
            bool swizzled;

        public:

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
             * Load an image from a file.
             *
             * @param const std::string& file File to load from.
             *
             * @return bool Success or not?
             */
            bool loadFile(const std::string& file);
            
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
