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
 * Bookmark Manager.
 * 
 * @package xMangaPSP
 */

#ifndef _BookmarkManager_CPP
#define _BookmarkManager_CPP

// BEGIN Includes
#include "BookmarkManager.h"

#include "tinyxml.h"
// END Includes

std::string bookmarkFile = "bookmarks.xml";

Bookmarks bookmarks;

TiXmlDocument* bookmarkXmlDoc;

void add(const std::string& name, const std::string& date, const std::string& type, const std::string& apiHandle) {

	// Get handle to traverse DOM
	TiXmlHandle docHandle = TiXmlHandle(bookmarkXmlDoc);

	// Bookmark element
	TiXmlElement* item = new TiXmlElement("bookmark");
	
	// Add Bookmark element
	docHandle.FirstChild("bookmarks").ToElement()->LinkEndChild(item);
	
	// Name element
	TiXmlElement* nameEl = new TiXmlElement("name");
	
	// Add name element
	item->LinkEndChild(nameEl);
	
	// Name text
	TiXmlText* nameText = new TiXmlText(name);
	
	// Add name text
	nameEl->LinkEndChild(nameText);
	
	// Date element
	TiXmlElement* dateEl = new TiXmlElement("date");
	
	// Add date element
	item->LinkEndChild(dateEl);
	
	TiXmlText* dateText;
	
	// Date text
	if (date == "")
		dateText = new TiXmlText(engine->getDateTime());
	else
		dateText = new TiXmlText(date);
	
	// Add date text
	dateEl->LinkEndChild(dateText);
	
	// Type element
	TiXmlElement* typeEl = new TiXmlElement("type");
	
	// Add type element
	item->LinkEndChild(typeEl);
	
	// Type text
	TiXmlText* typeText = new TiXmlText(type);
	
	// Add type text
	typeEl->LinkEndChild(typeText);
	
	// ApiHandle Element
	TiXmlElement* apiHandleEl = new TiXmlElement("apiHandle");
	
	// Add ApiHandle element
	item->LinkEndChild(apiHandleEl);
	
	// ApiHandle text
	TiXmlText* apiHandleText = new TiXmlText(apiHandle);
	
	// Add apiHandel text
	apiHandleEl->LinkEndChild(apiHandleText);

}

/**
 * Trys to load a bookmark file and creates it if it doesn't exist.
 * 
 * @param const std::string& file File to load/create.
 */
void BookmarkManager::loadBookmarks(const std::string& file) {

	// Set bookmark file
	if (file != "")
		bookmarkFile = file;
		
	bookmarkXmlDoc = new TiXmlDocument();
		
	// Try to load file
	if (!bookmarkXmlDoc->LoadFile(bookmarkFile)) {
		
		TiXmlDocument doc;
		
		TiXmlDeclaration* decl = new TiXmlDeclaration("1.0", "UTF-8", "yes");
		doc.LinkEndChild(decl);
		
		TiXmlElement* root = new TiXmlElement("bookmarks");
		doc.LinkEndChild(root);
            
        doc.SaveFile(bookmarkFile);
                
        // Now that we have created a new bookmark file try to reload it
        BookmarkManager::loadBookmarks(bookmarkFile);
	
	} else {
	
		// Load bookmarks
		BookmarkManager::reloadBookmarks();
	
	}

}

/**
 * Reload bookmarks from XML document.
 */
void BookmarkManager::reloadBookmarks() {

	// Clear old values
	bookmarks.clear();

	// Get handle to traverse DOM
    TiXmlHandle docHandle = TiXmlHandle(bookmarkXmlDoc);
    
    // Go to first item
    TiXmlElement *item = docHandle.FirstChild("bookmarks").FirstChild("bookmark").ToElement();
    
    if (item) {
    
    	for (item; item; item = item->NextSiblingElement()) {
    	
    		// Skip ill-formed items
    		if (
    			TiXmlHandle(item).FirstChild("name").ToElement()->GetText() == NULL ||
    			TiXmlHandle(item).FirstChild("date").ToElement()->GetText() == NULL ||
    			TiXmlHandle(item).FirstChild("type").ToElement()->GetText() == NULL ||
    			TiXmlHandle(item).FirstChild("apiHandle").ToElement()->GetText() == NULL
    		) { continue; }
    	
    		BookmarkItem bItem;
    		
    		bItem.name = TiXmlHandle(item).FirstChild("name").ToElement()->GetText();
    		bItem.date = TiXmlHandle(item).FirstChild("date").ToElement()->GetText();
    		bItem.type = TiXmlHandle(item).FirstChild("type").ToElement()->GetText();
    		bItem.apiHandle = TiXmlHandle(item).FirstChild("apiHandle").ToElement()->GetText();
    		
    		bookmarks.push_back(bItem);
    	
    	}
    
    }

}

/**
 * Save bookmarks.
 */
void BookmarkManager::saveBookmarks() {

	bookmarkXmlDoc->SaveFile(bookmarkFile);

}

/**
 * Get bookmarks.
 * 
 * @return Bookmarks the bookmarks.
 */
Bookmarks BookmarkManager::getBookmarks() {

	return bookmarks;

}

/**
 * Add a new bookmark.
 * 
 * @param const std::string& name Bookmark name.
 * @param const std::string& type Bookmark type.
 * @param const std::string& apiHandle Bookmark API handle.
 */
void BookmarkManager::addBookmark(const std::string& name, const std::string& type, const std::string& apiHandle) {

	// Use convenience function
	add(name, "", type, apiHandle);
	
	// Save
	BookmarkManager::saveBookmarks();
	
	// Reload
	BookmarkManager::reloadBookmarks();

}

/**
 * Delete a bookmark.
 * 
 * @param int index Index of bookmark.
 */
void BookmarkManager::deleteBookmark(int index) {

	// Remove from vector
	bookmarks.erase(bookmarks.begin() + index);
	
	// Delete old
	delete bookmarkXmlDoc;
	
	bookmarkXmlDoc = new TiXmlDocument();
	
	TiXmlDeclaration* decl = new TiXmlDeclaration("1.0", "UTF-8", "yes");
	bookmarkXmlDoc->LinkEndChild(decl);
		
	TiXmlElement* root = new TiXmlElement("bookmarks");
	bookmarkXmlDoc->LinkEndChild(root);
	
	unsigned int i = 0;
	
	for (i; i < bookmarks.size(); i++)	
		add(bookmarks[i].name, bookmarks[i].date, bookmarks[i].type, bookmarks[i].apiHandle);

	// Save
	BookmarkManager::saveBookmarks();
	
	// Reload
	BookmarkManager::reloadBookmarks();

}

#endif /* _BookmarkManager_CPP */
