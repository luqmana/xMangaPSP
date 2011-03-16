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
 * Type definition for an XML ui element.
 * Header file.
 * 
 * @package xMangaPSP
 */

#ifndef _Element_H
#define _Element_H

// BEGIN Includes
#include "xM/Gfx/Image.h"
#include "xM/Gfx/Text.h"

#include <string>
#include <vector>
#include <map>
// END Includes

// BEGIN Defines

// END Defines

namespace xM {

    namespace Ui {
    
        enum ElementType {
        
            NOOP,
            QUAD,
            TEXT,
            IMAGE,
            CUSTOM
        
        };
    
        typedef struct Element {
        
            ElementType type;
            std::string name;
            double x, y, offsetX, offsetY, paddingLeft, paddingRight;
            unsigned int colour, shadowColour;
            double width, height;
            std::string text;
            double size;
            Gfx::Text font;
            Gfx::Image image;
            std::map<std::string, std::string> attributes;
            
            // Really only used by custom elements
            std::vector<struct Element*> children;
                    
        } Element;
        
    }

}

#endif /* _Element_H */
