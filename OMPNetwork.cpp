/*
 * $Id: OMPNetwork.cpp 80 2009-12-28 02:39:01Z chaotic@luqmanrocks.co.cc $
 * 
 * This file is part of the OneMangaPSP application.
 *
 * Copyright (C) 2009  Luqman Aden <www.luqmanrocks.co.cc>.
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
 * Full License: LICENSE.txt
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

/**
 * Network handling stuff.
 * 
 * @package OneMangaPSP
 */

#ifndef _OMPNetwork_CPP
#define _OMPNetwork_CPP

// BEGIN Includes
#include "OMPNetwork.h"
// END Includes

/**
 * Write for cUrl response.
 */
static int writer(char *data, size_t size, size_t nmemb, std::string *writerData) {
 
	if (writerData == NULL)
		return false;
                        
	writerData->append(data, size * nmemb);
	
	return size * nmemb;
        
}

/**
 * Download a file using cURL.
 * 
 * @param std::string url The url to download from.
 * @param std::string &response Pointer to string object to populate with response. (Which can also be error message.)
 * 
 * @return int If -1 then error, other wise your good.
 */ 
int downloadFile(std::string url, std::string &response) {

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
        uAgent << "User-Agent: LeX/2.0 (SDL/OpenGL; U; PSP; en-CA; libC:" << LIBCURL_VERSION_MAJOR << "." << LIBCURL_VERSION_MINOR << "." << LIBCURL_VERSION_PATCH << ";) OneMangaPSP/" << _MAJOR_VERSION << "." << _MINOR_VERSION << " API/" << _API_VERSION;
        
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
        
        	return -1;
        
        }
	
	} else {
	
		// Unable to create curl connection
		
		// Set response to error message
        response.assign("Unable to create cURL connection.");
        	
        // Cleanup
        curl_easy_cleanup(curl);
		curl_slist_free_all(headersList);
        
        return -1;
	
	}
	
	// Cleanup
	curl_easy_cleanup(curl);
	curl_slist_free_all(headersList);

	return 1;

}

#endif /* _OMPNetwork_CPP */
