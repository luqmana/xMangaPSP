/*
 * $Id: LeXEngine.cpp 86 2010-01-03 04:12:17Z chaotic@luqmanrocks.co.cc $
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
 * LeX Engine.
 * 
 * @package OneMangaPSP
 */

#ifndef _LeXEngine_CPP
#define _LeXEngine_CPP

// BEGIN Includes
#include "LeXEngine.h"
#include "OMPNetwork.h"
#include "State.h"

// Extra string support
#include <sstream>

// PSP GU Support
#include <pspgu.h>

// Memory Managament Support
#include <malloc.h>

// File I/O Support
#include <pspiofilemgr.h>

// Net Support
#include <pspnet.h>
#include <pspnet_apctl.h>
#include <pspnet_inet.h>

// Time Stuff
#include <psprtc.h>

// ZIP-Support
#include <zzip/zzip.h>

// Standard headers
#include <stdarg.h>
#include <stdio.h>
// END Includes

/**
 * Constructor.
 * Init systems.
 *
 * @access public
 * 
 * @param int screenWidth The screen width.
 * @param int screenHeight The screen height.
 * @param int bitsPerPixel Bits-per-pixel.
 */
LeXEngine::LeXEngine(int screenWidth, int screenHeight, int bitsPerPixel) {

	// Call init function
	this->init(screenWidth, screenHeight, bitsPerPixel, LeXInitEVERYTHING, SDL_HWSURFACE | SDL_DOUBLEBUF);
		
}

/**
 * Constructor.
 * Init systems.
 *
 * @access public
 * 
 * @param int screenWidth The screen width.
 * @param int screenHeight The screen height.
 * @param int bitsPerPixel Bits-per-pixel.
 * @param LeXInitFlags Startup flags.
 */
LeXEngine::LeXEngine(int screenWidth, int screenHeight, int bitsPerPixel, int initFlags) {

	// Call init function
	this->init(screenWidth, screenHeight, bitsPerPixel, initFlags, SDL_HWSURFACE | SDL_DOUBLEBUF);

}

/**
 * Constructor.
 * Init systems.
 *
 * @access public
 * 
 * @param int screenWidth The screen width.
 * @param int screenHeight The screen height.
 * @param int bitsPerPixel Bits-per-pixel.
 * @param LeXInitFlags Startup flags.
 * @param Uint32 SDL Video mode flags.
 */
LeXEngine::LeXEngine(int screenWidth, int screenHeight, int bitsPerPixel, int initFlags, Uint32 videoModeFlags){

	// Call init function
	this->init(screenWidth, screenHeight, bitsPerPixel, initFlags, videoModeFlags);
		
}
		
/**
 * Destructor.
 */
LeXEngine::~LeXEngine() {

	// Loop through all states
	while (!this->states.empty()) {
	
		// Let state cleanup
		this->states.back()->shutdown();
		
		// Remove state from stack
		this->states.pop_back();
	
	}

	if ((this->initFlags & LeXInitEVERYTHING) || (this->initFlags & LeXInitNET)) {
	
		// Shutdown NET
		
		// Unload net modules
		sceNetApctlTerm();
		sceNetInetTerm();
		sceNetTerm();
		
		sceUtilityUnloadNetModule(PSP_NET_MODULE_INET);
		sceUtilityUnloadNetModule(PSP_NET_MODULE_COMMON);
		
	
	}
	
	if ((this->initFlags & LeXInitEVERYTHING) || (this->initFlags & LeXInitAUDIO)) {
			
		// Cleanup
		this->cleanUpMusic();
			
		// Shutdown audio system
		sceUtilityUnloadAvModule(PSP_AV_MODULE_AVCODEC);
	
	}
	
	if ((this->initFlags & LeXInitEVERYTHING) || (this->initFlags & LeXInitJOYSTICK)) {
			
		// Stop repeat thread
		stopJoystickRepeatThread();
			
		// Close Joysticks
		
		int i = 0;
		for (i = 0; i < SDL_NumJoysticks(); ++i) {
				
			SDL_JoystickClose(this->joysticks[i]);
		
		}
		
		// Shutdown SDL Joystick subsystem
		SDL_QuitSubSystem(SDL_INIT_JOYSTICK);
	
	}
		
	if ((this->initFlags & LeXInitEVERYTHING) || (this->initFlags & LeXInitCURL)) {
	
		// Shutdown libCurl
		curl_global_cleanup();
	
	}
	
	if ((initFlags & LeXInitEVERYTHING) || (initFlags & LeXInitTTF)) {
	
		// Shutdown SDL TrueType Font Library
		TTF_Quit();
	
	}
	
	// Shutdown SDL Fast Events Library
	FE_Quit();

	// Quit all other SDL subsystems
	SDL_Quit();
			
}

/**
 * Init systems.
 * 
 * @access private
 * 
 * @param int screenWidth The screen width.
 * @param int screenHeight The screen height.
 * @param int bitsPerPixel Bits-per-pixel.
 * @param LeXInitFlags Startup flags.
 * @param Uint32 SDL Video mode flags.
 */
