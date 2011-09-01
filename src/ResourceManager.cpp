/**
 * This file is part of the xMangaPSP application.
 *
 * Copyright (C) Luqman Aden <www.luqmanrocks.co.cc>.
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
 * The resource manager.
 * 
 * @package xMangaPSP
 */

#ifndef _ResourceManager_CPP
#define _ResourceManager_CPP

// BEGIN Includes
#include "xM/Engine/FileManager.h"
#include "xM/Engine/ResourceManager.h"
// END Includes

namespace xM {

	namespace Engine {
	
        /**
         * The singleton instance.
         */
        ResourceManager* ResourceManager::rMInstance = NULL;
    
        /**
         * Get an instance of the class.
         * 
         * @return ResourceManager* Singleton instance.
         */
        ResourceManager* ResourceManager::getInstance(void) {
        
            // Create instance if necessary
            if (rMInstance == NULL)
                rMInstance = new ResourceManager;

            return rMInstance;
        
        }
                
        /**
         * Returns a cached image. If not cached, loads it.
         * 
         * @param const std::string& image The image.
         * 
         * @return Gfx::Image* The image.
         */
        Gfx::Image* ResourceManager::getImage(const std::string& image) {
                        
            const std::string key = image;
                                    
            // Check if cached
            if (images.find(key) != images.end())
                return images.find(key)->second;
                
            // Not cached :(
            Gfx::Image* img = new Gfx::Image();
            
            if (!img->loadFile(image)) {
            
            	delete img;
            	
                return NULL;
                
            }
            
            images.insert(std::pair<const std::string, Gfx::Image*>(key, img));
            
            return img;
        
        }
        
        /**
         * Returns a cached font. If not cached, loads it.
         * 
         * @param const std::string& font The font.
         * @param unsigned int loadOps Font loading options.
         *
         * @return intraFont* The font.
         */
        intraFont* ResourceManager::getFont(const std::string& font, unsigned int loadOps) {
        
            std::pair<const std::string, unsigned int> key = std::pair<const std::string, unsigned int>(font, loadOps);
        
            // Check if cached
            if (fonts.find(key) != fonts.end())
                return fonts.find(key)->second;
                
            // Not cached :(
            intraFont* iFont = intraFontLoad(font.c_str(), loadOps);
            
            if (!iFont)
                return NULL;
            
            fonts.insert(std::pair<std::pair<const std::string, unsigned int>, intraFont*>(key, iFont));
            
            return iFont;
        
        }
        
        /**
         * A magic wrapper for the FileManager.
         * 
         * Tries to load a resource from different sources.
         * (FileSystem, Resource File (resources.zip), PSAR) (in that order)
         * 
         * @param const std::string& file The file.
         * 
         * @return std::string The resource.
         */
        std::string ResourceManager::getRes(const std::string& file) {
        
            // Explicit location
            if (file.find("@") != std::string::npos)
                return FileManager::getInstance()->read(file);
                
            std::string res = "";

            // Try FS
            res = FileManager::getInstance()->readFromFS(file);
            if (res != "")
                return res;
                            
            // Try ZIP
            res = FileManager::getInstance()->readFromZIP("resources.zip", file);
            if (res != "")
                return res;

            // Finally try PSAR
            return FileManager::getInstance()->readFromPSAR(file);
        
        }
		    		        					
	}

}

#endif /* _ResourceManager_CPP */
