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

#include <pspkerneltypes.h>
#include <pspnet.h>
#include <pspnet_apctl.h>
#include <pspnet_inet.h>
#include <pspnet_resolver.h>
#include <psputility.h>
#include <psphttp.h>
#include <pspssl.h>

#include <curl/curl.h>
#include <curl/easy.h>

#include <sstream>
// END Includes

#define RETURN_ERROR_SET_STRING(res, err) \
			res.assign(err); \
			return false;

namespace xM {

	namespace Net {
			
	    /**
	     * Init the network modules.
	     */
		void init() {
		
		    // Load the right modules
		    sceUtilityLoadNetModule(PSP_NET_MODULE_COMMON);
            sceUtilityLoadNetModule(PSP_NET_MODULE_INET);
			sceUtilityLoadNetModule(PSP_NET_MODULE_PARSEURI);
			sceUtilityLoadNetModule(PSP_NET_MODULE_PARSEHTTP);
            sceUtilityLoadNetModule(PSP_NET_MODULE_HTTP);
            sceUtilityLoadNetModule(PSP_NET_MODULE_SSL);

            // mem, thrd priority, stack, thread priority, stack
            sceNetInit(0x20000, 0x20, 0x1000, 0x20, 0x1000);
            sceNetInetInit();
            sceNetResolverInit();
            sceNetApctlInit(0x1600, 0x42);

            sceSslInit(0x28000);
            sceHttpInit(0x25800);
            sceHttpsInit(0, 0, 0, 0);
            sceHttpsLoadDefaultCert(0, 0);
            sceHttpLoadSystemCookie();
		
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
        /*static int writer(char *data, size_t size, size_t nmemb, std::string* writerData) {
         
	        if (writerData == NULL)
		        return false;
                                
	        writerData->append(data, size * nmemb);
	
	        return size * nmemb;
                
        }*/
        
        /**
         * Download a file using cURL.
         * 
         * @param const std::string& url The url to download from.
         * @param std::string &response Pointer to string object to populate with response. (Which can also be error message.)
         * 
         * @return bool Success
         */ 
        /*bool downloadFile(const std::string& url, std::string &response) {

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

                // Set timeout to 10 seconds
                curl_easy_setopt(curl, CURLOPT_TIMEOUT, 10);
                
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

        }*/

		bool downloadFile(const std::string& urlString, std::string &response) {

			const char* url = urlString.c_str();
			int templateThingy, connection, request, ret, status, dataEnd, bytesWritten;
			unsigned char readBuffer[8192];
		
			// User Agent
			std::stringstream uAgent;
			uAgent << "User-Agent: xMangaPSP/" << _MAJOR_VERSION << "." << _MINOR_VERSION << " API/" << _API_VERSION;
			templateThingy = sceHttpCreateTemplate((char*) uAgent.str().c_str(), 1, 0);
			if(templateThingy < 0) {
				RETURN_ERROR_SET_STRING(response, "Unable to create HTTP template."); }

			ret = sceHttpSetResolveTimeOut(templateThingy, 3000000);
			if(ret < 0) {
				RETURN_ERROR_SET_STRING(response, "Error setting resolve timeout."); }

			ret = sceHttpSetRecvTimeOut(templateThingy, 60000000);
			if(ret < 0) {
				RETURN_ERROR_SET_STRING(response, "Error setting resv timeout."); }

			ret = sceHttpSetSendTimeOut(templateThingy, 60000000);
			if(ret < 0) {
				RETURN_ERROR_SET_STRING(response, "Error setting send timeout."); }

			connection = sceHttpCreateConnectionWithURL(templateThingy, url, 0);
			if(connection < 0) {
				RETURN_ERROR_SET_STRING(response, "Error creating connection."); }

			request = sceHttpCreateRequestWithURL(connection, PSP_HTTP_METHOD_GET, (char*) url, 0);
			if(request < 0) {
				RETURN_ERROR_SET_STRING(response, "Error creating request."); }

			ret = sceHttpSendRequest(request, 0, 0);
			if(ret < 0) {
				RETURN_ERROR_SET_STRING(response, "Error sending request."); }

			ret = sceHttpGetStatusCode(request, &status);
			if(ret < 0) {
				RETURN_ERROR_SET_STRING(response, "Error [generic][sceHttpGetStatusCode]."); }

			if(status != 200) {
				RETURN_ERROR_SET_STRING(response, "HTTP error: reponse NOT 200 OK."); }

			dataEnd = 0;
			bytesWritten = 0;

			while(dataEnd == 0) {

				ret = sceHttpReadData(request, readBuffer, 8192);
				
				if(ret < 0) {
					RETURN_ERROR_SET_STRING(response, "Error reading HTTP data."); }

				if(ret == 0)
					dataEnd = 1;

				if(ret > 0) {

					bytesWritten += ret;
					response.append((char*)readBuffer, ret);

				}

			}

			sceHttpDeleteRequest(request);
			sceHttpDeleteConnection(connection);
			sceHttpDeleteTemplate(templateThingy);

			return true;

		}
		
		/**
		 * Shutdown the network modules.
		 */
		void shutdown() {
		
			sceHttpSaveSystemCookie();
			sceHttpsEnd();
			sceHttpEnd();
			sceSslEnd();
		    sceNetApctlTerm();
		    sceNetResolverTerm();
		    sceNetInetTerm();
		    sceNetTerm();
		
			sceUtilityUnloadNetModule(PSP_NET_MODULE_SSL);
			sceUtilityUnloadNetModule(PSP_NET_MODULE_HTTP);
			sceUtilityUnloadNetModule(PSP_NET_MODULE_PARSEHTTP);
			sceUtilityUnloadNetModule(PSP_NET_MODULE_PARSEURI);
		    sceUtilityUnloadNetModule(PSP_NET_MODULE_INET);
		    sceUtilityUnloadNetModule(PSP_NET_MODULE_COMMON);
		
		}
			
	}

}

#endif /* _Net_CPP */
