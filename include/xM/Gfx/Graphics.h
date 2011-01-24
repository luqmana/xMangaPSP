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
		
			unsigned int colour;
			float x, y, z;
		
		} Vertex;
		
		extern unsigned int __attribute__((aligned(16))) displayList[262144];
		extern void *frameBuffer0;
	
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
		 * @param float z Z position.
		 * @param float w Quad width.
		 * @param float h Quad height.
		 * @param unsigned int colour Coolour of quad.
		 */
		void drawQuad(float x, float y, float z, float w, float h, unsigned int colour);
		
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
		void drawQuad(float x, float y, float z, float w, float h, unsigned int colour, float rotate);
			
	}

}

#endif /* _Graphics_H */
