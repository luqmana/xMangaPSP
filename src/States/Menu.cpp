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
#include "xM/States/About.h"
#include "xM/States/MangaSelect.h"
#include "xM/States/Menu.h"
#include "xM/Net/Net.h"
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
            selected = 0;
            
            this->menuList.push_back("Read Manga");
            this->menuList.push_back("Recent Manga");
            this->menuList.push_back("Bookmarks");
            this->menuList.push_back("Options");
            this->menuList.push_back("About");
            this->menuList.push_back("Quit");
            
            extraElements = new Ui::ExtraElements;
            
            lInfo.selected = &this->selected;
            lInfo.list = &this->menuList;

            parser.registerCustomElementHandler("list", extraElements, (void*)&lInfo);
            parser.registerCustomElementHandler("bouncyBox", extraElements);
            parser.parseFile("ui/menu.xml");
                        
            // Create our local mailbox
            localBox = sceKernelCreateMbx("MenuStateBox", 0, NULL);
            
            // Setup the defaults for the message struct
            msg = new Manga::APIMessage;
            SceKernelMsgPacket hdr = {0};
            msg->header = hdr;
            msg->returnBox = &localBox;
                       
			if (!Net::isConnected())                        
            	Ui::Dialog::net();
                                                            
        }

        /**
         * Clean up code.
         */
        void Menu::cleanUp(void) {

			sceKernelDeleteMbx(localBox);

            parser.deRegisterCustomElementHandler("list");
            parser.deRegisterCustomElementHandler("bouncyBox");
            
            delete extraElements;
            
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
        
            Manga::APIMessage* rMsg = NULL;
            
            sceKernelPollMbx(localBox, (void**)&rMsg);
                
            if (rMsg != NULL) {
            
                if (activeDialog == 2) {
                
                	// At this point
                	// switch to manga state
                	// if it went well
                	
                	if (rMsg->result) {
                	
                		Engine::StateManager::getInstance()->changeState(new MangaSelect());
                	
                	} else {
                	
                		Ui::Dialog::msg(*(std::string*)rMsg->what);                	
                		delete (std::string*)rMsg->what;
                		
                	}
                	
                    activeDialog = 0;
                    
				}
                
                return;
                
            }
                    
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
            // END Menu Traversing Logic
                            	        
	        if (doAction) {
	        
	            switch (selected) {
	            
	                case 0:
	                	                    
	                    if (activeDialog != 2) {
	                    
	                    	// Send the manga list request
	                        msg->type = Manga::RequestMangaList;
	                        sceKernelSendMbx(Manga::mangaAPIMbx, (void*)msg);
	                    
	                        // not really a dialog
	                        // but allows some control
	                        activeDialog = 2;
	                        
	                    }
	                
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
	                    	                
	                    break;
	            
	            }
	            
	            doAction = false;
	        
	        }
	        	        
        }

        /**
         * Done with the logic? Draw what's needed then.
         */
        void Menu::draw(void) {
                                
            // Draw based on XML
            parser.draw();
                        
        }
                
    }

}

#endif /* _MenuState_CPP */
