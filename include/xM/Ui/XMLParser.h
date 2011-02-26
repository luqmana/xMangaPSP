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
 * The XML UI parser.
 * Header file.
 * 
 * @package xMangaPSP
 */

#ifndef _XMLParser_H
#define _XMLParser_H

// BEGIN Includes
#include "xM/Gfx/Graphics.h"
#include "xM/Gfx/Text.h"

#include <tinyxml.h>
#include <string>
#include <vector>
// END Includes

// BEGIN Defines

// END Defines

namespace xM {

    namespace Ui {
    
        enum ElementType {
        
            NOOP,
            QUAD,
            TEXT,
            IMAGE
        
        };
    
        typedef struct {
        
            ElementType type;
            double x, y;
            unsigned int colour, shadowColour;
            double width, height;
            std::string text;
            double size;
            Gfx::Text font;
                    
        } Element;

        class XMLParser {
        private:
        
            TiXmlDocument* uiXMLFile;
            std::string file;
            std::vector<Element*> uiElements;
        
        public:
            
            /**
             * Default constructor.
             */
            XMLParser();
            
            /**
             * Destructor.
             */
            ~XMLParser();
            
            /**
             * Loads an XML ui file and parse it.
             * 
             * @param const std::string& file The file to parse.
             */
            void loadFile(const std::string& xFile);
            
            /**
             * Draws the UI described by the XML file.
             */
            void draw();
                    
        };
        
    }

}

#endif /* _XMLParser_H */
