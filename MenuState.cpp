/*
 * $Id: MenuState.cpp 86 2010-01-03 04:12:17Z chaotic@luqmanrocks.co.cc $
 * 
 * This file is part of the OneMangaPSP application.
 *
 * Copyright (C) 2009  Luqman Aden <www.luqmanrocks.co.cc>.
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
 * MenuState Class.
 * 
 * @package OneMangaPSP
 */

#ifndef _MenuState_CPP
#define _MenuState_CPP

// BEGIN Includes
#include "OneMangaPSP.h"
#include "States/MenuState.h"
#include "States/AboutState.h"
#include "States/BookmarkState.h"
#include "States/MangaSelectState.h"
#include "States/RecentMangaState.h"
#include "OMPUtil.h"
// END Includes

MenuState::MenuState() {

	OMStrArray tempMap;

	tempMap["name"] = "Manga";
	tempMap["description"] = "Read your favourite manga.";
	tempMap["action"] = "readManga";
	
	menuItems.push_back(tempMap);
	tempMap.clear();
	
	tempMap["name"] = "Recent Manga";
	tempMap["description"] = "Check out the latest additions.";
	tempMap["action"] = "recentManga";
	
	menuItems.push_back(tempMap);
	tempMap.clear();
	
	tempMap["name"] = "Bookmarks";
	tempMap["description"] = "Pick up where you left off.";
	tempMap["action"] = "viewBookmarks";
	
	menuItems.push_back(tempMap);
	tempMap.clear();
	
	/*tempMap["name"] = "Options";
	tempMap["description"] = "Change some aspects of how the program operates.";
	tempMap["action"] = "editOptions";
	
	menuItems.push_back(tempMap);
	tempMap.clear();*/
	
	tempMap["name"] = "About";
	tempMap["description"] = "See more info on OneMangaPSP.";
	tempMap["action"] = "about";
	
	menuItems.push_back(tempMap);
	tempMap.clear();
	
	tempMap["name"] = "Quit";
	tempMap["description"] = "Quit the application.";
	tempMap["action"] = "quit";
	
	menuItems.push_back(tempMap);
	tempMap.clear();
			
}

void MenuState::init() {
	    
	// Loads common resources if necessary
    OMPUtil::loadCommonResources();    
	    	
	SDL_Color textColour = {239, 237, 230};
	
	ResourceManager::getFont("SansBold15")->setColour(textColour);
	
	// Load main UI
	OMPUtil::loadMainUI();
	
	// Main UI
	this->textures["MainUI"] = ResourceManager::getTexture("MainUI");
	
	// Time	
	this->textures["Time"] = ResourceManager::getFont("SansBold15")->getGlTexture(engine->getTime().c_str());
	
	// Title text
	this->textures["TitleText"] = ResourceManager::getFont("SansBold15")->getGlTexture("Menu");
	
	// Description Label
	this->textures["DescriptionLabel"] = ResourceManager::getFont("SansBold15")->getGlTexture("Description:");
	
	// Action description
	this->textures["ActionDescription"] = ResourceManager::getFont("SansBold15")->getGlTexture("Select an item:");
		
	// Set some default values
	selected = 0;
	minList = selected;
	maxList = minList + 3;
	doAction = false;
	mangaListApiRequestId = -1;
	recentMangaListApiRequestId = -1;
					
}

void MenuState::shutdown() {
	
	// Free resources
    glDeleteTextures(1, &this->textures["TitleText"].texture);
    glDeleteTextures(1, &this->textures["Time"].texture);
    glDeleteTextures(1, &this->textures["DescriptionLabel"].texture);
    glDeleteTextures(1, &this->textures["ActionDescription"].texture);
    
    // Unload main UI
	OMPUtil::unloadMainUI();

}

void MenuState::pause() {



}

void MenuState::resume() {



}