void LeXEngine::init(int screenWidth, int screenHeight, int bitsPerPixel, int initFlags, Uint32 videoModeFlags) {

	this->screenWidth		=	screenWidth;
	this->screenHeight		=	screenHeight;
	this->bitsPerPixel		=	bitsPerPixel;
	this->initFlags			=	initFlags;
	this->videoModeFlags	=	videoModeFlags;
			
	// SDL Initalization
	// Video and Timer Subsystem always initiated
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) < 0) {
	
		this->logMsg("LeXEngine::init() - Unable to initialize SDL Video and/or Timer subsystem [%s].", SDL_GetError());
	
	}
	
	// Init Fast Events Library
	if (FE_Init() == -1) {

		this->logMsg("LeXEngine::init() - Unable to initialize SDL Fast Events Library [%s].", FE_GetError());
	
	}
					
	// If NOEXTRA flag set return here
	if (initFlags & LeXInitNOEXTRA)
		return;
	
	if ((initFlags & LeXInitEVERYTHING) || (initFlags & LeXInitNET)) {
	
		// Init NET
		
		// Load net modules
		sceUtilityLoadNetModule(PSP_NET_MODULE_COMMON);
		sceUtilityLoadNetModule(PSP_NET_MODULE_INET);
		
		// I have no clue what these numbers mean. :)
		sceNetInit(128 * 1024, 42, 4 * 1024, 42, 4 * 1024);
		sceNetInetInit();
		sceNetApctlInit(0x8000, 48);
	
	}
	
	if ((initFlags & LeXInitEVERYTHING) || (initFlags & LeXInitAUDIO)) {
	
		// Init audio modules
		sceUtilityLoadAvModule(PSP_AV_MODULE_AVCODEC);
			
	}

	if ((initFlags & LeXInitEVERYTHING) || (initFlags & LeXInitJOYSTICK)) {
	
		// Init SDL Joystick subsystem
		if (SDL_InitSubSystem(SDL_INIT_JOYSTICK) < 0 ) {
		
			this->logMsg("LeXEngine::init() - Unable to initialize SDL Joystick subsystem [%s].", SDL_GetError());
		
		}
	
		// Enable joystick events
		SDL_JoystickEventState(SDL_ENABLE);
		
		// Enable repeat events
		startJoystickRepeatThread();
		
		// Open Joysticks
		
		int i = 0;
		SDL_Joystick *joy;
		
		for (i = 0; i < SDL_NumJoysticks(); ++i) {
				
			joy = SDL_JoystickOpen(i);
			
			if (!joy) {
			
				this->logMsg("LeXEngine::init() - Unable to initialize SDL Joystick #%d.", i);
			
			} else {
				
				this->joysticks.push_back(joy);
			
			}
		
		}
	
	}
		
	if ((initFlags & LeXInitEVERYTHING) || (initFlags & LeXInitCURL)) {
	
		// Init libCurl
		if (curl_global_init(CURL_GLOBAL_ALL) != 0) {
		
			this->logMsg("LeXEngine:init() - Unable to initialize libCurl.");
		
		}
	
	}
	
	if ((initFlags & LeXInitEVERYTHING) || (initFlags & LeXInitTTF)) {
	
		// Init SDL TrueType Font Library
		if (TTF_Init() == -1) {
		
			this->logMsg("LeXEngine::init() - Unable to initialize SDL TTF [%s].", TTF_GetError());
		
		}
	
	}
	
	// Setup screen
	this->setupScreen();

}

/**
 * Setup the screen.
 * 
 * @access public
 */
void LeXEngine::setupScreen() {

	// OpenGL Specific Options
	if (this->initFlags & LeXInitOpenGL) {
	
		// Added the video mode flags for SDL if necessary
		if (!(this->videoModeFlags & SDL_OPENGL))
			this->videoModeFlags |= SDL_OPENGL;
			
		if (!(this->videoModeFlags & SDL_FULLSCREEN))
			this->videoModeFlags |= SDL_FULLSCREEN;
			
		if (this->videoModeFlags & SDL_HWSURFACE)
			this->videoModeFlags = (~SDL_HWSURFACE);
			
		if (this->videoModeFlags & SDL_SWSURFACE)
			this->videoModeFlags = (~SDL_SWSURFACE);
			
		if (this->videoModeFlags & SDL_DOUBLEBUF)
			this->videoModeFlags = (~SDL_DOUBLEBUF);
	
		// Enable OpenGL Double Buffer
		if (this->initFlags & LeXInitOpenGLDB)
			SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
		
		// At least 5 Bits each for RGBA
		SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 5);
		SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 5);
		SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 5);
		SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 5);	
			
		// Screen Depth (Usually 16bit or 24bit)
		SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, this->bitsPerPixel);
	
	}

	// Setup screen
	this->screen = SDL_SetVideoMode(this->screenWidth, this->screenHeight, this->bitsPerPixel, this->videoModeFlags);
	
	if ((initFlags & LeXInitEVERYTHING) || (initFlags & LeXInitNOCURSOR)) {
	
		// Hide cursor
		SDL_ShowCursor(SDL_DISABLE);
	
	}
	
	if (this->screen == NULL) {
	
		this->logMsg("LeXEngine::setupScreen() - Unable to initialize SDL screen [%s].", SDL_GetError());
	
	}
		
}

/**
 * Sets up OpenGL environment.
 * 
 * @access public
 */
