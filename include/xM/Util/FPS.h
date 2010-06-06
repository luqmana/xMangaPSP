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
 * Header file.
 * 
 * @package xMangaPSP
 */

#ifndef _FPS_H
#define _FPS_H

// BEGIN Includes
#include <stdio.h>
#include <psptypes.h>
#include <psprtc.h>
#include <pspdebug.h>
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
		extern char fpsDisplay[100];
		
		// Used to calculate FPS
		extern u32 tickResolution;
		extern u64 fpsTicksNow;
		extern u64 fpsTicksLast;
		
		/**
		 * Displays the FPS.
		 */
		void FPS(void);
			
	}

}

#endif /* _FPS_H */
