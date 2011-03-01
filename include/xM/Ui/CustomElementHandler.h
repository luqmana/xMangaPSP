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
 * Abstract class to be inherited by classes wishing to handle a custom element in an XML UI file.
 * Header file.
 * 
 * @package xMangaPSP
 */

#ifndef _CustomElementHandler_H
#define _CustomElementHandler_H

// BEGIN Includes
#include "xM/Ui/XMLParser.h"
// END Includes

// BEGIN Defines

// END Defines

namespace xM {

    namespace Ui {
    
        class CustomElementHandler {        
        public:
            
            /**
             * A callback function definition to handle custom elements in an XML UI file.
             * 
             * @param XMLParser* parser Pointer to the current XML parser.
             * @param Element* customElement The custom element to be rendered.
             */
            virtual void renderElement(XMLParser* parser, Element* customElement) = 0;
                    
        };
        
    }

}

#endif /* _CustomElementHandler_H */
