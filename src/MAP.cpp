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
 * Manga Acquisition Protocol Implementation
 * 
 * @package xMangaPSP
 */

#ifndef _MAP_CPP
#define _MAP_CPP

// BEGIN Includes
#include "xM/Manga/MAP.h"
#include "xM/Net/Net.h"
#include "xM/Util/cJSON.h"
#include "xM/Util/Log.h"
// END Includes

namespace xM {

    namespace Manga {
            
        /**
         * Default constructor.
         * 
         * @param const std::string& epoint API endpoint.
         */
        MAP::MAP(const std::string& epoint) {
        
            this->endpoint = epoint;
            this->mangaList = new MangaList;
            this->chapterList = new ChapterList;
            this->error = "";
            this->loadedMangaList = false;
        
        }   
        
        /**
         * Default destructor.
         */
        MAP::~MAP() {
        
            delete this->mangaList;
            delete this->chapterList;
        
        }
        
        /**
         * Get the last error string.
         * 
         * @return std::string& The last error.
         */
        std::string& MAP::getError() {
        
            return this->error;
        
        }
        
        /**
         * Set the MAP API endpoint.
         * 
         * @param const std::string& epoint New endpoint
         */
        void MAP::setEndpoint(const std::string& epoint) {
        
            this->endpoint = epoint;
            
            // New endpoint
            this->loadedMangaList = false;
        
        }
        
        /**
         * Attempt to load the manga list.
         * 
         * @return bool Success or not.
         */
        bool MAP::loadMangaList() {
        
            // No point in loading again
            if (this->loadedMangaList)
                return true;
        
            std::string response;
                                  
            // Attempt to download mangalist
            if (Net::downloadFile(this->endpoint, response)) {
                                                          
                // clear mangalist
                this->mangaList->names.clear();
                this->mangaList->apiHandles.clear();
                                                                 
                // No error, try to parse JSON
                
                cJSON* root = cJSON_Parse(response.c_str());
                
                if (root == 0) {
                
                    Util::logMsg("Can't parse JSON [%s] [%s].", this->endpoint.c_str(), response.c_str());
                    
                    this->error = "Unable to parse JSON.";
                    
                    return false;
                
                }                
                
                cJSON* mangas = cJSON_GetObjectItem(root, "Manga");

                for (int i = 0; i < cJSON_GetArraySize(mangas); i++) {
                                
                    this->mangaList->names.push_back(cJSON_GetObjectItem(cJSON_GetArrayItem(mangas, i), "name")->valuestring);
                    this->mangaList->apiHandles.push_back(cJSON_GetObjectItem(cJSON_GetArrayItem(mangas, i), "apiHandle")->valuestring);
                                    
                }
                cJSON_Delete(root);
                                    
                this->loadedMangaList = true;
                                    
                return true;
                
            } else {
            
                Util::logMsg("Can't download [%s] [%s].", this->endpoint.c_str(), response.c_str());
                
                this->error = response.c_str();
                
                return false;
                
            }
        
        }
        
        /**
         * Returns the loaded manga list or an empty list.
         * 
         * @return MangaList* The manga list.
         */
        MangaList* MAP::getMangaList() {
        
            return this->mangaList;
        
        }
        
        /**
         * Load the chapter list for a specific manga.
         * 
         * @param const std::string& mangaSlug The slug of the manga in the MangaList
         * 
         * @return bool Success or not.
         */
        bool MAP::loadChapterList(const std::string& mangaSlug) {
                
            std::string response;
            std::string url = this->endpoint + mangaSlug + "/";
                                  
            // Attempt to download chapterlist
            if (Net::downloadFile(url, response)) {
                                                          
                // clear chapterlist
                this->chapterList->mangaSlug = mangaSlug;
                this->chapterList->names.clear();
                this->chapterList->apiHandles.clear();
                                                                 
                // No error, try to parse JSON
                
                cJSON* root = cJSON_Parse(response.c_str());
                
                if (root == 0) {
                
                    Util::logMsg("Can't parse JSON [%s] [%s].", this->endpoint.c_str(), response.c_str());
                    
                    this->error = "Unable to parse JSON.";
                    
                    return false;
                
                }
                
                cJSON* err = cJSON_GetObjectItem(root, "Error");
                if (err != 0) {
                
                    this->error = "API Error: ";
                    this->error.append(cJSON_GetObjectItem(err, "msg")->valuestring);
                    this->error.append(" [" + mangaSlug + "]");
                    Util::logMsg("%s", this->error.c_str());
                    
                    return false;
                
                }
                
                cJSON* chapters = cJSON_GetObjectItem(root, "Chapters");

                for (int i = 0; i < cJSON_GetArraySize(chapters); i++) {
                                
                    this->chapterList->names.push_back(cJSON_GetObjectItem(cJSON_GetArrayItem(chapters, i), "name")->valuestring);
                    this->chapterList->apiHandles.push_back(cJSON_GetObjectItem(cJSON_GetArrayItem(chapters, i), "apiHandle")->valuestring);
                                    
                }
                cJSON_Delete(root);
                                    
                return true;
                
            } else {
            
                Util::logMsg("Can't download [%s] [%s].", url.c_str(), response.c_str());
                
                this->error = response.c_str();
                
                return false;
                
            }
        
        }
        
        /**
         * Returns the loaded chapter list or an empty list.
         * 
         * @return ChapterList The chapter list.
         */
        ChapterList* MAP::getChapterList() {
        
            return this->chapterList;
        
        }
            
    }

}

#endif /* _MAP_CPP */
