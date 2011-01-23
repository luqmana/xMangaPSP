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
 * ChapterSelectState Class.
 * 
 * @package xMangaPSP
 */

#ifndef _ChapterSelectState_CPP
#define _ChapterSelectState_CPP

// BEGIN Includes
#include "xMangaPSP.h"
#include "States/ChapterSelectState.h"
#include "States/MangaSelectState.h"
#include "States/ImageSelectState.h"
#include "BookmarkManager.h"
#include "xMPUtil.h"
// END Includes

void ChapterSelectState::init() {

	// Set chapter list
	chapterList = MangaAPI::getChapterList();

	// Load main UI
	xMPUtil::loadMainUI();

	// Get resource
    this->textures["MainUI"] = ResourceManager::getTexture("MainUI");
    
    SDL_Color textColour = {239, 237, 230};
	
	ResourceManager::getFont("SansBold15")->setColour(textColour);
	
	// Time	
	this->textures["Time"] = ResourceManager::getFont("SansBold15")->getGlTexture(engine->getTime().c_str());
	
	// Title text
	this->textures["TitleText"] = ResourceManager::getFont("SansBold15")->getGlTexture("Chapters (%d)", this->chapterList.list.size());
	
	// Manga name
	this->textures["MangaName"] = ResourceManager::getFont("SansBold15")->getGlTexture(this->chapterList.manga.c_str());
		
	// Action description
	this->textures["ActionDescription"] = ResourceManager::getFont("SansBold15")->getGlTexture("Select a chapter:");
	
	// Set some default values
	selected = 0;
	minList = selected;
	maxList = minList + 3;
	doAction = false;
	mangaApiRequestId = -1;
	
}

void ChapterSelectState::shutdown() {

	// Free resources
    glDeleteTextures(1, &this->textures["TitleText"].texture);
    glDeleteTextures(1, &this->textures["Time"].texture);
    glDeleteTextures(1, &this->textures["ActionDescription"].texture);
    glDeleteTextures(1, &this->textures["MangaName"].texture);
    
    // Unload main UI
	xMPUtil::unloadMainUI();

}

void ChapterSelectState::pause() {



}

void ChapterSelectState::resume() {



}

void ChapterSelectState::handleInput() {

   SDL_Event event;

    while (FE_PollEvent(&event)) {
        
        switch (event.type) {
        
        	case SDL_QUIT:
        	
        		engine->quitGame();
        	
        		break;
        		
        	case SDL_xMANGAAPIEVENT:
        	
        		// Check for corresponding event
				if ((int)event.user.data1 == mangaApiRequestId) {
				
					// Error/Success checking
					if ((int)event.user.code == xMangaApiError) {
					
						engine->logMsg("MangaSelectState: Manga API error! [%s]", MangaAPI::getError().c_str());
						engine->showPspMsgDialog("Unable to load Image list. Please verify your internet connection works and try restarting the app.", false);
						
					} else if ((int)event.user.code == xMangaApiSuccess) {
					
						printf("Loaded image list!\n");
						mangaApiRequestId = -1;
						this->changeState(new ImageSelectState());
						
					}
				
				}
        	
        		break;
        		
        	case SDL_JOYBUTTONDOWN:
        	
        		switch (event.jbutton.button) {
        		
        			case PSP_BUTTON_SELECT:
        			
        				BookmarkManager::addBookmark(this->chapterList.manga, "manga", this->chapterList.mangaApiHandle);
        				
        				engine->showPspMsgDialog("Added bookmark.");
        			
        				break;
        		
        			case PSP_BUTTON_CIRCLE:
        			
        				// Back to manga select
        				if (mangaApiRequestId == -1)
        					this->changeState(new MangaSelectState());
        			
        				break;
        		       	        	        	
					case PSP_BUTTON_CROSS:
					
						// Set doAction flag
						if (mangaApiRequestId == -1)
							doAction = true;
					
						break;
        		       	        	        	
        	    	case PSP_BUTTON_DOWN:
        	    	
        	    		// Update selected item
        	    		if (mangaApiRequestId == -1) {
        	    		
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
        	    		if (mangaApiRequestId == -1) {
        	    		
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
        	    		if (mangaApiRequestId == -1)
        	    			selected = 0;
        	    	
        	    		break;
        	    		
        	    	case PSP_BUTTON_RTRIGGER:
        	    	
        	    		// Go to bottom
        	    		if (mangaApiRequestId == -1)
        	    			selected = this->chapterList.list.size() - 1;
        	    	
        	    		break;
        	    		
        	    }
        	            	        	        	        	        	
        		break;
        		        
        }

    }

}

void ChapterSelectState::handleLogic() {

	// BEGIN Menu Traversing Logic
	if (selected < 0)
		selected = 0;
	if ((unsigned int)selected > (this->chapterList.list.size() - 1))
		selected = this->chapterList.list.size() - 1;
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
		
		// Load Image List		
		mangaApiRequestId = MangaAPI::requestImageList(MangaStreamAPI, this->chapterList.manga, this->chapterList.mangaApiHandle, this->chapterList.list[selected].name, this->chapterList.list[selected].apiHandle);
		
		// Reset flag
		doAction = false;
	
	}
	// END Action Handling

}

void ChapterSelectState::render() {

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
	
	// Draw Manga Name
	engine->renderGlTexture(10, 209, this->textures["MangaName"]);
	
	// Display options
	do {
	
		// Make sure to only show 4
		if ((unsigned int) i >= this->chapterList.list.size())
			break;
		
		// Get the item name
		itemText = this->chapterList.list[i].name;
						
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
	
	if (mangaApiRequestId != -1)
		xMPUtil::drawLoadingIcon();
		
	glFlush();

}

#endif /* _ChapterSelectState_CPP */
