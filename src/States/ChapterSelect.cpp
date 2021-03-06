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
 * Chapter selection state class.
 * 
 * @package xMangaPSP
 */

#ifndef _ChapterSelectState_CPP
#define _ChapterSelectState_CPP

// BEGIN Includes
#include "xM/Engine/InputManager.h"
#include "xM/Engine/StateManager.h"
#include "xM/States/ImageSelect.h"
#include "xM/States/ChapterSelect.h"
#include "xM/States/MangaSelect.h"
#include "xM/Manga/MangaElements.h"
#include "xM/Ui/Dialogs.h"
#include "xM/Util/Log.h"
#include "xM/Util/Utils.h"
// END Includes

namespace xM {

    namespace States {

        /**
         * Start up code.
         */
        void ChapterSelect::init(void) {
        
            // Make sure the chapter list has actually been loaded
            if (Manga::mapImp->getChapterList() == NULL) {
            
                Engine::StateManager::getInstance()->popState();
                return;
                
            }
        
            // init/reset some vars
            this->doAction = false;
            this->activeDialog = 0;
            this->selected = 0;
            this->chapterList = *Manga::mapImp->getChapterList();      
                        
            // setup the list element
            this->lInfo.selected = &this->selected;
            this->lInfo.list = &this->chapterList.names;

            // Register the XML UI parsers
            this->parser.registerCustomElementHandler("list", &this->extraElements, (void*)&this->lInfo);
            this->parser.registerCustomElementHandler("bouncyBox", &this->extraElements);
            
            // The replace thingies
            this->parser.addTextSubstitute("chapterCount", Util::toString(this->chapterList.names.size()));
            
            // read in the XML and generate the UI
            this->parser.parseFile("ui/chapter.xml");
                        
            // Create our local mailbox
            this->localBox = sceKernelCreateMbx("ChapterSelectStateBox", 0, NULL);
            
            // Setup the defaults for the message struct
            SceKernelMsgPacket hdr = {0};
            this->msg.header = hdr;
            this->msg.returnBox = &this->localBox;
                                                                                    
        }

        /**
         * Clean up code.
         */
        void ChapterSelect::cleanUp(void) {

            sceKernelDeleteMbx(this->localBox);

            this->parser.deRegisterCustomElementHandler("list");
            this->parser.deRegisterCustomElementHandler("bouncyBox");
                                    
        }

        /**
         * Pause state.
         */
        void ChapterSelect::pause(void) { }

        /**
         * Resume state.
         */
        void ChapterSelect::resume(void) { }

        /**
         * Poll for input, read event state etc
         */
        void ChapterSelect::handleEvents(void) {

            // Get pointer to input manager
            Engine::InputManager* iM = Engine::InputManager::getInstance();
            
#if __xM_DEBUG
            // DEBUG: Reload XML on-the-fly            
            if (iM->pressed(PSP_CTRL_START)) {
            
                Util::logMsg("Reloading XML ui file.");
                this->parser.parseFile("ui/chapter.xml");    
            
            }
#endif
            
            if (this->activeDialog == 0) {

                // Handle scrolling up/down
                if (iM->pressed(PSP_CTRL_DOWN))
                    this->selected += 1;
                else if (iM->pressed(PSP_CTRL_UP))
                    this->selected -= 1;
                else if (iM->pressed(PSP_CTRL_RTRIGGER))
                    this->selected = this->chapterList.names.size() - 1;
                else if (iM->pressed(PSP_CTRL_LTRIGGER))
                    this->selected = 0;
                    
                // User made a selection, indicate that
                if (iM->pressed(PSP_CTRL_CROSS))
                    this->doAction = true;
                    
                // Leave state
                if (iM->pressed(PSP_CTRL_CIRCLE))
                    Engine::StateManager::getInstance()->popState();

            }
            
        }

        /**
         * Now do something with the data we got from events and what not.
         */
        void ChapterSelect::handleLogic(void) {
        
            //Check for any new messages in mailbox
            Manga::APIMessage* rMsg = NULL;
            sceKernelPollMbx(this->localBox, (void**)&rMsg);
                                        
            // BEGIN Menu Traversing Logic
            if ((signed int)this->selected < 0)
                this->selected = 0;
            if (this->selected > (this->chapterList.names.size() - 1))
                this->selected = this->chapterList.names.size() - 1;
            // END Menu Traversing Logic
            
            // Handle any active requests
            switch (this->activeDialog) {
            
                // No outstanding requests
                case 0:
                
                    if (this->doAction) {
            
                        // Send the image list request
                        this->msg.type = Manga::RequestImageList;
                        this->msg.what = (void*)new std::string(this->chapterList.apiHandles[selected]);
                        this->msg.index = selected;
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
                            
                        // Loaded successfully
                        if (rMsg->type == Manga::RequestImageList && rMsg->result == true) {
                        
                            // Now that the image list is loaded, let's go to image select
                            Engine::StateManager::getInstance()->pushState(new States::ImageSelect());

                            this->activeDialog = 0;
                            return;

                        } else if (rMsg->type == Manga::RequestImageList && rMsg->result == false) {
                        
                            // Something failed, display error message                  
                            Ui::Dialog::msg(*(std::string*)rMsg->what);                 
                            delete (std::string*)rMsg->what;

                            this->activeDialog = 0;
                            
                        }
                                                
                    }
                    
                    break;

            }
                                                                            
        }

        /**
         * Done with the logic? Draw what's needed then.
         */
        void ChapterSelect::draw(void) {
                                
            // Draw based on XML
            this->parser.draw();

            if (this->activeDialog == 1)
                Gfx::drawLoadingOverlay("Loading image list...");
                        
        }
                
    }

}

#endif /* _ChapterSelectState_CPP */
