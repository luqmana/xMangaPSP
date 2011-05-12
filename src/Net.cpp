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

#include <curl/curl.h>
#include <curl/easy.h>

#include <sstream>
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
         * Write for cUrl response.
         */
        static int writer(char *data, size_t size, size_t nmemb, std::string* writerData) {
         
	        if (writerData == NULL)
		        return false;
                                
	        writerData->append(data, size * nmemb);
	
	        return size * nmemb;
                
        }
        
        /**
         * Download a file using cURL.
         * 
         * @param const std::string& url The url to download from.
         * @param std::string &response Pointer to string object to populate with response. (Which can also be error message.)
         * 
         * @return bool Success
         */ 
        bool downloadFile(const std::string& url, std::string &response) {

	        // Error buffer
	        char errorBuffer[CURL_ERROR_SIZE];
	
	        // cURL Handle
	        CURL *curl = curl_easy_init();
	
	        // cURL Code
	        CURLcode res;
	
	        // HTTP Header list
	        struct curl_slist *headersList = NULL;
	
	        // Make sure we have a valid handle
	        if (curl != NULL) {
	
		        // Setup error buffer
		        curl_easy_setopt(curl, CURLOPT_ERRORBUFFER, errorBuffer);
		
		        // Set Options:
		
		        // Allow following 'Location' headers
		        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
		
		        // Set URL
		        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
		
		        // User Agent
		        std::stringstream uAgent;
                uAgent << "User-Agent: xMangaPSP/" << _MAJOR_VERSION << "." << _MINOR_VERSION << " API/" << _API_VERSION;
                
                // Add User Agent to headers list
                headersList = curl_slist_append(headersList, uAgent.str().c_str());
                
                // Set Connection to close
                headersList = curl_slist_append(headersList, "Connection: Close");
                
                // Set encoding to ALL
                curl_easy_setopt(curl, CURLOPT_ENCODING, "");
                
                // Set HTTP Headers
                curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headersList);
                
                // Set writer function
                curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writer);
                
                // Set write buffer
                curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
                
                // Perform request
                res = curl_easy_perform(curl);
                
                // Error
                if (res != CURLE_OK) {
                
                	// Set response to error message
                	response.assign(errorBuffer);
                	
                	// Cleanup
                	curl_easy_cleanup(curl);
			        curl_slist_free_all(headersList);
                
                	return false;
                
                }
	
	        } else {
	
		        // Unable to create curl connection
		
		        // Set response to error message
                response.assign("Unable to create cURL connection.");
                	
                // Cleanup
                curl_easy_cleanup(curl);
		        curl_slist_free_all(headersList);
                
                return false;
	
	        }
	
	        // Cleanup
	        curl_easy_cleanup(curl);
	        curl_slist_free_all(headersList);

	        return true;

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
