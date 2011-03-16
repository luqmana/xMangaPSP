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
 * Menu state class.
 * 
 * @package xMangaPSP
 */

#ifndef _MenuState_CPP
#define _MenuState_CPP

// BEGIN Includes
#include "xM/States/Menu.h"
#include "xM/Engine/FileManager.h"
#include "xM/Engine/InputManager.h"
#include "xM/Net/Net.h"
#include "xM/Ui/Dialogs.h"
#include "xM/Util/Log.h"
#include "xM/Util/Utils.h"
// END Includes

namespace xM {

    namespace States {

        /**
         * Start up code.
         */
        void Menu::init(void) {

            parser.registerCustomElementHandler("menuList", this);
            parser.parseFile("ui/menu.xml");
            
            this->menuList.push_back("Read Manga");
            this->menuList.push_back("Recent Manga");
            this->menuList.push_back("Bookmarks");
            this->menuList.push_back("Options");
            this->menuList.push_back("About");
            this->menuList.push_back("Quit");
                                                
        }

        /**
         * Clean up code.
         */
        void Menu::cleanUp(void) {

            parser.deRegisterCustomElementHandler("menuList");

        }

        /**
         * Pause state.
         */
        void Menu::pause(void) {



        }

        /**
         * Resume state.
         */
        void Menu::resume(void) {



        }

        /**
         * Poll for input, read event state etc
         */
        void Menu::handleEvents(void) {

            Engine::InputManager* iM = Engine::InputManager::getInstance();
            
            // Reload XML on-the-fly
            if (__xM_DEBUG && iM->pressed(PSP_CTRL_LTRIGGER)) {
            
                Util::logMsg("Reloading XML ui file.");
            
                parser.parseFile("ui/menu.xml");    
            
            }
            
            if (iM->pressed(PSP_CTRL_DOWN))
                selected += 1;
            else if (iM->pressed(PSP_CTRL_UP))
                selected -= 1;
            
        }

        /**
         * Now do something with the data we got from events and what not.
         */
        void Menu::handleLogic(void) {
                    
            // BEGIN Menu Traversing Logic
	        if ((signed int)this->selected < 0)
		        this->selected = 0;
	        if (this->selected > (this->menuList.size() - 1))
		        this->selected = this->menuList.size() - 1;
	        if (this->selected < this->minList) {
	
		        this->minList = this->selected;
		        this->maxList = this->minList + this->maxItems;	
		
	        }
	        if (this->selected > this->maxList) {
	
		        this->maxList = this->selected;
		        this->minList = this->maxList - this->maxItems;
	
	        }
	        // END Menu Traversing Logic

        }

        /**
         * Done with the logic? Draw what's needed then.
         */
        void Menu::draw(void) {
            
            // Draw based on XML
            parser.draw();
            
        }
        
        /**
         * A callback function definition to handle the setup of a custom element read from XML UI file.
         * 
         * @param XMLParser* parser Pointer to the current XML parser.
         * @param Element* customElement The custom element to be setup.
         */
        void Menu::initElement(Ui::XMLParser* parser, Ui::Element* customElement) {
        
            if (customElement->name == "menuList") {
        
                customElement->x = Util::stringToInt(customElement->attributes["x"]);
                customElement->y = Util::stringToInt(customElement->attributes["y"]);
                
                this->maxItems = Util::stringToInt(customElement->attributes["maxItems"]) - 1;
                this->minList = 0;
                this->selected = 0;
                this->maxList = this->minList + this->maxItems;
            
            }
                    
        }
        
        /**
         * A callback function definition to handle custom elements in an XML UI file.
         * 
         * @param XMLParser* parser Pointer to the current XML parser.
         * @param Element* customElement The custom element to be rendered.
         */
        void Menu::renderElement(Ui::XMLParser* parser, Ui::Element* customElement) {
            
            if (customElement->name == "menuList") {
            
                unsigned int i = this->minList;
                std::string itemText;
                int x = customElement->x;
                int y = customElement->y;
                
                do {

	                // Make sure to only show 4
	                if (i >= this->menuList.size())
		                break;
	
	                // Get the item name
	                itemText = this->menuList[i];
				
				    if (i == selected) {	
					
	                    // Active elements					
	                    for (unsigned int k = 0; k < customElement->children.size(); k++) {
		                
		                    if (customElement->children[k]->attributes["whence"] == "active") {
	                            
                                customElement->children[k]->x = x;
                                customElement->children[k]->y = y;
                                
                                if (customElement->children[k]->type == Ui::TEXT)
                                    customElement->children[k]->text = itemText;
                                
                                parser->renderElement(customElement->children[k]);
	                                
                            }
	                        	            
	                    }
	                
	                } else {

	                    // Inactive elements					
	                    for (unsigned int k = 0; k < customElement->children.size(); k++) {
		                
		                    if (customElement->children[k]->attributes["whence"] == "inactive") {
	                            
                                customElement->children[k]->x = x;
                                customElement->children[k]->y = y;
                                
                                if (customElement->children[k]->type == Ui::TEXT)
                                    customElement->children[k]->text = itemText;
                                
                                parser->renderElement(customElement->children[k]);
	                                
                            }
	                        	            
	                    }
	                
	                }
	
	                //printf("(%d, %d)\n", x, y);
	                x += customElement->offsetX;
	                y += customElement->offsetY;
	                i++;

                } while (i <= this->maxList);
                
            }
        
        }

    }

}

#endif /* _State_CPP */
