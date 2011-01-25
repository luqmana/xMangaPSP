/**
 * This file is part of the xMangaPSP application.
 *
 * Copyright (C) luqman Aden <www.luqmanrocks.co.cc>.
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
 * Sets up callbacks on the PSP.
 * Header file.
 * 
 * @package xMangaPSP
 */

#ifndef _Callbacks_H
#define _Callbacks_H

// BEGIN Includes
#include <pspkernel.h>
// END Includes

namespace xM {

	namespace Stn {
			
		/**
		 * Sets up the standard callback handling thread.
		 */
		void setupCallbacks(void);
			
	}

}

#endif /* _Callbacks_H */
