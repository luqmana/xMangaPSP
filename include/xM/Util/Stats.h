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
 * Stat utilities.
 * Header file.
 * 
 * @package xMangaPSP
 */

#ifndef _STATS_H
#define _STATS_H

// BEGIN Includes
#include "xM/Util/Timer.h"

#include <stdio.h>
#include <psptypes.h>
#include <psprtc.h>
#include <pspdebug.h>
#include <string>
// END Includes

namespace xM {

	namespace Util {
	
		/**
		 * Frames-per-second
		 */
		extern int fps;
		
		/**
		 * FPS info string.
		 */
		extern std::string fpsDisplay;
		
		// Used to calculate FPS
		extern xM::Util::Timer fpsTimer;
		
		/**
		 * Displays the FPS.
		 */
		void FPS(void);
		
		/**
		 * Displays free memory.
		 */
		void MEM(void);
			
	}

}

#endif /* _STATS_H */
