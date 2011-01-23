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
 * MangaViewState Class.
 * 
 * @package xMangaPSP
 */

#ifndef _MangaViewState_CPP
#define _MangaViewState_CPP

// BEGIN Includes
#include "xMangaPSP.h"
#include "States/MangaViewState.h"
#include "States/ImageSelectState.h"
#include "xMPUtil.h"
#include "BookmarkManager.h"
// END Includes

MangaViewState::MangaViewState(int imageIndex) {

	this->imageIndex = imageIndex;

}

void MangaViewState::init() {

	// Set image list
	imageList = MangaAPI::getImageList();
	
	// Set manga image
	mImage = MangaAPI::getMangaImage();
    
    SDL_Color textColour = {239, 237, 230};
	
	ResourceManager::getFont("SansBold15")->setColour(textColour);
	
	// Time	
	this->textures["Time"] = ResourceManager::getFont("SansBold15")->getGlTexture(engine->getTime().c_str());
	
	// Page of text
	this->textures["PagePath"] = ResourceManager::getFont("SansBold15")->getGlTexture("Image %d of %d", imageIndex + 1, this->imageList.list.size());
	
	// Set some default values
	doAction = false;
	action = -1;
	mangaApiRequestId = -1;
	posX = -(this->mImage->w - 480);
	posY = 0;
	scaleWidth = 480;
	scaleHeight = ((480 * this->mImage->h) / this->mImage->w);
	scaled = true;
	fullscreen = true;
	
	//printf("w - %d h - %d nw - %d nh - %d\n", this->mImage->w, this->mImage->h, scaleWidth, scaleHeight);
	
}

void MangaViewState::shutdown() {

	// Free resources
    glDeleteTextures(1, &this->textures["PagePath"].texture);
    glDeleteTextures(1, &this->textures["Time"].texture);
    
    // Unload main UI
	xMPUtil::unloadMainUI();

}

void MangaViewState::pause() {



}

void MangaViewState::resume() {



}

void MangaViewState::handleInput() {

	SDL_Event event;

	while (FE_PollEvent(&event)) {
        
        switch (event.type) {
        
        	case SDL_QUIT:
        	
        		engine->quitGame();
        	
        		break;
        		
        	case SDL_xMANGAAPIEVENT:
        	
        		// Check for corresponding event
				if ((action == 1 || action == 2) && (int)event.user.data1 == mangaApiRequestId) {
				
					// Error/Success checking
					if ((int)event.user.code == xMangaApiError) {
					
						engine->logMsg("MangaViewState: Manga API error! [%s]", MangaAPI::getError().c_str());
						
						if ((int)event.user.data2 == xMangaApiNoNextChapter)
							engine->showPspMsgDialog("Sorry but the next chapter isn't out yet. Check back later.", false);
						else if ((int)event.user.data2 == xMangaApiNoPrevChapter)
						    engine->showPspMsgDialog("Sorry but there is no previous chapter.", false);
						else
							engine->showPspMsgDialog("Unable to load next/prev image. Please verify your internet connection works and try restarting the app.", false);
						
						action = -1;
						
					} else if ((int)event.user.code == xMangaApiSuccess) {
											
						// Set manga image
						mImage = MangaAPI::getMangaImage();
						
						// Set image list
						imageList = MangaAPI::getImageList();
						
						// Set new index
						imageIndex = (int)event.user.data2;
						
						// Delete old texture
						glDeleteTextures(1, &this->textures["PagePath"].texture);
						
						// Make new page of texture
						this->textures["PagePath"] = ResourceManager::getFont("SansBold15")->getGlTexture("Image %d of %d", imageIndex + 1, this->imageList.list.size());
						
						// Reset positions
						posY = 0;
						if (scaled)
							posX = -(this->scaleWidth - 480);
						else
							posX = -(this->mImage->w - 480);
												
						// Re-calculate scaled height
						this->scaleHeight = ((480 * this->mImage->h) / this->mImage->w);
						
						printf("Loaded next/prev image!\n");
						
						action = -1;
						
					}
				
				}
        	
        		break;
        		
        	/*case SDL_JOYAXISMOTION:
        	
        		// Compensate for junk events
        		if ((event.jaxis.value < -4096) || (event.jaxis.value > 4096)) {
        		
        			if (event.jaxis.axis == 0) {
        			
        				printf("Moved left or right on joystick [%d].\n", event.jaxis.value / 500);
        				
        				posX -= (event.jaxis.value / 500);
        				        			
        			} else if (event.jaxis.axis == 1) {
        			
        				printf("Moved up or down on joystick [%d].\n", event.jaxis.value / 500);
        				
        				posY -= (event.jaxis.value / 500);
        			
        			}
        		
        		}
        	
        		break;*/
        		
        	case SDL_JOYBUTTONDOWN:
        	
        		switch (event.jbutton.button) {
        		
        			case PSP_BUTTON_SELECT:
        			
        				BookmarkManager::addBookmark(this->imageList.manga + ":" + this->imageList.chapter + ":" + this->imageList.list[imageIndex].image, "image", this->imageList.mangaApiHandle + ":" + this->imageList.chapterApiHandle + ":" + this->imageList.list[imageIndex].image);
        				
        				engine->showPspMsgDialog("Added bookmark.");
        			
        				break;
        		
        			case PSP_BUTTON_TRIANGLE:
        			
        				// Set to opposite
        				fullscreen = !fullscreen;
        				
        				break;
        		
        			case PSP_BUTTON_CROSS:
        			        		
						// Set to opposite		
        				scaled = !scaled;
        				
        				// Reset y pos
        				posY = 0;
        				
        				// Reset x pos
        				if (scaled)
        					posX = -(this->scaleWidth - 480);	
        				else
        					posX = -(this->mImage->w - 480);
        				
        				break;
        		
        			case PSP_BUTTON_CIRCLE:
        			
        				// Back to chapter select
        				if (action == -1)
        					this->changeState(new ImageSelectState());
        			
        				break;
						
					case PSP_BUTTON_RTRIGGER:
					
						if (action == -1) {
							// Set doAction flag
							doAction = true;
							action = 1; // 1 = Next image
						}
					
						break;
						
					case PSP_BUTTON_LTRIGGER:
					
						if (action == -1) {
							// Set doAction flag
							doAction = true;
							action = 2; // 2 = Prev image
						}
					
						break;
        		       	        	        	
        	    	case PSP_BUTTON_DOWN:
        	    	
        	    		// New position
        	    		if (SDL_JoystickGetButton(engine->getJoystick(0), PSP_BUTTON_SQUARE))
        	    			posY -= 80;
        	    		else
        	    			posY -= 60;
        	    	
        	    		// Allow repetition
        	    		repeatJoystick(event.jbutton.button);
        	    
        	    		break;
        	    	
        	    	case PSP_BUTTON_UP:
        	            	   
						// New position
						if (SDL_JoystickGetButton(engine->getJoystick(0), PSP_BUTTON_SQUARE))
							posY += 80;
						else
							posY += 60;
        	            	    		
        	    		// Allow repetition
        	    		repeatJoystick(event.jbutton.button);
        	    
        	    		break;
        	    		
        	    	case PSP_BUTTON_RIGHT:
        	    	
        	    		// New position
        	    		if (SDL_JoystickGetButton(engine->getJoystick(0), PSP_BUTTON_SQUARE))
        	    			posX -= 80;
        	    		else
        	    			posX -= 60;
        	    	
        	    		// Allow repetition
        	    		repeatJoystick(event.jbutton.button);
        	    
        	    		break;
        	    	
        	    	case PSP_BUTTON_LEFT:
        	            	   
						// New position
						if (SDL_JoystickGetButton(engine->getJoystick(0), PSP_BUTTON_SQUARE))
							posX += 80;
						else
							posX += 60;
        	            	    		
        	    		// Allow repetition
        	    		repeatJoystick(event.jbutton.button);
        	    
        	    		break;
        	    		
        	    }
        	            	        	        	        	        	
        		break;
        		        
        }

    }

}

