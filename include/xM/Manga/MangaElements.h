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
// END Includes

namespace xM {

	namespace Manga {
			
	    // Manga list item
		typedef struct {

			std::string name;
			std::string apiHandle;

		} MangaListItem;

		// Manga list
		typedef std::vector<MangaListItem> MangaList;

		// Chapter list item
		typedef struct {

			std::string name;
			std::string apiHandle;

		} ChapterListItem;

		// Chapter list
		typedef struct {

			MangaList* mangaList;
			std::vector<ChapterListItem> list;
	
		} ChapterList;

		// Image list item
		typedef struct {

			std::string image;

		} ImageListItem;

		// Image list
		typedef struct {

			ChapterList* chapterList;
			std::vector<ImageListItem> list;
	
		} ImageList;
			
	}

}

#endif /* _MangaElements_H */
