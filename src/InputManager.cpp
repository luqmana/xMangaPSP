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
         * Constructor.
         */
        InputManager::InputManager() {
            
            sceCtrlSetSamplingCycle(1);
            sceCtrlSetSamplingMode(PSP_CTRL_MODE_ANALOG);
            repeat = false;

        }

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
         * Reads input.
         */
        void InputManager::readInput(void) {
        
            sceCtrlPeekBufferPositive(&this->pad, 1);
            
            if (this->repeat) {
            
                if (this->pad.Buttons != this->repeatButtons) {
                
                    this->repeat = false;
                    this->repeatButtons = 0;
                    this->buttons = this->lastButtons = this->pad.Buttons;
                    this->repeatTimer.start();
                
                } else {
                
                    if (this->repeatTimer.getDeltaTicks(false) > 0.075) {
                    
                        this->buttons = this->lastButtons = this->pad.Buttons;
                        this->repeatTimer.start();
                    
                    } else {
                    
                        this->lastButtons = this->pad.Buttons;
                        this->buttons = 0;
                    
                    }
                
                }
            
            } else {
                        
                if (this->lastButtons == this->pad.Buttons) {
                                                        
                    if (this->repeatTimer.getDeltaTicks(false) > 0.15) {
                    
                        this->repeatButtons = this->pad.Buttons;
                        this->repeat = true;
                        this->repeatTimer.start();
                    
                    } else {
                
                        this->lastButtons = this->pad.Buttons;
                        this->buttons = 0;
                        
                    }
                                        
                } else {
                
                    this->buttons = this->lastButtons = this->pad.Buttons;
                    this->repeatTimer.start();
                    
                }
            
            }
        
        }
        
        /**
         * Check whether a certain button has been pressed.
         * 
         * @param PspCtrlButton Which button to check
         */
        bool InputManager::pressed(unsigned int button) {
        
            return (this->buttons != 0) && (this->buttons & button);
        
        }
        
        /**
         * Returns a copy of the current pad data.
         * 
         * @return SceCtrlData
         */
        SceCtrlData InputManager::getCtrlData() {
        
            return this->pad;
        
        }

    }

}

#endif /* _InputManager_CPP */
