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
 * About state class.
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
#include "xM/Ui/ExtraElements.h"
#include "xM/Util/Log.h"
#include "xM/Util/Timer.h"
#include "xM/Util/Utils.h"
// END Includes

namespace xM {

    namespace States {

        /**
         * Start up code.
         */
        void About::init(void) {
                                
            action = 0;
            state = 0;
            
            extraElements = new Ui::ExtraElements();

            parser.registerCustomElementHandler("bouncyBox", extraElements);
            parser.parseFile("ui/about.xml");
            
            genesisSplash = Engine::ResourceManager::getInstance()->getImage("genesis.png");
            
            if (genesisSplash != NULL)
            	genesisSplash->swizzle();
            else
            	Util::logMsg("Genesis splash wasn't loaded properly!");
                                                
        }

        /**
         * Clean up code.
         */
        void About::cleanUp(void) {

            parser.deRegisterCustomElementHandler("bouncyBox");
            
            delete extraElements;
            
        }

        /**
         * Pause state.
         */
        void About::pause(void) {



        }

        /**
         * Resume state.
         */
        void About::resume(void) {



        }

        /**
         * Poll for input, read event state etc
         */
        void About::handleEvents(void) {

            Engine::InputManager* iM = Engine::InputManager::getInstance();
            
            // Reload XML on-the-fly
            if (__xM_DEBUG && iM->pressed(PSP_CTRL_LTRIGGER)) {
            
                Util::logMsg("Reloading XML ui file.");
            
                parser.parseFile("ui/about.xml");    
            
            }
            
            if (iM->pressed(PSP_CTRL_CIRCLE))
                action = 1;
                
            if (iM->pressed(PSP_CTRL_CROSS))
                action = 2;
            
        }

        /**
         * Now do something with the data we got from events and what not.
         */
        void About::handleLogic(void) {
        	        
	        if (action == 1) {
	        
	            action = 0;
	            Engine::StateManager::getInstance()->popState();
	        
	        } else if (action == 2) {
	        
	            state = (state == 0) ? 1 : 0;
	            action = 0;
	        
	        }
	        	        
        }

        /**
         * Done with the logic? Draw what's needed then.
         */
        void About::draw(void) {
                            
            if (state == 0)    
                parser.draw();
            else if (state == 1)
                genesisSplash->draw(0, 0);
            
        }
        
    }

}

#endif /* _AboutState_CPP */
