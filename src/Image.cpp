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

#ifndef _Image_CPP
#define _Image_CPP

// BEGIN Includes
#include "xM/Gfx/Graphics.h"
#include "xM/Gfx/Image.h"
#include "xM/Util/Log.h"
#include "xM/Util/Utils.h"
#include "xM/Gfx/PicoPNG.h"

#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <math.h>

#include <pspiofilemgr.h>
// END Includes

// BEGIN Defines

// END Defines

namespace xM {

    namespace Gfx {

        /**
         * Return whether the image is swizzled or not.
         * 
         * @return bool Swizzle status.
         */
        bool Image::isSwizzled() {

            return this->swizzled;

        }

        /**
         * Load an image from a file.
         *
         * @param const std::string& file File to load from.
         *
         * @return bool Success or not?
         */
        bool Image::loadFile(const std::string& file) {

            if (__xM_DEBUG)
                Util::logMsg("Image::loadFile — %s", file.c_str());

            this->reset();

            SceUID fD = sceIoOpen(file.c_str(), PSP_O_RDONLY, 0777);

            if (fD < 0) {

                if (__xM_DEBUG)
                    Util::logMsg("Image::loadFile — Unable to open image file. [%s]", file.c_str());

                return false;

            }

            // Seek to end of file
            long imageSize = sceIoLseek32(fD, 0, PSP_SEEK_END);

            if (__xM_DEBUG)
                Util::logMsg("Image size: %d", (int) imageSize);

            // Back to the beginning
            sceIoLseek(fD, 0, 0);

            unsigned char* buffer = (unsigned char*) malloc(imageSize);

            if (buffer == NULL) {

                if (__xM_DEBUG)
                    Util::logMsg("Image::loadFile — Unable to allocate memory.");

                return false;

            }

            printf("Before read.\n");

            // Read in the png
            sceIoRead(fD, buffer, imageSize);

            printf("Before decode.\n");

            ImageSegment mainSegment;

            // Decode the png
            decodePNG(mainSegment.pixels, mainSegment.width, mainSegment.height, (const unsigned char*) buffer, imageSize, true);

            mainSegment.p2Width = Util::nextPow2(mainSegment.width);
            mainSegment.p2Height = Util::nextPow2(mainSegment.height);

            this->width = mainSegment.width;
            this->height = mainSegment.height;
            this->p2Width = mainSegment.p2Width;
            this->p2Height = mainSegment.p2Height;

            // Well, no need for the buffer now
            free(buffer);

            if (!(mainSegment.width > 512 || mainSegment.height > 512)) {

                // No need to create more segments but just pretend there is only one segment

                mainSegment.x = 0;
                mainSegment.y = 0;

                this->segments.push_back(mainSegment);

            } else {

                // Okay, we need to create segments since the image is too large

                // First we figure out roughly how many segments of 512x512 we need
                int wFit = ceil((float) mainSegment.width / 512);
                int hFit = ceil((float) mainSegment.height / 512);

                int i = 0;

                for (i; i < hFit; ++i) {

                    int k = 0;

                    for (k; k < wFit; ++k) {

                        ImageSegment segment;

                        // Handle case for final segment which might not be 512px
                        if (k == (wFit - 1))
                            segment.width = mainSegment.width - (512 * (wFit - 1));
                        else
                            segment.width = 512;

                        segment.p2Width = Util::nextPow2(segment.width);

                        // Handle case for final segment which might not be 512px
                        if (i == (hFit - 1))
                            segment.height = mainSegment.height - (512 * (hFit - 1));
                        else
                            segment.height = 512;

                        segment.p2Height = Util::nextPow2(segment.height);

                        // Calculate coordinate of segment in terms of the whole image
                        segment.x = 512 * k;
                        segment.y = 512 * i;

                        // Reserve enough size                        
                        segment.pixels.resize(segment.width * segment.height * sizeof (uint32_t));

                        unsigned int y = segment.y;

                        for (y; y < (segment.height + segment.y); ++y) {

                            //memcpy(&segment.pixels[y * segment.width * sizeof (uint32_t)], &mainSegment.pixels[y * mainSegment.width * sizeof (uint32_t)], segment.width * sizeof (uint32_t));

                            unsigned int x = segment.x;

                            for (x; x < (segment.width + segment.x); ++x) {

                                memcpy(&segment.pixels[0] + ((y - segment.y) * segment.width * 4) + ((x - segment.x) * 4), &mainSegment.pixels[0] + ((y + segment.y) * mainSegment.width * 4) + ((x + segment.x) * 4), 4);

                            }

                        }

                        this->segments.push_back(segment);

                    }

                }


            }

            return true;

        }

