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
 * 
 * @package xMangaPSP
 */

#ifndef _Image_CPP
#define _Image_CPP

// BEGIN Includes
#include "xM/Engine/ResourceManager.h"
#include "xM/Gfx/Graphics.h"
#include "xM/Gfx/Image.h"
#include "xM/Util/Log.h"
#include "xM/Util/Utils.h"
#include "xM/Util/Timer.h"

#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <math.h>
#include <iostream>
#include <sstream>
#include <string>

#include <pspiofilemgr.h>

#include <jpeglib.h>

#include "LodePNG/LodePNG.h"
// END Includes

namespace xM {

    namespace Gfx {
        
#if __xM_DEBUG
        Util::Timer loadTimer;
#endif

        /**
         * Return whether the image is swizzled or not.
         * 
         * @return bool Swizzle status.
         */
        bool Image::isSwizzled() {

            return this->swizzled;

        }
        
        /**
         * Whether rendered image is scaled or not.
         * 
         * @return bool Scale status.
         */
        bool Image::isScaled() {
        
            return this->scaled;
        
        }
        
        /**
         * Toggle scale status.
         */
        void Image::toggleScale() {
        
            this->scaled = !this->scaled;
        
        }

        /**
         * Load an image from memory.
         *
         * @param const std::string& imageBuffer The image in memory.
         * 
         * @return bool Success or not?
         */
        bool Image::loadData(const std::string& imageBuffer) {
            
            // Holds the big picture    
            ImageSegment* mainSegment = new ImageSegment;
     
            // Load image (could be PNG, JPEG, etc) otherwise get out
            if (!this->loadImage(imageBuffer, mainSegment))
                return false;
                             
            mainSegment->p2Width = Util::nextPow2(mainSegment->width);
            mainSegment->p2Height = Util::nextPow2(mainSegment->height);
            
            mainSegment->sWidth = 480;
            mainSegment->sHeight = ceil(((float)mainSegment->height / mainSegment->width) * mainSegment->sWidth);

            this->width = mainSegment->width;
            this->height = mainSegment->height;
            this->sWidth = mainSegment->sWidth;
            this->sHeight = mainSegment->sHeight;
            this->p2Width = mainSegment->p2Width;
            this->p2Height = mainSegment->p2Height;
        
#if __xM_DEBUG            
            loadTimer.start();
#endif
            if (mainSegment->width < 512 && mainSegment->height < 512) {

                // No need to create more segments but just pretend there is only one segment

                mainSegment->x = mainSegment->sX = 0;
                mainSegment->y = mainSegment->sY = 0;

                this->segments.push_back(mainSegment);
#if __xM_DEBUG
                Util::logMsg("segmentizer - %f", loadTimer.getDeltaTicks(true));
#endif
                return true;

            } else {

                // Okay, we need to create segments since the image is too large

                // First we figure out roughly how many segments of 512x512 we need
                int wFit = ceil((float) mainSegment->width / 512);
                int hFit = ceil((float) mainSegment->height / 512);
#if __xM_DEBUG
                Util::logMsg("Segments: \t\t%d\n", hFit * wFit);
#endif
                                
                int i = 0;

                do {

                    int k = 0;

                    do {
                    
                        ImageSegment* segment = new ImageSegment;

                        // Handle case for final segment which might not be 512px
                        if (k == (wFit - 1))
                            segment->width = mainSegment->width - (512 * (wFit - 1));
                        else
                            segment->width = 512;

                        segment->sWidth = ceil(((float)segment->width / mainSegment->width) * mainSegment->sWidth);
                        segment->p2Width = Util::nextPow2(segment->width);
                        
                        // Handle case for final segment which might not be 512px
                        if (i == (hFit - 1))
                            segment->height = mainSegment->height - (512 * (hFit - 1));
                        else
                            segment->height = 512;

                        segment->sHeight = ceil(((float)segment->height / mainSegment->height) * mainSegment->sHeight);
                        segment->p2Height = Util::nextPow2(segment->height);

                        // Calculate coordinate of segment in terms of the whole image
                        segment->x = 512 * k;
                        segment->y = 512 * i;
                        
                        segment->sX = ceil(((float)segment->x / segment->width) * segment->sWidth);
                        segment->sY = ceil(((float)segment->y / segment->height) * segment->sHeight);
                        
                        // Reserve enough size                        
                        segment->pixels = (unsigned char*) malloc(segment->width * segment->height * 4);

                        for (unsigned int line = 0; line < (segment->height); ++line)
                            memcpy(segment->pixels + (line * segment->width * 4),
                                    mainSegment->pixels + ((line + segment->y) * mainSegment->width * 4) + (segment->x * 4), 4 * segment->width);

                        this->segments.push_back(segment);

                        ++k;

                    } while (k < wFit);

                    ++i;

                } while (i < hFit);


            }

            free(mainSegment->pixels);
            delete mainSegment;
#if __xM_DEBUG
            Util::logMsg("segmentizer - %f", loadTimer.getDeltaTicks(true));
#endif
            return true;

        }

