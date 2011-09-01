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
            this->action = 0;
            this->activeDialog = 0;
            this->y = 0;
            this->image = Manga::mapImp->getImage();

            // Aligns to the right since that's how most manga is read (RTL)
            // @TODO: Make this an option?
            if (this->image->img->width > 480)
                this->x = -this->image->img->width + 480;
            else
                this->x = 0;
                                                
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
                if (iM->pressed(PSP_CTRL_UP))
                    this->y += 30;
                if (iM->pressed(PSP_CTRL_LEFT))
                    this->x += 30;
                if (iM->pressed(PSP_CTRL_RIGHT))
                    this->x -= 30;

                // Navigating to next/prev image
                if (iM->pressed(PSP_CTRL_RTRIGGER)) {

                    this->doAction = true;
                    this->action = 1; // 1 = Next image

                } else if (iM->pressed(PSP_CTRL_LTRIGGER)) {

                    this->doAction = true;
                    this->action = 2; // 2 = Prev image

                }
                    
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

            if (this->x < (signed)(-this->image->img->width) + 480)
                this->x = -this->image->img->width + 480;

            if (this->y < (signed)(-this->image->img->height) + 272)
                this->y = -this->image->img->height + 272;
        
        	//Check for any new messages in mailbox
            Manga::APIMessage* rMsg = NULL;
            sceKernelPollMbx(this->localBox, (void**)&rMsg);
                            	                    
            // Handle any active requests
            switch (this->activeDialog) {
            
            	// No outstanding requests
            	case 0:
            	
            		if (this->doAction) {
            
            			if (this->action == 1) {

                            if ((unsigned int)this->image->index == (Manga::mapImp->getImageList()->images.size() - 1)) {
                                
                                // argh...load next list
                                // Need to make sure next chapter exists first
                                if ((unsigned int)Manga::mapImp->getImageList()->index < (Manga::mapImp->getChapterList()->names.size() - 1)) {
                                    
                                    // Need to load next image list
                                    int index = Manga::mapImp->getImageList()->index + 1;
                                    // Send the image list request
                                    this->msg.type = Manga::RequestImageList;
                                    this->msg.what = (void*)new std::string(Manga::mapImp->getChapterList()->apiHandles[index]);
                                    this->msg.index = index;
                                    sceKernelSendMbx(Manga::mangaAPIMbx, (void*)&this->msg);

                                    this->activeDialog = 3;

                                } else {

                                    // No more chapters after this                                    
                                    Ui::Dialog::msg("No chapter after this has been released!\nSorry~ Check back later!");
                                    this->activeDialog = 0;

                                }

                            } else {

                                // the easy case
                   
                                this->msg.type = Manga::RequestImage;
                                this->msg.what = (void*)new std::string(Manga::mapImp->getImageList()->images[this->image->index + 1]);
                                this->msg.index = this->image->index + 1;
                                sceKernelSendMbx(Manga::mangaAPIMbx, (void*)&this->msg);

                                this->activeDialog = 1;

                            }

                            this->action = 0;
                            
                        } else if (this->action == 2) {

                            if ((unsigned int)this->image->index == 0) {
                                
                                // argh...load prev list

                                // Need to make sure prev chapter exists first
                                if (Manga::mapImp->getImageList()->index > 0) {
                                    
                                    // Need to load next image list
                                    int index = Manga::mapImp->getImageList()->index - 1;
                                    // Send the image list request
                                    this->msg.type = Manga::RequestImageList;
                                    this->msg.what = (void*)new std::string(Manga::mapImp->getChapterList()->apiHandles[index]);
                                    this->msg.index = index;
                                    sceKernelSendMbx(Manga::mangaAPIMbx, (void*)&this->msg);

                                    this->activeDialog = 4;

                                } else {

                                    // No more chapters before this                                    
                                    Ui::Dialog::msg("This is the first chapter. There is no chapter before this!\nSilly reader~ Press O to go back instead.");
                                    this->activeDialog = 0;

                                }

                            } else {

                                // the easy case
                   
                                this->msg.type = Manga::RequestImage;
                                this->msg.what = (void*)new std::string(Manga::mapImp->getImageList()->images[this->image->index - 1]);
                                this->msg.index = this->image->index - 1;
                                sceKernelSendMbx(Manga::mangaAPIMbx, (void*)&this->msg);

                                this->activeDialog = 2;

                            }

                            this->action = 0;
                            
                        }

                        this->doAction = 0;
				
					}
            	
					break;

                case 1:
                case 2:
                case 5:
                case 6:

                    // There's a response in the mailbox!
                    if (rMsg != NULL) {
                            
                        // Loaded successfully, switch to new image
                        if (rMsg->type == Manga::RequestImage && rMsg->result == true) {
                              
                            // Reset vars
                            this->y = 0;
                            // Aligns to the right since that's how most manga is read (RTL)
                            // @TODO: Make this an option?
                            if (this->image->img->width > 480)
                                this->x = -this->image->img->width + 480;
                            else
                                this->x = 0;

                            this->activeDialog = 0;

                            return;
                        
                        } else if (rMsg->type == Manga::RequestImage && rMsg->result == false) {
                        
                            // Something failed, display error message                  
                            Ui::Dialog::msg(*(std::string*)rMsg->what);                 
                            delete (std::string*)rMsg->what;

                            if (this->activeDialog == 5 || this->activeDialog == 6) {
                                
                                //Engine::StateManager::getInstance()->changeState(new States::ImageSelect());
                                return;

                            }

                            this->activeDialog = 0;
                            
                        }
                                                
                    }

                    break;

                case 3:

                    // There's a response in the mailbox!
                    if (rMsg != NULL) {
                            
                        // Loaded successfully, switch to new image
                        if (rMsg->type == Manga::RequestImageList && rMsg->result == true) {
                              
                            // Now that the image list is loaded, we load the first image!

                            // Send the image request
                            this->msg.type = Manga::RequestImage;
                            this->msg.index = 0;
                            this->msg.what = (void*)new std::string(Manga::mapImp->getImageList()->images[this->msg.index]);
                            sceKernelSendMbx(Manga::mangaAPIMbx, (void*)&this->msg);

                            this->activeDialog = 5;
                        
                        } else if (rMsg->type == Manga::RequestImageList && rMsg->result == false) {
                        
                            // Something failed, display error message                  
                            Ui::Dialog::msg(*(std::string*)rMsg->what);                 
                            delete (std::string*)rMsg->what;

                            this->activeDialog = 0;

                            // Change to chapter select
                            Engine::StateManager::getInstance()->changeState(new States::ChapterSelect());
                            return;
                            
                        }
                                                
                    }

                    break;

                case 4:

                    // There's a response in the mailbox!
                    if (rMsg != NULL) {
                            
                        // Loaded successfully, switch to new image
                        if (rMsg->type == Manga::RequestImageList && rMsg->result == true) {
                              
                            // Now that the image list is loaded, we load the last image!

                            // Send the image request
                            this->msg.type = Manga::RequestImage;
                            this->msg.index = Manga::mapImp->getImageList()->images.size() - 1;
                            this->msg.what = (void*)new std::string(Manga::mapImp->getImageList()->images[this->msg.index]);
                            sceKernelSendMbx(Manga::mangaAPIMbx, (void*)&this->msg);

                            this->activeDialog = 6;
                        
                        } else if (rMsg->type == Manga::RequestImageList && rMsg->result == false) {
                        
                            // Something failed, display error message                  
                            Ui::Dialog::msg(*(std::string*)rMsg->what);                 
                            delete (std::string*)rMsg->what;

                            this->activeDialog = 0;

                            // Change to chapter select
                            Engine::StateManager::getInstance()->changeState(new States::ChapterSelect());
                            return;
                            
                        }
                                                
                    }

                    break;
					            
            }
                                        	        	        	        
        }

        /**
         * Done with the logic? Draw what's needed then.
         */
        void ImageView::draw(void) {
                 
            //if (this->activeDialog == 0)
                this->image->img->draw(this->x, this->y);     
            
            if (this->activeDialog == 1 || this->activeDialog == 5)
                Gfx::drawLoadingOverlay("Loading next image...");
            else if (this->activeDialog == 2 || this->activeDialog == 6)
                Gfx::drawLoadingOverlay("Loading prev image...");
            else if (this->activeDialog == 3)
                Gfx::drawLoadingOverlay("Loading next chapter's imagelist...");
            else if (this->activeDialog == 4)
                Gfx::drawLoadingOverlay("Loading prev chapter's imagelist...");
                        
        }
                
    }

}

#endif /* _ImageViewState_CPP */
