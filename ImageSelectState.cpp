/*
 * $Id: ImageSelectState.cpp 86 2010-01-03 04:12:17Z chaotic@luqmanrocks.co.cc $
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
 * ImageSelectState Class.
 * 
 * @package OneMangaPSP
 */

#ifndef _ImageSelectState_CPP
#define _ImageSelectState_CPP

// BEGIN Includes
#include "OneMangaPSP.h"
#include "States/ImageSelectState.h"
#include "States/ChapterSelectState.h"
#include "States/MangaViewState.h"
#include "OMPUtil.h"
#include "BookmarkManager.h"
// END Includes

void ImageSelectState::init() {

	// Set image list
	imageList = MangaAPI::getImageList();

	// Load main UI
	OMPUtil::loadMainUI();

	// Get resource
    this->textures["MainUI"] = ResourceManager::getTexture("MainUI");
    
    SDL_Color textColour = {239, 237, 230};
	
	ResourceManager::getFont("SansBold15")->setColour(textColour);
	
	// Time	
	this->textures["Time"] = ResourceManager::getFont("SansBold15")->getGlTexture(engine->getTime().c_str());
	
	// Title text
	this->textures["TitleText"] = ResourceManager::getFont("SansBold15")->getGlTexture("Images (%d)", this->imageList.list.size());
	
	// Manga name
	this->textures["MangaName"] = ResourceManager::getFont("SansBold15")->getGlTexture(this->imageList.manga.c_str());
	
	// Chapter name
	this->textures["ChapterName"] = ResourceManager::getFont("SansBold15")->getGlTexture(this->imageList.chapter.c_str());
		
	// Action description
	this->textures["ActionDescription"] = ResourceManager::getFont("SansBold15")->getGlTexture("Select an image:");
	
	// Set some default values
	selected = 0;
	minList = selected;
	maxList = minList + 3;
	doAction = false;
	mangaApiRequestId = -1;
	
}

void ImageSelectState::shutdown() {

	// Free resources
    glDeleteTextures(1, &this->textures["TitleText"].texture);
    glDeleteTextures(1, &this->textures["Time"].texture);
    glDeleteTextures(1, &this->textures["ActionDescription"].texture);
    glDeleteTextures(1, &this->textures["MangaName"].texture);
    glDeleteTextures(1, &this->textures["ChapterName"].texture);
    
    // Unload main UI
	OMPUtil::unloadMainUI();

}

void ImageSelectState::pause() {



}

void ImageSelectState::resume() {



}

void ImageSelectState::handleInput() {

   	SDL_Event event;

	while (FE_PollEvent(&event)) {
        
        switch (event.type) {
        
        	case SDL_QUIT:
        	
        		engine->quitGame();
        	
        		break;
        		
        	case SDL_OMMANGAAPIEVENT:
        	
        		// Check for corresponding event
				if ((int)event.user.data1 == mangaApiRequestId) {
				
					// Error/Success checking
					if ((int)event.user.code == OMMangaApiError) {
					
						engine->logMsg("MangaSelectState: Manga API error! [%s]", MangaAPI::getError().c_str());
						engine->showPspMsgDialog("Unable to load Images. Please verify your internet connection works and try restarting the app.", false);
						
					} else if ((int)event.user.code == OMMangaApiSuccess) {
					
						printf("Loaded images!\n");
						mangaApiRequestId = -1;
						this->changeState(new MangaViewState((int)event.user.data2));
						
					}
				
				}
        	
        		break;
        		
        	case SDL_JOYBUTTONDOWN:
        	
        		switch (event.jbutton.button) {
        		
        			case PSP_BUTTON_SELECT:
        			
        				BookmarkManager::addBookmark(this->imageList.manga + ":" + this->imageList.chapter, "chapter", this->imageList.mangaApiHandle + ":" + this->imageList.chapterApiHandle);
        				
        				engine->showPspMsgDialog("Added bookmark.");
        			
        				break;
        		
        			case PSP_BUTTON_CIRCLE:
        			
        				// Back to chapter select
        				if (mangaApiRequestId == -1)
        					this->changeState(new ChapterSelectState());
        			
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
        	    			selected = this->imageList.list.size() - 1;
        	    	
        	    		break;
        	    		
        	    }
        	            	        	        	        	        	
        		break;
        		        
        }

    }

}

void ImageSelectState::handleLogic() {

	// BEGIN Menu Traversing Logic
	if (selected < 0)
		selected = 0;
	if ((unsigned int)selected > (this->imageList.list.size() - 1))
		selected = this->imageList.list.size() - 1;
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
		
		// Pre-load some images before going on to image state
		mangaApiRequestId = MangaAPI::requestImage(OneMangaAPI, this->imageList, selected);
		
		// Reset flag
		doAction = false;
	
	}
	// END Action Handling

}

void ImageSelectState::render() {

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
	OMPUtil::drawBatteryIcon();
	
	// Draw Time
	engine->renderGlTexture(425, 8, this->textures["Time"]);
	
	// Draw Title Text
	engine->renderGlTexture(9, 8, this->textures["TitleText"]);
	
	// Draw Action Description
	engine->renderGlTexture(19, 44, this->textures["ActionDescription"]);
	
	// Draw Manga Name
	engine->renderGlTexture(10, 209, this->textures["MangaName"]);
	
	// Draw Chapter Name
	engine->renderGlTexture(10, 232, this->textures["ChapterName"]);
	
	// Display options
	do {
	
		// Make sure to only show 4
		if ((unsigned int) i >= this->imageList.list.size())
			break;
		
		// Get the item name
		itemText = this->imageList.list[i].image;
						
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
		OMPUtil::drawLoadingIcon();
			
	glFlush();

}

#endif /* _ImageSelectState_CPP */
