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
 * The state manager.
 * Header file.
 * 
 * @package xMangaPSP
 */

#ifndef _StateManager_CPP
#define _StateManager_CPP

// BEGIN Includes
#include "xM/Engine/StateManager.h"
// END Includes

// BEGIN Defines

// END Defines

namespace xM {

	namespace Engine {
	
	    StateManager* StateManager::sMInstance = NULL;
				    
        /**
         * Get an instance of the class.
         * 
         * @return StateManager* Singleton instance.
         */
        StateManager* StateManager::getInstance(void) {
        
            // Create instance if necessary
            if (sMInstance == NULL)
                sMInstance = new StateManager;
                
            return sMInstance;
        
        }
    
        /**
         * Start up code.
         */
        void StateManager::init(void) {
        
            
        
        }
        
        /**
         * Clean up code.
         */
        void StateManager::cleanUp(void) {
        
            // Loop through all states
	        while (!this->states.empty()) {
	
		        // Let state cleanup
		        this->states.back()->cleanUp();
		
		        // Remove state from stack
		        this->states.pop_back();
	
	        }
        
        }
        
        /**
         * Go to a new state. Delete previous state, allowing it to cleanup.
         * 
         * @param Base* state The new state.
         */
        void StateManager::changeState(States::Base* state) {
        
            // Handle current state
            if (!this->states.empty()) {
            
                // Let current state cleanup
                this->states.back()->cleanUp();
                
                // Remove it
                this->states.pop_back();
            
            }
            
            // Add new state
            this->states.push_back(state);
            
            // Let new state setup
            this->states.back()->init();
        
        }
        
        /**
         * Go to a new state. Pause previous state.
         * 
         * @param Base* state The new State.
         */
        void StateManager::pushState(States::Base* state) {
        
            // Let current state pause if necessary
            if (!this->states.empty())
                this->states.back()->pause();
                
            // Add new state
            this->states.push_back(state);
            
            // Let new state setup
            this->states.back()->init();
        
        }
        
        /**
         * Delete current state, allowing it to cleanup. Resume previous state.
         */
        void StateManager::popState(void) {
        
            // Handle current state
            if (!this->states.empty()) {
            
                // Let current state cleanup
                this->states.back()->cleanUp();
                
                // Remove it
                this->states.pop_back();
            
            }
            
            // Resume state, if necessary
            if (!this->states.empty())
                this->states.back()->resume();
        
        }
        
        /**
         * Return a pointer to the current state.
         * 
         * @return Base* The current state.
         */
        States::Base* StateManager::getCurrentState(void) {
        
            return this->states.back();
        
        }
        		        
        /**
         * Poll for input, read event state etc
         */
        void StateManager::handleEvents(void) {
        
            // Let each state handle their own events and whatnot
            this->states.back()->handleEvents();
        
        }
        
        /**
         * Now do something with the data we got from events and what not.
         */
        void StateManager::handleLogic(void) {
        
            // Let each state handle their own logic
            this->states.back()->handleLogic();
        
        }
        
        /**
         * Done with the logic? Draw what's needed then.
         */
        void StateManager::draw(void) {
        
            // Let each state render their own content
            this->states.back()->draw();
        
        }
                            			
	}

}

#endif /* _StateManager_CPP */
