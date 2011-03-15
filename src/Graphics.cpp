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
 * 
 * @package xMangaPSP
 */

#ifndef _Graphics_CPP
#define _Graphics_CPP

// BEGIN Includes
#include "xM/Gfx/Graphics.h"
#include "xM/Util/Log.h"

#include <cstring>
// END Includes

namespace xM {

    namespace Gfx {

        /**
         * Stores the graphics commands.
         */
        unsigned int __attribute__((aligned(16))) displayList[262144];

        /**
         * Memory buffer containing a frame of data.
         */
        void *frameBuffer0;

        /**
         * Get a pointer to the frame buffer.
         *
         * @return void* Pointer to framebuffer.
         */
        void* getFrameBuffer(void) {

            return frameBuffer0;

        }

        /**
         * Get the display list.
         *
         * @return unsigned int* The display list.
         */
        unsigned int* getDisplayList(void) {

            return displayList;

        }

        /**
         * Initiates the GU.
         */
        void initGu(void) {

            // A sort of reset
            sceKernelDcacheWritebackAll();

            frameBuffer0 = 0;

            // No suprise what this does
            sceGuInit();

            sceGuStart(GU_DIRECT, displayList);

            // Set buffers
            sceGuDrawBuffer(GU_PSM_8888, frameBuffer0, BUF_WIDTH);
            sceGuDispBuffer(SCR_WIDTH, SCR_HEIGHT, (void*) 0x88000, BUF_WIDTH);
            sceGuDepthBuffer((void*) 0x110000, BUF_WIDTH);

            // Set viewport
            sceGuOffset(2048 - (SCR_WIDTH / 2), 2048 - (SCR_HEIGHT / 2));
            sceGuViewport(2048, 2048, SCR_WIDTH, SCR_HEIGHT);
            sceGuDepthRange(65535, 0);

            // Set render states
            resetRenderStates();
            
            // Reset sceGuTex* states
            sceGuTexMode(GU_PSM_8888, 0, 0, 0);
            sceGuTexFunc(GU_TFX_REPLACE, GU_TCC_RGBA);
            sceGuTexFilter(GU_LINEAR, GU_LINEAR);

            // Set clear colour as black
            sceGuClearColor(GU_COLOR(0.0f, 0.0f, 0.0f, 1.0f));

            // Set clear depth as 0
            sceGuClearDepth(0);

            sceGuFinish();
            sceGuSync(0, 0);

            sceDisplayWaitVblankStart();
            sceGuDisplay(GU_TRUE);


        }

        /**
         * Setups the graphic render states.
         */
        void resetRenderStates(void) {

            // Don't render anything outside specified range
            sceGuScissor(0, 0, SCR_WIDTH, SCR_HEIGHT);

            // Enable that ^
            sceGuEnable(GU_SCISSOR_TEST);

            // Disable depth testing
            sceGuDisable(GU_DEPTH_TEST);

            // Front face is clockwise
            sceGuFrontFace(GU_CW);

            // Enable culling
            sceGuEnable(GU_CULL_FACE);

            // Enable smooth shading
            sceGuShadeModel(GU_SMOOTH);

            // Clipping on
            sceGuEnable(GU_CLIP_PLANES);

            // Enable alpha blending
            sceGuEnable(GU_BLEND);
            sceGuBlendFunc(GU_ADD, GU_SRC_ALPHA, GU_ONE_MINUS_SRC_ALPHA, 0, 0);

            // Disable textures
            sceGuDisable(GU_TEXTURE_2D);

        }

        /**
         * Sets up the projection matrix in a perspective view.
         */
        void setUpPerspectiveView(void) {

            // Setup matrices
            sceGumMatrixMode(GU_PROJECTION);
            sceGumLoadIdentity(); // Reset
            sceGumPerspective(75.0f, 16.0f / 9.0f, 0.5f, 1000.0f);

            sceGumMatrixMode(GU_VIEW);
            sceGumLoadIdentity(); // Reset

            // Reset
            sceGumMatrixMode(GU_MODEL);
            sceGumLoadIdentity();

        }

