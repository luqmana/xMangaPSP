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
 * Timer class utility.
 * Header file.
 * 
 * @package xMangaPSP
 */

#ifndef _TIMER_H
#define _TIMER_H

// BEGIN Includes
#include <stdio.h>
#include <stdlib.h>
#include <psptypes.h>
#include <psprtc.h>
// END Includes

namespace xM {

    namespace Util {

        class Timer {
        private:

            /**
             * Resolution of the tick counter.
             * Ticks per second
             */
            u32 tickResolution;

            /**
             * The ticks when the timer was started.
             */
            u64 startTicks;

            /**
             * The ticks stored when the timer was paused.
             */
            u64 pauseTicks;

            /**
             * Has the timer been started?
             */
            bool started;

            /**
             * Has the timer been paused?
             */
            bool paused;

        public:

            /**
             * Initializes the timer.
             */
            Timer(void);

            /**
             * Start the timer.
             *
             * @access public
             */
            void start(void);

            /**
             * Stop the timer.
             *
             * @access public
             */
            void stop(void);

            /**
             * Pause the timer.
             *
             * @access public
             */
            void pause(void);

            /**
             * Unpause the timer.
             *
             * @access public
             */
            void unpause(void);

            /**
             * Get the delta of ticks.
             *
             * @access public
             *
             * @param bool reset[optional] Reset after getting delta?
             *
             * @return double
             */
            double getDeltaTicks(bool reset = false);

            /**
             * Is timer started?
             *
             * @return bool
             */
            bool isStarted(void);

            /**
             * Is timer paused?
             *
             * @return bool
             */
            bool isPaused(void);

        };

    }

}


#endif /* _TIMER_H */
