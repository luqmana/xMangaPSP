/*
 * $Id: OMText.cpp 81 2009-12-29 02:58:45Z chaotic@luqmanrocks.co.cc $
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
 * Text handler.
 * 
 * @package OneMangaPSP
 */

#ifndef _OMText_CPP
#define _OMText_CPP

// BEGIN Includes
#include "OMText.h"

#include <stdarg.h>
// END Includes

/**
 * Constructor.
 * 
 * @access public
 * 
 * @param const std::string& fontFile The font file.
 * @param int size Point size.
 */
OMText::OMText(const std::string& fontFile, int size) {

	// Load font
	font = TTF_OpenFont(fontFile.c_str(), size);
	
	// Error checking
	if (!font)
		engine->logMsg("OMText::OMText: Unable to load font [%s:%s].", fontFile.c_str(), TTF_GetError());
		
	// Set default colour
	SDL_Color defaultColour = {0, 0, 0};	
	this->setColour(defaultColour);

}

/**
 * Constructor.
 * 
 * @access public
 * 
 * @param const std::string& pakFile The pak file.
 * @param const std::string& fontFile The font file.
 * @param int size Point size.
 */
OMText::OMText(const std::string& pakFile, const std::string& fontFile, int size) {

	// Load font
	font = engine->ttfFontFromPak(pakFile, fontFile, size);
	
	// Error checking
	if (!font)
		engine->logMsg("OMText::OMText: Unable to load font [%s@%s:%s].", pakFile.c_str(), fontFile.c_str(), TTF_GetError());

}
		
/**
 * Constructor.
 * 
 * @access public
 * 
 * @param TTF_Font* font The font file.
 */
OMText::OMText(TTF_Font* font) {

	// Use preloaded font
	this->font = font;
	
	// Error checking
	if (!font)
		engine->logMsg("OMText::OMText: Unable to used preloaded font.");
		
	// Set default colour
	SDL_Color defaultColour = {0, 0, 0};	
	this->setColour(defaultColour);

}

/**
 * Destructor.
 */
OMText::~OMText() {

	// Free font
	TTF_CloseFont(font);
	
	font = NULL;

}

/*void OMText::glDrawExtra(int x, int y, int w, int h, const char* text, ...) {

	// Arguments
	va_list options;
	
	// Formatted string
	char textBuffer[2048];
	
	// Initialize option list
	va_start(options, text);
	
	// Format text
	vsnprintf(textBuffer, (size_t) sizeof(textBuffer), text, options);
	
	// End option list
	va_end(options);
	
	std::string textString = std::string(textBuffer);

}*/

/**
 * Draw via OpenGL.
 * 
 * @access public
 * 
 * @param int x X position.
 * @param int y Y position.
 * @param const char* text The text to draw.
 */
void OMText::glDraw(int x, int y, const char* text, ...) {

	// Arguments
	va_list options;
	
	// Formatted string
	char textBuffer[2048];
	
	// Initialize option list
	va_start(options, text);
	
	// Format text
	vsnprintf(textBuffer, (size_t) sizeof(textBuffer), text, options);

	// Create texture
	OMTexture textTexture = engine->ttfTexture(textBuffer, this->font, this->colour);

	// Render onto screen
	engine->renderGlTexture(x, y, textTexture);
	
	// Free resources
	glDeleteTextures(1, &textTexture.texture);
	
	// End option list
	va_end(options);

}

/**
 * Get text texture.
 * 
 * @access public
 * 
 * @param const char* text The text to draw.
 */
OMTexture OMText::getGlTexture(const char* text, ...) {

	// Arguments
	va_list options;
	
	// Formatted string
	char textBuffer[2048];
	
	// Initialize option list
	va_start(options, text);
	
	// Format text
	vsnprintf(textBuffer, (size_t) sizeof(textBuffer), text, options);

	// Create texture
	return engine->ttfTexture(textBuffer, this->font, this->colour);

}
		
/**
 * Draw via SDL.
 * 
 * @access public
 * 
 * @param int x X position.
 * @param int y Y position.
 * @param const char* text The text to draw.
 */
void OMText::sdlDraw(int x, int y, const char* text, ...) {

	// Arguments
	va_list options;
	
	// Formatted string
	char textBuffer[2048];
	
	// Initialize option list
	va_start(options, text);
	
	// Format text
	vsnprintf(textBuffer, (size_t) sizeof(textBuffer), text, options);

	// Create surface
	SDL_Surface* textSurface = TTF_RenderText_Blended(this->font, textBuffer, this->colour);
	
	// End option list
	va_end(options);
	
	if (textSurface == NULL) {
	
		engine->logMsg("OMText::sdlDraw: Unable to create text surface [%s].", TTF_GetError());
		
		return;
	
	}
		
	// Draw onto screen
	engine->sdlApplySurfaceToScreen(x, y, textSurface);
	
	// Free resources
	SDL_FreeSurface(textSurface);

}

/**
 * Get text surface.
 * 
 * @access public
 * 
 * @param const char* text The text to draw.
 */
SDL_Surface* OMText::getSDLSurface(const char* text, ...) {

	// Arguments
	va_list options;
	
	// Formatted string
	char textBuffer[2048];
	
	// Initialize option list
	va_start(options, text);
	
	// Format text
	vsnprintf(textBuffer, (size_t) sizeof(textBuffer), text, options);

	// Create surface
	SDL_Surface* textSurface = TTF_RenderText_Blended(this->font, textBuffer, this->colour);
	
	// End option list
	va_end(options);
	
	if (textSurface == NULL) {
	
		engine->logMsg("OMText::sdlDraw: Unable to create text surface [%s].", TTF_GetError());
		
		return NULL;
	
	}
		
	return textSurface;

}

/**
 * Set text colour.
 * 
 * @access public
 * 
 * @param SDL_Color colour Text colour.
 */
void OMText::setColour(SDL_Color colour) {

	this->colour = colour;

}

#endif /* _OMText_CPP */
