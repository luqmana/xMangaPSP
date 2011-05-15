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

#ifndef _MangaSelectState_CPP
#define _MangaSelectState_CPP

// BEGIN Includes
#include "xM/Engine/InputManager.h"
#include "xM/Engine/ResourceManager.h"
#include "xM/Engine/StateManager.h"
#include "xM/States/MangaSelect.h"
#include "xM/States/Menu.h"
#include "xM/Ui/Dialogs.h"
#include "xM/Util/Log.h"
#include "xM/Util/Utils.h"
// END Includes

namespace xM {

    namespace States {

        /**
         * Start up code.
         */
        void MangaSelect::init(void) {
        
        	// Make sure the manga list has actually been loaded
        	if (Manga::mapImp->getMangaList() == NULL) {
        	
        		Engine::StateManager::getInstance()->popState();
        		return;
        		
        	}
        
        	// init/reset some vars
            this->doAction = false;
            this->activeDialog = 0;
            this->selected = 0;
            this->mangaList = *Manga::mapImp->getMangaList();      
            this->extraElements = new Ui::ExtraElements;
                        
			// setup the list element
            this->lInfo.selected = &this->selected;
            this->lInfo.list = &this->mangaList.names;

			// Register the XML UI parsers
            this->parser.registerCustomElementHandler("list", this->extraElements, (void*)&this->lInfo);
            this->parser.registerCustomElementHandler("bouncyBox", this->extraElements);
            
            this->parser.addTextSubstitute("mangaCount", Util::toString(this->mangaList.names.size()));
            
            // read in the XML and generate the UI
            this->parser.parseFile("ui/manga.xml");
                        
            // Create our local mailbox
            this->localBox = sceKernelCreateMbx("MangaSelectStateBox", 0, NULL);
            
            // Setup the defaults for the message struct
            this->msg = new Manga::APIMessage;
            SceKernelMsgPacket hdr = {0};
            this->msg->header = hdr;
            this->msg->returnBox = &this->localBox;
                                                                                    
        }

        /**
         * Clean up code.
         */
        void MangaSelect::cleanUp(void) {

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
        void MangaSelect::pause(void) { }

        /**
         * Resume state.
         */
        void MangaSelect::resume(void) {

			// Reload XML ui
			this->parser.parseFile("ui/manga.xml");
			
        }

        /**
         * Poll for input, read event state etc
         */
        void MangaSelect::handleEvents(void) {

			// Get pointer to input manager
            Engine::InputManager* iM = Engine::InputManager::getInstance();
            
#ifdef __xM_DEBUG            
            // DEBUG: Reload XML on-the-fly            
            if (iM->pressed(PSP_CTRL_LTRIGGER)) {
            
                Util::logMsg("Reloading XML ui file.");
                this->parser.parseFile("ui/manga.xml");    
            
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
                
            // Leave state
            if (iM->pressed(PSP_CTRL_CIRCLE))
            	Engine::StateManager::getInstance()->changeState(new Menu());
            
        }

        /**
         * Now do something with the data we got from events and what not.
         */
        void MangaSelect::handleLogic(void) {
        
        	//Check for any new messages in mailbox
            Manga::APIMessage* rMsg = NULL;
            sceKernelPollMbx(this->localBox, (void**)&rMsg);
                            	        
	        // BEGIN Menu Traversing Logic
	        if ((signed int)this->selected < 0)
                this->selected = 0;
            if (this->selected > (this->mangaList.names.size() - 1))
                this->selected = this->mangaList.names.size() - 1;
            // END Menu Traversing Logic
            
            // Handle any active requests
            switch (this->activeDialog) {
            
            	// No outstanding requests
            	case 0:
            	
            		if (this->doAction) {
            
            			// Send the manga list request
				        this->msg->type = Manga::RequestChapterList;
				        this->msg->what = (void*)new std::string(this->mangaList.apiHandles[selected]);
				        sceKernelSendMbx(Manga::mangaAPIMbx, (void*)this->msg);
				    
				        // not really a dialog
				        // but allows some control
				        this->activeDialog = 1;			
            			
						this->doAction = false;
				
					}
            	
					break;
					
				case 1:
				
					// There's a response in the mailbox!
            		if (rMsg != NULL) {
                            
                        // Loaded successfully, switch to new state
		            	if (rMsg->type == Manga::RequestChapterList && rMsg->result == true) {
		            	
		            		//Engine::StateManager::getInstance()->changeState(new States::MangaSelect());
		            		//return;
		            	
		            	} else {
		            	
		            		// Something failed, display error message                	
		            		Ui::Dialog::msg(*(std::string*)rMsg->what);                	
		            		delete (std::string*)rMsg->what;
		            		
		            	}
		            	
		                this->activeDialog = 0;
		                
					}
					
					break;
            
            }
                                        	        	        	        
        }

        /**
         * Done with the logic? Draw what's needed then.
         */
        void MangaSelect::draw(void) {
                                
            // Draw based on XML
            this->parser.draw();
                        
        }
                
    }

}

#endif /* _MangaSelectState_CPP */