void LeXEngine::setupOpenGL() {

	// Set clear colour to black
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

    // Setup viewport
    glViewport(0, 0, engine->getScreenWidth(), engine->getScreenHeight());

    // Initialize projection matrix
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    // Set up SDL like coordinate system (Point(0,0) is in top left)
    glOrtho(0.0f, engine->getScreenWidth(), engine->getScreenHeight(), 0.0f, -1.0f, 1.0f);

    // Initialize modelview matrix
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // This allows alpha blending
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Account for line/point thingy.
    glTranslatef(0.375f, 0.375f, 0);

    // Enable Smooth Shading
    glShadeModel(GL_SMOOTH);

    // Front faced is clockwise
    glFrontFace(GL_CW);

    // Disable depth testing as we won't use it
    glDisable(GL_DEPTH_TEST);
    
}

/**
 * Is game running?
 * 
 * @access public
 * 
 * @return bool Running?
 */
bool LeXEngine::isGameRunning() {

	return this->isRunning;

}

/**
 * Change isRunning to true. Use before loop.
 * 
 * @access public
 */
void LeXEngine::startGame() {

	this->isRunning = true;

}

/**
 * Change isRunning to false. Use in loop.
 * 
 * @access public
 */
void LeXEngine::quitGame() {

	this->isRunning = false;

}

/**
 * Return the current state.
 * 
 * @access public
 * 
 * @return State* The current state.
 */
State* LeXEngine::getCurrentState() {

	return states.back();

}

/**
 * Takes a pointer to a State as a parameter and then pushes that state onto the vector of pointers to States, before that it uses the clean function to remove the old state from the stack.
 * 
 * @access public
 * 
 * @param State* state The new state.
 */
void LeXEngine::changeState(State* state) {

	// Let current state cleanup
	// and remove it
	if (!states.empty()) {
	
		states.back()->shutdown();
		states.pop_back();
	
	}
	
	// Add new state	
	states.push_back(state);
	
	// Give pointer to state
	states.back()->assignEngine(this);
	
	// Init new state
	states.back()->init();

}
		
/**
 * Pauses the previous state before pushing a new state onto the stack, this state can then be removed and the previous state resumed.
 * Extrememly useful for pausing.
 * 
 * @access public
 * 
 * @param State* The new state.
 */
void LeXEngine::pushState(State* state) {

	// Pause current state
	if (!states.empty())
		states.back()->pause();
	
	// Add new state	
	states.push_back(state);
	
	// Give pointer to state
	states.back()->assignEngine(this);
	
	// Init new state
	states.back()->init();
	
}
		
/**
 * Remove and resume previous state.
 * 
 * @access public
 */
void LeXEngine::popState() {

	// Let current state cleanup
	// and remove it
	if (!states.empty()) {
	
		states.back()->shutdown();
		states.pop_back();
	
	}
	
	// Resume previous state
	if (!states.empty())
		states.back()->resume();

}

/**
 * Process user input.
 * 
 * @access public
 */
void LeXEngine::handleInput() {

	// Let the state handle its own input
	this->states.back()->handleInput();

}
		
/**
 * Perform logic and update state of program.
 * 
 * @access public
 */
void LeXEngine::handleLogic() {

	// Let the state manage its own logic
	this->states.back()->handleLogic();

}
		
/**
 * Draw onto screen.
 * 
 * @access public
 */
void LeXEngine::render() {

	// Let the state render its own objects
	this->states.back()->render();

}

/**
 * Update screen.
 * 
 * @access public
 */
void LeXEngine::updateScreen() {
	// Vertical sync
	sceDisplayWaitVblankStart();
	
	if ((this->initFlags & LeXInitOpenGL)) {
		
		// OpenGL Buffer Flip
		SDL_GL_SwapBuffers();
						
	} else {
	
		// Flip screens if double buffered or UpdateRect otherwise
		SDL_Flip(this->getScreen());
		
	}

}

/**
 * Get screen.
 * 
 * @access public
 * 
 * @return SDL_Surface* The screen.
 */
SDL_Surface* LeXEngine::getScreen() {

	return this->screen;

}

/**
 * Set screen.
 * 
 * @access public
 * 
 * @param SDL_Surface* screen The new screen.
 */
void LeXEngine::setScreen(SDL_Surface* screen) {

	this->screen = screen;

}

/**
 * Get Screen height.
 * 
 * @access public
 * 
 * @return int Screen height.
 */
int LeXEngine::getScreenHeight() {

	return this->screenHeight;

}
		
/**
 * Get Screen width.
 * 
 * @access public
 * 
 * @return int Screen width.
 */
int LeXEngine::getScreenWidth() {

	return this->screenWidth;

}

/**
 * Apply surface.
 * 
 * @access public
 * 
 * @param int x X position.
 * @param int y Y position.
 * @param SDL_Surface source The source surface.
 * @param SDL_Surface destination The destination surface.
 * @param SDL_Rect[Optional] clip Portion or NULL.
 */
void LeXEngine::sdlApplySurface(int x, int y, SDL_Surface* source, SDL_Surface* destination, SDL_Rect* clip) {

	// Holds offsets
	SDL_Rect offset;
	
	// Get offsets
	offset.x = x;
	offset.y = y;
	
	// Blit surface
	SDL_BlitSurface(source, clip, destination, &offset);

}

/**
 * Apply surface directly to screen.
 * 
 * @access public
 * 
 * @param int x X position.
 * @param int y Y position.
 * @param SDL_Surface source The source surface.
 * @param SDL_Rect[Optional] clip Portion or NULL.
 */
