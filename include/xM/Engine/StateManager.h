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
 * The state manager.
 * Header file.
 * 
 * @package xMangaPSP
 */

#ifndef _StateManager_H
#define _StateManager_H

// BEGIN Includes
#include <vector>

#include "xM/States/Base.h"
// END Includes

// BEGIN Defines

// END Defines

namespace xM {

    namespace Engine {
    
        class StateManager {
        
            private:
            
                /**
                 * This will be a singleton class so no need for a public constructor.
                 */
                StateManager(void) { };
                                            
                /**
                 * The stack of states.
                 */
                std::vector<States::Base*> states;
        
            public:

                /**
                 * Destructor.
                 */
                ~StateManager();
            
                /**
                 * The singleton instance.
                 */
                static StateManager* sMInstance;
            
                /**
                 * Get an instance of the class.
                 * 
                 * @return StateManager* Singleton instance.
                 */
                static StateManager* getInstance(void);
                            
                /**
                 * Go to a new state. Delete previous state, allowing it to cleanup.
                 * 
                 * @param Base* state The new state.
                 */
                void changeState(States::Base* state);
                
                /**
                 * Go to a new state. Pause previous state.
                 * 
                 * @param Base* state The new State.
                 */
                void pushState(States::Base* state);
                
                /**
                 * Delete current state, allowing it to cleanup. Resume previous state.
                 */
                void popState(void);
                
                /**
                 * Return a pointer to the current state.
                 * 
                 * @return Base* The current state.
                 */
                States::Base* getCurrentState(void);
                                
                /**
                 * Poll for input, read event state etc
                 */
                void handleEvents(void);
                
                /**
                 * Now do something with the data we got from events and what not.
                 */
                void handleLogic(void);
                
                /**
                 * Done with the logic? Draw what's needed then.
                 */
                void draw(void);
                        
        };
            
    }

}

#endif /* _StateManager_H */
