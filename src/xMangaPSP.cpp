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
#include "xM/Util/Stats.h"
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

	// A quad	
	xM::Gfx::Vertex __attribute__((aligned(16))) quad[4] = {
		{GU_COLOR(1.0f, 1.0f, 1.0f, 1.0f), -50.0f, -50.0f, 0.0f}, // Top-Left point
		{GU_COLOR(1.0f, 0.0f, 0.0f, 1.0f), 50.0f,  -50.0f, 0.0f}, // Top-Right point
		{GU_COLOR(0.0f, 1.0f, 0.0f, 1.0f), -50.0f, 50.0f, 0.0f}, // Bottom-Left point
		{GU_COLOR(0.0f, 0.0f, 1.0f, 1.0f), 50.0f, 50.0f, 0.0f} // Bottom-Right point
	};
	// A second quad	
	xM::Gfx::Vertex __attribute__((aligned(16))) quad2[4] = {
		{GU_COLOR(0.0f, 0.0f, 0.0f, 0.5f), -25.0f, -25.0f, 0.0f}, // Top-Left point
		{GU_COLOR(0.0f, 0.0f, 0.0f, 0.5f), 25.0f,  -25.0f, 0.0f}, // Top-Right point
		{GU_COLOR(0.0f, 0.0f, 0.0f, 0.5f), -25.0f, 25.0f, 0.0f}, // Bottom-Left point
		{GU_COLOR(0.0f, 0.0f, 0.0f, 0.5f), 25.0f, 25.0f, 0.0f} // Bottom-Right point
	};
	// Triangle
	xM::Gfx::Vertex __attribute__((aligned(16))) triangle[3] = {   
       {GU_COLOR(1.0f, 0.0f, 0.0f, 1.0f), 0.0f, -50.0f, 0.0f}, // Top, red
       {GU_COLOR(0.0f, 1.0f, 0.0f, 1.0f), 50.0f, 50.0f, 0.0f}, // Right, green
       {GU_COLOR(0.0f, 0.0f, 1.0f, 1.0f), -50.0f, 50.0f, 0.0f}, // Left, blue
	};
		
	// Initiate the GU
	xM::Gfx::initGu();	
	
	// Create a timer and start it
	xM::Util::Timer timer;
	timer.start();
	
	float rotate = 0.0f;
	
	while (1) {
	
		sceGuStart(GU_DIRECT, xM::Gfx::displayList);
		
		sceGuDisable(GU_TEXTURE_2D);
			
		// Clear screen [colour and depth]
		xM::Gfx::clearScreen();
		
		// Setup ortho view
		xM::Gfx::setUpOrthoView();
						
		sceGumMatrixMode(GU_MODEL);
		sceGumLoadIdentity(); // Reset
		{
			
			ScePspFVector3 pos = {240.0f, 160.0f, 0.0f};
			
			// Move
			sceGumTranslate(&pos);
			
			// Rotate
			sceGumRotateZ(-rotate);
		
		}
	
		// Draw the quad
		sceGumDrawArray(GU_TRIANGLE_STRIP, GU_COLOR_8888 | GU_VERTEX_32BITF | GU_TRANSFORM_3D, 4, 0, quad);
		
		sceGumMatrixMode(GU_MODEL);
		sceGumLoadIdentity(); // Reset
		{
			
			ScePspFVector3 pos = {240.0f, 160.0f, 0.0f};
			
			// Move
			sceGumTranslate(&pos);
			
			// Rotate
			sceGumRotateZ(rotate);
		
		}
	
		// Draw the quad
		sceGumDrawArray(GU_TRIANGLE_STRIP, GU_COLOR_8888 | GU_VERTEX_32BITF | GU_TRANSFORM_3D, 4, 0, quad2);
		
		sceGumLoadIdentity(); // Reset
		{
			
			ScePspFVector3 pos = {140.0f, 100.0f, 0.0f};
			
			// Move
			sceGumTranslate(&pos);
			
			// Rotate
			sceGumRotateZ(-rotate);
		
		}
		
		// Draw the triangle
		sceGumDrawArray(GU_TRIANGLES, GU_COLOR_8888 | GU_VERTEX_32BITF | GU_TRANSFORM_3D, 3, 0, triangle);
		
		sceGumLoadIdentity(); // Reset
		{
			
			ScePspFVector3 pos = {360.0f, 160.0f, 0.0f};
			
			// Move
			sceGumTranslate(&pos);
			
			// Rotate
			sceGumRotateZ(rotate);
		
		}
		
		// Another Triangle
		xM::Gfx::Vertex __attribute__((aligned(16))) triangle2[3] = {   
    	   {GU_COLOR((rand()%256) / 255.0f, 0.0f, 0.0f, (rand()%50) / 100.0f), 0.0f, -50.0f, 0.0f}, // Top
    	   {GU_COLOR(0.0f, (rand()%256) / 255.0f, 0.0f, 1.0f), 50.0f, 50.0f, 0.0f}, // Right
	       {GU_COLOR(0.0f, 0.0f, (rand()%256) / 255.0f, (rand()%85) / 100.0f), -50.0f, 50.0f, 0.0f}, // Left
		};
		
		// Draw the triangle
		sceGumDrawArray(GU_TRIANGLES, GU_COLOR_8888 | GU_VERTEX_32BITF | GU_TRANSFORM_3D, 3, 0, triangle2);
			
		rotate -= (1.0f * timer.getDeltaTicks(true));
	
		sceGuFinish();
		sceGuSync(0, 0);
	
		xM::Util::MEM();
		xM::Util::FPS();

		sceDisplayWaitVblankStart();
	
		xM::Gfx::frameBuffer0 = sceGuSwapBuffers();
	
	}
	
	xM::Gfx::shutdownGu();	
	
	// Sleep so thread doesn't immediately exit
	sceKernelSleepThread();
	
	sceKernelExitGame();
	
	return 0;

}

#endif /* _xMangaPSP_CPP */
