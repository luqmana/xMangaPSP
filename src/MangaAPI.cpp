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
 * MAP Co-ordinator
 * Header file.
 * 
 * @package xMangaPSP
 */

#ifndef _MangaAPI_CPP
#define _MangaAPI_CPP

// BEGIN Includes
#include "xM/Manga/MangaAPI.h"
#include "xM/Net/Net.h"
#include "xM/Util/Log.h"
// END Includes

namespace xM {

	namespace Manga {
	
	    SceUID mangaAPIThreadID = -1;
	    SceUID mangaAPIMbx;
				    
	    /**
	     * The manga api thread function.
	     */
	    int mangaAPIThread(SceSize args, void *argp) {
	    	    
            APIMessage* msg = NULL;
            APIMessage* sMsg = NULL;
            std::string* sRes = NULL;
	    	    
	        while (true) {
	        	            	            	            
	            int res = sceKernelReceiveMbxCB(mangaAPIMbx, (void**)&msg, NULL);
	            
	            if (!(res <  0) && msg != NULL) {
	            	                
	                if (sMsg != NULL) {
	                
	                    delete sMsg;
	                    sMsg = NULL;
	                    
	                }
	                
	                if (sRes != NULL) {
	                
	                    delete sRes;
	                    sRes = NULL;
	                
	                }
	                	            	            	                
	                std::string response;
	                	                
	                if (Net::downloadFile(*msg->text, response)) {
	                	                	                	         
                        sRes = new std::string(response);
	                	                	                    
	                    SceKernelMsgPacket hdr = {0};
            
                        sMsg = new APIMessage;
                        sMsg->header = hdr;
                        sMsg->text = sRes;
	                    sceKernelSendMbx(*msg->returnBox, (void*)sMsg);
	                    
	                } else
	                    Util::logMsg("Can't download [%s].", response.c_str());
	                
	            }
	        	        
	        }
	        
	        if (sMsg != NULL) {
            
                delete sMsg;
                sMsg = NULL;
                
            }
            
            if (sRes != NULL) {
            
                delete sRes;
                sRes = NULL;
            
            }
	        
	        return 0;
	    
	    }
	    
	    /**
		 * Setups the manga API handler thread.
		 */
	    void initMangaAPIThread() {
	    
	        // create message box
	        mangaAPIMbx = sceKernelCreateMbx("MangaAPIBox", 0, NULL);
	        	    
	        // create the thread
	        mangaAPIThreadID = sceKernelCreateThread("MangaAPIThread", mangaAPIThread, 0x22, 0x8000, PSP_THREAD_ATTR_USER | PSP_THREAD_ATTR_USBWLAN, NULL);
	        
	        // start it
	        if (mangaAPIThreadID > 0)
	            sceKernelStartThread(mangaAPIThreadID, 0, NULL);
	    
	    }
	    
	    /**
	     * Shuts down the manga API handler thread.
	     */
	    void shutdownMangaAPIThread() {
	    	    
	        if (mangaAPIThreadID > 0)
	            sceKernelDeleteThread(mangaAPIThreadID);
	            	    
	    }
			
	}

}

#endif /* _MangaAPI_CPP */
