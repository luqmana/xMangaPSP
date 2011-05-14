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
#include "xM/Engine/InputManager.h"
#include "xM/Engine/ResourceManager.h"
#include "xM/Engine/StateManager.h"
#include "xM/States/About.h"
#include "xM/States/MangaSelect.h"
#include "xM/States/Menu.h"
#include "xM/Net/Net.h"
#include "xM/Ui/Dialogs.h"
#include "xM/Util/Log.h"
// END Includes

namespace xM {

    namespace States {

        /**
         * Start up code.
         */
        void Menu::init(void) {
        
            // Cache it for the about state
            Engine::ResourceManager::getInstance()->getImage("genesis.png");
                   
			// init/reset some vars                         
            this->doAction = false;
            this->activeDialog = 0;
            this->selected = 0;
            this->extraElements = new Ui::ExtraElements;
            
            // Populate menu
            this->menuList.push_back("Read Manga");
            this->menuList.push_back("Recent Manga");
            this->menuList.push_back("Bookmarks");
            this->menuList.push_back("Options");
            this->menuList.push_back("About");
            this->menuList.push_back("Quit");
                      
			// setup the list element
            this->lInfo.selected = &this->selected;
            this->lInfo.list = &this->menuList;
            
			// Register the XML UI parsers
            this->parser.registerCustomElementHandler("list", this->extraElements, (void*)&this->lInfo);
            this->parser.registerCustomElementHandler("bouncyBox", this->extraElements);
            
            // read in the XML and generate the UI
            this->parser.parseFile("ui/menu.xml");
                        
            // Create our local mailbox
            this->localBox = sceKernelCreateMbx("MenuStateBox", 0, NULL);
            
            // Setup the defaults for the message struct
            this->msg = new Manga::APIMessage;
            SceKernelMsgPacket hdr = {0};
            this->msg->header = hdr;
            this->msg->returnBox = &this->localBox;
                       
			// Call up the net dialog if not connected
			if (!Net::isConnected())                        
            	Ui::Dialog::net();
                                                            
        }

        /**
         * Clean up code.
         */
        void Menu::cleanUp(void) {

			sceKernelDeleteMbx(this->localBox);

            this->parser.deRegisterCustomElementHandler("list");
            this->parser.deRegisterCustomElementHandler("bouncyBox");
            
            delete this->extraElements;
            
            if (this->msg != NULL)
            	delete this->msg;
            
        }

        /**
         * Pause state.
         */
        void Menu::pause(void) { }

        /**
         * Resume state.
         */
        void Menu::resume(void) {

			// Reload XML ui
			this->parser.parseFile("ui/menu.xml");

        }

        /**
         * Poll for input, read event state etc
         */
        void Menu::handleEvents(void) {
        
        	// Get pointer to input manager
            Engine::InputManager* iM = Engine::InputManager::getInstance();
            
#ifdef __xM_DEBUG            
            // DEBUG: Reload XML on-the-fly            
            if (iM->pressed(PSP_CTRL_LTRIGGER)) {
            
                Util::logMsg("Reloading XML ui file.");
                this->parser.parseFile("ui/menu.xml");    
            
            }
#endif
            
            // Handle scrolling up/down
            if (iM->pressed(PSP_CTRL_DOWN))
                this->selected += 1;
            else if (iM->pressed(PSP_CTRL_UP))
                this->selected -= 1;
                
            // User made a selection, indicate that
            if (iM->pressed(PSP_CTRL_CROSS))
                this->doAction = true;
                            
        }

        /**
         * Now do something with the data we got from events and what not.
         */
        void Menu::handleLogic(void) {
        
        	//Check for any new messages in mailbox
            Manga::APIMessage* rMsg = NULL;
            sceKernelPollMbx(this->localBox, (void**)&rMsg);
                
            // Handle any active requests
            switch (this->activeDialog) {
            
            	// Request confirmation for quiting
            	case 1:
            	
            		// User asked to confirm if quiting, act accordingly	        
			        if (Ui::Dialog::getMsgDialogResult() == Ui::Dialog::RESPONSE_YES) {
			        
			            Engine::StateManager::getInstance()->popState();			            
			        	return;
			        	
			        }
			            
			        this->activeDialog = 0;
            	
            		break;
            		
            	// Requested manga list
            	case 2:
            	
            		// There's a response in the mailbox!
            		if (rMsg != NULL) {
                            
                        // Loaded successfully, switch to new state
		            	if (rMsg->type == Manga::RequestMangaList && rMsg->result == true) {
		            	
		            		Engine::StateManager::getInstance()->changeState(new States::MangaSelect());
		            		return;
		            	
		            	} else {
		            	
		            		// Something failed, print error message                	
		            		Ui::Dialog::msg(*(std::string*)rMsg->what);                	
		            		delete (std::string*)rMsg->what;
		            		
		            	}
		            	
		                this->activeDialog = 0;
		                
					}
                                            	
            		break;
            		
            	// No outstanding requests
            	case 0:
            	
            		// An item was selected
            		if (this->doAction) {
            		
            			// Handle based on the selection
            			switch (this->selected) {
            			
            				// Read Manga
						    case 0:
						    	                    
						    	// Send the manga list request
						        this->msg->type = Manga::RequestMangaList;
						        sceKernelSendMbx(Manga::mangaAPIMbx, (void*)this->msg);
						    
						        // not really a dialog
						        // but allows some control
						        this->activeDialog = 2;
						    
						        break;
						        
						    case 1:
						    
						        break;
						        
						    case 2:
						    
						        break;
						        
						    case 3:
						    
						        break;
						     
						    // About    
						    case 4:
						   	
						        Engine::StateManager::getInstance()->pushState(new States::About());
						    
						        break;
						        
						    // Quit
						    case 5:
						    
						    	// Confirm user wants to leave
						        Ui::Dialog::msg("Do you want quit xMangaPSP?", true);
						        this->activeDialog = 1;
						        	                
						        break;
            			
            			}
            			
            			// Reset flag
            			this->doAction = false;
            		
            		}
            	
            		break;
            
            }
                                	        
	        // BEGIN Menu Traversing Logic
	        if ((signed int)this->selected < 0)
                this->selected = 0;
            if (this->selected > (this->menuList.size() - 1))
                this->selected = this->menuList.size() - 1;
            // END Menu Traversing Logic
                            	        	        	        
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
