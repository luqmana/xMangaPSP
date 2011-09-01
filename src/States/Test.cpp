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
 * Test state class.
 * Just to test out various features and stuff. NOT included in release.
 * 
 * @package xMangaPSP
 */

#ifndef _TestState_CPP
#define _TestState_CPP

// BEGIN Includes
#include "xM/Engine/InputManager.h"
#include "xM/Engine/StateManager.h"
#include "xM/States/Test.h"
#include "xM/Util/Log.h"
// END Includes

namespace xM {

    namespace States {

        /**
         * Start up code.
         */
        void Test::init(void) {
		
			// Parse the required UI file
            this->parser.parseFile("ui/test.xml");

			x = 120;
		
			fontTest.loadFont(Gfx::Font::LATIN_SANS_SERIF_REGULAR, 1.0, Gfx::Colour::RED,
						Gfx::Colour::GREEN, 0, INTRAFONT_ALIGN_LEFT | INTRAFONT_SCROLL_LEFT, 0);

		}

        /**
         * Clean up code.
         */
        void Test::cleanUp(void) {

            
			
        }

        /**
         * Pause state.
         */
        void Test::pause(void) { }

        /**
         * Resume state.
         */
        void Test::resume(void) { }

        /**
         * Poll for input, read event state etc
         */
        void Test::handleEvents(void) {
		
			// Get pointer to input manager
            Engine::InputManager* iM = Engine::InputManager::getInstance();
		
			// Return to previous state
            if (iM->pressed(PSP_CTRL_CIRCLE))
                Engine::StateManager::getInstance()->popState();
				
			// DEBUG: Reload XML on-the-fly            
            if (iM->pressed(PSP_CTRL_START)) {
            
                Util::logMsg("Reloading XML ui file.");
                this->parser.parseFile("ui/test.xml");    
            
            }

		}

        /**
         * Now do something with the data we got from events and what not.
         */
        void Test::handleLogic(void) { 
		
			
		
		}

        /**
         * Done with the logic? Draw what's needed then.
         */
        void Test::draw(void) {

			// Render UI from XML
            this->parser.draw();
		
            x = fontTest.drawColumn(x, 120, -1, "Bra bra blah!~");
            
        }
        
    }

}

#endif /* _TestState_CPP */
