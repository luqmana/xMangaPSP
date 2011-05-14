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
// END Includes

namespace xM {

    namespace States {

        /**
         * Start up code.
         */
        void About::init(void) {
                  
            // init/reset some vars
            this->action = 0;
            this->state = 0;
            this->extraElements = new Ui::ExtraElements();

			// Register the XML UI parsers
            this->parser.registerCustomElementHandler("bouncyBox", this->extraElements);
            this->parser.parseFile("ui/about.xml");
            
            // Get the splash image
            // (which should be cached, otherwise loaded now)
            this->genesisSplash = Engine::ResourceManager::getInstance()->getImage("genesis.png");
            
            // swizzle the splash image if it's been loaded
            if (this->genesisSplash != NULL)
            	this->genesisSplash->swizzle();
            else
            	Util::logMsg("Genesis splash wasn't loaded properly!");
                                                
        }

        /**
         * Clean up code.
         */
        void About::cleanUp(void) {

            this->parser.deRegisterCustomElementHandler("bouncyBox");
            
            delete this->extraElements;
            
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
            
            if (iM->pressed(PSP_CTRL_CIRCLE))
                action = 1;
                
            if (iM->pressed(PSP_CTRL_CROSS))
                action = 2;
            
        }

        /**
         * Now do something with the data we got from events and what not.
         */
        void About::handleLogic(void) {
        	        
	        switch (this->action) {
	        
	        	// Return to previous state
	        	case 1:
	        	
	        		Engine::StateManager::getInstance()->popState();
	        	
	        		break;
	        		
	        	// Switch screen
	        	case 2:
	        	
	        		this->state = (this->state == 0) ? 1 : 0;
	        	
	        		break;
	        		
	        	// Reset
	        	this->action = 0;
	        
	        }
	        	        
        }

        /**
         * Done with the logic? Draw what's needed then.
         */
        void About::draw(void) {

            switch (this->state) {
            
            	// Main screen
            	case 0:
            	
            		// Render UI from XML
            		this->parser.draw();
            	
            		break;
            		
            	// Splash screen
            	case 1:
            	
            		// Render the splash image
            		this->genesisSplash->draw(0, 0);
            	
            		break;
            
            }
            
        }
        
    }

}

#endif /* _AboutState_CPP */