void LeXEngine::sdlApplySurfaceToScreen(int x, int y, SDL_Surface* source, SDL_Rect* clip) {

	// Apply
	this->sdlApplySurface(x, y, source, this->getScreen(), clip);

}

/**
 * Scales an SDL Surface to the requested width and height with SDL_gfx.
 * 
 * @access public
 * 
 * @param SDL_Surface src Original surface.
 * @param double newWidth The new width.
 * @param double newHeight The new height.
 * 
 * @return SDL_Surface The scaled surface or NULL if error.
 */
SDL_Surface* LeXEngine::sdlScaleSurface(SDL_Surface *src, double newWidth, double newHeight) {

	if (newWidth == src->w && newHeight == src->h)
		return NULL;
		
	// Calculate scaling factors
	double scaleWidth = (1.0 / src->w) * newWidth;
	double scaleHeight = (1.0 / src->h) * newHeight;
	
	return zoomSurface(src, scaleWidth, scaleHeight, 1);

}

/**
 * Log a message to a log file.
 * 
 * @access public
 * 
 * @param char format Message format to log.
 */
void LeXEngine::logMsg(const char *format, ...) {

	// Holds the log msg
	std::stringstream logMsg;
	
	// Open the file
	SceUID fD = sceIoOpen("LeXLog.txt", PSP_O_WRONLY | PSP_O_APPEND | PSP_O_CREAT, 0777);
	
	// Construct log msg
	logMsg << time(NULL) << ": " << format << "\n";
	
	// Arguments
	va_list options;
	
	// Formatted string
	char buffer[2048];
	
	// Initialize option list
	va_start(options, format);
	
	// Format text
	vsnprintf(buffer, (size_t) sizeof(buffer), logMsg.str().c_str(), options);
	
	// Write the data to file
	sceIoWrite(fD, buffer, strlen(buffer));
	
	// Write the data to console
	printf(buffer);
	
	// Quit option list
	va_end(options);
	
	// Close the file
	sceIoClose(fD);

}
		
/**
 * Popup the standard PSP message dialog.
 * 
 * @access public
 * 
 * @param std::string msg Message to show.
 * @param bool[optional] yesNoOptions Whether to display the yes and no options.
 * 
 * @return pspUtilityMsgDialogParams
 */
pspUtilityMsgDialogParams LeXEngine::showPspMsgDialog(const std::string& msg, bool yesNoOptions) {

	/**
	 * The Message dialog structure.
	 */
	pspUtilityMsgDialogParams dialog;

	// Clear and set memory
	memset(&dialog, 0, sizeof(dialog));

	// Size of structure
    dialog.base.size = sizeof(dialog);
    
    // Prompt language
    // Use PSP settings so Jap PSP will have Jap language, English PSP will have English language
    sceUtilityGetSystemParamInt(PSP_SYSTEMPARAM_ID_INT_LANGUAGE, &dialog.base.language);
    
    // X/O button swap
    // Use PSP settings
    sceUtilityGetSystemParamInt(PSP_SYSTEMPARAM_ID_INT_UNKNOWN, &dialog.base.buttonSwap);

	// Thread priorites
    dialog.base.graphicsThread = 0x11;
    dialog.base.accessThread = 0x13;
    dialog.base.fontThread = 0x12;
    dialog.base.soundThread = 0x10;
	
	// Allocate 32Kb display list
    unsigned int* list = (unsigned int*)memalign(16, 1024 * 32);

    // Setup utility dialog
    dialog.mode = PSP_UTILITY_MSGDIALOG_MODE_TEXT;
    dialog.options = PSP_UTILITY_MSGDIALOG_OPTION_TEXT;

    if (yesNoOptions)
        dialog.options |= PSP_UTILITY_MSGDIALOG_OPTION_YESNO_BUTTONS | PSP_UTILITY_MSGDIALOG_OPTION_DEFAULT_NO;

    strcpy(dialog.message, msg.c_str());

    sceUtilityMsgDialogInitStart(&dialog);

    // Utility loop
    bool done = false;
    
    while (!done) {
    
        /*sceGuStart(GU_DIRECT, list);

		sceGuClearColor(0xff554433);
		sceGuClearDepth(0);
		sceGuClear(GU_COLOR_BUFFER_BIT | GU_DEPTH_BUFFER_BIT);
		
		sceGuFinish();
		sceGuSync(0, 0);*/

        switch(sceUtilityMsgDialogGetStatus()) {
        
        	case PSP_UTILITY_DIALOG_NONE:
			
				break;

			case PSP_UTILITY_DIALOG_VISIBLE:
			
				sceUtilityMsgDialogUpdate(2);
				
				break;

			case PSP_UTILITY_DIALOG_QUIT:
			
				sceUtilityMsgDialogShutdownStart();
				
				break;
				
			case PSP_UTILITY_DIALOG_FINISHED:
			
				done = true;
				
				break;
                
        }

		// Update screen
        sceDisplayWaitVblankStart();
        sceGuSwapBuffers();
        
    }

    // Free display list
    free(list);
    	
	return dialog;

}

/**
 * Popup the standard PSP error dialog.
 * 
 * @access public
 * 
 * @param int errorNum Error number.
 * 
 * @return pspUtilityMsgDialogParams
 */
