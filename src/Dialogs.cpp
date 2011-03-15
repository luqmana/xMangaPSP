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

#ifndef _Dialogs_CPP
#define _Dialogs_CPP

// BEGIN Includes
#include "xM/Ui/Dialogs.h"
#include "xM/Util/Utils.h"
#include "xM/Util/Log.h"

#include <string.h>

#include <psputility.h>
#include <psputility_msgdialog.h>
#include <psputility_netconf.h>
// END Includes

namespace xM {

    namespace Ui {
    
        namespace Dialog {
        
            pspUtilityMsgDialogParams mDialogParams;
            pspUtilityNetconfData nDialogData;
            bool msgDialogActive = false;
            bool netDialogActive = false;
            ResponseType dialogRes = RESPONSE_NONE;
            
            /**
             * Close any dialogs currently.
             */
            void abortDialogs() {
            
                sceUtilityMsgDialogShutdownStart();
                           
                bool done = (isMsgDialogActive()) ? false : true;
                                
                while (!done) {
                    
                    switch (sceUtilityMsgDialogGetStatus()) {
                    
                        case PSP_UTILITY_DIALOG_VISIBLE:
                        
                            sceUtilityMsgDialogUpdate(1);
                            sceUtilityMsgDialogAbort();
                            
                            break;
                            
                        case PSP_UTILITY_DIALOG_QUIT:
                        
                            sceUtilityMsgDialogShutdownStart();
                            
                            break;
                            
                        case PSP_UTILITY_DIALOG_NONE:
                        case PSP_UTILITY_DIALOG_FINISHED:
                        
                            done = true;
                        
                            break;
                    
                    }
                
                }
                
            }
            
            /**
             * Returns the dialog button pressed.
             * 
             * @return ResponseType
             */
            ResponseType getMsgDialogResult() {
                        
                return dialogRes;
            
            }
            
            /**
             * Returns whether any dialogs are currently active.
             * 
             * @return bool
             */
            bool isDialogActive() {
            
                return (isMsgDialogActive() || isNetDialogActive());
            
            }
            
            /**
             * Returns whether any msg (incl. error) dialogs are currently active.
             * 
             * @return bool
             */
            bool isMsgDialogActive() {
            
                return msgDialogActive;
            
            }
            
            /**
             * Returns whether any net dialogs are currently active.
             * 
             * @return bool
             */
            bool isNetDialogActive() {
            
                return netDialogActive;
            
            }
            
            /**
             * Render the dialogs.
             */
            void renderDialogs() {
            
                if (isMsgDialogActive()) {
                    
                    switch (sceUtilityMsgDialogGetStatus()) {
                    
                        // Shouldn't happen
                        case PSP_UTILITY_DIALOG_NONE:
                        
                            break;
                            
                        case PSP_UTILITY_DIALOG_VISIBLE:
                        case PSP_UTILITY_DIALOG_INIT:
                        
                            // No clue why we need to pass 1…
                            sceUtilityMsgDialogUpdate(1);
                            
                            break;
                            
                        case PSP_UTILITY_DIALOG_QUIT:

                            sceUtilityMsgDialogShutdownStart();
                                                        
                            break;
                            
                        case PSP_UTILITY_DIALOG_FINISHED:
                        
                            msgDialogActive = false;
                                                        
                            if (mDialogParams.buttonPressed == PSP_UTILITY_MSGDIALOG_RESULT_YES) {

                                dialogRes = RESPONSE_YES;
                                
                            } else if (mDialogParams.buttonPressed == PSP_UTILITY_MSGDIALOG_RESULT_NO) {

                                dialogRes = RESPONSE_NO;
                            
                            } else if (mDialogParams.buttonPressed == PSP_UTILITY_MSGDIALOG_RESULT_BACK) {

                                dialogRes = RESPONSE_BACK;
                                
                            } else if (mDialogParams.buttonPressed == PSP_UTILITY_MSGDIALOG_RESULT_UNKNOWN1 && (mDialogParams.options & PSP_UTILITY_MSGDIALOG_OPTION_YESNO_BUTTONS) != PSP_UTILITY_MSGDIALOG_OPTION_YESNO_BUTTONS) {
                            
                                dialogRes = RESPONSE_BACK;
                            
                            }
                                            
                            break;
                    
                    }
                
                }
                
                if (isNetDialogActive()) {
                
                    switch (sceUtilityNetconfGetStatus()) {
                    
                        // Shouldn't happen
                        case PSP_UTILITY_DIALOG_NONE:
                        
                            break;
                            
                        case PSP_UTILITY_DIALOG_VISIBLE:
                        case PSP_UTILITY_DIALOG_INIT:
                        
                            // No clue why we need to pass 1…
                            sceUtilityNetconfUpdate(1);
                            
                            break;
                            
                        case PSP_UTILITY_DIALOG_QUIT:

                            sceUtilityNetconfShutdownStart();
                                                        
                            break;
                            
                        case PSP_UTILITY_DIALOG_FINISHED:
                        
                            netDialogActive = false;
                                                                                                    
                            break;
                    
                    }
                
                }
            
            }
        
