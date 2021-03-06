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
 * Manga Acquisition Protocol Implementation
 * Manga Elements
 * Header file.
 * 
 * @package xMangaPSP
 */

#ifndef _MangaElements_H
#define _MangaElements_H

// BEGIN Includes
#include <vector>
#include <string>
#include <xM/Gfx/Image.h>
// END Includes

namespace xM {

    namespace Manga {
            
        // Manga list
        typedef struct {

            std::vector<std::string> names;
            std::vector<std::string> apiHandles;

        } MangaList;
        
        // Chapter list
        typedef struct {

            std::string mangaSlug;
            std::vector<std::string> names;
            std::vector<std::string> apiHandles;

        } ChapterList;

        // Image list
        typedef struct {

            int index;
            std::string mangaSlug;
            std::string chapterSlug;
            std::vector<std::string> images;

        } ImageList;

        // Manga Image
        typedef struct {
            
            int index;
            Gfx::Image* img;

        } MangaImage;
            
    }

}

#endif /* _MangaElements_H */
