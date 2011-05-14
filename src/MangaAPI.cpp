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
	    MAP* mapImp = NULL;
				    
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
	                
	                switch (msg->type) {
	                
	                	case SetEndpoint:
	                	
	                		if (msg->what != NULL) {
	                		
	                			mapImp->setEndpoint(*(std::string*)msg->what);
	                			delete (std::string*)msg->what;
	                			
	                		}
	                	
	                		break;
	                		
	                	case RequestMangaList:
	                	
	                		if (mapImp->loadMangaList()) {
	                		
	                			printf("Manga list loaded.\n");
	                			
	                			sMsg = new APIMessage;
	                			SceKernelMsgPacket hdr = {0};            
				                sMsg->header = hdr;
				                sMsg->returnBox = &mangaAPIMbx;
				                sMsg->what = NULL;
				                sMsg->type = RequestMangaList;
				                sMsg->result = true;
					            sceKernelSendMbx(*msg->returnBox, (void*)sMsg);             	
	                		
	                		} else {
	                		
	                			printf("Can't load [%s].\n", mapImp->getError().c_str());
	                			
	                			sMsg = new APIMessage;
	                			SceKernelMsgPacket hdr = {0};            
				                sMsg->header = hdr;
				                sMsg->returnBox = &mangaAPIMbx;
				                sMsg->what = (void*)new std::string(mapImp->getError());
				                sMsg->type = RequestMangaList;
				                sMsg->result = false;
					            sceKernelSendMbx(*msg->returnBox, (void*)sMsg);
	                		
	                		}
	                	
	                		break;
	                		
	                	case RequestChapterList:
						case RequestImageList:
						case RequestImage:
	                	
	                		break;
	                
	                }
	                	            	            	                	                
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
	            
	        mapImp = new MAP("http://omp.leonex.co.cc/api/");
	    
	    }
	    
	    /**
	     * Shuts down the manga API handler thread.
	     */
	    void shutdownMangaAPIThread() {
	    	    
	        if (mangaAPIThreadID > 0)
	            sceKernelDeleteThread(mangaAPIThreadID);
	        
	        delete mapImp;
	        	    
	    }
			
	}

}

#endif /* _MangaAPI_CPP */
