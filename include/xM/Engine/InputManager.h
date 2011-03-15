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
 * The input manager.
 * Header file.
 * 
 * @package xMangaPSP
 */

#ifndef _InputManager_H
#define _InputManager_H

// BEGIN Includes
#include "xM/Util/Timer.h"

#include <cstdlib>
#include <pspctrl.h>
// END Includes

// BEGIN Defines

// END Defines

namespace xM {

	namespace Engine {
	
		class InputManager {
		
            private:
            
                /**
                 * This will be a singleton class so no need for a public constructor.
                 */
                InputManager(void) { };
                
                /**
                 * Holds the button input.
                 */
                SceCtrlData pad;
                
                Util::Timer repeatTimer;
                bool repeat;
                unsigned int buttons;
                unsigned int lastButtons;
                unsigned int repeatButtons;
                                                            		
		    public:
		    
		        /**
                 * The singleton instance.
                 */
                static InputManager* iMInstance;
		    
		        /**
		         * Get an instance of the class.
		         * 
		         * @return InputManager* Singleton instance.
		         */
		        static InputManager* getInstance(void);
		    
		        /**
		         * Start up code.
		         */
		        void init(void);
		        
		        /**
		         * Reads input.
		         */
		        void readInput(void);
		        
		        /**
                 * Check whether a certain button has been pressed.
                 * 
                 * @param PspCtrlButton Which button to check
                 */
                bool pressed(PspCtrlButtons button);
		        		        		
		};
			
	}

}

#endif /* _InputManager_H */
