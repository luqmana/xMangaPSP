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
 * Graphic stuff.
 * Header file.
 * 
 * @package xMangaPSP
 */

#ifndef _Graphics_H
#define _Graphics_H

// BEGIN Includes
#include <pspdisplay.h>
#include <pspgu.h>
#include <pspgum.h>
#include <pspkernel.h>

#include <malloc.h>
// END Includes

// BEGIN Defines
#define BUF_WIDTH  (512)  // Buffer width
#define SCR_WIDTH  (480)  // Screen width
#define SCR_HEIGHT (272)  // Screen height
// END Defines

namespace xM {

    namespace Gfx {

        /**
         * Convenience type definition to define a point/pixel.
         */
        typedef struct {
            float u, v;
            unsigned int colour;
            float x, y, z;

        } Vertex;

        /**
         * Get a pointer to the frame buffer.
         *
         * @return void* Pointer to framebuffer.
         */
        void* getFrameBuffer(void);

        /**
         * Get the display list.
         *
         * @return unsigned int* The display list.
         */
        unsigned int* getDisplayList(void);

        /**
         * Initiates the GU.
         */
        void initGu(void);

        /**
         * Sets up the projection matrix in a perspective view.
         */
        void setUpPerspectiveView(void);

        /**
         * Sets up the projection matrix in a orthographic view.
         */
        void setUpOrthoView(void);

        /**
         * Clears the screen.
         * Colour and buffer.
         */
        void clearScreen(void);

        /**
         * Prepare the GU for rendering.
         */
        void beginFrame(void);

        /**
         * End rendering
         */
        void endFrame(void);

        /**
         * Perfoms a vertical sync and swaps the buffers.
         */
        void syncAndSwap(void);

        /**
         * Terminates the GU.
         */
        void shutdownGu(void);

        /**
         * Draw a simple one colour quad.
         *
         * @param float x X position.
         * @param float y Y position.
         * @param float w Quad width.
         * @param float h Quad height.
         * @param unsigned int colour Colour of quad.
         */
        void drawQuad(float x, float y, float w, float h, unsigned int colour);

        /**
         * Draw a simple one colour quad with rotation.
         *
         * @param float x X position.
         * @param float y Y position.
         * @param float w Quad width.
         * @param float h Quad height.
         * @param unsigned int colour Colour of quad.
         * @param float rotate Rotation
         */
        void drawQuad(float x, float y, float w, float h, unsigned int colour, float rotate);

        /**
         * Draw a quad.
         *
         * @param float x X position.
         * @param float y Y position.
         * @param float w Quad width.
         * @param float h Quad height.
         * @param unsigned int colourTopLeft Colour of quad extending from top left.
         * @param unsigned int colourTopRight Colour of quad extending from top right.
         * @param unsigned int colourBottomLeft Colour of quad extending from bottom left.
         * @param unsigned int colourBottomRight Colour of quad extending from bottom right.
         * @param float rotate Rotation
         */
        void drawQuad(float x, float y, float w, float h, unsigned int colourTopLeft,
                unsigned int colourTopRight, unsigned int colourBottomLeft, unsigned int colourBottomRight, float rotate);

        /**
         * Draw a quad. [The actual function]
         *
         * @param float x X position.
         * @param float y Y position.
         * @param float aW Actual width.
         * @param float aH Actual height.
         * @param float w Quad width.
         * @param float h Quad height.
         * @param unsigned int colourTopLeft Colour of quad extending from top left.
         * @param unsigned int colourTopRight Colour of quad extending from top right.
         * @param unsigned int colourBottomLeft Colour of quad extending from bottom left.
         * @param unsigned int colourBottomRight Colour of quad extending from bottom right.
         * @param float rotate Rotation
         */
        void drawQuad(float x, float y, float aW, float aH, float w, float h, unsigned int colourTopLeft,
                unsigned int colourTopRight, unsigned int colourBottomLeft, unsigned int colourBottomRight, float rotate);

    }

}

#endif /* _Graphics_H */
