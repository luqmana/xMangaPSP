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
 * Header for Resource Manager
 * 
 * @package xMangaPSP
 */

#ifndef _ResourceManager_H
#define _ResourceManager_H

// BEGIN Includes
#include "xMPStandard.h"
#include "xMangaPSP.h"
#include "xMText.h"
#include <vector>
// END Includes

namespace ResourceManager {

	/**
	 * Is a texture?
	 * 
	 * @param std::string key Key to access texture.
	 * 
	 * @return bool true|false
	 */
	bool isTexture(std::string key);

	/**
	 * Get a texture.
	 * 
	 * @param std::string key Key to access texture.
	 *
	 * @return xMTexture The texture.
	 */
	xMTexture getTexture(std::string key);
	
	/**
	 * Load a new texture into the ResourceManager.
	 * 
	 * @param std::string key Key to access texture.
	 * @param SDL_Surface* surface The SDL Surface to load from.
	 * @param SDL_Rect* clip The portion to use.
	 * 
	 * @return bool true|false Success or not.
	 */
	bool loadTextureFromSdlSurface(std::string key, SDL_Surface *surface, SDL_Rect *clip);
	
	/**
	 * Unloads a texture.
	 * 
	 * @param std::string key Key specifing texture.
	 */
	void unloadTexture(std::string key);
	
	/**
	 * Is a font?
	 * 
	 * @param std::string key Key to access font.
	 * 
	 * @return bool true|false
	 */
	bool isFont(std::string key);
	
	/**
	 * Get a font manager class.
	 * 
	 * @param std::string key Key to access font.
	 *
	 * @return xMText* The font.
	 */
	xMText* getFont(std::string key);
	
	/**
	 * Load a new font into the ResourceManager.
	 * 
	 * @param std::string key Key to access texture.
	 * @param xMText* font The font.
	 * 
	 * @return bool true|false Success or not.
	 */
	bool loadFont(std::string key, xMText *font);
	
	/**
	 * Unloads a font.
	 * 
	 * @param std::string key Key specifing font.
	 */
	void unloadFont(std::string key);
	
	/**
	 * Clean up and remove all fonts, surfaces and textures.
	 */
	void cleanUp();

};

#endif /* _ResourceManager_H */
