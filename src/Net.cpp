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
            sceUtilityLoadNetModule(PSP_NET_MODULE_SSL);
#if !(__xM_DEBUG)
            sceUtilityLoadNetModule(PSP_NET_MODULE_HTTP);
#endif

            // mem, thrd priority, stack, thread priority, stack
            sceNetInit(0x20000, 0x20, 0x1000, 0x20, 0x1000);
            sceNetInetInit();
            sceNetResolverInit();
            sceNetApctlInit(0x1600, 0x42);
            
#if !(__xM_DEBUG)
            sceSslInit(0x28000);
            sceHttpInit(0x25800);            
            sceHttpsInit(0, 0, 0, 0);
            sceHttpsLoadDefaultCert(0, 0);
            sceHttpLoadSystemCookie();
#endif
        
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
         * Download a file using cURL.
         * 
         * @param const std::string& url The url to download from.
         * @param std::string &response Pointer to string object to populate with response. (Which can also be error message.)
         * 
         * @return bool Success
         */ 
        bool downloadFile(const std::string& urlString, std::string &response) {

            const char* url = urlString.c_str();
            int templateThingy, connection, request, ret, status, dataEnd, bytesWritten;
            unsigned char readBuffer[8192];

#if __xM_DEBUG      
            // Have to load/unload per download cause otherwise reset via psplink is broken
            sceUtilityLoadNetModule(PSP_NET_MODULE_HTTP);
            sceSslInit(0x28000);
            sceHttpInit(0x25800);            
            sceHttpsInit(0, 0, 0, 0);
            sceHttpsLoadDefaultCert(0, 0);
            sceHttpLoadSystemCookie();
#endif

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
                RETURN_ERROR_SET_STRING(response, "HTTP Error: response NOT 200 OK."); }

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

#if __xM_DEBUG
            // Have to load/unload per download cause otherwise reset via psplink is broken
            sceHttpSaveSystemCookie();
            sceHttpsEnd();
            sceHttpEnd();
            sceSslEnd();
            sceUtilityUnloadNetModule(PSP_NET_MODULE_HTTP);
#endif
            
            return true;

        }
        
        /**
         * Shutdown the network modules.
         */
        void shutdown() {
        
#if !(__xM_DEBUG)
            // Have to load/unload per download cause otherwise reset via psplink is broken
            sceHttpSaveSystemCookie();
            sceHttpsEnd();
            sceHttpEnd();
            sceSslEnd();
#endif
        
            sceNetApctlTerm();
            sceNetResolverTerm();
            sceNetInetTerm();
            sceNetTerm();
        
#if !(__xM_DEBUG)
            sceUtilityUnloadNetModule(PSP_NET_MODULE_HTTP);
#endif
            sceUtilityUnloadNetModule(PSP_NET_MODULE_INET);
            sceUtilityUnloadNetModule(PSP_NET_MODULE_COMMON);
            sceUtilityUnloadNetModule(PSP_NET_MODULE_PARSEHTTP);
            sceUtilityUnloadNetModule(PSP_NET_MODULE_PARSEURI);
            sceUtilityUnloadNetModule(PSP_NET_MODULE_SSL);
        
        }
            
    }

}

#endif /* _Net_CPP */
