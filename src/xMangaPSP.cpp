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
#include "xM/Engine/App.h"
#include "xM/Engine/FileManager.h"
#include "xM/Engine/InputManager.h"
#include "xM/Engine/ResourceManager.h"
#include "xM/Engine/StateManager.h"
#include "xM/Gfx/Graphics.h"
#include "xM/Manga/MangaAPI.h"
#include "xM/Net/Net.h"
#include "xM/States/Menu.h"
#include "xM/Stn/Callbacks.h"
#include "xM/Ui/Dialogs.h"
#include "xM/Util/Stats.h"
#include "xM/Util/Log.h"
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
 * Use everything but 6MB as heap
 */
//PSP_HEAP_SIZE_KB(-6144);
PSP_HEAP_SIZE_KB(1024 * 15);

bool Engine::running = true;

int main(int argc, char **argv) {

    // Setup various callbacks for the PSP
    Stn::setupCallbacks();

    // Init psp debug screen
    pspDebugScreenInit();
        	
    // Init intraFont font library
    intraFontInit();
    
    // Red text, no background
    pspDebugScreenSetTextColor(Gfx::Colour::RED);
    pspDebugScreenEnableBackColor(0);
    
    // Initiate network support
    Net::init();
    
    // Initiate the Manga API Handler
    Manga::initMangaAPIThread();
    
    // Initiate the FileManager
    Engine::FileManager* fileManager = Engine::FileManager::getInstance();
    fileManager->init();
    
    // Initiate the ResourceManager
    Engine::ResourceManager* resourceManager = Engine::ResourceManager::getInstance();
    resourceManager->init();
    
    // Initiate the InputManager
    Engine::InputManager* inputManager = Engine::InputManager::getInstance();
    inputManager->init();
    
    // Initiate the StateManager and set the initial state
    Engine::StateManager* stateManager = Engine::StateManager::getInstance();
    stateManager->changeState(new States::Menu());

    // Initiate the GU
    Gfx::initGu();

    // Setup ortho view
    Gfx::setUpOrthoView();
    
    bool showFPSMEM = (__xM_DEBUG) ? true : false;
            
    while (Engine::isRunning()) {
    
        if (stateManager->getCurrentState() == NULL)
            break;

        // Begin frame
        Gfx::beginFrame();
        
        // Read input and handle events unless dialog active
		// The built-in dialog system handles its own input/events
        if (!Ui::Dialog::isDialogActive()) {
        
            inputManager->readInput();
            stateManager->handleEvents();
            
        }
        
        // Handle logic and update unless dialog active    
        if (!Ui::Dialog::isDialogActive())
            stateManager->handleLogic();
                    
        // Draw
        stateManager->draw();
                
        // Toggle stats
        if (__xM_DEBUG && inputManager->pressed(PSP_CTRL_RTRIGGER))
            showFPSMEM = !showFPSMEM;
        
        // Show some stats
        if (showFPSMEM) {
            
            Util::MEM();
            Util::FPS();
            
        }
        
        // Render a semi-transparent black quad covering the whole screen to
        // make dialogs better visible
        if (Ui::Dialog::isDialogActive())
            Gfx::drawQuad(0, 0, 480, 272, GU_COLOR(0.0f, 0.0f, 0.0f, 0.75f), 0);
        
        // End frame
        Gfx::endFrame();

        // Draw any active dialogs
        if (Ui::Dialog::isDialogActive())            
            Ui::Dialog::renderDialogs();
        
        // V-Sync and swap buffers
        Gfx::syncAndSwap();

    }
    
    // Close any outstanding dialogs
    Ui::Dialog::abortDialogs();
    
    // Let the states clean up
    stateManager->cleanUp();

    // Delete pointer to singleton StateManager
    delete Engine::StateManager::sMInstance;
        
    // Delete pointer to singleton InputManager
    delete Engine::InputManager::iMInstance;
    
    // Delete pointer to singleton ResourceManager
    delete Engine::ResourceManager::rMInstance;
    
    // Delete pointer to singleton FileManager
    delete Engine::FileManager::fMInstance;
    
    // Shutdown the Manga API Handler
    Manga::shutdownMangaAPIThread();
    
    // Shutdown network stuff
    Net::shutdown();

    // intraFont. Down.
    intraFontShutdown();
    
    // Shutdown the gu and graphics subsystem
    Gfx::shutdownGu();
    
    // Exit
    sceKernelExitGame();

    return 0;

}

#endif /* _xMangaPSP_CPP */
