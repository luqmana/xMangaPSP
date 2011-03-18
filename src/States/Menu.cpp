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
#include "xM/Engine/FileManager.h"
#include "xM/Engine/InputManager.h"
#include "xM/Engine/ResourceManager.h"
#include "xM/Engine/StateManager.h"
#include "xM/Net/Net.h"
#include "xM/States/About.h"
#include "xM/States/Menu.h"
#include "xM/Ui/Dialogs.h"
#include "xM/Util/Log.h"
#include "xM/Util/Timer.h"
#include "xM/Util/Utils.h"
// END Includes

namespace xM {

    namespace States {

        /**
         * Start up code.
         */
        void Menu::init(void) {
        
            // Cache it for the about state
            Engine::ResourceManager::getInstance()->getImage("genesis.png");
                                
            doAction = false;
            
            activeDialog = 0;

            parser.registerCustomElementHandler("menuList", this);
            parser.registerCustomElementHandler("bouncyBox", this);
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
            parser.deRegisterCustomElementHandler("bouncyBox");
            
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
                
            if (iM->pressed(PSP_CTRL_CROSS))
                doAction = true;
            
        }

        /**
         * Now do something with the data we got from events and what not.
         */
        void Menu::handleLogic(void) {
        
            if (activeDialog == 1) {
	        
	            if (Ui::Dialog::getMsgDialogResult() == Ui::Dialog::RESPONSE_YES)
	                Engine::StateManager::getInstance()->popState();
	                
	            activeDialog = 0;
	            
	        }
                            
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
	        
	        if (doAction) {
	        
	            switch (selected) {
	            
	                case 0:
	                
	                    break;
	                    
	                case 1:
	                
	                    break;
	                    
	                case 2:
	                
	                    break;
	                    
	                case 3:
	                
	                    break;
	                 
	                // About    
	                case 4:
	                
	                    doAction = false;
	                    Engine::StateManager::getInstance()->pushState(new About());
	                
	                    break;
	                    
	                // Quit
	                case 5:
	                
	                    Ui::Dialog::msg("Do you want quit xMangaPSP?", true);
	                    activeDialog = 1;
	                    doAction = false;
	                
	                    break;
	            
	            }
	        
	        }
	        	        
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
            
            } else if (customElement->name == "bouncyBox") {

                customElement->colour = (customElement->attributes.find("colour") == customElement->attributes.end()) ? Gfx::Colour::GRAY : Gfx::colourFromString(customElement->attributes["colour"]);
                customElement->width = (customElement->width == 0) ? 50 : customElement->width;
                customElement->height = (customElement->height == 0) ? 50 : customElement->height;
                
                customElement->rotate = 0;
                
                // Not actually size, but a modifier for the rotatation
                customElement->size = ((rand() % 4 + 1) % 2) ? 1 : -1;
                
                customElement->timer = new Util::Timer;
                customElement->timer->start();
                
                // now these are actually velocities, but stored in offset variable
                customElement->offsetX = (rand() % 4 + 1);
                customElement->offsetY = (rand() % 4 + 1);
            
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
                    
                    for (unsigned int k = 0; k < customElement->children.size(); k++) {
                    
                        if (this->minList > 0 && customElement->children[k]->attributes["whence"] == "scrollup") {
                        
                            parser->renderElement(customElement->children[k]);
                        
                        } else if (this->maxList < (this->menuList.size() - 1) && customElement->children[k]->attributes["whence"] == "scrolldown") {
                        
                            parser->renderElement(customElement->children[k]);
                        
                        }
                        	            
                    }
                    	                	
	                x += customElement->offsetX;
	                y += customElement->offsetY;
	                i++;

                } while (i <= this->maxList);
                
            }  else if (customElement->name == "bouncyBox") {
            
                customElement->x += customElement->offsetX;
                customElement->y += customElement->offsetY;
                                
                customElement->rotate -= ((float)(rand() % 3 + 1) * customElement->timer->getDeltaTicks(true)) * customElement->size;
                
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
                
                // Let it render a quad
                customElement->type = Ui::QUAD;
                parser->renderElement(customElement);
                customElement->type = Ui::CUSTOM;
            
            }
        
        }

    }

}

#endif /* _MenuState_CPP */
