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
 * Timer class utility.
 * 
 * @package xMangaPSP
 */

#ifndef _TIMER_CPP
#define _TIMER_CPP

// BEGIN Includes
#include "xM/Util/Timer.h"
// END Includes

namespace xM {

    namespace Util {

        /**
         * Initializes the timer.
         */
        Timer::Timer(void) {

            startTicks = 0;
            pauseTicks = 0;

            paused = false;
            started = false;

            tickResolution = sceRtcGetTickResolution();

        }

        /**
         * Start the timer.
         *
         * @access public
         */
        void Timer::start(void) {

            // Start the timer
            started = true;

            // Unpause
            paused = false;

            // Get the current ticks
            sceRtcGetCurrentTick(&startTicks);

        }

        /**
         * Stop the timer.
         *
         * @access public
         */
        void Timer::stop(void) {

            // Stop the timer
            started = false;

            // Unpause
            paused = false;

        }

        /**
         * Pause the timer.
         *
         * @access public
         */
        void Timer::pause(void) {

            // If the timer is running and isn't already paused
            if (isStarted() && !isPaused()) {

                // Pause the timer
                paused = true;

                // Calculate the paused ticks
                sceRtcGetCurrentTick(&pauseTicks);
                pauseTicks = pauseTicks - startTicks;

            }

        }

        /**
         * Unpause the timer.
         *
         * @access public
         */
        void Timer::unpause(void) {

            // If the timer is paused
            if (isPaused()) {

                // Unpause
                paused = false;

                // Reset the starting ticks
                sceRtcGetCurrentTick(&startTicks);
                startTicks = startTicks - pauseTicks;

                // Reset the paused ticks
                pauseTicks = 0;

            }

        }

        /**
         * Get the delta of ticks.
         *
         * @access public
         *
         * @param bool reset[optional] Reset after getting delta?
         *
         * @return double
         */
        double Timer::getDeltaTicks(bool reset) {

            // If the timer is running
            if (isStarted()) {

                // If the timer is paused
                if (isPaused()) {

                    // Return the number of ticks when the the timer was paused
                    return pauseTicks / (float) tickResolution;

                } else {

                    u64 now;
                    sceRtcGetCurrentTick(&now);

                    // The current time minus the start time
                    double delta = (now - startTicks) / (float) tickResolution;

                    // Whether to reset
                    if (reset)
                        start();

                    return delta;

                }

            }

            // Timer isn't running
            return 0;

        }

        /**
         * Is timer started?
         *
         * @return bool
         */
        bool Timer::isStarted(void) {

            return started;

        }

        /**
         * Is timer paused?
         *
         * @return bool
         */
        bool Timer::isPaused(void) {

            return paused;

        }

    }

}

#endif /* _TIMER_CPP */
