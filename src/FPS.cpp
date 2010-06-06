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
 * FPS calc utility.
 * sceRtcGetCurrentTick( &fpsTickNow );

 * @package xMangaPSP
 */

#ifndef _FPS_CPP
#define _FPS_CPP

// BEGIN Includes
#include "xM/Gfx/Graphics.h"
#include "xM/Util/FPS.h"
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
		char fpsDisplay[100];
		
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
				sprintf(fpsDisplay, "FPS: %d", fps);
				fps = 0;
							
			}
			
			pspDebugScreenSetOffset((int)xM::Gfx::frameBuffer0);
			pspDebugScreenSetXY(0, 0);
			pspDebugScreenPrintf(fpsDisplay);
		
		}
			
	}

}

#endif /* _FPS_CPP */