pspUtilityMsgDialogParams LeXEngine::showPspErrDialog(const unsigned int errorNum) {

	/**
	 * The Message dialog structure.
	 */
	pspUtilityMsgDialogParams dialog;

	// Clear and set memory
	memset(&dialog, 0, sizeof(dialog));

	// Size of structure
    dialog.base.size = sizeof(dialog);
    
    // Prompt language
    // Use PSP settings so Jap PSP will have Jap language, English PSP will have English language
    sceUtilityGetSystemParamInt(PSP_SYSTEMPARAM_ID_INT_LANGUAGE, &dialog.base.language);
    
    // X/O button swap
    // Use PSP settings
    sceUtilityGetSystemParamInt(PSP_SYSTEMPARAM_ID_INT_UNKNOWN, &dialog.base.buttonSwap);

	// Thread priorites
    dialog.base.graphicsThread = 0x11;
    dialog.base.accessThread = 0x13;
    dialog.base.fontThread = 0x12;
    dialog.base.soundThread = 0x10;
	
	// Allocate 32Kb display list
    unsigned int* list = (unsigned int*)memalign(16, 1024 * 32);

    // Setup utility dialog
    dialog.mode = PSP_UTILITY_MSGDIALOG_MODE_ERROR;
	dialog.options = PSP_UTILITY_MSGDIALOG_OPTION_ERROR;
    dialog.errorValue = errorNum;

    sceUtilityMsgDialogInitStart(&dialog);

    // Utility loop
    bool done = false;
    
    while (!done) {
    
        /*sceGuStart(GU_DIRECT, list);

		sceGuClearColor(0xff554433);
		sceGuClearDepth(0);
		sceGuClear(GU_COLOR_BUFFER_BIT | GU_DEPTH_BUFFER_BIT);
		
		sceGuFinish();
		sceGuSync(0, 0);*/

        switch(sceUtilityMsgDialogGetStatus()) {
        
        	case PSP_UTILITY_DIALOG_NONE:
			
				break;

			case PSP_UTILITY_DIALOG_VISIBLE:
			
				sceUtilityMsgDialogUpdate(2);
				
				break;

			case PSP_UTILITY_DIALOG_QUIT:
			
				sceUtilityMsgDialogShutdownStart();
				
				break;
				
			case PSP_UTILITY_DIALOG_FINISHED:
			
				done = true;
				
				break;
                
        }

		// Update screen
        sceDisplayWaitVblankStart();
        sceGuSwapBuffers();
        
    }

    // Free display list
    free(list);
    	
	return dialog;

}

/**
 * Popup the standard PSP net dialog.
 * 
 * @access public * 
 * @return pspUtilityNetconfData
 */
pspUtilityNetconfData LeXEngine::showPspNetDialog() {

	/**
	 * The Net dialog structure.
	 */
	pspUtilityNetconfData dialog;

	// Clear and set memory
	memset(&dialog, 0, sizeof(dialog));

	// Size of structure
    dialog.base.size = sizeof(dialog);
    
    // Prompt language
    // Use PSP settings so Jap PSP will have Jap language, English PSP will have English language
    sceUtilityGetSystemParamInt(PSP_SYSTEMPARAM_ID_INT_LANGUAGE, &dialog.base.language);
    
    // X/O button swap
    // Use PSP settings
    sceUtilityGetSystemParamInt(PSP_SYSTEMPARAM_ID_INT_UNKNOWN, &dialog.base.buttonSwap);

	// Thread priorites
    dialog.base.graphicsThread = 0x11;
    dialog.base.accessThread = 0x13;
    dialog.base.fontThread = 0x12;
    dialog.base.soundThread = 0x10;
	
	// Allocate 32Kb display list
    unsigned int* list = (unsigned int*)memalign(16, 1024 * 32);

    // Setup utility dialog
    dialog.action = PSP_NETCONF_ACTION_CONNECTAP;
        
    struct pspUtilityNetconfAdhoc adhocparam;
	memset(&adhocparam, 0, sizeof(adhocparam));
	dialog.adhocparam = &adhocparam;

	sceUtilityNetconfInitStart(&dialog);

    // Utility loop
    bool done = false;
    
    while (!done) {
    
        /*sceGuStart(GU_DIRECT, list);

		sceGuClearColor(0xff554433);
		sceGuClearDepth(0);
		sceGuClear(GU_COLOR_BUFFER_BIT | GU_DEPTH_BUFFER_BIT);
		
		sceGuFinish();
		sceGuSync(0, 0);*/

        switch(sceUtilityNetconfGetStatus()) {
        
        	case PSP_UTILITY_DIALOG_NONE:
			
				break;

			case PSP_UTILITY_DIALOG_VISIBLE:
			
				sceUtilityNetconfUpdate(2);
				
				break;

			case PSP_UTILITY_DIALOG_QUIT:
			
				sceUtilityNetconfShutdownStart();
				
				break;
				
			case PSP_UTILITY_DIALOG_FINISHED:
			
				done = true;
				
				break;
                
        }

		// Update screen
        sceDisplayWaitVblankStart();
        sceGuSwapBuffers();
        
    }

    // Free display list
    free(list);
	
	return dialog;

}

/**
 * Load a resource from a resource pak. (Basically a zip file)
 * 
 * @public
 * 
 * @param std::string pakFile The Pak file.
 * @param std::string resource Resource file.
 * 
 * @return std::string The resource.
 */