        /**
         * Reset/
         */
        void Image::reset() {

            /*if (!this->pixels.empty())
                this->pixels.clear();

            this->width = 0;
            this->height = 0;
            this->swizzled = false;
            this->pixels.clear();*/

        }

        /**
         * Rearrange the pixels to optimize speed.
         *
         * @link http://wiki.ps2dev.org/psp:ge_faq
         */
        void Image::swizzle() {

            for (unsigned int r = 0; r < this->segments.size(); ++r) {

                unsigned int i, j;
                unsigned int rowblocks = (this->segments[r].width * sizeof (u32) / 16);
                long size = this->segments[r].width * this->segments[r].height * 8;

                unsigned char* out = (unsigned char*) malloc(size * sizeof (unsigned char));

                for (j = 0; j < this->segments[r].height; ++j) {

                    for (i = 0; i < this->segments[r].width * sizeof (u32); ++i) {

                        unsigned int blockx = i / 16;
                        unsigned int blocky = j / 8;

                        unsigned int x = (i - blockx * 16);
                        unsigned int y = (j - blocky * 8);
                        unsigned int blockIndex = blockx + (blocky * rowblocks);
                        unsigned int blockAddress = blockIndex * 16 * 8;

                        out[blockAddress + x + y * 16] = this->segments[r].pixels[i + j * this->segments[r].width * sizeof (uint32_t)];

                    }

                }

                // Copy swizzled data
                this->segments[r].pixels.resize(size);
                memcpy(&this->segments[r].pixels[0], out, size);

                // Free temporary
                free(out);

            }

            // Flip bool
            this->swizzled = true;

        }

        /**
         * Render an image onto the screen.
         *
         * @param float x X position to render to.
         * @param float y Y position to render to.
         */
        void Image::draw(float x, float y) {

            this->draw(x, y, this->width, this->height, this->p2Width, this->p2Height, 0, 0, 0);

        }

        /**
         * Render an image onto the screen and rotate it.
         *
         * @param float x X position to render to.
         * @param float y Y position to render to.
         * @param float rotate Rotation option!
         */
        void Image::draw(float x, float y, float rotate) {

            this->draw(x, y, this->width, this->height, this->p2Width, this->p2Height, 0, 0, rotate);

        }

        /**
         * Render part of an image onto the screen.
         *
         * @param float x X position to render to.
         * @param float y Y position to render to.
         * @param int rWidth Width to render.
         * @param int rHight Height to render.
         */
        void Image::draw(float x, float y, int rWidth, int rHeight) {

            this->draw(x, y, rWidth, rHeight, rWidth, rHeight, 0, 0, 0);

        }

        /**
         * Render part of an image onto the screen.
         *
         * @param float x X position to render to.
         * @param float y Y position to render to.
         * @param int aWidth Actual width.
         * @param int aHeight Actual height.
         * @param int rWidth Width to render.
         * @param int rHight Height to render.
         */
        void Image::draw(float x, float y, int aWidth, int aHeight, int rWidth, int rHeight) {

            this->draw(x, y, aWidth, aHeight, rWidth, rHeight, 0, 0, 0);

        }

        /**
         * Render part of an image onto the screen.
         *
         * @param float x X position to render to.
         * @param float y Y position to render to.
         * @param int aWidth Actual width.
         * @param int aHeight Actual height.
         * @param int rWidth Width to render.
         * @param int rHeight Height to render.
         * @param float xOffset Source x offset.
         * @param float yOffset Source y offset.
         * @param float rotate Rotation
         */
        void Image::draw(float x, float y, int aWidth, int aHeight,
                int rWidth, int rHeight, float xOffset, float yOffset, float rotate) {

            // Enable 2D textures
            sceGuEnable(GU_TEXTURE_2D);

            sceGuTexMode(GU_PSM_8888, 0, 0, this->isSwizzled());
            sceGuTexFunc(GU_TFX_REPLACE, GU_TCC_RGBA);
            sceGuTexFilter(GU_LINEAR, GU_LINEAR);
            sceGuTexScale(1.0f, 1.0f);
            sceGuTexOffset(0.0f, 0.0f);

            // Apply texture
            sceGuTexImage(0, rWidth, rHeight, aWidth, &this->segments[0].pixels[0]);

            unsigned int colour = GU_COLOR(1.0f, 1.0f, 1.0f, 1.0f);

            // Draw quad for it
            drawQuad(x, y, rWidth, rHeight, aWidth, aHeight, colour, colour, colour, colour, rotate);

            // Disable again
            sceGuDisable(GU_TEXTURE_2D);

        }

    }

}

#endif /* _Image_CPP */
