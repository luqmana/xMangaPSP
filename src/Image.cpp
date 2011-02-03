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
#include "xM/Gfx/PicoPNG.h"

#include <stdio.h>
#include <string.h>
#include <malloc.h>

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

            if (!fD) {

                if (__xM_DEBUG)
                    Util::logMsg("Image::loadFile — Unable to open image file. [%s]", file.c_str());

                return false;

            }

            // Seek to end of file
            long imageSize = sceIoLseek32(fD, 0, PSP_SEEK_END);

            // Back to the beginning
            sceIoLseek(fD, 0, 0);

            unsigned char* buffer = (unsigned char*) malloc(imageSize);

            if (buffer == NULL) {

                if (__xM_DEBUG)
                    Util::logMsg("Image::loadFile — Unable to allocate memory.");

                return false;

            }

            sceIoRead(fD, buffer, imageSize);

            decodePNG(this->pixels, this->width, this->height, (const unsigned char*)buffer, imageSize, true);

            // Well, no need for the buffer now
            free(buffer);

            // Swizzle
            this->swizzle();
            
            return true;

        }

        /**
         * Reset/
         */
        void Image::reset() {

            if (!this->pixels.empty())
                this->pixels.clear();

            this->width = 0;
            this->height = 0;
            this->swizzled = false;
            this->pixels.clear();

        }

        /**
         * Rearrange the pixels to optimize speed.
         *
         * @link http://wiki.ps2dev.org/psp:ge_faq
         */
        void Image::swizzle() {

            unsigned int i, j;
            unsigned int rowblocks = (this->width * sizeof(u32) / 16);
            long size = this->width * this->height * 8;

            unsigned char* out = (unsigned char*) malloc(size * sizeof(unsigned char));

            for (j = 0; j < this->height; ++j) {

                for (i = 0; i < this->width * sizeof (u32); ++i) {

                    unsigned int blockx = i / 16;
                    unsigned int blocky = j / 8;

                    unsigned int x = (i - blockx * 16);
                    unsigned int y = (j - blocky * 8);
                    unsigned int blockIndex = blockx + (blocky * rowblocks);
                    unsigned int blockAddress = blockIndex * 16 * 8;

                    out[blockAddress + x + y * 16] = this->pixels[i + j * this->width * sizeof(u32)];

                }

            }

            // Copy swizzled data into pixels
            this->pixels.resize(size);
            memcpy(&this->pixels[0], out, size);

            // Flip bool
            this->swizzled = true;

            // Free temporary
            free(out);

        }

        /**
         * Render an image onto the screen.
         *
         * @param float x X position to render to.
         * @param float y Y position to render to.
         */
        void Image::draw(float x, float y) {

            this->draw(x, y, this->width, this->height, 0, 0, 0);

        }

        /**
         * Render an image onto the screen and rotate it.
         *
         * @param float x X position to render to.
         * @param float y Y position to render to.
         * @param float rotate Rotation option!
         */
        void Image::draw(float x, float y, float rotate) {

            this->draw(x, y, this->width, this->height, 0, 0, rotate);

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

            this->draw(x, y, rWidth, rHeight, 0, 0, 0);

        }

        /**
         * Render part of an image onto the screen.
         *
         * @param float x X position to render to.
         * @param float y Y position to render to.
         * @param int rWidth Width to render.
         * @param int rHeight Height to render.
         * @param float xOffset Source x offset.
         * @param float yOffset Source y offset.
         * @param float rotate Rotation
         */
        void Image::draw(float x, float y, int rWidth, int rHeight, float xOffset, float yOffset, float rotate) {

            // Enable 2D textures
            sceGuEnable(GU_TEXTURE_2D);

            sceGuTexMode(GU_PSM_8888, 0, 0, this->isSwizzled());
            sceGuTexFunc(GU_TFX_REPLACE, GU_TCC_RGBA);
            sceGuTexFilter(GU_LINEAR, GU_LINEAR);
            //sceGuTexScale(1.0f, 1.0f);
            //sceGuTexOffset(0.0f, 0.0f);

            // Apply texture
            sceGuTexImage(0, rWidth, rHeight, rWidth, &this->pixels[0]);

            // Draw quad for it
            drawQuad(x, y, rWidth, rHeight, GU_COLOR(1.0f, 1.0f, 1.0f, 1.0f), rotate);
/*Vertex quad[4] = {
                {0, 0, GU_COLOR(1.0f, 1.0f, 1.0f, 1.0f), -(rWidth / 2), -(rHeight / 2), 0.0f}, // Top-Left point
                {1, 0, GU_COLOR(1.0f, 1.0f, 1.0f, 1.0f), (rWidth / 2), -(rHeight / 2), 0.0f}, // Top-Right point
                {0, 1, GU_COLOR(1.0f, 1.0f, 1.0f, 1.0f), -(rWidth / 2), (rHeight / 2), 0.0f}, // Bottom-Left point
                {1, 1, GU_COLOR(1.0f, 1.0f, 1.0f, 1.0f), (rWidth / 2), (rHeight / 2), 0.0f} // Bottom-Right point
            };

            sceGumMatrixMode(GU_MODEL);
            sceGumLoadIdentity(); // Reset
            {

                ScePspFVector3 pos = {x + (rWidth / 2), y + (rHeight / 2), 0.0f};

                // Move
                sceGumTranslate(&pos);

                // Rotate
                sceGumRotateZ(rotate);

            }

            Vertex* finalQuad = (Vertex*) sceGuGetMemory(sizeof (Vertex) * 4);
            memcpy(finalQuad, quad, sizeof (Vertex) * 4);

            // Draw the quad
            sceGumDrawArray(GU_TRIANGLE_STRIP, GU_TEXTURE_32BITF | GU_COLOR_8888 | GU_VERTEX_32BITF, 4, 0, finalQuad);*/
            // Disable em again
            sceGuDisable(GU_TEXTURE_2D);

        }

    }

}

#endif /* _Image_CPP */
