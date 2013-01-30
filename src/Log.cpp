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
 * Logging utility.
 *
 * @package xMangaPSP
 */

#ifndef _LOG_CPP
#define _LOG_CPP

// BEGIN Includes
#include "xM/Util/Log.h"

#include <pspiofilemgr.h>

#include <sstream>
#include <string.h>

#include <time.h>

#include <psprtc.h>

#include <stdarg.h>
#include <stdlib.h>
// END Includes

namespace xM {

    namespace Util {

        // FD for log file
        // Should probably close this somewhere…
        SceUID logFd = -1;

        /**
         * Close log file.
         */
        void closeLogFile() {
            sceIoClose(logFd);
        }

        /**
         * Log a message to a log file.
         * 
         * @access public
         * 
         * @param char* format Message format to log.
         */
        void logMsg(const char* format, ...) {

            // Holds the log msg
            std::stringstream logMsg;

            // Open the file if necessary
            if (logFd == -1) {
                logFd = sceIoOpen("xMLog.log", PSP_O_WRONLY | PSP_O_APPEND | PSP_O_CREAT, 0777);
                atexit(closeLogFile);
            }

            // Construct log msg
            logMsg << time(NULL) << ": " << format << "\n";

            // Arguments
            va_list options;

            // Formatted string
            char buffer[512];

            // Initialize option list
            va_start(options, format);

            // Format text
            vsnprintf(buffer, (size_t) sizeof (buffer), logMsg.str().c_str(), options);

            // Write the data to file
            sceIoWrite(logFd, buffer, strlen(buffer));

            // Write the data to console
            printf(buffer);

            // Quit option list
            va_end(options);

        }

    }

}

#endif /* _LOG_CPP */
