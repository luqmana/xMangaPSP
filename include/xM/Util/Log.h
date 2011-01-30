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
 * Logging utility.
 * Header file.
 * 
 * @package xMangaPSP
 */

#ifndef _LOG_H
#define _LOG_H

// BEGIN Includes

// END Includes

// BEGIN Defines

// END Defines

namespace xM {

	namespace Util {
	
		/**
         * Log a message to a log file.
         * 
         * @access public
         * 
         * @param char* format Message format to log.
         */
        void logMsg(const char* format, ...);
			
	}

}

#endif /* _LOG_H */
