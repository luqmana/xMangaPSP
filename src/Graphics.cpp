/**
 * This file is part of the xMangaPSP application.
 *
 * Copyright (C) 2010  Luqman Aden <www.luqmanrocks.co.cc>.
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
// END Includes

namespace xM {

	namespace Gfx {
			
		void *displayList;
		void *frameBuffer0;
	
		/**
		 * Initiates the GU.
		 */
		void initGu(void) {
		
			displayList = memalign(16, 640);
			frameBuffer0 = 0;
		
			// No suprise what this does
			sceGuInit();
			
			sceGuStart(GU_DIRECT, displayList);
			
			// Set buffers
			sceGuDrawBuffer(GU_PSM_8888, frameBuffer0, BUF_WIDTH);
			sceGuDispBuffer(SCR_WIDTH, SCR_HEIGHT, (void*)0x88000, BUF_WIDTH);
			sceGuDepthBuffer((void*)0x110000, BUF_WIDTH);
 
			sceGuOffset(2048 - (SCR_WIDTH / 2), 2048 - (SCR_HEIGHT / 2));
			sceGuViewport(2048, 2048, SCR_WIDTH, SCR_HEIGHT);
			sceGuDepthRange(65535, 0);
			
			// Set render states
			sceGuScissor(0, 0, SCR_WIDTH, SCR_HEIGHT);
			sceGuEnable(GU_SCISSOR_TEST);
			sceGuDepthFunc(GU_GEQUAL);
			sceGuEnable(GU_DEPTH_TEST);
			//sceGuFrontFace(GU_CW);
			//sceGuShadeModel(GU_SMOOTH);
			sceGuEnable(GU_CULL_FACE);
			sceGuEnable(GU_CLIP_PLANES);
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
 
 			// Clear colour to black
			sceGuClearColor(GU_COLOR(0.0f, 0.0f, 0.0f, 1.0f)); 
			// Clear depth 0
			sceGuClearDepth(0);
		
		}
		
		/**
		 * Terminates the GU.
		 */
		void shutdownGu(void) {
		
			sceGuTerm();
			
			free(displayList);
			free(frameBuffer0);
		
		}
			
	}

}

#endif /* _Graphics_CPP */
