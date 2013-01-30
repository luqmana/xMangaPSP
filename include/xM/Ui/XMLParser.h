/**
 * This file is part of the xMangaPSP application.
 *
 * Copyright (C) Luqman Aden <www.luqman.ca>.
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
#include "xM/Gfx/Image.h"
#include "xM/Gfx/Text.h"
#include "xM/Ui/Element.h"
#include "xM/Ui/CustomElementHandler.h"

#include <tinyxml.h>
#include <string>
#include <vector>
#include <map>
// END Includes

// BEGIN Defines

// END Defines

namespace xM {

    namespace Ui {
    
        class XMLParser {
        private:
        
            TiXmlDocument* uiXMLFile;
            std::string file;
            std::vector<Element*> uiElements;
            std::map<std::string, CustomElementHandler*> customElementHandlers;
            std::map<std::string, void*> customElementHandlersData;
            std::map<std::string, std::string> textSubstitutes;
        
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
            void parseFile(const std::string& xFile);
            
            /**
             * Parse a single XML element.
             * 
             * @param TiXmlElement* xmlElement Element to parse.
             */
            Element* parseElement(TiXmlElement* xmlElement);
            
            /**
             * Render a specific element.
             * 
             * @param Element* e Element to render.
             */
            void renderElement(Element* e);
            
            /**
             * Draws the UI described by the XML file.
             */
            void draw();
            
            /**
             * Register a custom element handler.
             * 
             * @param const std::string& element The custom element.
             * @param CustomElementHandler* handler Pointer to the handling class.
             * @param void* data[optional] Some extra data to pass to handler on init.
             */
            void registerCustomElementHandler(const std::string& element, CustomElementHandler* handler, void* data = NULL);
            
            /**
             * Deregister a custom element handler.
             * 
             * @param const std::string& element The custom element.
             */
            void deRegisterCustomElementHandler(const std::string& element);
         
            /**
             * Add a token to be replaced in text elements.
             * 
             * @param const std::string& key The token to replace.
             * @param const std::string& replace What to replace with.
             */
            void addTextSubstitute(const std::string& key, const std::string& replace);
                    
        };
        
    }

}

#endif /* _XMLParser_H */
