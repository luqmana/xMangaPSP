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
#include "xM/Engine/StateManager.h"
#include "xM/Stn/Callbacks.h"
#include "xM/Gfx/Graphics.h"
#include "xM/Ui/Dialogs.h"
#include "xM/Util/Stats.h"
#include "xM/Util/Log.h"
#include "xM/States/Menu.h"

#include <intraFont.h>

#include <pspnet.h>
#include <pspnet_apctl.h>
#include <pspnet_inet.h>
#include <psputility.h>
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
 * Use roughly 15MB as heap
 */
PSP_HEAP_SIZE_KB(15360);

bool Engine::running = true;

int main(int argc, char **argv) {

    // Setup various callbacks for the PSP
    Stn::setupCallbacks();

    // Init psp debug screen
    pspDebugScreenInit();
    
    //t
    sceUtilityLoadNetModule(PSP_NET_MODULE_COMMON);
sceUtilityLoadNetModule(PSP_NET_MODULE_INET);

// I have no clue what these numbers mean. :)
sceNetInit(128 * 1024, 42, 4 * 1024, 42, 4 * 1024);
sceNetInetInit();
sceNetApctlInit(0x8000, 48);
    //t
    	
    // Init intraFont font library
    intraFontInit();
    
    // Red text
    pspDebugScreenSetTextColor(0xFF0000FF);
    
    // Initiate the FileManager
    Engine::FileManager* fileManager = Engine::FileManager::getInstance();
    fileManager->init();
    
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
            
    while (Engine::isRunning()) {

        // Begin frame
        Gfx::beginFrame();
        
        // Read input unless dialog active
        if (!Ui::Dialog::isDialogActive())
            inputManager->readInput();

        // Handle events unless dialog active
        if (!Ui::Dialog::isDialogActive())
            stateManager->handleEvents();
        
        // Handle logic and update unless dialog active    
        if (!Ui::Dialog::isDialogActive())
            stateManager->handleLogic();
                        
        // Draw
        stateManager->draw();
        
        // Show some stats
        Util::MEM();
        Util::FPS();
        
        // End frame
        Gfx::endFrame();

        // Draw any active dialogs
        if (Ui::Dialog::isDialogActive())
            Ui::Dialog::renderDialogs();
        
        // V-Sync and swap buffers
        Gfx::syncAndSwap();

    }
    
    Util::logMsg("abortDialogs");
    // Close any outstanding dialogs
    Ui::Dialog::abortDialogs();
    
    Util::logMsg("cleanUp");
    // Let the states clean up
    stateManager->cleanUp();

    Util::logMsg("delete sMInstance");
    // Delete pointer to singleton StateManager
    delete Engine::StateManager::sMInstance;
        
    Util::logMsg("delete iMInstance");
    // Delete pointer to singleton InputManager
    delete Engine::InputManager::iMInstance;
    
    Util::logMsg("delete fMInstance");
    // Delete pointer to singleton FileManager
    delete Engine::FileManager::fMInstance;

    Util::logMsg("intraFontShutdown");
    // intraFont. Down.
    intraFontShutdown();

    Util::logMsg("shutdownGu");
    // Shutdown the gu and graphics subsystem
    Gfx::shutdownGu();
    
    Util::logMsg("sceKernelExitGame");
    // Exit
    sceKernelExitGame();

    return 0;

}

#endif /* _xMangaPSP_CPP */
