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
 * Header for text handler class.
 * 
 * @package xMangaPSP
 */

#ifndef _xMText_H
#define _xMText_H

// BEGIN Includes
#include "xMangaPSP.h"
// END Includes

class xMText {

	public:
	
		/**
		 * Constructor.
		 * 
		 * @access public
		 * 
		 * @param const std::string& fontFile The font file.
		 * @param int size Point size.
		 */
		xMText(const std::string& fontFile, int size);
		
		/**
		 * Constructor.
		 * 
		 * @access public
		 * 
		 * @param const std::string& pakFile The pak file.
		 * @param const std::string& fontFile The font file.
		 * @param int size Point size.
		 */
		xMText(const std::string& pakFile, const std::string& fontFile, int size);
		
		/**
		 * Constructor.
		 * 
		 * @access public
		 * 
		 * @param TTF_Font* font The font file.
		 */
		xMText(TTF_Font* font);
		
		/**
		 * Destructor.
		 */
		~xMText();
		
		/**
		 * Draw via OpenGL.
		 * 
		 * @access public
		 * 
		 * @param int x X position.
		 * @param int y Y position.
		 * @param const char* text The text to draw.
		 */
		void glDraw(int x, int y, const char* text, ...);
		
		/**
 		 * Get text texture.
 		 * 
 		 * @access public
 		 * 
 		 * @param const char* text The text to draw.
 		 */
		xMTexture getGlTexture(const char* text, ...);
		
		/**
		 * Draw via SDL.
		 * 
		 * @access public
		 * 
		 * @param int x X position.
		 * @param int y Y position.
		 * @param const char* text The text to draw.
		 */
		void sdlDraw(int x, int y, const char* text, ...);
		
		/**
 		 * Get text surface.
		 * 
 		 * @access public
		 * 
		 * @param const char* text The text to draw.
		 */
		SDL_Surface* getSDLSurface(const char* text, ...);
		
		/**
		 * Set text colour.
		 * 
		 * @access public
		 * 
		 * @param SDL_Color colour Text colour.
		 */
		void setColour(SDL_Color colour);
		
	private:
	
		/**
		 * Font reference.
		 */
		TTF_Font* font;
		
		/**
		 * Colour in which to draw text.
		 */
		SDL_Color colour;

};

#endif /* _xMText_H */