void MangaViewState::handleLogic() {

	// BEGIN Time Handling
	if (this->textures["Time"].texture) {
	
		glDeleteTextures(1, &this->textures["Time"].texture);
		this->textures["Time"] = ResourceManager::getFont("SansBold15")->getGlTexture(engine->getTime().c_str());
		
	}
	//END Time Handling
	
	// BEGIN Action Handling	
	if (doAction == true) {
				
		if (action == 1) {
		
			// Request next image
			mangaApiRequestId = MangaAPI::requestNextImage(MangaStreamAPI, this->imageList, this->imageIndex);
		
		} else if (action == 2) {
		
			// Request prev image
			mangaApiRequestId = MangaAPI::requestPrevImage(MangaStreamAPI, this->imageList, this->imageIndex);
		
		}
		
		// Reset flag
		doAction = false;
	
	}
	// END Action Handling
	
	// BEGIN Image Position Handling
	if (posY > 0)
		posY = 0;
		
	if (posX > 0)
		posX = 0;
			
	int w = (scaled) ? (this->scaleWidth - 480) / ((1.0 / this->mImage->w) * scaleWidth) : this->mImage->w - 480;
	int h = (scaled) ? (this->scaleHeight - 272) / ((1.0 / this->mImage->h) * scaleHeight)  : this->mImage->h - 272;
				
	if (posY < -(h))
		posY = -(h);
								
	if (posX < -(w))
		posX = -(w);
	// END Image Position Handling

}

void MangaViewState::render() {
			
	unsigned int i = 0;
	
	// Scale [if necessary]
	if (scaled)
		glScalef((1.0 / this->mImage->w) * scaleWidth, (1.0 / this->mImage->h) * scaleHeight, 1.0);
	
	for (i; i < this->mImage->sections.size(); i++) {
	
		int x = this->mImage->sections[i].x + posX - 1;
		int y = this->mImage->sections[i].y + posY - 1;
						
		// TODO: Only render if within viewing area
		engine->renderGlTexture(x, y, this->mImage->sections[i].texture, true);
			
	}
	
	// Original size [if necessary]
	if (scaled)
		glScalef((1.0 / scaleWidth) * this->mImage->w, (1.0 / scaleHeight) * this->mImage->h, 1.0);
		
	if (!fullscreen) {
	
		// Color 3
    	glColor4f(0.23046875f, 0.23046875f, 0.22265625f, 0.8f);
	
		glBegin(GL_QUADS);
            		
			glVertex2i(6, 5);
			glVertex2i(475, 5);      
			glVertex2i(475, 26);
			glVertex2i(6, 26);
    
		glEnd();
		
		glColor4f(1.0, 1.0, 1.0, 0.8);
		
		// Draw Battery Icons
		xMPUtil::drawBatteryIcon();
		
		// Draw Time
		engine->renderGlTexture(425, 8, this->textures["Time"]);
		
		// Draw page of
		engine->renderGlTexture(9, 8, this->textures["PagePath"]);
		
		glColor4f(1.0, 1.0, 1.0, 1.0);
	
	}
		
	// Draw loading indicator
	if (action != -1)
		xMPUtil::drawLoadingIcon(448, 240, false);

}

#endif /* _MangaViewState_CPP */
