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
 * OMP Utils.
 * 
 * @package xMangaPSP
 */

#ifndef _xMPUtil_CPP
#define _xMPUtil_CPP

// BEGIN Includes
#include "xMPUtil.h"
#include "ResourceManager.h"
#include <psppower.h>
// END Includes

/**
 * Bool to keep track of state of resources.
 */
bool loadedCommonResources = false;

/**
 * Variable to display loading icons.
 */
int loadingIndex = 0;

/**
 * Convenience function to load resources needed by main ui.
 */
void xMPUtil::loadMainUI() {

		SDL_Surface* image = NULL;

		// BEGIN MainUI loading
		
	    // Load surface
		image = engine->sdlSurfaceFromPakFile("Res.xmr", "MainUI.png");
		
		// Error check
		if (image == NULL)
			engine->logMsg("xMPUtil: Unable to load SDL_Surface from Pak file. [MainUI][%s]", SDL_GetError());
					
		// Load surface into resource manager
		if (!ResourceManager::loadTextureFromSdlSurface("MainUI", image, NULL))
			engine->logMsg("xMPUtil: Unable to load surface into Resource Manager [MainUI].");
		
		// Free resources
	    SDL_FreeSurface(image);
	    	    
	    // END MainUI loading

}

/**
 * Convenience function to unload resources needed by main ui.
 */
void xMPUtil::unloadMainUI() {

	ResourceManager::unloadTexture("MainUI");

}

/**
 * Convenience function to load some common resources.
 */
void xMPUtil::loadCommonResources() {

		// Only load if not otherwise loaded
		if (loadedCommonResources)
			return;

		SDL_Surface* image;
	    
	    // BEGIN Battery loading
	    
	    // Load surface
		image = engine->sdlSurfaceFromPakFile("Res.xmr", "BatteryIcons.png");
		
		// Error check
		if (image == NULL)
			engine->logMsg("xMPUtil: Unable to load SDL_Surface from Pak file. [BatteryIcons][%s]", SDL_GetError());
			
		// Load surface into resource manager [Battery Icons]
		if (!ResourceManager::loadTextureFromSdlSurface("BatteryIcons", image, NULL))
			engine->logMsg("xMPUtil: Unable to load surface into Resource Manager [BatteryIcons].");
		
		// Free resources
	    SDL_FreeSurface(image);
	    // END Battery loading
	    
	    // BEGIN Loading Icon loading
	    // Load surface
		image = engine->sdlSurfaceFromPakFile("Res.xmr", "LoadingIcons.png");
		
		// Error check
		if (image == NULL)
			engine->logMsg("xMPUtil: Unable to load SDL_Surface from Pak file. [LoadingIcons][%s]", SDL_GetError());

		// Load surface into resource manager [Loading Icons]
		if (!ResourceManager::loadTextureFromSdlSurface("LoadingIcons", image, NULL))
			engine->logMsg("xMPUtil: Unable to load surface into Resource Manager [LoadingIcons].");
		
		// Free resources
	    SDL_FreeSurface(image);	    
	    // END Loading Icon loading
	    
	    // BEGIN Font loading
	   	
	    // Load font
	    xMText *font = new xMText("SansBold.ttf", 15);
	        
	    // Put font in Resource Manager
	    if (!ResourceManager::loadFont("SansBold15", font))
	    	engine->logMsg("xMPUtil: Unable to load font into Resource Manager [SansBold15].");
	    
	    // END Font loading
	    
	    loadedCommonResources = true;

}

/**
 * Convenience function to draw battery icon.
 */
void xMPUtil::drawBatteryIcon() {

	// Get Battery icons
	xMTexture texture = ResourceManager::getTexture("BatteryIcons");
	
	// Calculate positions
	GLfloat minX = 0.0;
	GLfloat minY = 0.0;
	GLfloat maxX = 0.0;
	GLfloat maxY = 0.0;
	int x = 403, y = 10;
	
	// Get battery level
	int batteryLevel = scePowerGetBatteryLifePercent();
	
	if (batteryLevel >= 0 && batteryLevel <= 11) {
	
		minX = 36.0 / texture.pw;
		maxX = (GLfloat) 54 / texture.pw;
		minY = 0.0 / texture.ph;
		maxY = (GLfloat) 13 / texture.ph;
		
	} else if (batteryLevel >= 12 && batteryLevel <= 24) {
	
		minX = 18.0 / texture.pw;
		maxX = (GLfloat) 36 / texture.pw;
		minY = 13.0 / texture.ph;
		maxY = (GLfloat) 26 / texture.ph;
		
	} else if (batteryLevel >= 25 && batteryLevel <= 49) {
	
		minX = 18.0 / texture.pw;
		maxX = (GLfloat) 36 / texture.pw;
		minY = 0.0 / texture.ph;
		maxY = (GLfloat) 13 / texture.ph;
		
	} else if (batteryLevel >= 50 && batteryLevel <= 74) {
	
		minX = 0.0 / texture.pw;
		maxX = (GLfloat) 18 / texture.pw;
		minY = 13.0 / texture.ph;
		maxY = (GLfloat) 26 / texture.ph;
		
	} else if (batteryLevel >= 75 && batteryLevel <= 100) {
	
		minX = 0.0 / texture.pw;
		maxX = (GLfloat) 18 / texture.pw;
		minY = 0.0 / texture.ph;
		maxY = (GLfloat) 13 / texture.ph;
		
	}
	
	// Enable 2D Textures
	glEnable(GL_TEXTURE_2D);
	
	glBindTexture(GL_TEXTURE_2D, texture.texture);
	
	glBegin(GL_QUADS);
        
		glTexCoord2f(minX, minY); glVertex2i(x, y);
		glTexCoord2f(maxX, minY); glVertex2i(x + 18, y);      
		glTexCoord2f(maxX, maxY); glVertex2i(x + 18, y + 13);
		glTexCoord2f(minX, maxY); glVertex2i(x, y + 13);
		        
	glEnd();
	
	// Display charging icon if necessary
	if (scePowerIsBatteryCharging()) {
	
		x = 385;
		y = 10;
		minX = 36.0 / texture.pw;
		maxX = (GLfloat) 54 / texture.pw;
		minY = 13.0 / texture.ph;
		maxY = (GLfloat) 26 / texture.ph;
	
		glBegin(GL_QUADS);
        
			glTexCoord2f(minX, minY); glVertex2i(x, y);
			glTexCoord2f(maxX, minY); glVertex2i(x + 18, y);      
			glTexCoord2f(maxX, maxY); glVertex2i(x + 18, y + 13);
			glTexCoord2f(minX, maxY); glVertex2i(x, y + 13);
		        
		glEnd();
	
	}
	
	// Disable 2D Textures
	glDisable(GL_TEXTURE_2D);

}

