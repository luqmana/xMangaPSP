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
 * RecentMangaState Class.
 * 
 * @package xMangaPSP
 */

#ifndef _RecentMangaState_CPP
#define _RecentMangaState_CPP

// BEGIN Includes
#include "xMangaPSP.h"
#include "States/RecentMangaState.h"
#include "States/ImageSelectState.h"
#include "States/MenuState.h"
#include "xMPUtil.h"
// END Includes

void RecentMangaState::init() {

	// Set manga list
	recentMangaList = MangaAPI::getRecentMangaList();

	// Load main UI
	xMPUtil::loadMainUI();

	// Get resource
    this->textures["MainUI"] = ResourceManager::getTexture("MainUI");
    
    SDL_Color textColour = {239, 237, 230};
	
	ResourceManager::getFont("SansBold15")->setColour(textColour);
	
	// Time	
	this->textures["Time"] = ResourceManager::getFont("SansBold15")->getGlTexture(engine->getTime().c_str());
	
	// Title text
	this->textures["TitleText"] = ResourceManager::getFont("SansBold15")->getGlTexture("Recent Manga (%d)", this->recentMangaList.size());
		
	// Action description
	this->textures["ActionDescription"] = ResourceManager::getFont("SansBold15")->getGlTexture("Select a manga:");
	
	// Set some default values
	selected = 0;
	minList = selected;
	maxList = minList + 3;
	doAction = false;
	action = -1;
	mangaListApiRequestId = -1;
	chapterListApiRequestId = -1;
	imageListApiRequestId = -1;
	
}

void RecentMangaState::shutdown() {

	// Free resources
    glDeleteTextures(1, &this->textures["TitleText"].texture);
    glDeleteTextures(1, &this->textures["Time"].texture);
    glDeleteTextures(1, &this->textures["ActionDescription"].texture);
    
    // Unload main UI
	xMPUtil::unloadMainUI();

}

void RecentMangaState::pause() {



}

void RecentMangaState::resume() {



}

void RecentMangaState::handleInput() {

   	SDL_Event event;

    while (FE_PollEvent(&event)) {
        
        switch (event.type) {
        
        	case SDL_QUIT:
        	
        		engine->quitGame();
        	
        		break;
        		
        	case SDL_xMANGAAPIEVENT:
        	
        		// Check for corresponding event
				if ((int)event.user.data1 == mangaListApiRequestId) {
				
					// Error/Success checking
					if ((int)event.user.code == xMangaApiError) {
					
						engine->logMsg("RecentMangaState: Manga API error! [%s]", MangaAPI::getError().c_str());
						engine->showPspMsgDialog("Unable to load Manga list. Please verify your internet connection works and try restarting the app.", false);
						
					} else if ((int)event.user.code == xMangaApiSuccess) {
					
						printf("Loaded manga list!\n");
						mangaListApiRequestId = -1;
						doAction = true;
						action = 2;
						
						
					}
				
				} else if ((int)event.user.data1 == chapterListApiRequestId) {
				
					// Error/Success checking
					if ((int)event.user.code == xMangaApiError) {
					
						engine->logMsg("RecentMangaState: Manga API error! [%s]", MangaAPI::getError().c_str());
						engine->showPspMsgDialog("Unable to load Chapter list. Please verify your internet connection works and try restarting the app.", false);
						
					} else if ((int)event.user.code == xMangaApiSuccess) {
					
						printf("Loaded chapter list!\n");
						chapterListApiRequestId = -1;
						doAction = true;
						action = 3;
						
						
					}
				
				} else if ((int)event.user.data1 == imageListApiRequestId) {
				
					// Error/Success checking
					if ((int)event.user.code == xMangaApiError) {
					
						engine->logMsg("RecentMangaState: Manga API error! [%s]", MangaAPI::getError().c_str());
						engine->showPspMsgDialog("Unable to load Image list. Please verify your internet connection works and try restarting the app.", false);
						
					} else if ((int)event.user.code == xMangaApiSuccess) {
					
						printf("Loaded image list!\n");
						imageListApiRequestId = -1;
						action = -1;
						this->changeState(new ImageSelectState());
						
					}
				
				}
        	
        		break;
        		
        	case SDL_JOYBUTTONDOWN:
        	
        		switch (event.jbutton.button) {
        		
        			case PSP_BUTTON_CIRCLE:
        			
        				// Back to menu
        				this->changeState(new MenuState());
        			
        				break;
        		       	        	        	
					case PSP_BUTTON_CROSS:
					
						// Set doAction flag
						if (action == -1) {
						
							doAction = true;
							action = 1;
							
						}
					
						break;
        		       	        	        	
        	    	case PSP_BUTTON_DOWN:
        	    	
        	    		// Update selected item
        	    		if (action == -1) {
        	    		
        	    			if (SDL_JoystickGetButton(engine->getJoystick(0), PSP_BUTTON_SQUARE))
        	    				selected += 10;
        	    			else
        	    				selected += 1;
        	    				
        	    		}
        	    		
        	    		// Allow repetition
        	    		repeatJoystick(event.jbutton.button);
        	    
        	    		break;
        	    	
        	    	case PSP_BUTTON_UP:
        	    
        	    		// Update selected item
        	    		if (action == -1) {
        	    		
        	    			if (SDL_JoystickGetButton(engine->getJoystick(0), PSP_BUTTON_SQUARE))
        	    				selected -= 10;
        	    			else
        	    				selected -= 1;
        	    			
        	    		}
        	    		
        	    		// Allow repetition
        	    		repeatJoystick(event.jbutton.button);
        	    
        	    		break;
        	    		
        	    	case PSP_BUTTON_LTRIGGER:
        	    	
        	    		// Go to top
        	    		if (action == -1)
        	    			selected = 0;
        	    	
        	    		break;
        	    		
        	    	case PSP_BUTTON_RTRIGGER:
        	    	
        	    		// Go to bottom
        	    		if (action == -1)
        	    			selected = this->recentMangaList.size() - 1;
        	    	
        	    		break;
        	    		
        	    }
        	            	        	        	        	        	
        		break;
        		        
        }

    }

}

