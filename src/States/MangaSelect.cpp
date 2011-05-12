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
#include "xM/Engine/FileManager.h"
#include "xM/Engine/InputManager.h"
#include "xM/Engine/ResourceManager.h"
#include "xM/Engine/StateManager.h"
#include "xM/States/MangaSelect.h"
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
        void MangaSelect::init(void) {
        
        	if (Manga::mapImp->getMangaList() == NULL) {
        	
        		Engine::StateManager::getInstance()->popState();
        		return;
        		
        	}
        
            doAction = false;
            activeDialog = 0;
            selected = 0;
                        
            extraElements = new Ui::ExtraElements;
            
            this->mangaList = Manga::mapImp->getMangaList()->names;
            
            lInfo.selected = &this->selected;
            lInfo.list = &this->mangaList;

            parser.registerCustomElementHandler("list", extraElements, (void*)&lInfo);
            parser.registerCustomElementHandler("bouncyBox", extraElements);
            parser.parseFile("ui/manga.xml");
                        
            // Create our local mailbox
            localBox = sceKernelCreateMbx("MangaSelectStateBox", 0, NULL);
            
            // Setup the defaults for the message struct
            msg = new Manga::APIMessage;
            SceKernelMsgPacket hdr = {0};
            msg->header = hdr;
            msg->returnBox = &localBox;
                                                                                    
        }

        /**
         * Clean up code.
         */
        void MangaSelect::cleanUp(void) {

			sceKernelDeleteMbx(localBox);

            parser.deRegisterCustomElementHandler("list");
            parser.deRegisterCustomElementHandler("bouncyBox");
            
            delete extraElements;
            
        }

        /**
         * Pause state.
         */
        void MangaSelect::pause(void) {



        }

        /**
         * Resume state.
         */
        void MangaSelect::resume(void) {



        }

        /**
         * Poll for input, read event state etc
         */
        void MangaSelect::handleEvents(void) {

            Engine::InputManager* iM = Engine::InputManager::getInstance();
            
            // Reload XML on-the-fly
            if (__xM_DEBUG && iM->pressed(PSP_CTRL_LTRIGGER)) {
            
                Util::logMsg("Reloading XML ui file.");
            
                parser.parseFile("ui/manga.xml");    
            
            }
            
            if (iM->pressed(PSP_CTRL_DOWN))
                selected += 1;
            else if (iM->pressed(PSP_CTRL_UP))
                selected -= 1;
                
            if (iM->pressed(PSP_CTRL_CROSS))
                doAction = true;
                
            if (iM->pressed(PSP_CTRL_CIRCLE))
            	Engine::StateManager::getInstance()->changeState(new Menu());
            
        }

        /**
         * Now do something with the data we got from events and what not.
         */
        void MangaSelect::handleLogic(void) {
                            	        
	        // BEGIN Menu Traversing Logic
	        if ((signed int)this->selected < 0)
                this->selected = 0;
            if (this->selected > (this->mangaList.size() - 1))
                this->selected = this->mangaList.size() - 1;
            // END Menu Traversing Logic
                            	        	        	        
        }

        /**
         * Done with the logic? Draw what's needed then.
         */
        void MangaSelect::draw(void) {
                                
            // Draw based on XML
            parser.draw();
                        
        }
                
    }

}

#endif /* _MangaSelectState_CPP */