std::string LeXEngine::loadResourceFromPak(const std::string& pakFile, const std::string& resource) {

	// Extensions
	zzip_strings_t ext[] = {"", 0};
	
	// Construct the path to the resource
	std::stringstream path;
	path << pakFile << "/" << resource;
	
	// Open the path
	ZZIP_FILE *file = zzip_open_ext_io(path.str().c_str(), O_RDONLY | O_BINARY, ZZIP_PREFERZIP | ZZIP_CASELESS, ext, 0);
			
	// Will hold the final result
	std::string finalBuffer;
	
	// Temporary buffer
	char *buffer;
	
	// Obtain filesize
	
	// Seek to end
	zzip_seek(file, 0, SEEK_END);
	
	// Get position [so since we're at the end means filesize]
	long filesize = zzip_tell(file);
	
	// Go back to start of the file
	zzip_rewind(file);
	
	// Alloc enough memory for file
	buffer = (char *) malloc(sizeof(char) * filesize);
	
	// Read file
	int read = zzip_fread(buffer, 1, filesize, file);
	
	if (read != filesize) {
	
		this->logMsg("LeXEngine::loadResourceFromPak() - Unable to completely read resource from pack [%s@%s][R: %d - F: %d].", pakFile.c_str(), resource.c_str(), read, filesize);
	
	}
				
	// Alloc to std::string		
	finalBuffer.append(buffer, filesize);
	
	// Close handle
	zzip_fclose(file);
	
	// Free memory
	free(buffer);
		
	// Return contents
	return finalBuffer;

}

/**
 * Load a SDL Surface from an image in memory[RAW].
 * 
 * @access public
 * 
 * @param std::string buffer Data to load.
 * 
 * @return SDL_Surface* SDL_Surface.
 */
SDL_Surface* LeXEngine::sdlSurfaceFromImageRaw(const std::string& buffer) {

	// Surface for the image
	SDL_Surface* loadedImage = NULL;
	
	// Optimized image to use
	SDL_Surface* optimizedImage = NULL;
	
	SDL_RWops *rw = SDL_RWFromConstMem((void*)buffer.c_str(), buffer.size());
		
	// Load from MEM
	loadedImage = IMG_Load_RW(rw, 1);
				
	if (!loadedImage) {
		
		this->logMsg("LeXEngine::sdlSurfaceFromImageRaw() - Unable to load data from mem [%s].", IMG_GetError());
		
		return NULL;
		
	} else {
	
		// Optimize image
		optimizedImage = SDL_DisplayFormatAlpha(loadedImage);
		
		// Free up old image
		SDL_FreeSurface(loadedImage);
	
	}
		
	// Return optimized image
	return optimizedImage;

}

/**
 * Load a SDL Surface from an image in a file.
 * 
 * @access public
 * 
 * @param std::string file File to load.
 * 
 * @return SDL_Surface* SDL_Surface.
 */
SDL_Surface* LeXEngine::sdlSurfaceFromImageFile(const std::string& file) {

	// Surface for the image
	SDL_Surface* loadedImage = NULL;
	
	// Optimized image to use
	SDL_Surface* optimizedImage = NULL;

	// Load from file
	loadedImage = IMG_Load(file.c_str());
		
	if (!loadedImage) {
		
		this->logMsg("LeXEngine::sdlSurfaceFromImageFile() - Unable to load data [file:%s] [%s].", file.c_str(), IMG_GetError());
		
		return NULL;
		
	} else {
	
		// Optimize image
		optimizedImage = SDL_DisplayFormatAlpha(loadedImage);
		
		// Free up old image
		SDL_FreeSurface(loadedImage);
	
	}
		
	// Return optimized image
	return optimizedImage;
		
}

/**
 * Load a SDL Surface from an image in a pak file.
 * 
 * @access public
 * 
 * @param std::string pakFile The Pak file.
 * @param std::string imageFile Image file.
 * 
 * @return SDL_Surface* SDL_Surface.
 */
SDL_Surface* LeXEngine::sdlSurfaceFromPakFile(const std::string& pakFile, const std::string& imageFile) {

	// Construct the path to the resource
	std::stringstream path;
	path << pakFile << "/" << imageFile;

	// Surface for the image
	SDL_Surface* loadedImage = NULL;
	
	// Optimized image to use
	SDL_Surface* optimizedImage = NULL;

	SDL_RWops* rw = SDL_RWFromZZIP(path.str().c_str());
	
	if (rw == 0) {
	
		this->logMsg("LeXEngine::sdlSurfaceFromPafFile() - Unable to create SDL_RWops from Zip.");
		return NULL;
		
	}

	// Load from pak file
	loadedImage = IMG_Load_RW(rw, 1);
		
	if (!loadedImage) {
		
		this->logMsg("LeXEngine::sdlSurfaceFromPakFile() - Unable to load data [pak:%s][file:%s] [%s].", pakFile.c_str(), imageFile.c_str(), IMG_GetError());
		
		return NULL;
		
	} else {
	
		// Optimize image
		optimizedImage = SDL_DisplayFormatAlpha(loadedImage);
		
		// Free up old image
		SDL_FreeSurface(loadedImage);
	
	}
		
	// Return optimized image
	return optimizedImage;
		
}

/**
 * Load an OpenGL Texture from SDL Surface.
 * 
 * @access public
 * 
 * @param SDL_Surface* surface The SDL Surface.
 * @param SDL_Rect*[optional] clip The portion to use.
 * @param bool smooth Use GL_LINEAR.
 * 
 * @return OMTexture The texture + extra info.
 */