        /**
         * Sets up the projection matrix in a orthographic view.
         */
        void setUpOrthoView(void) {

            // Setup matrices
            sceGumMatrixMode(GU_PROJECTION);
            sceGumLoadIdentity(); // Reset
            sceGumOrtho(0.0f, SCR_WIDTH, SCR_HEIGHT, 0.0f, -1.0f, 1.0f);

            sceGumMatrixMode(GU_VIEW);
            sceGumLoadIdentity(); // Reset

            // Reset
            sceGumMatrixMode(GU_MODEL);
            sceGumLoadIdentity();

        }

        /**
         * Clears the screen.
         * Colour and buffer.
         */
        void clearScreen(void) {

            // Clear screen and depth buffer
            sceGuClear(GU_COLOR_BUFFER_BIT | GU_DEPTH_BUFFER_BIT);

        }

        /**
         * Prepare the GU for rendering.
         */
        void beginFrame(void) {

            // Render in direct mode
            sceGuStart(GU_DIRECT, displayList);

            // Clear screen [colour and depth]
            clearScreen();

            resetRenderStates();

        }

        /**
         * End rendering
         */
        void endFrame(void) {

            sceGuFinish();

            // Wait for the currently executing display list
            sceGuSync(GU_SYNC_FINISH, GU_SYNC_WHAT_DONE);

        }

        /**
         * Perfoms a vertical sync and swaps the buffers.
         */
        void syncAndSwap(void) {

            // V-sync
            sceDisplayWaitVblankStart();

            // Swap the front and back buffers
            frameBuffer0 = sceGuSwapBuffers();

        }

        /**
         * Terminates the GU.
         */
        void shutdownGu(void) {

            sceGuTerm();

        }

        /**
         * Draw a simple one colour quad.
         *
         * @param float x X position.
         * @param float y Y position.
         * @param float w Quad width.
         * @param float h Quad height.
         * @param unsigned int colour Colour of quad.
         */
        void drawQuad(float x, float y, float w, float h, unsigned int colour) {

            drawQuad(x, y, w, h, colour, 0);

        }

        /**
         * Draw a simple one colour quad.
         *
         * @param float x X position.
         * @param float y Y position.
         * @param float w Quad width.
         * @param float h Quad height.
         * @param unsigned int colour Colour of quad.
         * @param float rotate Rotation
         */
        void drawQuad(float x, float y, float w, float h, unsigned int colour, float rotate) {
            
            drawQuad(x, y, w, h, colour, colour, colour, colour, rotate);

        }

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
                unsigned int colourTopRight, unsigned int colourBottomLeft, unsigned int colourBottomRight, float rotate) {

            Vertex quad[4] = {
                {0, 0, colourTopLeft, -(w / 2), -(h / 2), 0.0f}, // Top-Left point
                {1, 0, colourTopRight, (w / 2), -(h / 2), 0.0f}, // Top-Right point
                {0, 1, colourBottomLeft, -(w / 2), (h / 2), 0.0f}, // Bottom-Left point
                {1, 1, colourBottomRight, (w / 2), (h / 2), 0.0f} // Bottom-Right point
            };

            sceGumMatrixMode(GU_MODEL);
            sceGumLoadIdentity(); // Reset
            {

                ScePspFVector3 pos = {x + (w / 2), y + (h / 2), 0.0f};

                // Move
                sceGumTranslate(&pos);

                // Rotate
                sceGumRotateZ(rotate);

            }

            Vertex* finalQuad = (Vertex*) sceGuGetMemory(sizeof (Vertex) * 4);
            memcpy(finalQuad, quad, sizeof (Vertex) * 4);

            // Draw the quad
            sceGumDrawArray(GU_TRIANGLE_STRIP, GU_TEXTURE_32BITF | GU_COLOR_8888 | GU_VERTEX_32BITF | GU_TRANSFORM_3D, 4, 0, finalQuad);

        }

    }

}

#endif /* _Graphics_CPP */
