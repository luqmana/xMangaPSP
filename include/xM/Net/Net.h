/**
 * This file is part of the xMangaPSP application.
 *
 * Copyright (C) Luqman Aden <www.luqman.ca>.
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
 * Net stuff.
 * Header file.
 * 
 * @package xMangaPSP
 */

#ifndef _Net_H
#define _Net_H

// BEGIN Includes
#include <string>
// END Includes

namespace xM {

	namespace Net {
			
	    /**
	     * Init the network modules.
	     */
		void init();
		
		/**
		 * Checks whether PSP is connected to AP.
		 * From Criptych @ ps2dev forums.
		 * 
		 * @return bool Connection state.
		 */
		bool isConnected();
		
		/**
         * Download a file using cURL.
         * 
         * @param const std::string& url The url to download from.
         * @param std::string &response Pointer to string object to populate with response. (Which can also be error message.)
         * 
         * @return bool Success
         */ 
        bool downloadFile(const std::string& url, std::string &response);
		
		/**
		 * Shutdown the network modules.
		 */
		void shutdown();
			
	}

}

#endif /* _Net_H */
