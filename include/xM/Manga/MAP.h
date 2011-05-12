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
 * Header file.
 * 
 * @package xMangaPSP
 */

#ifndef _MAP_H
#define _MAP_H

// BEGIN Includes
#include "xM/Manga/MangaElements.h"

#include <string>
// END Includes

namespace xM {

	namespace Manga {
			
	    class MAP {
	    private:
	    
	    	std::string endpoint;
	    	std::string error;
	    	MangaList* mangaList;
	    
	    public:
	    
	        /**
	         * Default constructor.
	         * 
	         * @param const std::string& endpoint API endpoint.
	         */
	        MAP(const std::string& epoint);	        
	        
	        /**
	         * Default destructor.
	         */
	        ~MAP();
	        
	        /**
	         * Get the last error string.
	         * 
	         * @return std::string& The last error.
	         */
	        std::string& getError();
	        
	        /**
	         * Set the MAP API endpoint.
	         * 
	         * @param const std::string& epoint New endpoint
	         */
	        void setEndpoint(const std::string& epoint);
	        
	        /**
	         * Attempt to load the manga list.
	         * 
	         * @return bool Success or not.
	         */
	        bool loadMangaList();
	        
	        /**
	         * Returns the loaded manga list or an empty list.
	         * 
	         * @return MangaList* The manga list.
	         */
	        MangaList* getMangaList();
	        
	        /**
	         * Load the chapter list for a specific manga.
	         * 
	         * @param int mangaID The ID of the manga in the MangaList
	         * 
	         * @return bool Success or not.
	         */
	        //bool loadChapterList(int mangaID);
	        
	        /**
	         * Returns the loaded chapter list or an empty list.
	         * 
	         * @return ChapterList The chapter list.
	         */
	        //ChapterList* getChapterList();
	        
	        /**
	         * Load the image list for a specific chapter and manga.
	         * 
	         * @param int mangaID The ID of the manga in the MangaList
	         * @param int chapterID The ID of the chapter in the ChapterList
	         * 
	         * @return bool Success or not.
	         */
	        //bool loadImageList(int mangaID, int chapterID);
	        
	        /**
	         * Returns the loaded image list or an empty list.
	         * 
	         * @return ImageList The image list.
	         */
	        //ImageList getImageList();
	        
	        /**
	         * Load the image list for a specific chapter and manga.
	         * 
	         * @param int mangaID The ID of the manga in the MangaList
	         * @param int chapterID The ID of the chapter in the ChapterList
	         * @param int imageID The ID of the image in the ImageList
	         * 
	         * @return bool Success or not.
	         */
	        //bool loadImage(int mangaID, int chapterID, int imageID);
	        
	        /**
	         * Returns the loaded image.
	         * 
	         * @return MangaImage The image.
	         */
	        //MangaImage getImage();
	    
	    };
			
	}

}

#endif /* _MAP_H */