        /**
         * Load an image from a file.
         *
         * @param const std::string& file File to load from.
         *
         * @return bool Success or not?
         */
        bool Image::loadFile(const std::string& file) {

#if __xM_DEBUG
            Util::logMsg("Image::loadFile — %s", file.c_str());
#endif
            this->reset();

            // Load the image from wherever (FS, cache, PSAR, zip)
            std::string imageBuffer = Engine::ResourceManager::getInstance()->getRes(file);      
            if (imageBuffer == "")
                return false;
            
            return this->loadData(imageBuffer);

        }
        
        /**
         * Determine if this is a PNG image.
         * 
         * @param const std::string& imgBuffer Reference to the image buffer.
         * 
         * @return bool Yes or no.
         */
        bool Image::isPNG(const std::string& imgBuffer) {
            
            // PNG `magic` sequence
            unsigned char sig[8] = { 0x89, 0x50, 0x4E, 0x47, 0x0D, 0x0A, 0x1A, 0x0A };
            
            unsigned char rsig[8];
            
            // Read in from the supposed png data
            memcpy(rsig, &imgBuffer[0], 8);
            
            // Compare away!
            return memcmp(sig, rsig, 8) == 0;
        
        }
        
        /**
         * Determine if this is a JPEG image.
         * 
         * @param const std::string& imgBuffer Reference to the image buffer.
         * 
         * @return bool Yes or no.
         */
        bool Image::isJPEG(const std::string& imgBuffer) {
        
            // JPEG `magic` at beginning of file
            unsigned char bMagic[2] = { 0xFF, 0xD8 };
            
            // JPEG `magic` at end of file
            unsigned char eMagic[2] = { 0xFF, 0xD9 };
            
            unsigned char cB[2];
            unsigned char cE[2];
            
            // Read in from the supposed jpg data
            memcpy(cB, &imgBuffer[0], 2);
            memcpy(cE, &imgBuffer[0] + (imgBuffer.size() - 2), 2);
                        
            // Compare away!
            int rB = memcmp(bMagic, cB, 2);
            int rE = memcmp(eMagic, cE, 2);
                    
            return (rB == 0) && (rE == 0);
        
        }
        
