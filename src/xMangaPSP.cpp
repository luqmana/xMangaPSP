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
 * Main function for xMangaPSP
 * 
 * @package xMangaPSP
 */

#ifndef _xMangaPSP_CPP
#define _xMangaPSP_CPP

// BEGIN Includes
#include "xM/Engine/StateManager.h"
#include "xM/Stn/Callbacks.h"
#include "xM/Gfx/Graphics.h"
#include "xM/Util/Stats.h"

#include "xM/States/Menu.h"
// END Includes

// Less typing
using namespace xM;

/**
 * Some information and instructions.
 * We want the app to run in User Mode since kernel mode is not possible.
 */
PSP_MODULE_INFO("xMangaPSP", PSP_MODULE_USER, _MAJOR_VERSION, _MINOR_VERSION);

/**
 * Main thread name.
 */
PSP_MAIN_THREAD_NAME("xMangaPSP");

/**
 * Run the main thread in usermode.
 */
PSP_MAIN_THREAD_ATTR(PSP_THREAD_ATTR_USER);

/**
 * Set the PSP heap size.
 * Use everything but the specified amount. So about 1.5MB left for the system [i.e. plugins, threads, etc]
 */
//PSP_HEAP_SIZE_KB(-1536);

int main(int argc, char **argv) {

	// Setup various callbacks for the PSP
	Stn::setupCallbacks();
	
	// Init psp debug screen
    pspDebugScreenInit();
    
    // Red text
    pspDebugScreenSetTextColor(0xFF0000FF);
	
	// Initiate the StateManager and set the initial state
	Engine::StateManager* stateManager = Engine::StateManager::getInstance();
	stateManager->changeState(new States::Menu());
			
	// Initiate the GU
	Gfx::initGu();	
		
	while (true) {
	
	    // Handle user/system input/events
		stateManager->handleEvents();
		
		// Handle logic and update accordingly
		stateManager->handleLogic();
		
		// Rendering to display list begin
		sceGuStart(GU_DIRECT, xM::Gfx::displayList);
		
		// Clear screen [colour and depth]
		xM::Gfx::clearScreen();
		
		// Setup ortho view
		xM::Gfx::setUpOrthoView();
		
		// Draw
		stateManager->draw();
		
		// The accompanying finish
		sceGuFinish();
		
		// Wait for the currently executing display list
		sceGuSync(GU_SYNC_FINISH, GU_SYNC_WHAT_DONE);
	
	    // Show some stats
		Util::MEM();
		Util::FPS();

        // Vsync and swap buffers		
		Gfx::syncAndSwap();
	
	}
	
	// Let the states clean up
	stateManager->cleanUp();
	
	// Delete pointer
	delete Engine::StateManager::sMInstance;
	
	xM::Gfx::shutdownGu();	
	
	// Sleep so thread doesn't immediately exit
	sceKernelSleepThread();
	
	sceKernelExitGame();
	
	return 0;

}

#endif /* _xMangaPSP_CPP */
