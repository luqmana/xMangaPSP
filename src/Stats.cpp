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
 * Stat utilities.
 * 
 * @package xMangaPSP
 */

#ifndef _STATS_CPP
#define _STATS_CPP

// BEGIN Includes
#include "xM/Gfx/Graphics.h"
#include "xM/Util/Stats.h"
#include "xM/Util/Utils.h"

#include <string>
#include <pspsysmem.h>
// END Includes

namespace xM {

	namespace Util {
	
		/**
		 * Frames-per-second
		 */
		int fps = 0;
		
		/**
		 * FPS info string.
		 */
		std::string fpsDisplay;
		
		// Used to calculate FPS
		xM::Util::Timer fpsTimer;		
		
		/**
		 * Displays the FPS.
		 */
		void FPS(void) {
		
			if (!fpsTimer.isStarted())
				fpsTimer.start();
								
			// Increment
			fps++;
			
			if (fpsTimer.getDeltaTicks() >= 1.0f) {
			
				fpsTimer.start();
				fpsDisplay.assign("FPS: ");
				fpsDisplay.append(xM::Util::toString(fps));
				fps = 0;
							
			}
			
			pspDebugScreenSetOffset((int)xM::Gfx::frameBuffer0);
			pspDebugScreenSetXY(0, 0);
			pspPrintf(fpsDisplay.c_str());
		
		}
		
		/**
		 * Displays free memory.
		 */
		void MEM(void) {
		
			pspDebugScreenSetOffset((int)xM::Gfx::frameBuffer0);
			pspDebugScreenSetXY(0, 0);
			pspPrintf("\nFree Mem: %d (%d)", sceKernelTotalFreeMemSize() / 1048576, sceKernelTotalFreeMemSize());
		
		}
			
	}

}

#endif /* _STATS_CPP */
