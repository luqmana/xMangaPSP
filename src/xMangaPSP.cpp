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
 * Main function for xMangaPSP
 * 
 * @package xMangaPSP
 */

#ifndef _xMangaPSP_CPP
#define _xMangaPSP_CPP

// BEGIN Includes
#include "xM/Std/Callbacks.h"
#include "xM/Gfx/Graphics.h"
#include "xM/Util/FPS.h"
#include "xM/Util/Timer.h"
// END Includes

/**
 * Some information and instructions.
 * We want the app to run in User Mode since kernel mode is not possible.
 */
PSP_MODULE_INFO("xMangaPSP", PSP_MODULE_USER, _MAJOR_VERSION, _MINOR_VERSION);

/**
 * Run the main thread in usermode.
 */
PSP_MAIN_THREAD_ATTR(PSP_THREAD_ATTR_USER);

int main(int argc, char **argv) {

	// Setup various callbacks for the PSP
	xM::Std::setupCallbacks();
	
	// Init psp debug screen
    pspDebugScreenInit();
    // Red text
    pspDebugScreenSetTextColor(0xFF0000FF);
	
	// A triangle
	xM::Gfx::Vertex __attribute__((aligned(16))) triangle[3] = {
		{ GU_COLOR(1.0f, 0.0f, 0.0f, 0.0f),  0.0f,  1.0f, 0.0f},     // Top point
		{ GU_COLOR(0.0f, 1.0f, 0.0f, 0.0f),  1.0f, -1.0f, 0.0f},     // Right point
		{ GU_COLOR(0.0f, 0.0f, 1.0f, 0.0f), -1.0f, -1.0f, 0.0f}      // Left point
	};
	
	// A quad
	xM::Gfx::Vertex __attribute__((aligned(16))) quad[4] = {
		{ GU_COLOR(1.0f, 1.0f, 1.0f, 0.5f), -1.0f,  1.0f, 0.0f},     // Top-Left point
		{ GU_COLOR(1.0f, 0.0f, 0.0f, 0.0f),  1.0f,  1.0f, 0.0f},     // Top-Right point
		{ GU_COLOR(0.0f, 1.0f, 0.0f, 0.0f), -1.0f, -1.0f, 0.0f},     // Bottom-Left point
		{ GU_COLOR(0.0f, 0.0f, 1.0f, 0.0f),  1.0f, -1.0f, 0.0f}      // Bottom-Right point
	};
	
	xM::Gfx::initGu();
	
	xM::Gfx::setUpPerspectiveView();
	
	// Create a timer and start it
	xM::Util::Timer timer;
	timer.start();
	
	float rTri = 0.0f;
	float rQuad= 0.0f;
	
	while (1) {
	
		sceGuStart(GU_DIRECT, xM::Gfx::displayList);
	
		// Clear screen [colour and depth]
		sceGuClear(GU_COLOR_BUFFER_BIT | GU_DEPTH_BUFFER_BIT);
	
		sceGumMatrixMode(GU_MODEL);
		sceGumLoadIdentity(); // Reset
		{
			// Move 1.5 units left and 3 units in
			ScePspFVector3 move = {-1.5f, 0.0f, -3.0f};
			sceGumTranslate(&move);
			sceGumRotateY(rTri);
		
		}
	
		// Draw the triangle
		sceGumDrawArray(GU_TRIANGLES, GU_COLOR_8888 | GU_VERTEX_32BITF | GU_TRANSFORM_3D, 3, 0, triangle);
	
		sceGumLoadIdentity(); // Reset
		{
			// Move 3 units right
			ScePspFVector3 move = {1.5f, 0.0f, -3.0f};
			sceGumTranslate(&move);
			sceGumRotateX(rQuad);
		
		}
	
		// Draw the quad
		sceGumDrawArray(GU_TRIANGLE_STRIP, GU_COLOR_8888 | GU_VERTEX_32BITF | GU_TRANSFORM_3D, 4, 0, quad);
	
		double dT = timer.getDeltaTicks(true);
		
		rTri += (1.0f * dT);
		rQuad -= (1.0f * dT);
	
		sceGuFinish();
		sceGuSync(0, 0);
	
		xM::Util::FPS();
	
		//sceDisplayWaitVblankStart();
	
		xM::Gfx::frameBuffer0 = sceGuSwapBuffers();
	
	}
	
	xM::Gfx::shutdownGu();	
	
	// Sleep so thread doesn't immediately exit
	sceKernelSleepThread();
	
	sceKernelExitGame();
	
	return 0;

}

#endif /* _xMangaPSP_CPP */
