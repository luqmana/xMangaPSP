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
 * Special class which implements extra XML ui elements.
 * Header file.
 * 
 * @package xMangaPSP
 */

#ifndef _ExtraElements_H
#define _ExtraElements_H

// BEGIN Includes
#include "xM/Ui/Element.h"
#include "xM/Ui/CustomElementHandler.h"
#include "xM/Ui/XMLParser.h"

#include <string>
#include <vector>
// END Includes

// BEGIN Defines

// END Defines

namespace xM {

    namespace Ui {
    
        typedef struct {
        
            unsigned int* selected;
            std::vector<std::string>* list;
        
        } ListInfo;

        class ExtraElements : public Ui::CustomElementHandler {
        public:

            /**
             * A callback function definition to handle the setup of a custom element read from XML UI file.
             * 
             * @param XMLParser* parser Pointer to the current XML parser.
             * @param Element* customElement The custom element to be setup.
             * @param void* data[optional] Some extra data to pass to handler on init.
             */
            void initElement(Ui::XMLParser* parser, Ui::Element* customElement, void* data = NULL);
            
            /**
             * A callback function definition to handle rendering custom elements in an XML UI file.
             * 
             * @param XMLParser* parser Pointer to the current XML parser.
             * @param Element* customElement The custom element to be rendered.
             */
            void renderElement(Ui::XMLParser* parser, Ui::Element* customElement);

        };

    }

}

#endif /* _ExtraElements_H */