        /**
         * Attempts to figure out what type of image is in the buffer
         * and load it accordingly. Only PNG & JPEG supported currently.
         * 
         * @param const std::string& imgBuffer Reference to the image buffer.
         * @param ImageSegment* destImg Where to store the decoded result.
         * 
         * @return bool Whether it all worked out.
         */
        bool Image::loadImage(const std::string& imgBuffer, ImageSegment* destImg) {
                    
            // Figure out what type of image this is
            if (this->isPNG(imgBuffer)) {
            
                // well, what dya know? It's a PNG! Rejoice!
                
                // Decode the png
                int r = LodePNG_decode32(&destImg->pixels, &destImg->width, &destImg->height, (const unsigned char*)imgBuffer.c_str(), imgBuffer.size());
                
                if (r != 0)
                    return false;

                return true;
                
            } else if (this->isJPEG(imgBuffer)) {

                struct jpeg_decompress_struct cinfo;
                struct jpeg_error_mgr jerr;
                cinfo.err = jpeg_std_error(&jerr);

                unsigned int loc = 0;

                // Initialize JPEG decompression object
                jpeg_create_decompress(&cinfo);

                // Specify data source (our buffer in this case)
                jpeg_mem_src(&cinfo, (unsigned char*)imgBuffer.c_str(), imgBuffer.size());

                // Read in jpeg parameters
                jpeg_read_header(&cinfo, true);

                // Start decompressor
                jpeg_start_decompress(&cinfo);

                // Set the width, height and allocate enough space
                destImg->width = cinfo.output_width;
                destImg->height = cinfo.output_height;

                unsigned int sz = destImg->width * destImg->height * 4;
                
                //destImg->pixels = (unsigned char*) memalign(16, sz);
                destImg->pixels = (unsigned char*) malloc(sz);

                unsigned int stride = cinfo.output_width * cinfo.output_components;
                unsigned char* line = (unsigned char*) malloc(stride);
                unsigned int pos = 0;
#if __xM_DEBUG        
                loadTimer.start();
#endif
                // Read in image
                while (cinfo.output_scanline < cinfo.output_height) {
                    
                    // read!
                    jpeg_read_scanlines(&cinfo, &line, 1);

                    // Add an alpha component for every pixel
                    int l = 0;
                    for (loc = 0; loc < stride; loc++) {
                        
                        destImg->pixels[pos] = line[loc];
                        pos += sizeof(unsigned char);
                        l++;

                        if (l == 3) {
                            
                            destImg->pixels[pos] = 0xFF;
                            pos += sizeof(unsigned char);
                            l = 0;

                        }


                    }
                    
                }
#if __xM_DEBUG
                Util::logMsg("read_image - %f", loadTimer.getDeltaTicks(true));
#endif
                // Cleanup
                jpeg_finish_decompress(&cinfo);
                jpeg_destroy_decompress(&cinfo);
                free(line);
            
                return true;
                                            
            } else {
            
                // Bah, we don't support you!
                Util::logMsg("Unknown image format...");
                
                return false;
            
            }
                    
            return false;
        
        }
        
        /**
         * Whether an image is already loaded.
         * 
         * @return bool
         */
        bool Image::isLoaded() {
        
            return (this->segments.size() != 0);
        
        }

        /**
         * Reset/
         */
        void Image::reset() {

            if (this->segments.size() != 0) {

                for (unsigned int i = 0; i < this->segments.size(); ++i) {

                    free(this->segments[i]->pixels);
                    delete this->segments[i];

                }

                this->segments.clear();

            }

            this->width = this->sWidth = 0;
            this->height = this->sHeight = 0;
            this->p2Width = 0;
            this->p2Height = 0;
            this->swizzled = false;
            this->scaled = false;
            
        }

        /**
         * Rearrange the pixels to optimize speed.
         *
         * @link http://wiki.ps2dev.org/psp:ge_faq
         */
        void Image::swizzle() {
        
            if (this->swizzled)
                return;

            for (unsigned int r = 0; r < this->segments.size(); ++r) {

                int width = this->segments[r]->width;
                int byteWidth = width * 4;
                int height = this->segments[r]->p2Height;

                unsigned char* swiz = (unsigned char*) malloc(width * height * 4);

                unsigned int blockX, blockY;
                unsigned int j;

                unsigned int widthBlocks = (byteWidth / 16);
                unsigned int heightBlocks = (height / 8);

                unsigned int srcPitch = (byteWidth - 16) / 4;
                unsigned int srcRow = byteWidth * 8;

                const unsigned char* ySrc = this->segments[r]->pixels;
                unsigned int* dst = (unsigned int*) swiz;

                for (blockY = 0; blockY < heightBlocks; ++blockY) {

                    const unsigned char* xSrc = ySrc;

                    for (blockX = 0; blockX < widthBlocks; ++blockX) {

                        const unsigned int* src = (unsigned int*) xSrc;

                        for (j = 0; j < 8; ++j) {

                            *(dst++) = *(src++);
                            *(dst++) = *(src++);
                            *(dst++) = *(src++);
                            *(dst++) = *(src++);
                            src += srcPitch;

                        }

                        xSrc += 16;

                    }

                    ySrc += srcRow;

                }

                // Set new swizzled data
                free(this->segments[r]->pixels);
                this->segments[r]->pixels = swiz;
                sceKernelDcacheWritebackRange(this->segments[r]->pixels, width * height * 4);

            }

            // Flip bool
            this->swizzled = true;

        }

