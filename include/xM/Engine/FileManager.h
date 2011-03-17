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
 * The file manager.
 * Header file.
 * 
 * @package xMangaPSP
 */

#ifndef _FileManager_H
#define _FileManager_H

// BEGIN Includes
#include <string>
// END Includes

// BEGIN Defines
#ifndef O_BINARY
#define O_BINARY 0
#endif

#ifndef PSP_O_BINARY
#define PSP_O_BINARY O_BINARY
#endif
// END Defines

namespace xM {

	namespace Engine {
	
		class FileManager {
		
            private:
            
                /**
                 * This will be a singleton class so no need for a public constructor.
                 */
                FileManager(void) { };
                                            		
		    public:
		    
		        /**
                 * The singleton instance.
                 */
                static FileManager* fMInstance;
		    
		        /**
		         * Get an instance of the class.
		         * 
		         * @return FileManager* Singleton instance.
		         */
		        static FileManager* getInstance(void);
		        
		        /**
                 * Set up some stuff.
                 */
                void init(void);
                
                /**
                 * Magic read function!
                 * 
                 * Reads the specified file from a multitude of possible locations choosing based on input.
                 * 
                 * "PSAR@afile.ext" => would load 'afile.ext' from the PSAR section of the EBOOT
                 * "resources.zip@afile.ext" => would load 'afile.ext' from a zip file name 'resources.zip'
                 * "afile.ext" => would simply be loaded from the filesystem.
                 * 
                 * @param const std::string& file The file.
                 * 
                 * @return std::string The contents of the file.
                 */
                std::string read(const std::string& file);
		        
		        /**
                 * Read a file in from the PSAR archive (zip file) in the EBOOT.
                 * 
                 * @param const std::string& file The file within the PSAR.
                 * 
                 * @return std::string The contents of the file.
                 */
                std::string readFromPSAR(const std::string& file);
                
                /**
                 * Read a file in from a zip file.
                 * 
                 * @param const std::string& zip The zip file.
                 * @param const std::string& file The file within the zip.
                 * 
                 * @return std::string The contents of the file.
                 */
                std::string readFromZIP(const std::string& zip, const std::string& file);
                
                /**
                 * Read a file in from the filesystem. (ms0 or host0 usually)
                 * 
                 * @param const std::string& file The file.
                 * 
                 * @return std::string The contents of the file.
                 */
                std::string readFromFS(const std::string& file);
		    		        		
		};
			
	}

}

#endif /* _FileManager_H */
