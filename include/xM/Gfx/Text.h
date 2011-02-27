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
 * Header file.
 * 
 * @package xMangaPSP
 */

#ifndef _Text_H
#define _Text_H

// BEGIN Includes
#include "xM/Gfx/Graphics.h"

#include <intraFont.h>
#include <vector>
// END Includes

// BEGIN Defines

// END Defines

namespace xM {

    namespace Gfx {

        namespace Font {
        
            enum Fonts {
        
                LATIN_SANS_SERIF_REGULAR,
                LATIN_SERIF_REGULAR,
                LATIN_SANS_SERIF_ITALIC,
                LATIN_SERIF_ITALIC,
                LATIN_SANS_SERIF_BOLD,
                LATIN_SERIF_BOLD,
                LATIN_SANS_SERIF_ITALIC_BOLD,
                LATIN_SERIF_ITALIC_BOLD,
                LATIN_SANS_SERIF_REGULAR_SMALL,
                LATIN_SERIF_REGULAR_SMALL,
                LATIN_SANS_SERIF_ITALIC_SMALL,
                LATIN_SERIF_ITALIC_SMALL,
                LATIN_SANS_SERIF_BOLD_SMALL,
                LATIN_SERIF_BOLD_SMALL,
                LATIN_SANS_SERIF_ITALIC_BOLD_SMALL,
                LATIN_SERIF_ITALIC_BOLD_SMALL,
                
                JAPANESE_SJIS       =   100,
                JAPANESE_UTF8       =   150,
                
                KOREAN_UTF8         =   200,
                
                SYMBOLS             =   300,
                
                CHINESE             =   400             
            
            };
            
        }

        class Text {
        private:

            intraFont* font;
            std::vector<intraFont*> altFonts;

        public:

            /**
             * Default constructor.
             */
            Text();
            
            /**
             * Constructor.
             *
             * @param Fonts font The font to load.
             * @param unsigned int colour Text colour.
             */
            Text(Font::Fonts font, float size, unsigned int colour, unsigned int shadowColour, unsigned int loadOps, unsigned int styleOps);
            
            /**
             * Destructor.
             */
            ~Text();
            
            /**
             * Unloads the current font (if loaded) and loads new font.
             * Previous settings such as colour, shadow colour etc are lost.
             *
             * @param Fonts font Font to load.
             */
            void loadFont(Font::Fonts font, float size, unsigned int colour, unsigned int shadowColour, unsigned int loadOps = 0, unsigned int styleOps = 0);
            
            /**
             * Loads an alternate font.
             * Can be called multiple times.
             *
             * @param Fonts font Font to load.
             */
            void loadAltFont(Font::Fonts font, float size, unsigned int colour, unsigned int shadowColour, unsigned int loadOps = 0, unsigned int styleOps = 0);
            
            /**
             * Draw the text beginning at position (x, y).
             * 
             * @param float x X position.
             * @param float y Y position.
             * @param const char* text The text to draw.
             */
            void draw(float x, float y, const char* text, ...);
            
            /**
             * Draw the text beginning at position (x, y) with line wrapping.
             * 
             * @param float x X position.
             * @param float y Y position.
             * @param float width Maximum width before automatic linebreak.
             * @param const char* text The text to draw.
             */
            void drawColumn(float x, float y, float width, const char* text, ...);
                        
            /**
             * Returns a pointer to the intraFont object.
             *
             * @return intraFont* The pointer.
             */
            intraFont* getFont();

        };

    }

}

#endif /* _Text_H */
