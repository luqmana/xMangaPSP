/*
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
 * Full License: LICENSE.txt
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

/**
 * Header for Bookmark Manager.
 * 
 * @package xMangaPSP
 */

#ifndef _BookmarkManager_H
#define _BookmarkManager_H

// BEGIN Includes
#include "xMPStandard.h"
#include "xMangaPSP.h"
// END Includes

typedef struct {

	std::string name;
	std::string date;
	std::string type;
	std::string apiHandle;
	
} BookmarkItem;

typedef std::vector<BookmarkItem> Bookmarks;

namespace BookmarkManager {

	/**
	 * Trys to load a bookmark file and creates it if it doesn't exist.
	 * 
	 * @param const std::string& file File to load/create.
	 */
	void loadBookmarks(const std::string& file = "");
	
	/**
	 * Reload bookmarks from XML document.
	 */
	void reloadBookmarks();
	
	/**
	 * Save bookmarks.
	 */
	void saveBookmarks();
	
	/**
	 * Get bookmarks.
	 * 
	 * @return Bookmarks the bookmarks.
	 */
	Bookmarks getBookmarks();
	
	/**
	 * Add a new bookmark.
	 * 
	 * @param const std::string& name Bookmark name.
	 * @param const std::string& type Bookmark type.
	 * @param const std::string& apiHandle Bookmark API handle.
	 */
	void addBookmark(const std::string& name, const std::string& type, const std::string& apiHandle);
	
	/**
	 * Delete a bookmark.
	 * 
	 * @param int index Index of bookmark.
	 */
	void deleteBookmark(int index);

};

#endif /* _BookmarkManager_H */
