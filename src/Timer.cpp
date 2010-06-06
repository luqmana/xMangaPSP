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
 * Timer class utility.
 * 
 * @package xMangaPSP
 */

#ifndef _TIMER_CPP
#define _TIMER_CPP

// BEGIN Includes
#include "xM/Util/Timer.h"
// END Includes

namespace xM {

	namespace Util {
	
		Timer::Timer(void) {
		
			sceRtcGetCurrentTick(&timeLast);
			tickResolution = sceRtcGetTickResolution();
		
		}
		
		/**
		 * Get the difference.
		 * 
		 * @access public
		 * 
		 * @return double
		 */
		double Timer::getDeltaTime(void) {
		
			sceRtcGetCurrentTick(&timeNow);
			double deltaTime = (timeNow - timeLast) / (float)tickResolution;
			timeLast = timeNow;
			
			return deltaTime;
		
		}
	
	}

}

#endif /* _TIMER_CPP */
