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
 * Net stuff.
 * 
 * @package xMangaPSP
 */

#ifndef _Net_CPP
#define _Net_CPP

// BEGIN Includes
#include "xM/Net/Net.h"

#include <pspnet.h>
#include <pspnet_apctl.h>
#include <pspnet_inet.h>
#include <psputility.h>
// END Includes

namespace xM {

	namespace Net {
			
	    /**
	     * Init the network modules.
	     */
		void init() {
		
		    // Load the right modules
		    sceUtilityLoadNetModule(PSP_NET_MODULE_COMMON);
            sceUtilityLoadNetModule(PSP_NET_MODULE_INET);

            // mem, thrd priority, stack, thread priority, stack
            sceNetInit(128 * 1024, 42, 4 * 1024, 42, 4 * 1024);
            sceNetInetInit();
            sceNetApctlInit(0x8000, 48);
		
		}
		
		/**
		 * Checks whether PSP is connected to AP.
		 * From Criptych @ ps2dev forums.
		 * 
		 * @return bool Connection state.
		 */
		bool isConnected() {
		
		    int state;
		    
		    if (sceNetApctlGetState(&state) < 0)
		        return false;
		        
		    return state == PSP_NET_APCTL_STATE_GOT_IP;
		
		}
		
		/**
		 * Shutdown the network modules.
		 */
		void shutdown() {
		
		    sceNetApctlTerm();
		    sceNetInetTerm();
		    sceNetTerm();
		
		    sceUtilityUnloadNetModule(PSP_NET_MODULE_INET);
		    sceUtilityUnloadNetModule(PSP_NET_MODULE_COMMON);
		
		}
			
	}

}

#endif /* _Net_CPP */
