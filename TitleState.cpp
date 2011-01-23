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
 * TitleState Class.
 * 
 * @package xMangaPSP
 */

#ifndef _TitleState_CPP
#define _TitleState_CPP

// BEGIN Includes
#include "xMangaPSP.h"
#include "States/TitleState.h"
#include "States/MenuState.h"
// END Includes

TitleState::TitleState(int which) {

	this->which = which;

}

void TitleState::init() {

	SDL_Surface* image = NULL;
		
	// BEGIN Splash loading
	
	// Load surface
    if (this->which == 1)
		image = engine->sdlSurfaceFromPakFile("Res.xmr", "Splash1.png");
	else if (this->which == 2)
		image = engine->sdlSurfaceFromPakFile("Res.xmr", "Splash2.png");
    	
	// Error check
	if (image == NULL)
		engine->logMsg("MenuState: Unable to load SDL_Surface from Pak file. [Splash%d][%s]", this->which, SDL_GetError());
	
	// Convert surface to texture
    this->textures["Splash"] = engine->glTextureFromSDLSurface(image);
    
    // Error check
	if (!this->textures["Splash"].texture)
		engine->logMsg("MenuState: Error loading OpenGL texture from SDL_Surface [Splash%d].", this->which);
		
	// Free surface
	SDL_FreeSurface(image);
	
	// END Splash loading
	
	// Default values
	timer = new Timer();
	
	// Start timer
	timer->start();
	
}

void TitleState::shutdown() {

	// Free resources
	glDeleteTextures(1, &this->textures["Splash"].texture);
	
	delete timer;

}

void TitleState::pause() {



}

void TitleState::resume() {



}

void TitleState::handleInput() {

	SDL_Event event;

    while (FE_PollEvent(&event)) {
        
        switch (event.type) {
        
        	case SDL_QUIT:
        	
        		engine->quitGame();
        	
        		break;
        		
        	case SDL_JOYBUTTONDOWN:
        	
        		switch (event.jbutton.button) {	
        		       	        	        	
        	    	case PSP_BUTTON_START:
        	    	
        	    		// If start is pressed, skip to next state
        	    		
        	    		if (this->which == 1)
							this->changeState(new TitleState(2));
						else if (this->which == 2) {
						
							// Connect to internet
							engine->showPspNetDialog();
							
							// Start BG Music
							engine->playBGMusic(bgMusicFile, true);
						
							this->changeState(new MenuState());
							
						}
        	    
        	    		break;
        	    		
        	    }
        	            	        	        	        	        	
        		break;
        		        
        }

    }

}

void TitleState::handleLogic() {

	if (timer->getTicks() >= 3000) {
	
		if (this->which == 1)
			this->changeState(new TitleState(2));
		else if (this->which == 2) {
		
			// Connect to internet
			engine->showPspNetDialog();
		
			// Start BG Music
			engine->playBGMusic(bgMusicFile, true);
		
			this->changeState(new MenuState());
			
		}
	
	}

}

void TitleState::render() {

	// Draw Splash
	engine->renderGlTexture(0, 0, this->textures["Splash"]);

	glFlush();

}

#endif /* _TitleState_CPP */
