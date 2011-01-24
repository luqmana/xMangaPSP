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

#include <cstring>
// END Includes

namespace xM {

	namespace Gfx {
			
		unsigned int __attribute__((aligned(16))) displayList[262144];
		void *frameBuffer0;
	
		/**
		 * Initiates the GU.
		 */
		void initGu(void) {
		
		    // A sort of reset
		    sceKernelDcacheWritebackAll();
		
			//displayList = memalign(16, 1600);
			frameBuffer0 = 0;
		
			// No suprise what this does
			sceGuInit();
			
			sceGuStart(GU_DIRECT, displayList);
			
			// Set buffers
			sceGuDrawBuffer(GU_PSM_8888, frameBuffer0, BUF_WIDTH);
			sceGuDispBuffer(SCR_WIDTH, SCR_HEIGHT, (void*)0x88000, BUF_WIDTH);
			sceGuDepthBuffer((void*)0x110000, BUF_WIDTH);
 
 			// Set viewport
			sceGuOffset(2048 - (SCR_WIDTH / 2), 2048 - (SCR_HEIGHT / 2));
			sceGuViewport(2048, 2048, SCR_WIDTH, SCR_HEIGHT);
			sceGuDepthRange(65535, 0);
			
			// Set render states
			
			// Don't render anything outside specified range
			sceGuScissor(0, 0, SCR_WIDTH, SCR_HEIGHT);
			
			// Enable that ^
			sceGuEnable(GU_SCISSOR_TEST);
			
			// Disable depth testing
			sceGuDisable(GU_DEPTH_TEST);
			
			// Front face is clockwise
			sceGuFrontFace(GU_CW);
			
			// Enable culling
			//sceGuEnable(GU_CULL_FACE);
			
			// Enable smooth shading
			sceGuShadeModel(GU_SMOOTH);
			
			// Clipping on
			sceGuEnable(GU_CLIP_PLANES);
			
			// Enable alpha blending
			sceGuEnable(GU_BLEND);
			sceGuBlendFunc(GU_ADD, GU_SRC_ALPHA, GU_ONE_MINUS_SRC_ALPHA, 0, 0);
						
			// Set clear colour as black
			sceGuClearColor(GU_COLOR(0.0f, 0.0f, 0.0f, 1.0f)); 
			
			// Set clear depth as 0
			sceGuClearDepth(0);
			
        	sceGuFinish();
			sceGuSync(0,0);
 
			sceDisplayWaitVblankStart();
			sceGuDisplay(GU_TRUE);

		
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
					
		}
		
		/**
		 * Clears the screen.
		 * Colour and buffer.
		 */
		void clearScreen(void) {
		
			// Clear screen and depth buffer
			sceGuClear(GU_COLOR_BUFFER_BIT|GU_DEPTH_BUFFER_BIT);
		
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
			
			free(displayList);
			free(frameBuffer0);
		
		}
		
		/**
		 * Draw a simple one colour quad.
		 * 
		 * @param float x X position.
		 * @param float y Y position.
		 * @param float z Z position.
		 * @param float w Quad width.
		 * @param float h Quad height.
		 * @param unsigned int colour Coolour of quad.
		 */
		void drawQuad(float x, float y, float z, float w, float h, unsigned int colour) {
		
		    drawQuad(x, y, z, w, h, colour, 0);
		
		}
		
		/**
		 * Draw a simple one colour quad.
		 * 
		 * @param float x X position.
		 * @param float y Y position.
		 * @param float z Z position.
		 * @param float w Quad width.
		 * @param float h Quad height.
		 * @param unsigned int colour Coolour of quad.
		 * @param float rotate Rotation
		 */
		void drawQuad(float x, float y, float z, float w, float h, unsigned int colour, float rotate) {
						
		    Vertex quad[4] = {
		        {colour, -(w / 2), -(h / 2), 0.0f}, // Top-Left point
		        {colour, (w / 2),  -(h / 2), 0.0f}, // Top-Right point
		        {colour, -(w / 2), (h / 2), 0.0f}, // Bottom-Left point
		        {colour, (w / 2), (h / 2), 0.0f} // Bottom-Right point
	        };
	        
	        sceGumMatrixMode(GU_MODEL);
		    sceGumLoadIdentity(); // Reset
		    {
			
			    ScePspFVector3 pos = {x, y, z};
			
			    // Move
			    sceGumTranslate(&pos);
			    
			    // Rotate
			    sceGumRotateZ(rotate);
		
		    }
	
	        Vertex* finalQuad = (Vertex*) sceGuGetMemory(sizeof(Vertex) * 4);
            memcpy(finalQuad, quad, sizeof(Vertex) * 4);
	
		    // Draw the quad
		    sceGumDrawArray(GU_TRIANGLE_STRIP, GU_COLOR_8888 | GU_VERTEX_32BITF | GU_TRANSFORM_3D, 4, 0, finalQuad);
		
		}
			
	}

}

#endif /* _Graphics_CPP */
