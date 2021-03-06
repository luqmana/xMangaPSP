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
 * Image selection state class.
 * 
 * @package xMangaPSP
 */

#ifndef _ImageSelectState_CPP
#define _ImageSelectState_CPP

// BEGIN Includes
#include "xM/Engine/InputManager.h"
#include "xM/Engine/StateManager.h"
#include "xM/States/ImageView.h"
#include "xM/States/ImageSelect.h"
#include "xM/States/ImageSelect.h"
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
        void ImageSelect::init(void) {
        
            // Make sure the image list has actually been loaded
            if (Manga::mapImp->getImageList() == NULL) {
            
                Engine::StateManager::getInstance()->popState();
                return;
                
            }
        
            // init/reset some vars
            this->doAction = false;
            this->activeDialog = 0;
            this->selected = 0;
            this->imageList = *Manga::mapImp->getImageList();      
                        
            // setup the list element
            this->lInfo.selected = &this->selected;
            this->lInfo.list = &this->imageList.images;

            // Register the XML UI parsers
            this->parser.registerCustomElementHandler("list", &this->extraElements, (void*)&this->lInfo);
            this->parser.registerCustomElementHandler("bouncyBox", &this->extraElements);
            
            // The replace thingies
            this->parser.addTextSubstitute("imageCount", Util::toString(this->imageList.images.size()));
            
            // read in the XML and generate the UI
            this->parser.parseFile("ui/images.xml");
                        
            // Create our local mailbox
            this->localBox = sceKernelCreateMbx("ImageSelectStateBox", 0, NULL);
            
            // Setup the defaults for the message struct
            SceKernelMsgPacket hdr = {0};
            this->msg.header = hdr;
            this->msg.returnBox = &this->localBox;
                                                                                    
        }

        /**
         * Clean up code.
         */
        void ImageSelect::cleanUp(void) {

            sceKernelDeleteMbx(this->localBox);

            this->parser.deRegisterCustomElementHandler("list");
            this->parser.deRegisterCustomElementHandler("bouncyBox");
                                    
        }

        /**
         * Pause state.
         */
        void ImageSelect::pause(void) { }

        /**
         * Resume state.
         */
        void ImageSelect::resume(void) { }

        /**
         * Poll for input, read event state etc
         */
        void ImageSelect::handleEvents(void) {

            // Get pointer to input manager
            Engine::InputManager* iM = Engine::InputManager::getInstance();
            
#if __xM_DEBUG
            // DEBUG: Reload XML on-the-fly            
            if (iM->pressed(PSP_CTRL_START)) {
            
                Util::logMsg("Reloading XML ui file.");
                this->parser.parseFile("ui/images.xml");    
            
            }
#endif
            
            if (this->activeDialog == 0) {

                // Handle scrolling up/down
                if (iM->pressed(PSP_CTRL_DOWN))
                    this->selected += 1;
                else if (iM->pressed(PSP_CTRL_UP))
                    this->selected -= 1;
                else if (iM->pressed(PSP_CTRL_RTRIGGER))
                    this->selected = this->imageList.images.size() - 1;
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
        void ImageSelect::handleLogic(void) {
        
            //Check for any new messages in mailbox
            Manga::APIMessage* rMsg = NULL;
            sceKernelPollMbx(this->localBox, (void**)&rMsg);
                                        
            // BEGIN Menu Traversing Logic
            if ((signed int)this->selected < 0)
                this->selected = 0;
            if (this->selected > (this->imageList.images.size() - 1))
                this->selected = this->imageList.images.size() - 1;
            // END Menu Traversing Logic
            
            // Handle any active requests
            switch (this->activeDialog) {
            
                // No outstanding requests
                case 0:
                
                    if (this->doAction) {
            
                        // Send the image request
                        this->msg.type = Manga::RequestImage;
                        this->msg.what = (void*)new std::string(this->imageList.images[selected]);
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
                            
                        // Loaded successfully, switch to a new state
                        if (rMsg->type == Manga::RequestImage && rMsg->result == true) {
                    
                            Engine::StateManager::getInstance()->pushState(new States::ImageView());

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
        void ImageSelect::draw(void) {
                                
            // Draw based on XML
            this->parser.draw();

            if (this->activeDialog == 1)
                Gfx::drawLoadingOverlay("Loading image...");
                        
        }
                
    }

}

#endif /* _ImageSelectState_CPP */
