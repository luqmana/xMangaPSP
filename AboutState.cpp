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
 * AboutState Class.
 * 
 * @package xMangaPSP
 */

#ifndef _AboutState_CPP
#define _AboutState_CPP

// BEGIN Includes
#include "xMangaPSP.h"
#include "States/AboutState.h"
#include "States/MenuState.h"
#include "xMPUtil.h"
// END Includes

void AboutState::init() {

	// Start Credits Music
	engine->playBGMusic("credits.mp3", true);

	xMStrArray tempMap;
	
	tempMap["contributer"] = "Chaotic(X[SinZ])";
	tempMap["contribution"] = "The creator.";
	
	creditItems.push_back(tempMap);
	tempMap.clear();
	
	tempMap["contributer"] = "The Guys at ##psp-programming";
	tempMap["contribution"] = "All they help they provided.";
	
	creditItems.push_back(tempMap);
	tempMap.clear();
	
	tempMap["contributer"] = "MangaStream.com";
	tempMap["contribution"] = "Providing a wonderful place for reading manga online.";
	
	creditItems.push_back(tempMap);
	tempMap.clear();
	
	tempMap["contributer"] = "The Mangakas/Scanlators";
	tempMap["contribution"] = "Bringing us the manga.";
	
	creditItems.push_back(tempMap);
	tempMap.clear();
	
	tempMap["contributer"] = "Th3man / Baba";
	tempMap["contribution"] = "Providing me a PSP[go] to test with.";
	
	creditItems.push_back(tempMap);
	tempMap.clear();
	
	tempMap["contributer"] = "PSPSDK Devs";
	tempMap["contribution"] = "Making the necessary tools.";
	
	creditItems.push_back(tempMap);
	tempMap.clear();
	
	tempMap["contributer"] = "Samoi, kgsws, etc";
	tempMap["contribution"] = "For providing/helping with the signing tool.";
	
	creditItems.push_back(tempMap);
	tempMap.clear();

	// Load main UI
	xMPUtil::loadMainUI();

	// Get resource
    this->textures["MainUI"] = ResourceManager::getTexture("MainUI");
    
    SDL_Color textColour = {239, 237, 230};
	
	ResourceManager::getFont("SansBold15")->setColour(textColour);
	
	// Time	
	this->textures["Time"] = ResourceManager::getFont("SansBold15")->getGlTexture(engine->getTime().c_str());
	
	// Title text
	this->textures["TitleText"] = ResourceManager::getFont("SansBold15")->getGlTexture("About & Credits");
	
	std::stringstream info;
	info << "xMangaPSP v" << _MAJOR_VERSION << "." << _MINOR_VERSION << " Contributers:";
	
	// Info text
	this->textures["Info"] = ResourceManager::getFont("SansBold15")->getGlTexture(info.str().c_str());
	
	// Contribution Label
	this->textures["ContributionLabel"] = ResourceManager::getFont("SansBold15")->getGlTexture("Contribution:");
	
	// Set some default values
	selected = 0;
	minList = selected;
	maxList = minList + 3;
    	
}

void AboutState::shutdown() {

	// Free resources
    glDeleteTextures(1, &this->textures["TitleText"].texture);
    glDeleteTextures(1, &this->textures["Time"].texture);
    glDeleteTextures(1, &this->textures["Info"].texture);
    glDeleteTextures(1, &this->textures["ContributionLabel"].texture);
    
    // Unload main UI
	xMPUtil::unloadMainUI();
	
	// Restart BG Music
	engine->playBGMusic(bgMusicFile, true);

}

void AboutState::pause() {



}

void AboutState::resume() {



}

void AboutState::handleInput() {

	SDL_Event event;

    while (FE_PollEvent(&event)) {
        
        switch (event.type) {
        
        	case SDL_QUIT:
        	
        		engine->quitGame();
        	
        		break;
        		
        	case SDL_JOYBUTTONDOWN:
        	
        		switch (event.jbutton.button) {	
        		       	        	        	
        	    	case PSP_BUTTON_CIRCLE:
        	    	
        	    		// Go back to menu
        	    		this->changeState(new MenuState());
        	    
        	    		break;
        	    		
        	    	case PSP_BUTTON_DOWN:
        	    	
        	    		// Update selected item
        	    		selected += 1;
        	    		
        	    		// Allow repetition
        	    		repeatJoystick(event.jbutton.button);
        	    
        	    		break;
        	    	
        	    	case PSP_BUTTON_UP:
        	    
        	    		// Update selected item
        	    		selected -= 1;
        	    		
        	    		// Allow repetition
        	    		repeatJoystick(event.jbutton.button);
        	    
        	    		break;
        	    		
        	    	case PSP_BUTTON_LTRIGGER:
        	    	
        	    		// Go to top
        	    		selected = 0;
        	    	
        	    		break;
        	    		
        	    	case PSP_BUTTON_RTRIGGER:
        	    	
        	    		// Go to bottom
        	    		selected = this->creditItems.size() - 1;
        	    	
        	    		break;
        	    		
        	    }
        	            	        	        	        	        	
        		break;
        		        
        }

    }

}

void AboutState::handleLogic() {
		
	// BEGIN Menu Traversing Logic
	if (selected < 0)
		selected = 0;
	if ((unsigned int)selected > (this->creditItems.size() - 1))
		selected = this->creditItems.size() - 1;
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

}

void AboutState::render() {

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
	xMPUtil::drawBatteryIcon();
	
	// Draw Time
	engine->renderGlTexture(425, 8, this->textures["Time"]);
	
	// Draw Title Text
	engine->renderGlTexture(9, 8, this->textures["TitleText"]);
	
	// Draw Info Text
	engine->renderGlTexture(19, 44, this->textures["Info"]);
	
	// Draw Contribution label
	engine->renderGlTexture(10, 209, this->textures["ContributionLabel"]);
	
	// Display options
	do {
	
		// Make sure to only show 4
		if ((unsigned int) i >= this->creditItems.size())
			break;
		
		// Get the item name
		itemText = this->creditItems[i]["contributer"];
						
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
	
			itemDesc = std::string(this->creditItems[i]["contribution"]);
		
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
	
	glFlush();

}

#endif /* _AboutState_CPP */
