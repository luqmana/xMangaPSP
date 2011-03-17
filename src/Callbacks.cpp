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
 * Sets up callbacks on the PSP.
 * 
 * @package xMangaPSP
 */

#ifndef _Callbacks_CPP
#define _Callbacks_CPP

// BEGIN Includes
#include "xM/Engine/App.h"
#include "xM/Stn/Callbacks.h"
// END Includes

namespace xM {

    namespace Stn {

        /**
         * The standard callback thread id.
         */
        int callbackThreadId = -1;

        /**
         * The exit callback id.
         */
        int exitCallbackId = -1;

        /**
         * The standard exit callback.
         * Triggered when HOME button is pressed.
         *
         * @return int
         */
        int exitCallback(int arg1, int arg2, void *common) {

            Engine::quit();

            return 0;

        }

        /**
         * Standard callback thread handling function.
         * Registers various callbacks.
         *
         * @return int
         */
        int callbackThread(SceSize args, void *argp) {

            // Create callback
            exitCallbackId = sceKernelCreateCallback("Standard Exit Callback", exitCallback, NULL);

            // Register callback
            sceKernelRegisterExitCallback(exitCallbackId);

            // Let the thread sleep
            sceKernelSleepThreadCB();

            return 0;

        }

        /**
         * Sets up the standard callback handling thread.
         */
        void setupCallbacks(void) {

            // Create the thread
            callbackThreadId = sceKernelCreateThread("StandardCallbackThread", callbackThread, 0x11, 0xFA0, 0, 0);

            // Start it
            sceKernelStartThread(callbackThreadId, 0, 0);

        }

    }

}

#endif /* _Callbacks_CPP */
