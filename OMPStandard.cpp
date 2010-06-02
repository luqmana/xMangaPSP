/*
 * $Id: OMPStandard.cpp 84 2009-12-31 03:42:09Z chaotic@luqmanrocks.co.cc $
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
 * Standard stuff.
 * 
 * @package OneMangaPSP
 */

#ifndef _OMPStandard_CPP
#define _OMPStandard_CPP

// BEGIN Includes
#include "OMPStandard.h"
#include "OneMangaPSP.h"
// END Includes

SDL_Thread *joystickRepeatThread = NULL;
int joystickRepeatButton = -1;
int oldJoystickRepeatButton = -1;

/**
 * Tokenize string.
 * 
 * @param const string& str Original string.
 * @param std::vector<std::string>& tokens Tokens.
 * @param const std::string& delimiters Delimiter.
 */
void tokenize(const std::string& str, std::vector<std::string>& tokens, const std::string& delimiters) {

    // Skip delimiters at beginning.
    std::string::size_type lastPos = str.find_first_not_of(delimiters, 0);
    
    // Find first "non-delimiter".
    std::string::size_type pos = str.find_first_of(delimiters, lastPos);

    while (std::string::npos != pos || std::string::npos != lastPos) {
    
        // Found a token, add it to the vector.
        tokens.push_back(str.substr(lastPos, pos - lastPos));
        
        // Skip delimiters.  Note the "not_of"
        lastPos = str.find_first_not_of(delimiters, pos);
        
        // Find next "non-delimiter"
        pos = str.find_first_of(delimiters, lastPos);
        
    }
    
}

/**
 * Map button index to name.
 */
std::string getButtonName(Uint8 button) {

	switch (button) {
	
		case PSP_BUTTON_TRIANGLE:
		
			return "Triangle";
			
			break;
			
		case PSP_BUTTON_CIRCLE:
		
			return "Circle";
			
			break;
			
		case PSP_BUTTON_CROSS:
		
			return "Cross";
			
			break;
			
		case PSP_BUTTON_SQUARE:
		
			return "Square";
			
			break;
			
		case PSP_BUTTON_LTRIGGER:
		
			return "Left Trigger";
			
			break;
			
		case PSP_BUTTON_RTRIGGER:
		
			return "Right Trigger";
			
			break;
			
		case PSP_BUTTON_DOWN:
			
			return "Down";
			
			break;
			
		case PSP_BUTTON_LEFT:
		
			return "Left";
			
			break;
			
		case PSP_BUTTON_UP:
		
			return "Up";
			
			break;
			
		case PSP_BUTTON_RIGHT:
		
			return "Right";
			
			break;
			
		case PSP_BUTTON_SELECT:
		
			return "Select";
			
			break;
			
		case PSP_BUTTON_START:
		
			return "Start";
			
			break;
			
		case PSP_BUTTON_HOME:
		
			return "Home";
			
			break;
			
		case PSP_BUTTON_HOLD:
		
			return "Hold";
			
			break;
	
	}
	
	return "Unknown";

}

/**
 * The standard exit callback.
 *
 * @return int
 */
int standardExitCallback(int arg1, int arg2, void *common) {

	// Run exit flag
	engine->quitGame();
                
	return 0;
        
}

/**
 * The standard callback thread.
 */
int standardCallbackThreadHandler(SceSize args, void *argp) {

	// Standard exit callback id.
	int callbackId;
    
    // Create the callback    
	callbackId = sceKernelCreateCallback("Standard Exit Callback", standardExitCallback, NULL);
	
	// Register the callback
	sceKernelRegisterExitCallback(callbackId);

	// Let the thread sleep
	sceKernelSleepThreadCB();
        
	return 0;

}

/**
 * Creates the standard update thread which handles exiting via home button.
 */
void setupStandardCallbacks() {

	// Standard Update Thread Id
	int threadId;
	
	// Create the Standard Update Thread
	threadId = sceKernelCreateThread("Standard Update Thread", standardCallbackThreadHandler, 0x11, 0xFA0, 0, 0);
	
	// if no error start thread
	if (threadId >= 0)
		sceKernelStartThread(threadId, 0, 0);

}

/**
 * Call to get repeated events for joystick.
 * 
 * @param int button Button to repeat.
 */
void repeatJoystick(int button) {

	joystickRepeatButton = button;

}

/**
 * Joystick repeat handler.
 * Fires SDL_JOYBUTTONDOWN if key is held.
 */
int joystickRepeatHandler(void*) {

	// Loop forever		
	while (true) {
		
		// Only do repeat events if a new button was pressed
		if (joystickRepeatButton != oldJoystickRepeatButton) {
		
			// Wait 650ms to see if we are to fire continous events
			SDL_Delay(650);
			
			// Check button state
			// @TODO: Handle multiple joysticks
			if (SDL_JoystickGetButton(engine->getJoystick(0), joystickRepeatButton)) {
			
				// Button still being held, so we start continous events
				while (true) {
				
					// If button is let go, then stop and send correct event
					// @TODO: Handle multiple joysticks
					if (!SDL_JoystickGetButton(engine->getJoystick(0), joystickRepeatButton)) {
					
						SDL_Event evt;
		
						evt.type = SDL_JOYBUTTONUP;
						evt.jbutton.type = SDL_JOYBUTTONUP;
						evt.jbutton.which = 0; // @TODO: Handle multiple joysticks
						evt.jbutton.button = joystickRepeatButton;
						evt.jbutton.state = SDL_RELEASED;
	
						// Put event on to event quene
						FE_PushEvent(&evt);
					
						break;
						
					}
				
					SDL_Event evt;
		
					evt.type = SDL_JOYBUTTONDOWN;
					evt.jbutton.type = SDL_JOYBUTTONDOWN;
					evt.jbutton.which = 0; // @TODO: Handle multiple joysticks
					evt.jbutton.button = joystickRepeatButton;
					evt.jbutton.state = SDL_PRESSED;
	
					// Put event on to event quene
					FE_PushEvent(&evt);
				
					// Wait 90ms in between events
					SDL_Delay(90);
				
				}
			
			}
		
		}
				
		// Reset old variable
		oldJoystickRepeatButton = joystickRepeatButton;
	
		// Wait 100ms till we start checking for continous events again
		SDL_Delay(100);
	
	}
	
	return 0;

}

/**
 * Creates a thread to handle repeat joystick events.
 */
void startJoystickRepeatThread() {

	joystickRepeatThread = SDL_CreateThread(joystickRepeatHandler, NULL);
	
	if (joystickRepeatThread == NULL)
		engine->logMsg("Unable to create Joystick Button Repeat thread [%s].", SDL_GetError());

}

/**
 * Stop joystick repeat thread.
 */
void stopJoystickRepeatThread() {

	SDL_KillThread(joystickRepeatThread);
	
}

#endif /* _OMPStandard_CPP */
