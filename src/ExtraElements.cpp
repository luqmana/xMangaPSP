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
 * Special class which implements extra XML ui elements.
 * 
 * @package xMangaPSP
 */

#ifndef _ExtraElements_CPP
#define _ExtraElements_CPP

// BEGIN Includes
#include "xM/Ui/ExtraElements.h"
#include "xM/Util/Timer.h"
#include "xM/Util/Utils.h"
// END Includes

namespace xM {

    namespace Ui {
        
        /**
         * A callback function definition to handle the setup of a custom element read from XML UI file.
         * 
         * @param XMLParser* parser Pointer to the current XML parser.
         * @param Element* customElement The custom element to be setup.
         * @param void* data[optional] Some extra data to pass to handler on init.
         */
        void ExtraElements::initElement(Ui::XMLParser* parser, Ui::Element* customElement, void* data) {
        
            if (customElement->name == "bouncyBox") {

                customElement->colour = (customElement->attributes.find("colour") == customElement->attributes.end()) ? Gfx::Colour::GRAY : Gfx::colourFromString(customElement->attributes["colour"]);
                customElement->width = (customElement->width == 0) ? 50 : customElement->width;
                customElement->height = (customElement->height == 0) ? 50 : customElement->height;
                
                customElement->rotate = 0;
                
                // Not actually size, but a modifier for the rotatation
                customElement->size = ((rand() % 4 + 1) % 2) ? 1 : -1;
                
                // use the void* for a timer
                customElement->custom1 = (void*)new Util::Timer;
                ((Util::Timer*)customElement->custom1)->start();
                
                // now these are actually velocities, but stored in offset variable
                customElement->offsetX = (rand() % 4 + 1);
                customElement->offsetY = (rand() % 4 + 1);
            
            } else if (Util::hasEnding(customElement->name, "list")) {
            
                if (data != NULL) {
                
                    ListInfo* lInfo = (ListInfo*)data;
                    
                    // store these pointers
                    customElement->custom1 = (void*)lInfo->selected;
                    customElement->custom2 = (void*)lInfo->list;
                
                }
                        
                // Oh the (ab)use xD
                
                // actually maxItems
                customElement->colour = (customElement->attributes.find("maxItems") == customElement->attributes.end()) ? 4 : Util::stringToInt(customElement->attributes["maxItems"]) - 1;
                
                // actually minList
                customElement->width = 0;
                
                // actually maxList
                customElement->height = customElement->width + customElement->colour;
                                            
            }
                    
        }
        
        /**
         * A callback function definition to handle custom elements in an XML UI file.
         * 
         * @param XMLParser* parser Pointer to the current XML parser.
         * @param Element* customElement The custom element to be rendered.
         */
        void ExtraElements::renderElement(Ui::XMLParser* parser, Ui::Element* customElement) {
            
            if (customElement->name == "bouncyBox") {
            
                customElement->x += customElement->offsetX;
                customElement->y += customElement->offsetY;
                                
                customElement->rotate -= ((float)(rand() % 3 + 1) * ((Util::Timer*)customElement->custom1)->getDeltaTicks(true)) * customElement->size;
                
                if ((customElement->x + customElement->width) > 480 || customElement->x < 0) {
                
                    customElement->offsetX = (rand() % 4 + 1);
                    customElement->offsetX *= (customElement->x < 0) ? 1 : -1;
                    customElement->x += customElement->offsetX; 
                    
                    // Change rotation direction
                    customElement->size *= -1;
                    
                }
                
                if ((customElement->y + customElement->height) > 272 || customElement->y < 0) {
                
                    customElement->offsetY = (rand() % 4 + 1);
                    customElement->offsetY *= (customElement->y < 0) ? 1 : -1;
                    customElement->y += customElement->offsetY; 
                    
                    // Change rotation direction
                    customElement->size *= -1;
                    
                }
                
                // Let it render as a quad
                customElement->type = Ui::QUAD;
                parser->renderElement(customElement);
                customElement->type = Ui::CUSTOM;
            
            } else if (Util::hasEnding(customElement->name, "list")) {
                                    
                unsigned int minList, maxList, maxItems;
                minList = customElement->width;
                maxList = customElement->height;
                maxItems = customElement->colour;
                                
                unsigned int selected = *((unsigned int*) customElement->custom1);
                std::vector<std::string>* list = (std::vector<std::string>*) customElement->custom2;
                                                
                // BEGIN Menu Traversing Logic
                if (selected < minList) {

	                minList = customElement->width = selected;
	                maxList = customElement->height = minList + maxItems;	
	
                }
                if (selected > maxList) {

	                maxList = customElement->height = selected;
	                minList = customElement->width = maxList - maxItems;

                }
                // END Menu Traversing Logic
            
                unsigned int i = minList;
                std::string itemText;
                int x = customElement->x;
                int y = customElement->y;
                
                do {

	                if (i >= list->size())
		                break;
	
	                // Get the item name
	                itemText = (*list)[i];
	                
	                if (i == selected) {
				
                        for (unsigned int k = 0; k < customElement->children.size(); k++) {
	                    
	                        if (customElement->children[k]->whence == "active") {
                                
                                customElement->children[k]->x = x;
                                customElement->children[k]->y = y;
                                
                                if (customElement->children[k]->type == Ui::TEXT)
                                    customElement->children[k]->text = itemText;
                                
                                parser->renderElement(customElement->children[k]);
                                    
                            }
                            	            
                        }
                    
                    } else {
				
                        for (unsigned int k = 0; k < customElement->children.size(); k++) {
	                    
	                        if (customElement->children[k]->whence == "inactive") {
                            
                                customElement->children[k]->x = x;
                                customElement->children[k]->y = y;
                                
                                if (customElement->children[k]->type == Ui::TEXT)
                                    customElement->children[k]->text = itemText;
                                
                                parser->renderElement(customElement->children[k]);
                            
                            }
                            	            
                        }
                    
                    }
                    
                    for (unsigned int k = 0; k < customElement->children.size(); k++) {
                    
                        if (minList > 0 && customElement->children[k]->whence == "scrollup") {
                        
                            parser->renderElement(customElement->children[k]);
                        
                        } else if (maxList < (list->size() - 1) && customElement->children[k]->whence == "scrolldown") {
                        
                            parser->renderElement(customElement->children[k]);
                        
                        }
                        	            
                    }
                    	                	
	                x += customElement->offsetX;
	                y += customElement->offsetY;
	                i++;

                } while (i <= maxList);
                
            }
        
        }

    }

}

#endif /* _ExtraElements_CPP */
