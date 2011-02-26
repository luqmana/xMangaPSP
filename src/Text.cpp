/**
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
 * Full License: GNU-LICENSE
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

/**
 * Text wrapper class for intraFont.
 * 
 * @package xMangaPSP
 */

#ifndef _Text_CPP
#define _Text_CPP

// BEGIN Includes
#include "xM/Gfx/Text.h"
#include "xM/Util/Log.h"

#include <sstream>
#include <stdarg.h>
// BEGIN Defines

// END Defines

namespace xM {

    namespace Gfx {

        /**
         * Default constructor.
         */
        Text::Text() {
               
            this->font = NULL;
        
        }
        
        /**
         * Constructor.
         *
         * @param Fonts font The font to load.
         * @param unsigned int colour Text colour.
         */
        Text::Text(Font::Fonts font, float size, unsigned int colour, unsigned int shadowColour, unsigned int loadOps, unsigned int styleOps) {
        
            this->loadFont(font, size, colour, shadowColour, loadOps, styleOps);    
        
        }
        
        /**
         * Destructor.
         */
        Text::~Text() {
        
            // Unload the font.
        
            if (this->font != NULL)
                intraFontUnload(this->font);
        
        }
        
        /**
         * Unloads the current font (if loaded) and loads new font.
         * Previous settings such as colour, shadow colour etc are lost.
         *
         * @param Fonts font Font to load.
         */
        void Text::loadFont(Font::Fonts font, float size, unsigned int colour, unsigned int shadowColour, unsigned int loadOps, unsigned int styleOps) {
        
            if (this->font != NULL)
                intraFontUnload(this->font);
                
            std::stringstream file; 
               
            switch (font) {
            
                case Font::LATIN_SANS_SERIF_REGULAR:
                case Font::LATIN_SERIF_REGULAR:
                case Font::LATIN_SANS_SERIF_ITALIC:
                case Font::LATIN_SERIF_ITALIC:
                case Font::LATIN_SANS_SERIF_BOLD:
                case Font::LATIN_SERIF_BOLD:
                case Font::LATIN_SANS_SERIF_ITALIC_BOLD:
                case Font::LATIN_SERIF_ITALIC_BOLD:
                case Font::LATIN_SANS_SERIF_REGULAR_SMALL:
                case Font::LATIN_SERIF_REGULAR_SMALL:
                case Font::LATIN_SANS_SERIF_ITALIC_SMALL:
                case Font::LATIN_SERIF_ITALIC_SMALL:
                case Font::LATIN_SANS_SERIF_BOLD_SMALL:
                case Font::LATIN_SERIF_BOLD_SMALL:
                case Font::LATIN_SANS_SERIF_ITALIC_BOLD_SMALL:
                case Font::LATIN_SERIF_ITALIC_BOLD_SMALL:
                
                    file << "flash0:/font/ltn" << font << ".pgf";
                                                        
                    break;
                                
                case Font::JAPANESE_SJIS:
                
                    file << "flash0:/font/jpn0.pgf";
                    
                    loadOps |= INTRAFONT_STRING_SJIS;
                    
                    break;
                    
                case Font::JAPANESE_UTF8:
                
                    file << "flash0:/font/jpn0.pgf";
                    
                    loadOps |= INTRAFONT_STRING_UTF8;
                
                    break;
                    
                case Font::KOREAN_UTF8:
                
                    file << "flash0:/font/kr0.pgf";
                    
                    loadOps |= INTRAFONT_STRING_UTF8;
                
                    break;
            
                case Font::SYMBOLS: // May not be available on all systems
                
                    file << "flash0:/font/arib.pgf";
                
                    break;
            
                case Font::CHINESE:
                
                    file << "flash0:/font/gb3s1518.pgf";
                
                    break;
            
            }
            
            // Load the font
            this->font = intraFontLoad(file.str().c_str(), loadOps);
                        
            if (!this->font) {
            
                if (__xM_DEBUG)
                    Util::logMsg("Text::loadFont — Unable to load font.");
                    
                this->font = NULL;
            
                return;
                
            }
            
            intraFontSetStyle(this->font, size, colour, shadowColour, styleOps);
                    
        }
        
        /**
         * Draw the text beginning at position (x, y).
         * 
         * @param float x X position.
         * @param float y Y position.
         * @param const char* text The text to draw.
         */
        void Text::draw(float x, float y, const char* text, ...) {
        
            if (this->font == NULL)
                return;
                
            // Arguments
            va_list options;

            // Formatted string
            char buffer[512];

            // Initialize option list
            va_start(options, text);

            // Format text
            vsnprintf(buffer, (size_t) sizeof (buffer), text, options);
                
            intraFontPrint(this->font, x, y, buffer);
            
            // Quit option list
            va_end(options);
            
            // Because intraFont messes them up
            resetRenderStates();
        
        }
        
        /**
         * Draw the text beginning at position (x, y) with line wrapping.
         * 
         * @param float x X position.
         * @param float y Y position.
         * @param float width Maximum width before automatic linebreak.
         * @param const char* text The text to draw.
         */
        void Text::drawColumn(float x, float y, float width, const char* text, ...) {
        
            if (this->font == NULL)
                return;
                
            // Arguments
            va_list options;

            // Formatted string
            char buffer[512];

            // Initialize option list
            va_start(options, text);

            // Format text
            vsnprintf(buffer, (size_t) sizeof (buffer), text, options);
                
            intraFontPrintColumn(this->font, x, y, width, buffer);
            
            // Quit option list
            va_end(options);
                            
            // Because intraFont messes them up
            resetRenderStates();
        
        }
        
        /**
         * Enable utf8 encoding.
         */
        void Text::setEncodingToUtf8() {
        
            if (this->font == NULL)
                return;
                
            intraFontSetEncoding(this->font, INTRAFONT_STRING_UTF8);
        
        }
        
        /**
         * Returns a pointer to the intraFont object.
         *
         * @return intraFont* The pointer.
         */
        intraFont* Text::getFont() {
        
            return this->font;
        
        }

    }

}

#endif /* _Text_CPP */