/**
 * Draw the loading indicator.
 * 
 * @param int x[optional] X position.
 * @param int y[optional] Y position.
 * @param bool withBG Draw BG?
 */
void xMPUtil::drawLoadingIcon(int x, int y, bool withBG) {
	
	// Restart
	if (loadingIndex > 11)
		loadingIndex = 0;
		
	// Get texture	
	xMTexture texture = ResourceManager::getTexture("LoadingIcons");
	
	// Calculate positions
	GLfloat minX = 0.0;
	GLfloat minY = 0.0;
	GLfloat maxX = 0.0;
	GLfloat maxY = 0.0;
	
	if (loadingIndex == 0 || loadingIndex == 4 || loadingIndex == 8) {
	
		minX = 0.0 / texture.pw;
		maxX = (GLfloat) 32 / texture.pw;
		
	} else if (loadingIndex == 1 || loadingIndex == 5 || loadingIndex == 9) {
	
		minX = 32.0 / texture.pw;
		maxX = (GLfloat) 64 / texture.pw;
		
	} else if (loadingIndex == 2 || loadingIndex == 6 || loadingIndex == 10) {
	
		minX = 64.0 / texture.pw;
		maxX = (GLfloat) 96 / texture.pw;
		
	} else if (loadingIndex == 3 || loadingIndex == 7 || loadingIndex == 11) {
	
		minX = 96.0 / texture.pw;
		maxX = (GLfloat) 128 / texture.pw;
		
	}
	
	if (loadingIndex >= 0 && loadingIndex <= 3) {
	
		minY = 0.0 / texture.ph;
		maxY = (GLfloat) 32 / texture.ph;
		
	} else if (loadingIndex >= 4 && loadingIndex <= 7) {
	
		minY = 32.0 / texture.ph;
		maxY = (GLfloat) 64 / texture.ph;
		
	} else if (loadingIndex >= 8 && loadingIndex <= 11) {
	
		minY = 64.0 / texture.ph;
		maxY = (GLfloat) 96 / texture.ph;
		
	}
						
	glColor4f(1.0, 1.0, 1.0, 0.8);
						
	// Enable 2D Textures
	glEnable(GL_TEXTURE_2D);
	
	glBindTexture(GL_TEXTURE_2D, texture.texture);
	
	// Draw BG [If necessary]
	if (withBG) {
			
		glBegin(GL_QUADS);
		
			glTexCoord2f(0.0 / texture.pw, 96.0 / texture.ph); glVertex2i(x, y);
			glTexCoord2f(64.0 / texture.pw, 96.0 / texture.ph); glVertex2i(x + 64, y);      
			glTexCoord2f(64.0 / texture.pw, 160.0 / texture.ph); glVertex2i(x + 64, y + 64);
			glTexCoord2f(0.0 / texture.pw, 160.0 / texture.ph); glVertex2i(x, y + 64);
		
		glEnd();
		
		x += 16;
		y += 16;
		
	}
	
	glBegin(GL_QUADS);
        
		glTexCoord2f(minX, minY); glVertex2i(x, y);
		glTexCoord2f(maxX, minY); glVertex2i(x + 32, y);      
		glTexCoord2f(maxX, maxY); glVertex2i(x + 32, y + 32);
		glTexCoord2f(minX, maxY); glVertex2i(x, y + 32);
		        
	glEnd();
	
	// Disable 2D Textures
	glDisable(GL_TEXTURE_2D);
	
	glColor4f(1.0, 1.0, 1.0, 1.0);
	
	// Increment counter	
	loadingIndex++;

}

#endif /* _xMPUtil_CPP */