void MenuState::handleInput() {

    SDL_Event event;

    while (FE_PollEvent(&event)) {
        
        switch (event.type) {
        
        	case SDL_QUIT:
        	
        		engine->quitGame();
        	
        		break;
        		
        	case SDL_OMMANGAAPIEVENT:
        	
        		// Check for corresponding event
				if ((int)event.user.data1 == mangaListApiRequestId) {
				
					// Error/Success checking
					if ((int)event.user.code == OMMangaApiError) {
					
						engine->logMsg("MenuState: Manga API error! [%s]", MangaAPI::getError().c_str());
						engine->showPspMsgDialog("Unable to load Manga list. Please verify your internet connection works and try restarting the app.", false);
						
					} else if ((int)event.user.code == OMMangaApiSuccess) {
					
						printf("Loaded manga list!\n");
						mangaListApiRequestId = -1;
						this->changeState(new MangaSelectState());
						
					}
				
				} else if ((int)event.user.data1 == recentMangaListApiRequestId) {
				
					// Error/Success checking
					if ((int)event.user.code == OMMangaApiError) {
					
						engine->logMsg("MenuState: Manga API error! [%s]", MangaAPI::getError().c_str());
						engine->showPspMsgDialog("Unable to load Recent Manga list. Please verify your internet connection works and try restarting the app.", false);
						
					} else if ((int)event.user.code == OMMangaApiSuccess) {
					
						printf("Loaded recent manga list!\n");
						recentMangaListApiRequestId = -1;
						this->changeState(new RecentMangaState());
						
					}
				
				}
        	
        		break;
        		
        	case SDL_JOYBUTTONDOWN:
        	
        		switch (event.jbutton.button) {	
        		       	        	        	        		       	        	        	
					case PSP_BUTTON_CROSS:
					
						// Set doAction flag
						if (mangaListApiRequestId == -1 && recentMangaListApiRequestId == -1)
							doAction = true;
					
						break;
        		       	        	        	
        	    	case PSP_BUTTON_DOWN:
        	    	
        	    		// Update selected item
        	    		if (mangaListApiRequestId == -1 && recentMangaListApiRequestId == -1)
        	    			selected += 1;
        	    		
        	    		// Allow repetition
        	    		repeatJoystick(event.jbutton.button);
        	    
        	    		break;
        	    	
        	    	case PSP_BUTTON_UP:
        	    
        	    		// Update selected item
        	    		if (mangaListApiRequestId == -1 && recentMangaListApiRequestId == -1)
        	    			selected -= 1;
        	    		
        	    		// Allow repetition
        	    		repeatJoystick(event.jbutton.button);
        	    
        	    		break;
        	    		
        	    	case PSP_BUTTON_LTRIGGER:
        	    	
        	    		// Go to top
        	    		if (mangaListApiRequestId == -1 && recentMangaListApiRequestId == -1)
        	    			selected = 0;
        	    	
        	    		break;
        	    		
        	    	case PSP_BUTTON_RTRIGGER:
        	    	
        	    		// Go to bottom
        	    		if (mangaListApiRequestId == -1 && recentMangaListApiRequestId == -1)
        	    			selected = this->menuItems.size() - 1;
        	    	
        	    		break;
        	    		
        	    }
        	            	        	        	        	        	
        		break;
        		        
        }

    }

}

