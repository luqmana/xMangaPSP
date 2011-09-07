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
#include "xM/Gfx/Image.h"
#include "xM/Manga/MAP.h"
#include "xM/Net/Net.h"
#include "xM/Util/cJSON.h"
#include "xM/Util/Timer.h"
#include "xM/Util/Log.h"
#include "xM/Engine/ResourceManager.h"

#include <string.h>
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
            this->imageList = new ImageList;
            this->mangaImage = new MangaImage;
            this->error = "";
            this->loadedMangaList = false;
        
        }   
        
        /**
         * Default destructor.
         */
        MAP::~MAP() {
        
            delete this->mangaList;
            delete this->chapterList;
            delete this->imageList;
            delete this->mangaImage;
        
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

            Util::Timer loadTimer;
            loadTimer.start();
                                  
            // Attempt to download mangalist
            if (Net::downloadFile(this->endpoint, response)) {

                Util::logMsg("downloadFile - %f", loadTimer.getDeltaTicks(true));
                                                          
                // clear mangalist
                this->mangaList->names.clear();
                this->mangaList->apiHandles.clear();
                                                                 
                // No error, try to parse JSON
                
                loadTimer.start();

                cJSON* root = cJSON_Parse(response.c_str());

                Util::logMsg("Parse JSON - %f", loadTimer.getDeltaTicks(true));
                
                if (root == 0) {
                
                    Util::logMsg("Can't parse JSON [%s] [%s].", this->endpoint.c_str(), response.c_str());
                    
                    this->error = "Unable to parse JSON.";
                    
                    return false;
                
                }                
                
                cJSON* mangas = cJSON_GetObjectItem(root, "Manga");
                if (!mangas) {
                    
                    this->error = "JSON Error: Improperly structured response.";
                    Util::logMsg("%s", this->error.c_str());

                    return false;

                }

                loadTimer.start();

                int size = cJSON_GetArraySize(mangas);

                this->mangaList->names.reserve(size);
                this->mangaList->apiHandles.reserve(size);

                for (int i = 0; i < size; i++) {
                                
                    this->mangaList->names.push_back(cJSON_GetObjectItem(cJSON_GetArrayItem(mangas, i), "name")->valuestring);
                    this->mangaList->apiHandles.push_back(cJSON_GetObjectItem(cJSON_GetArrayItem(mangas, i), "apiHandle")->valuestring);
                                    
                }

                Util::logMsg("Populate list - %f", loadTimer.getDeltaTicks(true));

                cJSON_Delete(root);
                                    
                this->loadedMangaList = true;
                                    
                return true;
                
            } else {
            
                Util::logMsg("Can't download [%s] [%s].", this->endpoint.c_str(), response.c_str());
                
                this->error = "Net Error: " + response;
                
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

            Util::Timer loadTimer;
            loadTimer.start();
                                  
            // Attempt to download chapterlist
            if (Net::downloadFile(url, response)) {

                Util::logMsg("downloadFile - %f", loadTimer.getDeltaTicks(true));
                                                          
                // clear chapterlist
                this->chapterList->mangaSlug = mangaSlug;
                this->chapterList->names.clear();
                this->chapterList->apiHandles.clear();
                                                                 
                // No error, try to parse JSON
                
                loadTimer.start();

                cJSON* root = cJSON_Parse(response.c_str());

                Util::logMsg("Parse JSON - %f", loadTimer.getDeltaTicks(true));
                
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
                if (!chapters) {
                    
                    this->error = "JSON Error: Improperly structured response.";
                    this->error.append(" [" + mangaSlug + "]");
                    Util::logMsg("%s", this->error.c_str());

                    return false;

                }

                loadTimer.start();

                int size = cJSON_GetArraySize(chapters);

                this->chapterList->names.reserve(size);
                this->chapterList->apiHandles.reserve(size);

                for (int i = 0; i < size; i++) {
                                
                    this->chapterList->names.push_back(cJSON_GetObjectItem(cJSON_GetArrayItem(chapters, i), "name")->valuestring);
                    this->chapterList->apiHandles.push_back(cJSON_GetObjectItem(cJSON_GetArrayItem(chapters, i), "apiHandle")->valuestring);
                                    
                }

                Util::logMsg("Populate list - %f", loadTimer.getDeltaTicks(true));

                cJSON_Delete(root);
                                    
                return true;
                
            } else {
            
                Util::logMsg("Can't download [%s] [%s].", url.c_str(), response.c_str());
                
                this->error = "Net Error: " + response;
                
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

        /**
         * Load the image list for a specific chapter and manga.
         * 
         * @param const std::string& mangaSlug The slug of the manga in the MangaList
         * @param const std::string& chapterSlug The slug of the chapter in the ChapterList
         * @param int index Chapter index.
         * 
         * @return bool Success or not.
         */
        bool MAP::loadImageList(const std::string& mangaSlug, const std::string& chapterSlug, int index) {
            
            std::string response;
            std::string url = this->endpoint + mangaSlug + "/" + chapterSlug + "/";

            Util::Timer loadTimer;
            loadTimer.start();
                                  
            // Attempt to download imagelist
            if (Net::downloadFile(url, response)) {

                Util::logMsg("downloadFile - %f", loadTimer.getDeltaTicks(true));
                
                // clear imagelist
                this->imageList->mangaSlug = mangaSlug;
                this->imageList->chapterSlug = chapterSlug;
                this->imageList->images.clear();
                                                                 
                // No error, try to parse JSON
                
                loadTimer.start();

                cJSON* root = cJSON_Parse(response.c_str());

                Util::logMsg("Parse JSON - %f", loadTimer.getDeltaTicks(true));
                
                if (root == 0) {
                
                    Util::logMsg("Can't parse JSON [%s] [%s].", this->endpoint.c_str(), response.c_str());
                    
                    this->error = "Unable to parse JSON.";
                    
                    return false;
                
                }
                
                cJSON* err = cJSON_GetObjectItem(root, "Error");
                if (err != 0) {
                
                    this->error = "API Error: ";
                    this->error.append(cJSON_GetObjectItem(err, "msg")->valuestring);
                    this->error.append(" [" + mangaSlug + ":" + chapterSlug + "]");
                    Util::logMsg("%s", this->error.c_str());
                    
                    return false;
                
                }
                
                cJSON* images = cJSON_GetObjectItem(root, "Images");
                if (!images) {
                    
                    this->error = "JSON Error: Improperly structured response.";
                    this->error.append(" [" + mangaSlug + ":" + chapterSlug + "]");
                    Util::logMsg("%s", this->error.c_str());

                    return false;

                }

                loadTimer.start();

                int size = cJSON_GetArraySize(images);

                this->imageList->images.reserve(size);

                for (int i = 0; i < size; i++)
                    this->imageList->images.push_back(cJSON_GetObjectItem(cJSON_GetArrayItem(images, i), "image")->valuestring);

                Util::logMsg("Populate list - %f", loadTimer.getDeltaTicks(true));

                cJSON_Delete(root);

                this->imageList->index = index;
                                    
                return true;
                
            } else {
            
                Util::logMsg("Can't download [%s] [%s].", url.c_str(), response.c_str());
                
                this->error = "Net Error: " + response;
                
                return false;
                
            }

        }
        
        /**
         * Returns the loaded image list or an empty list.
         * 
         * @return ImageList The image list.
         */
        ImageList* MAP::getImageList() {
            
            return this->imageList;

        }

        /**
         * Load the image list for a specific chapter and manga.
         * 
         * @param const std::string& mangaSlug The slug of the manga in the MangaList
         * @param const std::string& chapterSlug The slug of the chapter in the ChapterList
         * @param const std::string& imageSlug The slug of the image in the ImageList
         * @param int index Image index.
         * 
         * @return bool Success or not.
         */
        bool MAP::loadImage(const std::string& mangaSlug, const std::string& chapterSlug, const std::string& imageSlug, int index) {
            
            std::string response;
            std::string url = this->endpoint + mangaSlug + "/" + chapterSlug + "/" + imageSlug + "/";

            Util::Timer loadTimer;
            loadTimer.start();
                                  
            // Attempt to download imagelist
            if (Net::downloadFile(url, response)) {

                Util::logMsg("downloadFile - %f", loadTimer.getDeltaTicks(true));

                unsigned char jCheck[6] = { '{', '"', 'A', 'P', 'I', '"' };

                // 'Tis a JSON message?
                if (memcmp(jCheck, &response[0], 6) == 0) {
                    
                    cJSON* root = cJSON_Parse(response.c_str());
                    
                    if (root == 0) {
                        
                        this->error = "Something is just wack yo!";
                        Util::logMsg("%s", this->error.c_str());
                    
                        return false;

                    }

                    cJSON* err = cJSON_GetObjectItem(root, "Error");
                    if (err != 0) {
                    
                        this->error = "API Error: ";
                        this->error.append(cJSON_GetObjectItem(err, "msg")->valuestring);
                        this->error.append(" [" + mangaSlug + ":" + chapterSlug + ":" + imageSlug + "]");
                        Util::logMsg("%s", this->error.c_str());
                        
                        return false;
                    
                    }

                    this->error = "Format Error: No clue what kinda data we got.";
                    this->error.append(" [" + mangaSlug + ":" + chapterSlug + ":" + imageSlug + "]");
                    Util::logMsg("%s", this->error.c_str());
                    
                    return false;

                }
                    
                // so probably an image...hopefully

                delete this->mangaImage->img;

                this->mangaImage->img = new Gfx::Image;
                this->mangaImage->index = index;

                loadTimer.start();
                if (!this->mangaImage->img->loadData(response)) {
                    
                    this->error = "Image Error: Can't load image.";
                    this->error.append(" [" + mangaSlug + ":" + chapterSlug + ":" + imageSlug + "]");
                    Util::logMsg("%s", this->error.c_str());
                    
                    return false;

                }
                Util::logMsg("loadData - %f", loadTimer.getDeltaTicks(true));

                response.clear();

                if (!this->mangaImage->img->isLoaded()) {
                    
                    this->error = "Image Error: Can't load image.";
                    this->error.append(" [" + mangaSlug + ":" + chapterSlug + ":" + imageSlug + "]");
                    Util::logMsg("%s", this->error.c_str());
                    
                    return false;
                    
                }

                //* Swizzling not functioning well, so disabled till further notice
                loadTimer.start();
                this->mangaImage->img->swizzle();
                Util::logMsg("swizzle - %f", loadTimer.getDeltaTicks(true));
                //*/

                return true;

            } else {
                
                Util::logMsg("Can't download [%s] [%s].", url.c_str(), response.c_str());
                
                this->error = "Net Error: " + response;
                
                return false;

            }

        }

        /**
         * Returns the loaded image.
         * 
         * @return MangaImage The image.
         */
        MangaImage* MAP::getImage() {
            
            return this->mangaImage;

        }
            
    }

}

#endif /* _MAP_CPP */
