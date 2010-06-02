/*
 * $Id: ResourceManager.cpp 86 2010-01-03 04:12:17Z chaotic@luqmanrocks.co.cc $
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
 * Resource Manager.
 * 
 * @package OneMangaPSP
 */

#ifndef _ResourceManager_CPP
#define _ResourceManager_CPP

// BEGIN Includes
#include "ResourceManager.h"
// END Includes

// All the textures
std::map <std::string, OMTexture> textures;

// All the fonts
std::map <std::string, OMText* > fonts;

/**
 * Is a texture?
 * 
 * @param std::string key Key to access texture.
 * 
 * @return bool true|false
 */
bool ResourceManager::isTexture(std::string key) {

	return (textures.find(key) != textures.end());

}

/**
 * Get a texture.
 * 
 * @param std::string key Key to access texture.
 *
 * @return OMTexture The texture.
 */
OMTexture ResourceManager::getTexture(std::string key) {

	return textures[key];
	
}
	
/**
 * Load a new texture into the ResourceManager.
 * 
 * @param std::string key Key to access texture.
 * @param SDL_Surface* surface The SDL Surface to load from.
 * @param SDL_Rect* clip[optional] The portion to use.
 * 
 * @return bool true|false Success or not.
 */
bool ResourceManager::loadTextureFromSdlSurface(std::string key, SDL_Surface *surface, SDL_Rect *clip) {

	// Null pointer check
	if (surface == NULL) {
	
		engine->logMsg("ResourceManager::loadTextureFromSdlSurface: NULL surface passed to function.");
		
		return false;
	
	}
	
	// Key already taken check
	if (key.empty() || ResourceManager::isTexture(key)) {
	
		engine->logMsg("ResourceManager::loadTextureFromSdlSurface: Key already in use or empty [%s].", key.c_str());
		
		return false;
		
	}
		
	// Try to convert to texture
	textures[key] = engine->glTextureFromSDLSurface(surface, clip);
	
	// Error check
	if (!textures[key].texture) {
	
		engine->logMsg("ResourceManager::loadTextureFromSdlSurface: Error loading OpenGL texture from SDL_Surface [%s].", key.c_str());
	
		return false;
		
	}
	
	// We got here so we good :)
	return true;

}

/**
 * Unloads a texture.
 * 
 * @param std::string key Key specifing texture.
 */
void ResourceManager::unloadTexture(std::string key) {

	if (ResourceManager::isTexture(key)) {
	
		// Free
		glDeleteTextures(1, &textures[key].texture);
		
		// Remove from textures
		textures.erase(key);
	
	}

}

/**
 * Is a font?
 * 
 * @param std::string key Key to access font.
 * 
 * @return bool true|false
 */
bool ResourceManager::isFont(std::string key) {

	return (fonts.find(key) != fonts.end());

}

/**
 * Get a font manager class.
 * 
 * @param std::string key Key to access font.
 *
 * @return OMText* The font.
 */
OMText* ResourceManager::getFont(std::string key) {

	return fonts[key];

}
	
/**
 * Load a new font into the ResourceManager.
 * 
 * @param std::string key Key to access texture.
 * @param OMText* font The font.
 * 
 * @return bool true|false Success or not.
 */
bool ResourceManager::loadFont(std::string key, OMText *font) {

	// Null pointer check
	if (font == NULL) {
	
		engine->logMsg("ResourceManager::loadFont: NULL font passed to function.");
		
		return false;
	
	}
	
	// Key already taken check
	if (key.empty() || ResourceManager::isFont(key)) {
	
		engine->logMsg("ResourceManager::loadFont: Key already in use or empty [%s].", key.c_str());
		
		return false;
		
	}

	// Add font
	fonts[key] = font;
	
	// We got here so we good :)
	return true;

}

/**
 * Unloads a font.
 * 
 * @param std::string key Key specifing font.
 */
void ResourceManager::unloadFont(std::string key) {

	if (ResourceManager::isFont(key)) {
	
		// Free
		delete fonts[key];
		
		// Remove from textures
		fonts.erase(key);
	
	}

}

/**
 * Clean up and remove all fonts, surfaces and textures.
 */
void ResourceManager::cleanUp() {

	// Cleanup fonts
	// Frees OpenGL textures
	for (std::map<std::string, OMTexture>::const_iterator iter = textures.begin(); iter != textures.end(); ++iter)
		glDeleteTextures(1, &iter->second.texture);

	// Cleanup fonts
	// Calls each fonts destructor method
	for (std::map<std::string, OMText* >::const_iterator iter = fonts.begin(); iter != fonts.end(); ++iter)
		delete iter->second;

}

#endif /* _ResourceManager_CPP */