void MenuState::handleLogic() {

	// BEGIN Menu Traversing Logic
	if (selected < 0)
		selected = 0;
	if ((unsigned int)selected > (this->menuItems.size() - 1))
		selected = this->menuItems.size() - 1;
	if (selected < minList) {
	
		minList = selected;
		maxList = minList + 3;	
		
	}
	if (selected > maxList) {
	
		maxList = selected;
		minList = maxList - 3;
	
	}
	// END Menu Traversing Logic
	
	// BEGIN Time Handling
	if (this->textures["Time"].texture) {
	
		glDeleteTextures(1, &this->textures["Time"].texture);
		this->textures["Time"] = ResourceManager::getFont("SansBold15")->getGlTexture(engine->getTime().c_str());
		
	}
	//END Time Handling
	
	// BEGIN Action Handling
	if (doAction == true) {
	
		std::string action = this->menuItems[selected]["action"];
		
		if (action == "readManga") {

			// Load Manga List		
			mangaListApiRequestId = MangaAPI::requestMangaList(OneMangaAPI);
			
		} else if (action == "recentManga") {
		
			// Info
			engine->showPspMsgDialog("Please be patient when loading recent manga as the manga list, chapter list AND image list have to be loaded.");
				
			// Load recent manga list
			recentMangaListApiRequestId = MangaAPI::requestRecentMangaList(OneMangaAPI);
			
		} else if (action == "viewBookmarks") {
		
			this->changeState(new BookmarkState());
			
		} else if (action == "editOptions") {
		
			// switch to options state
			
		} else if (action == "about") {
		
			this->changeState(new AboutState());
		
		} else if (action == "quit") {
		
			// Make sure they want to quit
			if (engine->showPspMsgDialog("Are you sure you want to quit?", true).buttonPressed == PSP_UTILITY_MSGDIALOG_RESULT_YES)
				engine->quitGame();
		
		}
	
		// Reset flag
		doAction = false;
	
	}
	// END Action Handling
		
}

void MenuState::render() {
	
	// Variables for where to place items
	int textLoc = 74, selectedIndicatorLoc = 69, i = minList;
	
	// Colours
	SDL_Color colour1 = {239, 237, 230};
	SDL_Color colour2 = {0, 0, 0};
	
	// Holds text/description
	std::string itemText;
	std::string itemDesc;
	
    // Draw MainUI
	engine->renderGlTexture(0, 0, this->textures["MainUI"]);
		
	// Draw Battery Icons
	OMPUtil::drawBatteryIcon();
	
	// Draw Time
	engine->renderGlTexture(425, 8, this->textures["Time"]);
	
	// Draw Title Text
	engine->renderGlTexture(9, 8, this->textures["TitleText"]);
		
	// Draw Description label
	engine->renderGlTexture(10, 209, this->textures["DescriptionLabel"]);
	
	// Draw Action Description
	engine->renderGlTexture(19, 44, this->textures["ActionDescription"]);
	
	// Display options
	do {
	
		// Make sure to only show 4
		if ((unsigned int) i >= this->menuItems.size())
			break;
		
		// Get the item name
		itemText = this->menuItems[i]["name"];
						
		// Active element						
		if (i == selected) {
		
			// Draw selected item indicator
			glBegin(GL_QUADS);
        
        		// Color 1
        		glColor3f(0.93359375f, 0.92578125f, 0.8984375f);
        		
				glVertex2i(10, selectedIndicatorLoc);
				glVertex2i(10 + 447, selectedIndicatorLoc);      
				glVertex2i(10 + 447, selectedIndicatorLoc + 27);
				glVertex2i(10, selectedIndicatorLoc + 27);
        
			glEnd();
			
			// Set active item colour
			ResourceManager::getFont("SansBold15")->setColour(colour2);
	
			itemDesc = std::string(this->menuItems[i]["description"]);
		
		}
		
		// Draw item text onto screen
		ResourceManager::getFont("SansBold15")->glDraw(19, textLoc, itemText.c_str());
		
		// Reset colour
		ResourceManager::getFont("SansBold15")->setColour(colour1);
						
		// Increment the counters
		textLoc += 30;
		selectedIndicatorLoc += 30;
		i++;
	
	} while (i <= maxList);
	
	// Draw item description onto screen
	ResourceManager::getFont("SansBold15")->glDraw(10, 232, itemDesc.c_str());
                
	if (mangaListApiRequestId != -1 || recentMangaListApiRequestId != -1)
		OMPUtil::drawLoadingIcon();
                                	
	// Flush
    glFlush();

}

#endif /* _MenuState_CPP */
