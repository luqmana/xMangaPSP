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

#include <stdio.h>
#include <png.h>
#include <malloc.h>
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

            //this->reset();

            // File pointer
            FILE* fp;

            // Variables to read/parse png
            unsigned char sig[8];
            png_structp png_ptr;
            png_infop info_ptr;

            if ((fp = fopen(file.c_str(), "rb")) == NULL) {

                if (__xM_DEBUG) Util::logMsg("Image::loadFile — Unable to get file handle. [%s]", file.c_str());
                return false;

            }

            // Read in the sig
            fread(sig, sizeof (*sig), sizeof (sig), fp);

            if (!png_check_sig(sig, sizeof (*sig))) {

                fclose(fp);

                if (__xM_DEBUG) Util::logMsg("Image::loadFile — PNG sig does not validate. [%s]", file.c_str());

                return false;

            }

            if ((png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL)) == NULL) {

                fclose(fp);

                if (__xM_DEBUG) Util::logMsg("Image::loadFile — Unable to create png_structp. [%s]", file.c_str());

                return false;

            }

            if ((info_ptr = png_create_info_struct(png_ptr)) == NULL) {

                png_destroy_read_struct(&png_ptr, NULL, NULL);
                fclose(fp);

                if (__xM_DEBUG) Util::logMsg("Image::loadFile — Unable to create png_infop. [%s]", file.c_str());

                return false;

            }

            // libpng-style error handling
            if (setjmp(png_jmpbuf(png_ptr))) {

                png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
                fclose(fp);

                if (__xM_DEBUG) Util::logMsg("Image::loadFile — Some error occurred. [%s]", file.c_str());

                return false;

            }

            // Pass the file pointer on to libpng
            png_ptr->io_ptr = (png_voidp) fp;

            // Tell it we did the header check so it should skip the first 8 bytes
            png_set_sig_bytes(png_ptr, 8);

            // Read in the info
            png_read_info(png_ptr, info_ptr);

            // Get the info
            this->width = png_get_image_width(png_ptr, info_ptr);
            this->height = png_get_image_height(png_ptr, info_ptr);
            this->colourType = png_get_color_type(png_ptr, info_ptr);
            this->bitDepth = png_get_bit_depth(png_ptr, info_ptr);
            this->channels = png_get_channels(png_ptr, info_ptr);

            // BEGIN normalization
            // We want all our images to be RGBA

            // Extract multiple pixels with bit depths of 1, 2, and 4
            png_set_packing(png_ptr);

            // Strip 16 bit/color files down to 8 bits/co
            if (this->bitDepth == 16)
                png_set_strip_16(png_ptr);

            // Palette to RGB
            if (this->colourType == PNG_COLOR_TYPE_PALETTE) {

                png_set_palette_to_rgb(png_ptr);
                this->channels = 3;

            }

            // Grayscale to RGB
            if (this->colourType == PNG_COLOR_TYPE_GRAY || this->colourType == PNG_COLOR_TYPE_GRAY_ALPHA) {

                png_set_gray_to_rgb(png_ptr);
                this->channels = 3;

            }
            if (this->colourType == PNG_COLOR_TYPE_GRAY_ALPHA) {

                png_set_gray_to_rgb(png_ptr);
                this->channels = 4;

            }

            // tRNS chunk to alpha
            if (png_get_valid(png_ptr, info_ptr, PNG_INFO_tRNS)) {

                png_set_tRNS_to_alpha(png_ptr);
                this->channels += 1;

            }

            // END Normalization

            png_read_update_info(png_ptr, info_ptr);

            // Allocate the image data buffers
            //this->pixels = (unsigned char*) malloc(sizeof(png_bytep) * this->height);

            png_bytep* rowPtrs = new png_bytep[this->height];

            this->pixels = new char[this->width * this->height * this->bitDepth * this->channels / 8];

            for (size_t i = 0; i < this->height; i++)
                rowPtrs[i] = (png_bytep)this->pixels + i;

            // Read the image now!
            png_read_image(png_ptr, (png_bytep*)this->pixels);

            // Cleanup
            delete[] (png_bytep) rowPtrs;
            png_read_end(png_ptr, info_ptr);
            png_destroy_read_struct(&png_ptr, &info_ptr, NULL);

            /*this->width = 128;
            this->height = 128;

            this->pixels = (char*) malloc(this->width * this->height * 4);

            for (unsigned int s = 0; s < this->width * this->height * 4; s++) {
                unsigned int *p = (unsigned int*) this->pixels;
             *p = 0xffff0000;
            }*/

            // Close the file
            fclose(fp);

            return true;

        }

        /**
         * Reset/
         */
        void Image::reset() {

            if (this->pixels != NULL)
                delete[] this->pixels;

            this->width = 0;
            this->height = 0;
            this->bitDepth = 0;
            this->colourType = 0;
            this->channels = 0;
            this->swizzled = false;
            this->pixels = NULL;

        }

        /**
         * Rearrange the pixels to optimize speed.
         */
        void Image::swizzle() {

            unsigned int i, j;

            unsigned int rowblocks = (this->width * sizeof (u32) / 16);
            long size = this->width * this->height * this->bitDepth;

            unsigned char* out = (unsigned char*) malloc(size * sizeof (unsigned char));

            for (j = 0; j < this->height; ++j) {

                for (i = 0; i < this->width * sizeof (u32); ++i) {

                    unsigned int blockx = i / 16;
                    unsigned int blocky = j / 8;

                    unsigned int x = (i - blockx * 16);
                    unsigned int y = (j - blocky * 8);
                    unsigned int blockIndex = blockx + ((blocky) * rowblocks);
                    unsigned int blockAddress = blockIndex * 16 * 8;

                    out[blockAddress + x + y * 16] = this->pixels[ i + j * this->width * sizeof (u32)];

                }

            }

            // Copy swizzled data into pixels
            memcpy(this->pixels, out, size);

            // Flip bool
            this->swizzled = !this->swizzled;

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

            this->draw(x, y, this->width, this->height, 0, 0);

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

            this->draw(x, y, rWidth, rHeight, 0, 0);

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
         */
        void Image::draw(float x, float y, int rWidth, int rHeight, float xOffset, float yOffset) {

            // ToDo: Render code.

            sceGuEnable(GU_TEXTURE_2D);

            sceGuTexMode(GU_PSM_8888, 0, 0, this->isSwizzled());
            sceGuTexFunc(GU_TFX_BLEND, GU_TCC_RGB);
            sceGuTexFilter(GU_LINEAR, GU_LINEAR);
            //sceGuTexScale(1.0f, 1.0f);
            //sceGuTexOffset(0.0f, 0.0f);

            // Apply texture
            sceGuTexImage(0, rWidth, rHeight, rWidth, this->pixels);

            // Draw quad for it
            drawQuad(x, y, rWidth, rHeight, GU_COLOR(0.0f, 0.0f, 0.0f, 1.0f));

            sceGuDisable(GU_TEXTURE_2D);

        }

    }

}

#endif /* _Image_CPP */
