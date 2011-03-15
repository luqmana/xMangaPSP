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
 * 
 * @package xMangaPSP
 */

#ifndef _InputManager_CPP
#define _InputManager_CPP

// BEGIN Includes
#include "xM/Engine/InputManager.h"
// END Includes

namespace xM {

    namespace Engine {

        InputManager* InputManager::iMInstance = NULL;

        /**
         * Get an instance of the class.
         * 
         * @return InputManager* Singleton instance.
         */
        InputManager* InputManager::getInstance(void) {

            // Create instance if necessary
            if (iMInstance == NULL)
                iMInstance = new InputManager();

            return iMInstance;

        }

        /**
         * Start up code.
         */
        void InputManager::init(void) {

            sceCtrlSetSamplingCycle(0);
            sceCtrlSetSamplingMode(PSP_CTRL_MODE_ANALOG);

        }
        
        /**
         * Reads input.
         */
        void InputManager::readInput(void) {
        
           sceCtrlPeekBufferPositive(&this->pad, 1);
           //sceCtrlReadBufferPositive(&this->pad, 1);  
        
        }
        
        /**
         * Check whether a certain button has been pressed.
         * 
         * @param PspCtrlButton Which button to check
         */
        bool InputManager::pressed(PspCtrlButtons button) {
        
            return (this->pad.Buttons != 0) && (this->pad.Buttons & button);
        
        }

    }

}

#endif /* _InputManager_CPP */
