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
 * Header file.
 * 
 * @package xMangaPSP
 */

#ifndef _TIMER_H
#define _TIMER_H

// BEGIN Includes
#include <stdio.h>
#include <stdlib.h>
#include <psptypes.h>
#include <psprtc.h>
// END Includes

namespace xM {

	namespace Util {
	
		class Timer {
		
			private:
			
				double deltaTime;
				
				u64 timeNow;
				u64 timeLast;
				u32 tickResolution;
				
			public:
			
				Timer(void);
				
				/**
				 * Get the difference.
				 * 
				 * @access public
				 * 
				 * @return double
				 */
				double getDeltaTime(void);
				
		};
			
	}
		
}


#endif /* _TIMER_H */
