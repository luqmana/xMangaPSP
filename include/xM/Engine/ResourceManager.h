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
 * Header file.
 * 
 * @package xMangaPSP
 */

#ifndef _ResourceManager_H
#define _ResourceManager_H

// BEGIN Includes
#include "xM/Engine/FileManager.h"
#include "xM/Gfx/Image.h"

#include <string>
#include <map>
// END Includes

// BEGIN Defines

// END Defines

namespace xM {

	namespace Engine {
	
		class ResourceManager {
		
            private:
            
                /**
                 * This will be a singleton class so no need for a public constructor.
                 */
                ResourceManager(void) { };
                                            		
		    public:
		    
		        /**
                 * The singleton instance.
                 */
                static ResourceManager* rMInstance;
		    
		        /**
		         * Get an instance of the class.
		         * 
		         * @return ResourceManager* Singleton instance.
		         */
		        static ResourceManager* getInstance(void);
		        
		        /**
                 * Set up some stuff.
                 */
                void init(void);
		        
		        /**
		         * Returns a cached image. If not cached, loads it.
		         * 
		         * @param const std::string& image The image.
		         * 
		         * @return Gfx::Image* The image.
		         */
		        Gfx::Image* getImage(const std::string& image);
		        
		        /**
		         * A magic wrapper for the FileManager.
		         * 
		         * Tries to load a resource from different sources. PSAR, Resource File (resources.zip), FS (in that order)
		         * 
		         * @param const std::string& file The file.
		         * 
		         * @return std::string The resource.
		         */
		        std::string getRes(const std::string& file);
		    		        		
		};
			
	}

}

#endif /* _FileManager_H */
