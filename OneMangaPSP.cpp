/*
 * $Id: OneMangaPSP.cpp 86 2010-01-03 04:12:17Z chaotic@luqmanrocks.co.cc $
 * 
 * This file is part of the OneMangaPSP application.
 *
 * Copyright (C) 2009  Luqman Aden <www.luqmanrocks.co.cc>.
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
 * Full License: LICENSE.txt
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

/**
 * The main entry point for our application.
 * 
 * @package OneMangaPSP
 */

#ifndef _OneMangaPSP_CPP
#define _OneMangaPSP_CPP

// BEGIN Includes
#include "OneMangaPSP.h"
#include "States/TitleState.h"
#include "MangaAPI.h"
#include "ResourceManager.h"
#include "BookmarkManager.h"
#include <psppower.h>
// END Includes

/**
 * Some information and instructions.
 * We want the app to run in User Mode since kernel mode is not possible.
 */
PSP_MODULE_INFO("OneMangaPSP", PSP_MODULE_USER, _MAJOR_VERSION, _MINOR_VERSION);

/**
 * Main thread name.
 */
PSP_MAIN_THREAD_NAME("OneMangaPSPMT");

/**
 * Run the main thread in usermode.
 */
PSP_MAIN_THREAD_ATTR(PSP_THREAD_ATTR_USER | PSP_THREAD_ATTR_VFPU);

/**
 * Set the PSP heap size.
 * Use everything but the specified amount. So about 1.5MB left for the system [i.e. plugins, threads, etc]
 */
PSP_HEAP_SIZE_KB(-1536);

/**
 * The LeXEngine object.
 */
LeXEngine *engine = NULL;

/**
 * Background music file.
 */
std::string bgMusicFile = "music.mp3";

/**
 * Main function for app.
 * Won't be using argc and argv so we comment out.
 * 
 * @return int
 */
int main(int /*argc*/, char**/*argv[]*/) {

    // Setup the standard callbacks
    setupStandardCallbacks();

    // Initialize the Engine
    engine = new LeXEngine(480, 272, 16, LeXInitEVERYTHING | LeXInitOpenGL | LeXInitOpenGLDB);

	// Start Manga API Handling Thread    
    startMangaAPIHandlerThread();

    // TODO: Set clock frequencies

	// Set up OpenGL Environment
	engine->setupOpenGL();

	// Load bookmark manager
	BookmarkManager::loadBookmarks();
			
    // Start at Title State
    engine->changeState(new TitleState(1));

    // Start game.
    engine->startGame();

    // Begin game loop
    while (engine->isGameRunning()) {

        // Handle User/System Input/Events
        engine->handleInput();

        // Handle logic and update accordingly
        engine->handleLogic();

        // Draw to screen (or off-screen buffer)
        engine->render();

        // Update screen
        engine->updateScreen();

    }

	// Stop Manga API Handling Thread
    stopMangaAPIHandlerThread();

	// Free resources
	ResourceManager::cleanUp();

    // Free more resources
    delete engine;
    
    // If we get here we exit game
    sceKernelExitGame();

    // Never actually get here but it's a requirement
    return 0;

}

#endif /* _OneMangaPSP_CPP */