        /**
         * This function does the actual rendering and works on a per-segment basis.
         * 
         * @param float x X position to render to.
         * @param float y Y position to render to.
         * @param const ImageClip* clip Src blitting region.
         * @param const unsigned int seg Which segment to render.
         */
        void Image::render(float x, float y, const ImageClip* clip, const unsigned int seg) {
            
            if (seg > (this->segments.size() - 1))
                return;

            unsigned int w, h;
            int offsetX, offsetY;

            // Bleh, don't feel like implementing clipping for the WHOLE image
            // so do clipping for first segment
            // TODO: fix that...
            if (seg == 0 && clip != NULL) {
                
                w = (clip->width == 0) ? this->segments[0]->width : clip->width;
                h = (clip->height == 0) ? this->segments[0]->height : clip->height;

                offsetX = clip->x;
                offsetY = clip->y;

            } else {
                
                w = this->segments[seg]->width;
                h = this->segments[seg]->height;
                
                offsetX = 0;
                offsetY = 0;

            }

            // Apply texture
            sceGuTexImage(0, this->segments[seg]->p2Width, this->segments[seg]->p2Height, this->segments[seg]->width, this->segments[seg]->pixels);

            Vertex2 image[2] = {
                { // Top-Left point
                    (offsetX / (float)this->segments[seg]->p2Width), (offsetY / (float)this->segments[seg]->p2Height),
                    x, y, 0.0f
                },
                { // Bottom-Right point
                    ((offsetX + w) / (float)this->segments[seg]->p2Width), ((offsetY + h) / (float)this->segments[seg]->p2Height),
                    x + w, y + h, 0.0f
                }
            };

            sceGumMatrixMode(GU_MODEL);
            sceGumLoadIdentity();

            Vertex2* finalImage = (Vertex2*) sceGuGetMemory(sizeof(Vertex2) * 2);
            memcpy(finalImage, image, sizeof(Vertex2) * 2);
            
            sceKernelDcacheWritebackAll();

            // Draw the quad
            sceGumDrawArray(GU_SPRITES, GU_TEXTURE_32BITF | GU_VERTEX_32BITF, 2, 0, finalImage);

        }

        /**
         * Render an image onto the screen.
         *
         * @param float x X position to render to.
         * @param float y Y position to render to.
         * @param const ImageClip* clip Src blitting region.
         */
        void Image::draw(float x, float y, const ImageClip* clip) {

            if (this->segments.size() == 0)
                return;

            // Enable 2D textures
            sceGuEnable(GU_TEXTURE_2D);

            sceGuTexMode(GU_PSM_8888, 0, 0, this->isSwizzled());
            sceGuTexFunc(GU_TFX_REPLACE, GU_TCC_RGBA);
            sceGuTexFilter(GU_LINEAR, GU_LINEAR);
            sceGuTexOffset(0.0f, 0.0f);
            
            if (!this->scaled)
                sceGuTexScale(1.0f, 1.0f);
            else
                sceGuTexScale((float)this->width / this->sWidth, (float)this->height / this->sHeight);

            for (unsigned int k = 0; k < this->segments.size(); ++k) {
                
                if (!this->scaled)
                    this->render(x + this->segments[k]->x, y + this->segments[k]->y, clip, k);
                else
                    this->render(x + this->segments[k]->sX, y + this->segments[k]->sY, clip, k);

            }

            // Disable again
            sceGuDisable(GU_TEXTURE_2D);

        }

    }

}

#endif /* _Image_CPP */