OMTexture LeXEngine::glTextureFromSDLSurface(SDL_Surface *surface, SDL_Rect *clip, bool smooth) {

	// Custom texture struct
	OMTexture texture;
	
	// Add info
	texture.texture = 0;
	
	// Temp vars
	int width = 1, height = 1, w = 0, h = 0, x = 0, y = 0;
	SDL_Surface *image;
	Uint32 savedFlags;
	Uint8 savedAlpha;
	
	// Check if custom sizes were passed
	if (clip != NULL) {
	
		w = clip->w;
		h = clip->h;
		x = clip->x;
		y = clip->y;
	
	} else {
		
		w = surface->w;
		h = surface->h;
	
	}
	
	// Get next power of two for width and height
	while (width < w)
		width <<= 1;
		
	while (height < h)
		height <<= 1;
			
	// Calculate texture coordinates
	texture.minX = 0.0;								// Min X
	texture.minY = 0.0;								// Min Y
	texture.maxX = (GLfloat)w / width;				// Max X
	texture.maxY = (GLfloat)h / height;				// Max Y
	texture.w	 = w;								// Original width
	texture.h	 = h;								// Original height
	texture.pw	 = width;							// Power of 2 width
	texture.ph	 = height;							// Power of 2 height
	
	// Create new surface [ Little Endian RGBA Masks for PSP ]
	image = SDL_CreateRGBSurface(SDL_SWSURFACE, width, height, 32, 0x000000FF, 0x0000FF00, 0x00FF0000, 0xFF000000);
	
	// Error!
	if (image == NULL)	{
	
		this->logMsg("LeXEngine::glTextureFromSDLSurface: Error creating new RGB Surface [%s].", SDL_GetError());
		
		return texture;
			
	}
		
	// Save alpha blending attributes
	savedFlags = surface->flags & (SDL_SRCALPHA | SDL_RLEACCELOK);
	savedAlpha = surface->format->alpha;
	
	if ((savedFlags & SDL_SRCALPHA) == SDL_SRCALPHA)
		SDL_SetAlpha(surface, 0, 0);

	SDL_Rect area = {x, y, w, h};
	SDL_BlitSurface(surface, &area, image, NULL);
	
	// Restore the alpha blending attributes
	if ((savedFlags & SDL_SRCALPHA) == SDL_SRCALPHA)
		SDL_SetAlpha(surface, savedFlags, savedAlpha);
		
	// Create an OpenGL texture for the image
	glGenTextures(1, &texture.texture);
		
	// Bind texture
	glBindTexture(GL_TEXTURE_2D, texture.texture);
	
	if (smooth) {
	
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		
	} else {
	
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	
	}
	
	// Copy data to texture
	glTexImage2D(GL_TEXTURE_2D,
			0,
			GL_RGBA,
			width, height,
			0,
			GL_RGBA,
			GL_UNSIGNED_BYTE,
			image->pixels
	);
		
	// Free mem
	SDL_FreeSurface(image);
	
	return texture;

}

/**
 * Render an OpenGL texture at desired coordinates.
 * 
 * @access public
 * 
 * @param int x X coordinate.
 * @param int y Y coordinate.
 * @param OMTexture texture The texture to render.
 * @param bool smooth Use GL_LINEAR.
 */
void LeXEngine::renderGlTexture(int x, int y, OMTexture texture, bool smooth) {

	if (glIsTexture(texture.texture) == GL_FALSE) {
	
		this->logMsg("LeXEngine::renderGlTexture: Invalid texture passed. To be rendered at (%d, %d).", x, y);
		return;
	
	}
	
	// Enable 2D Textures
	glEnable(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, texture.texture);
	
	if (smooth) {
	
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	
	}
	
	glBegin(GL_QUADS);
        
		glTexCoord2f(texture.minX, texture.minY); glVertex2i(x, y);
		glTexCoord2f(texture.maxX, texture.minY); glVertex2i(x + texture.w, y);      
		glTexCoord2f(texture.maxX, texture.maxY); glVertex2i(x + texture.w, y + texture.h);
		glTexCoord2f(texture.minX, texture.maxY); glVertex2i(x, y + texture.h);
        
	glEnd();
	
	// Disable 2D Textures
	glDisable(GL_TEXTURE_2D);

}

/**
 * Open from memory[RAW].
 * 
 * @access public
 * 
 * @param std::string buffer Data to load.
 * @oaram int size Font point size.
 * 
 * @return TTF_Font True Type Font.
 */
TTF_Font* LeXEngine::ttfFontFromRaw(const std::string& buffer, int size) {

	// Font to load
	TTF_Font *loadedFont = NULL;
	
	SDL_RWops *rw = SDL_RWFromConstMem((void*)buffer.c_str(), buffer.size());
	
	// Load font
	loadedFont = TTF_OpenFontRW(rw, 1, size);
	
	if (!loadedFont) {
	
		this->logMsg("LeXEngine::ttfFontFromRaw() - Unable to load data from mem [%s].", TTF_GetError());
		
		return NULL;
	
	}

	return loadedFont;

}

/**
 * Open from font in pak file.
 * 
 * @access public
 * 
 * @param std::string pakFile The Pak file.
 * @param std::string fontFile Font file.
 * @oaram int size Font point size.
 * 
 * @return TTF_Font True Type Font.
 */
