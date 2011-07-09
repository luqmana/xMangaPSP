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
#include "xM/Engine/StateManager.h"
#include "xM/States/About.h"
#include "xM/Util/Log.h"
// END Includes

namespace xM {

    namespace States {

        /**
         * Start up code.
         */
        void About::init(void) {
                  
			// Register the XML UI parsers
            this->parser.registerCustomElementHandler("bouncyBox", &this->extraElements);
            
            // Parse the required UI file
            this->parser.parseFile("ui/about.xml");
                                                
        }

        /**
         * Clean up code.
         */
        void About::cleanUp(void) {

            this->parser.deRegisterCustomElementHandler("bouncyBox");
                        
        }

        /**
         * Pause state.
         */
        void About::pause(void) { }

        /**
         * Resume state.
         */
        void About::resume(void) {

			// Reload XML ui
			this->parser.parseFile("ui/about.xml");

        }

        /**
         * Poll for input, read event state etc
         */
        void About::handleEvents(void) {

			// Get pointer to input manager
            Engine::InputManager* iM = Engine::InputManager::getInstance();
            
#ifdef __xM_DEBUG            
            // DEBUG: Reload XML on-the-fly            
            if (iM->pressed(PSP_CTRL_LTRIGGER)) {
            
                Util::logMsg("Reloading XML ui file.");
                this->parser.parseFile("ui/about.xml");    
            
            }
#endif
            
			// Return to previous state
            if (iM->pressed(PSP_CTRL_CIRCLE))
                Engine::StateManager::getInstance()->popState();
            
        }

        /**
         * Now do something with the data we got from events and what not.
         */
        void About::handleLogic(void) { }

        /**
         * Done with the logic? Draw what's needed then.
         */
        void About::draw(void) {

            // Render UI from XML
            this->parser.draw();
            
        }
        
    }

}

#endif /* _AboutState_CPP */