            /**
             * Pop up the standard PSP message dialog.
             * 
             * @param const std::string& umsg Message to display.
             * @param bool[optional] showYesNoOptions Whether to display the yes/no options.
             */
            void msg(const std::string& umsg, bool showYesNoOptions) {
            
                dialogRes = RESPONSE_NONE;
            
                // clear
                memset(&mDialogParams, 0, sizeof(mDialogParams));
                
                // set the size
                mDialogParams.base.size = sizeof(mDialogParams);
                
                // set language
                mDialogParams.base.language = Util::getSystemLanguage();
                
                // set button swap
                mDialogParams.base.buttonSwap = Util::areButtonsSwapped();
                
                // Thread priorities
                mDialogParams.base.graphicsThread =   0x11;
                mDialogParams.base.accessThread   =   0x13;
                mDialogParams.base.fontThread     =   0x12;
                mDialogParams.base.soundThread    =   0x10;
                
                // Setup dialog mode and options
                mDialogParams.mode = PSP_UTILITY_MSGDIALOG_MODE_TEXT;
                mDialogParams.options = PSP_UTILITY_MSGDIALOG_OPTION_TEXT;
                
                if (showYesNoOptions)
                    mDialogParams.options |= PSP_UTILITY_MSGDIALOG_OPTION_YESNO_BUTTONS | PSP_UTILITY_MSGDIALOG_OPTION_DEFAULT_NO;
                    
                // Copy the msg
                strcpy(mDialogParams.message, umsg.c_str());
                
                // init & start dialog
                int res = sceUtilityMsgDialogInitStart(&mDialogParams);
                                
                if (res == 0)
                    msgDialogActive = true;
            
            }
            
            /**
             * Pop up the standard PSP error dialog.
             * 
             * @param const unsigned int eNum Error number.
             */
            void err(const unsigned int eNum) {
            
                dialogRes = RESPONSE_NONE;
            
                // clear
                memset(&mDialogParams, 0, sizeof(mDialogParams));
                
                // set the size
                mDialogParams.base.size = sizeof(mDialogParams);
                
                // set language
                mDialogParams.base.language = Util::getSystemLanguage();
                
                // set button swap
                mDialogParams.base.buttonSwap = Util::areButtonsSwapped();
                
                // Thread priorities
                mDialogParams.base.graphicsThread =   0x11;
                mDialogParams.base.accessThread   =   0x13;
                mDialogParams.base.fontThread     =   0x12;
                mDialogParams.base.soundThread    =   0x10;
                
                // Setup dialog mode and options
                mDialogParams.mode = PSP_UTILITY_MSGDIALOG_MODE_ERROR;
                mDialogParams.options = PSP_UTILITY_MSGDIALOG_OPTION_ERROR;
                                    
                // Copy the error
                mDialogParams.errorValue = eNum;
                
                // init & start dialog
                int res = sceUtilityMsgDialogInitStart(&mDialogParams);
                
                if (res == 0)
                    msgDialogActive = true;
            
            }
            
            /**
             * Pop up the standard PSP wifi/net selection dialog.
             */
            void net() {
            
                // clear
                memset(&nDialogData, 0, sizeof(nDialogData));
                
                // set the size
                nDialogData.base.size = sizeof(nDialogData);
                
                // set language
                nDialogData.base.language = Util::getSystemLanguage();
                
                // set button swap
                nDialogData.base.buttonSwap = Util::areButtonsSwapped();
                
                // Thread priorities
                nDialogData.base.graphicsThread =   0x11;
                nDialogData.base.accessThread   =   0x13;
                nDialogData.base.fontThread     =   0x12;
                nDialogData.base.soundThread    =   0x10;
                
                nDialogData.action = PSP_NETCONF_ACTION_CONNECTAP;
                
                struct pspUtilityNetconfAdhoc adhocParam;
                memset(&adhocParam, 0, sizeof(adhocParam));
                nDialogData.adhocparam = &adhocParam;

                // init & start dialog
                int res = sceUtilityNetconfInitStart(&nDialogData);
                
                if (res == 0)
                    netDialogActive = true;
            
            }
        
        }
        
    }

}

#endif /* _Dialogs_CPP */
