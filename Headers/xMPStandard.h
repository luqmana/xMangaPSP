/*
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
 * Full License: LICENSE.txt
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

/**
 * Header for standard stuff.
 * 
 * @package xMangaPSP
 */

#ifndef _xMPStandard_H
#define _xMPStandard_H

// BEGIN Includes
#include <pspdebug.h>
#include <pspkernel.h>
#include <pspdisplay.h>
#include <string>
#include <string.h>
#include <vector>
#include <sstream>
#include <map>
#include <SDL/SDL_thread.h>
// END Includes

// BEGIN Definitions

// Useful macros
#define MIN(a, b)  	(((a) < (b)) ? (a) : (b))
#define MAX(a, b)  	(((a) > (b)) ? (a) : (b))
#define ABS(a)	   	(((a) < 0) ? -(a) : (a))

// Screen related stuff
#define BUF_WIDTH	(512)
#define SCR_WIDTH	(480)
#define SCR_HEIGHT	(272)
#define PIXEL_SIZE	(4)
#define FRAME_SIZE	(BUF_WIDTH * SCR_HEIGHT * PIXEL_SIZE)
#define ZBUF_SIZE	(BUF_WIDTH * SCR_HEIGHT * 2)

// PSP Button -> SDL Joystick Mapping
#define PSP_BUTTON_TRIANGLE 0
#define PSP_BUTTON_CIRCLE	1
#define PSP_BUTTON_CROSS	2
#define PSP_BUTTON_SQUARE	3
#define PSP_BUTTON_LTRIGGER	4
#define PSP_BUTTON_RTRIGGER	5
#define PSP_BUTTON_DOWN		6
#define PSP_BUTTON_LEFT		7
#define PSP_BUTTON_UP		8
#define PSP_BUTTON_RIGHT	9
#define PSP_BUTTON_SELECT	10
#define PSP_BUTTON_START	11
#define PSP_BUTTON_HOME		12
#define PSP_BUTTON_HOLD		13

// Makes typing it easier
#define pspPrintf 	pspDebugScreenPrintf
// END Definitions

// BEGIN Typedefs

// String Array created using a map
typedef std::map <std::string, std::string> xMStrArray;

// END Typedefs

/**
 * Tokenize string.
 * 
 * @param const string& str Original string.
 * @param std::vector<std::string>& tokens Tokens.
 * @param const std::string& delimiters Delimiter.
 */
void tokenize(const std::string& str, std::vector<std::string>& tokens, const std::string& delimiters);

/**
 * Map button index to name.
 */
std::string getButtonName(Uint8 button);

/**
 * The definition for the function which sets up the standard callbacks.
 */
void setupStandardCallbacks();

/**
 * Call to get repeated events for joystick.
 * 
 * @param int button Button to repeat.
 */
void repeatJoystick(int button);

/**
 * Creates a thread to handle repeat joystick events.
 */
void startJoystickRepeatThread();

/**
 * Stop joystick repeat thread.
 */
void stopJoystickRepeatThread();

#endif /* _xMPStandard_H */
