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
 * Native PSP dialogs.
 * Header file.
 * 
 * @package xMangaPSP
 */

#ifndef _Dialogs_H
#define _Dialogs_H

// BEGIN Includes
#include "xM/Gfx/Graphics.h"

#include <string>
// END Includes

// BEGIN Defines

// END Defines

namespace xM {

    namespace Ui {
    
        namespace Dialog {
        
            enum ResponseType {
        
                RESPONSE_NONE,
                RESPONSE_YES,
                RESPONSE_NO,
                RESPONSE_BACK
            
            };
            
            /**
             * Close any dialogs currently.
             */
            void abortDialogs();
        
            /**
             * Returns the dialog button pressed.
             * 
             * @return ResponseType
             */
            ResponseType getMsgDialogResult();
        
            /**
             * Returns whether any dialogs are currently active.
             * 
             * @return bool
             */
            bool isDialogActive();
            
            /**
             * Returns whether any msg (incl. error) dialogs are currently active.
             * 
             * @return bool
             */
            bool isMsgDialogActive();
            
            /**
             * Returns whether any net dialogs are currently active.
             * 
             * @return bool
             */
            bool isNetDialogActive();
            
            /**
             * Render the dialogs.
             */
            void renderDialogs();
                
            /**
             * Pop up the standard PSP message dialog.
             * 
             * @param const std::string& umsg Message to display.
             * @param bool[optional] showYesNoOptions Whether to display the yes/no options.
             */
            void msg(const std::string& umsg, bool showYesNoOptions = false);
            
            /**
             * Pop up the standard PSP error dialog.
             * 
             * @param const unsigned int eNum Error number.
             */
            void err(const unsigned int eNum);
            
            /**
             * Pop up the standard PSP wifi/net selection dialog.
             */
            void net();
        
        }
        
    }

}

#endif /* _Dialog_H */
