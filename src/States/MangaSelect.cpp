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
 * Manga selection state class.
 * 
 * @package xMangaPSP
 */

#ifndef _MangaSelectState_CPP
#define _MangaSelectState_CPP

// BEGIN Includes
#include "xM/Engine/InputManager.h"
#include "xM/Engine/StateManager.h"
#include "xM/States/MangaSelect.h"
#include "xM/Manga/MangaElements.h"
#include "xM/States/ChapterSelect.h"
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
            this->navChange = true; // Set to true initially to populate the list
            this->activeDialog = 0;
            this->selected = 0;
            this->navSel = 0;
            this->mangaList = *Manga::mapImp->getMangaList();      

            this->nList.reserve(28);
            this->nList.push_back("All");
            for (int c = 64; c < 91; c++) {

                std::string u(1, (char)c);

                for (unsigned int i = 0; i < this->mangaList.names.size(); i++) {
                    
                    int k = (int)(this->mangaList.names[i].data())[0];

                    if (c == 64 && this->nList.size() == 1) {
                        
                        // Non-alphabetic characters
                        if (k < 65 || (k > 90 && k < 97) || k > 122)
                            this->nList.push_back("#");

                    } else {
                    
                        if (c == k || (c + 32) == k) {

                            this->nList.push_back(u);
                            break;

                        }

                    }

                }

            }
                        
			// setup the list elements
            this->mLInfo.selected = &this->selected;
            this->mLInfo.list = &this->vMangaList;
            this->nLInfo.selected = &this->navSel;
            this->nLInfo.list = &this->nList;

			// Register the XML UI parsers
            this->parser.registerCustomElementHandler("mangalist", &this->extraElements, (void*)&this->mLInfo);
            this->parser.registerCustomElementHandler("navlist", &this->extraElements, (void*)&this->nLInfo);
            this->parser.registerCustomElementHandler("bouncyBox", &this->extraElements);
            
            this->parser.addTextSubstitute("mangaCount", Util::toString(this->mangaList.names.size()));
            
            // read in the XML and generate the UI
            this->parser.parseFile("ui/manga.xml");
                        
            // Create our local mailbox
            this->localBox = sceKernelCreateMbx("MangaSelectStateBox", 0, NULL);
            
            // Setup the defaults for the message struct
            SceKernelMsgPacket hdr = {0};
            this->msg.header = hdr;
            this->msg.returnBox = &this->localBox;
                                                                                    
        }

        /**
         * Clean up code.
         */
        void MangaSelect::cleanUp(void) {

			sceKernelDeleteMbx(this->localBox);

            this->parser.deRegisterCustomElementHandler("mangalist");
            this->parser.deRegisterCustomElementHandler("navlist");
            this->parser.deRegisterCustomElementHandler("bouncyBox");
                                    
        }

        /**
         * Pause state.
         */
        void MangaSelect::pause(void) { }

        /**
         * Resume state.
         */
        void MangaSelect::resume(void) { }

        /**
         * Poll for input, read event state etc
         */
        void MangaSelect::handleEvents(void) {

			// Get pointer to input manager
            Engine::InputManager* iM = Engine::InputManager::getInstance();
            
#if __xM_DEBUG
            // DEBUG: Reload XML on-the-fly            
            if (iM->pressed(PSP_CTRL_START)) {
            
                Util::logMsg("Reloading XML ui file.");
                this->parser.parseFile("ui/manga.xml");    
            
            }
#endif
            
            if (this->activeDialog == 0) {

                // Handle scrolling up/down/left/right
                if (iM->pressed(PSP_CTRL_DOWN))
                    this->selected += 1;
                else if (iM->pressed(PSP_CTRL_UP))
                    this->selected -= 1;
                else if (iM->pressed(PSP_CTRL_RTRIGGER))
                    this->selected = this->vMangaList.size() - 1;
                else if (iM->pressed(PSP_CTRL_LTRIGGER))
                    this->selected = 0;
                else if (iM->pressed(PSP_CTRL_RIGHT)) {

                    this->navSel += 1;
                    this->navChange = true;

                } else if (iM->pressed(PSP_CTRL_LEFT)) {

                    this->navSel -= 1;
                    this->navChange = true;

                }
                    
                // User made a selection, indicate that
                if (iM->pressed(PSP_CTRL_CROSS))
                    this->doAction = true;
                    
                // Leave state
                if (iM->pressed(PSP_CTRL_CIRCLE))
                	Engine::StateManager::getInstance()->changeState(new States::Menu());

            }
            
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
            if (this->selected > (this->vMangaList.size() - 1))
                this->selected = this->vMangaList.size() - 1;
            if ((signed int)this->navSel < 0)
                this->navSel = 0;
            if (this->navSel > (this->nList.size() - 1))
                this->navSel = this->nList.size() - 1;
            // END Menu Traversing Logic

            if (this->navChange) {
                
                // Clear current and reset selected
                this->vMangaList.clear();
                this->selected = 0;

                char l = (this->nList[this->navSel].data())[0];

                if (this->nList[this->navSel] == "All")
                    this->vMangaList.assign(this->mangaList.names.begin(), this->mangaList.names.end());
                else {

                    for (unsigned int i = 0; i < this->mangaList.names.size(); i++) {
                        
                        int k = (int)(this->mangaList.names[i].data())[0];
                        
                        if (l == '#') {

                            // Non-alphabetic characters
                            if (k < 65 || (k > 90 && k < 97) || k > 122)
                                this->vMangaList.push_back(this->mangaList.names[i]);

                        } else {
                            
                            if ((int)l == k || ((int)l + 32) == k)
                                this->vMangaList.push_back(this->mangaList.names[i]);

                        }

                    }

                }

                this->navChange = false;

            }
            
            // Handle any active requests
            switch (this->activeDialog) {
            
            	// No outstanding requests
            	case 0:
            	
            		if (this->doAction) {

                        // First need to find the real index
                        // 'selected' is not to be trusted as it relates to the alphabetical lists
                        int rIndex;
                        for (unsigned int i = 0; i < this->mangaList.names.size(); i++) {
                            if (this->mangaList.names[i] == this->vMangaList[selected])
                                rIndex = i;
                        }
            
            			// Send the chapter list request
				        this->msg.type = Manga::RequestChapterList;
				        this->msg.what = (void*)new std::string(this->mangaList.apiHandles[rIndex]);
                        this->msg.index = rIndex;
				        sceKernelSendMbx(Manga::mangaAPIMbx, (void*)&this->msg);
				    
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
		            	
		            		Engine::StateManager::getInstance()->pushState(new States::ChapterSelect());
                            this->activeDialog = 0;
		            		return;
		            	
		            	} else if (rMsg->type == Manga::RequestChapterList && rMsg->result == false) {
		            	
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

            if (this->activeDialog == 1)
                Gfx::drawLoadingOverlay("Loading chapter list...");
                        
        }
                
    }

}

#endif /* _MangaSelectState_CPP */
