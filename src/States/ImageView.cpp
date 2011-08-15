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
 * Image viewing state class.
 * 
 * @package xMangaPSP
 */

#ifndef _ImageViewState_CPP
#define _ImageViewState_CPP

// BEGIN Includes
#include "xM/Engine/InputManager.h"
#include "xM/Engine/StateManager.h"
#include "xM/States/ChapterSelect.h"
#include "xM/States/ImageView.h"
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
        void ImageView::init(void) {
        
        	// Make sure the image list has actually been loaded
        	if (Manga::mapImp->getImageList() == NULL) {
        	
        		Engine::StateManager::getInstance()->popState();
        		return;
        		
        	}
        
        	// init/reset some vars
            this->doAction = false;
            this->activeDialog = 0;
            this->x = this->y = 0;
            this->image = *Manga::mapImp->getImage();      
                                                
            // Create our local mailbox
            this->localBox = sceKernelCreateMbx("ImageViewStateBox", 0, NULL);
            
            // Setup the defaults for the message struct
            SceKernelMsgPacket hdr = {0};
            this->msg.header = hdr;
            this->msg.returnBox = &this->localBox;
                                                                                    
        }

        /**
         * Clean up code.
         */
        void ImageView::cleanUp(void) {

			sceKernelDeleteMbx(this->localBox);
                                    
        }

        /**
         * Pause state.
         */
        void ImageView::pause(void) { }

        /**
         * Resume state.
         */
        void ImageView::resume(void) { }

        /**
         * Poll for input, read event state etc
         */
        void ImageView::handleEvents(void) {

			// Get pointer to input manager
            Engine::InputManager* iM = Engine::InputManager::getInstance();
            
            if (this->activeDialog == 0) {

                // Handle moving
                if (iM->pressed(PSP_CTRL_DOWN))
                    this->y -= 30;
                else if (iM->pressed(PSP_CTRL_UP))
                    this->y += 30;
                else if (iM->pressed(PSP_CTRL_LEFT))
                    this->x += 30;
                else if (iM->pressed(PSP_CTRL_RIGHT))
                    this->x -= 30;
                    
                // Leave state
                if (iM->pressed(PSP_CTRL_CIRCLE))
                	Engine::StateManager::getInstance()->changeState(new States::ChapterSelect());

            }
            
        }

        /**
         * Now do something with the data we got from events and what not.
         */
        void ImageView::handleLogic(void) {

            if (this->x > 0)
                this->x = 0;

            if (this->y > 0)
                this->y = 0;

            if (this->x < (signed)(-this->image.img->width) + 480)
                this->x = -this->image.img->width + 480;

            if (this->y < (signed)(-this->image.img->height) + 272)
                this->y = -this->image.img->height + 272;
        
        	//Check for any new messages in mailbox
            Manga::APIMessage* rMsg = NULL;
            sceKernelPollMbx(this->localBox, (void**)&rMsg);
                            	                    
            // Handle any active requests
            switch (this->activeDialog) {
            
            	// No outstanding requests
            	case 0:
            	
            		if (this->doAction) {
            
            			
				
					}
            	
					break;
					            
            }
                                        	        	        	        
        }

        /**
         * Done with the logic? Draw what's needed then.
         */
        void ImageView::draw(void) {
                                
            this->image.img->draw(this->x, this->y);
                        
        }
                
    }

}

#endif /* _ImageViewState_CPP */