void RecentMangaState::handleLogic() {

	// BEGIN Menu Traversing Logic
	if (selected < 0)
		selected = 0;
	if ((unsigned int)selected > (this->recentMangaList.size() - 1))
		selected = this->recentMangaList.size() - 1;
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
				
		if (action == 1) {
				
			mangaListApiRequestId = MangaAPI::requestMangaList(MangaStreamAPI);
		
		} else if (action == 2) {
		
			chapterListApiRequestId = MangaAPI::requestChapterList(MangaStreamAPI, this->recentMangaList[selected].manga, this->recentMangaList[selected].mangaApiHandle);
		
		} else if (action == 3) {
		
			imageListApiRequestId = MangaAPI::requestImageList(MangaStreamAPI, this->recentMangaList[selected].manga, this->recentMangaList[selected].mangaApiHandle, this->recentMangaList[selected].chapter, this->recentMangaList[selected].chapterApiHandle);
		
		}
		
		// Reset flags
		doAction = false;
		action = 4;
	
	}
	// END Action Handling

}

void RecentMangaState::render() {

	// Variables for where to place items
	int textLoc = 74, selectedIndicatorLoc = 69, i = minList;
	
	// Colours
	SDL_Color colour1 = {239, 237, 230};
	SDL_Color colour2 = {0, 0, 0};
	
	// Holds text/description
	std::string itemText;

	// Draw MainUI
	engine->renderGlTexture(0, 0, this->textures["MainUI"]);
		
	// Draw Battery Icons
	xMPUtil::drawBatteryIcon();
	
	// Draw Time
	engine->renderGlTexture(425, 8, this->textures["Time"]);
	
	// Draw Title Text
	engine->renderGlTexture(9, 8, this->textures["TitleText"]);
	
	// Draw Action Description
	engine->renderGlTexture(19, 44, this->textures["ActionDescription"]);
	
	// Display options
	do {
	
		// Make sure to only show 4
		if ((unsigned int) i >= this->recentMangaList.size())
			break;
		
		// Get the item name
		itemText = this->recentMangaList[i].name;
						
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
			
			// Draw date/time onto screen
			ResourceManager::getFont("SansBold15")->glDraw(10, 209, this->recentMangaList[i].date.c_str());
			
			// Draw item description onto screen
			ResourceManager::getFont("SansBold15")->glDraw(10, 232, "Read %s.", itemText.c_str());
			
			// Set active item colour
			ResourceManager::getFont("SansBold15")->setColour(colour2);
						
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
	
	if (mangaListApiRequestId != -1 || chapterListApiRequestId != -1 || imageListApiRequestId != -1)
		xMPUtil::drawLoadingIcon();
		
	glFlush();

}

#endif /* _RecentMangaState_CPP */