TTF_Font* LeXEngine::ttfFontFromPak(const std::string& pakFile, const std::string& fontFile, int size) {

	// Font to load
	TTF_Font *loadedFont = NULL;
	
	// Construct the path to the resource
	std::stringstream path;
	path << pakFile << "/" << fontFile;
	
	SDL_RWops *rw = SDL_RWFromZZIP(path.str().c_str());
	
	if (rw == 0) {
	
		this->logMsg("LeXEngine::ttfFontFrompak() - Unable to get SDL_RWops [%s@%s].", pakFile.c_str(), fontFile.c_str());
	
	}	
	
	// Load font
	loadedFont = TTF_OpenFontRW(rw, 1, size);
	
	if (!loadedFont) {
	
		this->logMsg("LeXEngine::ttfFontFromPak() - Unable to load font from pak file [%s@%s][%s].", pakFile.c_str(), fontFile.c_str(), TTF_GetError());
		
		return NULL;
	
	}

	return loadedFont;

}

/**
 * Get an OpenGL texture made from the text.
 * 
 * @access public
 * 
 * @param const char* text Text.
 * @param TTF_Font* font The font to use.
 * @param SDL_Color textColour Text colour.
 * 
 * @return OMTexture The texture.
 */
OMTexture LeXEngine::ttfTexture(const char* text, TTF_Font *font, SDL_Color textColour) {

	OMTexture texture;

	// Invalid font test
	if (font == NULL) {
	
		this->logMsg("LeXEngine::ttfTexture: Invalid font passed to function.");
		
		return texture;
	
	}
	
	// Empty text test
	if (strlen(text) == 0) {
	
		this->logMsg("LeXEngine::ttfTexture: No text passed to function.");
		
		return texture;
	
	}

	// Make SDL Surface
	SDL_Surface *textSurface = TTF_RenderUTF8_Blended(font, text, textColour);
	
	// Error check
	if (textSurface == NULL) {
	
		this->logMsg("LeXEngine::ttfTexture: Unable to create SDL Surface [%s][%s].", text, TTF_GetError());
		
		return texture;
	
	}
	
	// Make Texture
	texture = this->glTextureFromSDLSurface(textSurface, NULL);
	
	// Free resources
	SDL_FreeSurface(textSurface);
	
	// Error check
	if (!texture.texture) {
	
		engine->logMsg("LeXEngine::ttfTexture: Error loading OpenGL texture from SDL_Surface.");
	
		return texture;
		
	}
	
	// No error :)
	return texture;

}

/**
 * Get jostick number {which}.
 * 
 * @access public
 * 
 * @param int which Which Joystick to return.
 * 
 * @return SDL_Joystick Joystick.
 */
SDL_Joystick* LeXEngine::getJoystick(int which) {

	if ((this->initFlags & LeXInitEVERYTHING) || (this->initFlags & LeXInitJOYSTICK)) {
	
		if ((unsigned)which >= this->joysticks.size())
			return NULL;
		else
			return this->joysticks[which];
		
	} else
		return NULL;

}

/**
 * Play backgound music.
 * 
 * @access public
 * 
 * @param const std::string& file Music file.
 * @param bool loop Loop music?
 */
void LeXEngine::playBGMusic(const std::string& file, bool loop) {

	this->cleanUpMusic();

	// Set loop
	pgeMp3Loop(loop);
	
	if (!pgeMp3Play(file.c_str()))
		this->logMsg("LeXEngine::playBGMusic: Unable to play/open mp3 file [%s].", file.c_str());

}

/**
 * Pause background music.
 * 
 * @access public
 */
void LeXEngine::pauseBGMusic() {

	if (pgeMp3IsPlaying())
		pgeMp3Pause();	
	else
		this->logMsg("LeXEngine::pauseBGMusic: WARN, tried to pause BG music when music isn't playing.");

}

/**
 * Stop background music from playing.
 * 
 * @access public
 */
void LeXEngine::stopBGMusic() {

	if (pgeMp3IsPlaying())
		pgeMp3Stop();
	else
		this->logMsg("LeXEngine::stopBGMusic: WARN, tried to stop BG music when music isn't playing.");

}

/**
 * Clean up music stuff.
 * 
 * @access private
 */
void LeXEngine::cleanUpMusic() {

	// Stop BG Music
	this->stopBGMusic();
				
}

/**
 * Get current time.
 * 
 * @access public
 * 
 * @return std::string Time in 24 hr format. Hour:Minute
 */
std::string LeXEngine::getTime() {

	pspTime time;

	// Get psp time
	sceRtcGetCurrentClockLocalTime(&time);
	
	char buffer[10];
	
	sprintf(buffer, "%02d:%02d", time.hour, time.minutes);
	
	return std::string(buffer);

}

/**
 * Get current date and time.
 * 
 * @access public
 * 
 * @return std::string DateTime
 */
std::string LeXEngine::getDateTime() {

	pspTime time;

	// Get psp time
	sceRtcGetCurrentClockLocalTime(&time);
	
	char buffer[100];
	
	sprintf(buffer, "%02d/%02d/%d @ %02d:%02d", time.day, time.month, time.year, time.hour, time.minutes);
	
	return std::string(buffer);

}

#endif /* _LeXEngine_CPP */
