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
 * MAP Co-ordinator
 * Header file.
 * 
 * @package xMangaPSP
 */

#ifndef _MangaAPI_H
#define _MangaAPI_H

// BEGIN Includes
#include "xM/Manga/MAP.h"

#include <pspkernel.h>

#include <string>
// END Includes

namespace xM {

    namespace Manga {
    
        enum RequestType {
        
            SetEndpoint,
            RequestMangaList,
            RequestChapterList,
            RequestImageList,
            RequestImage
        
        };
    
        struct APIMessage {
        
            SceKernelMsgPacket header;
            SceUID* returnBox;
            RequestType type;
            void* what;
            int index;
            bool result;
            
        };
    
        extern SceUID mangaAPIMbx;
        extern MAP* mapImp;
            
        /**
         * Setups the manga API handler thread.
         */
        void initMangaAPIThread();
        
        /**
         * Shuts down the manga API handler thread.
         */
        void shutdownMangaAPIThread();
            
    }

}

#endif /* _MangaAPI_H */
