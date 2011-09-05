/**
 * This file is part of the xMangaPSP application.
 *
 * Copyright (C) Luqman Aden <www.luqmanrocks.co.cc>.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your options) any later version.
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
 * Options state class.
 * 
 * @package xMangaPSP
 */

#ifndef _OptionsState_CPP
#define _OptionsState_CPP

// BEGIN Includes
#include "xM/Engine/InputManager.h"
#include "xM/Engine/StateManager.h"
#include "xM/States/Options.h"
#include "xM/Util/Log.h"
// END Includes

namespace xM {

    namespace States {

        /**
         * Start up code.
         */
        void Options::init(void) {
                  
			// Register the XML UI parsers
            this->parser.registerCustomElementHandler("bouncyBox", &this->extraElements);
                                                            
        }

        /**
         * Clean up code.
         */
        void Options::cleanUp(void) {

            this->parser.deRegisterCustomElementHandler("bouncyBox");
                        
        }

        /**
         * Pause state.
         */
        void Options::pause(void) { }

        /**
         * Resume state.
         */
        void Options::resume(void) { }

        /**
         * Poll for input, read event state etc
         */
        void Options::handleEvents(void) {

			// Get pointer to input manager
            Engine::InputManager* iM = Engine::InputManager::getInstance();
            
#if __xM_DEBUG
            // DEBUG: Reload XML on-the-fly            
            if (iM->pressed(PSP_CTRL_START)) {
            
                Util::logMsg("Reloading XML ui file.");

                this->parser.parseFile("ui/options.xml");    
            
            }
#endif
            
			// Return to previous state
            if (iM->pressed(PSP_CTRL_CIRCLE))
                Engine::StateManager::getInstance()->popState();
            
        }

        /**
         * Now do something with the data we got from events and what not.
         */
        void Options::handleLogic(void) { }

        /**
         * Done with the logic? Draw what's needed then.
         */
        void Options::draw(void) {

            // Render UI from XML
            this->parser.draw();
            
        }
        
    }

}

#endif /* _OptionsState_CPP */
