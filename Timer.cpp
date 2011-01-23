/* 
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
 * Full License: LICENSE.txt
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

/**
 * Timer class.
 * 
 * @package xMangaPSP
 */
 
#ifndef _Timer_CPP
#define _Timer_CPP

/* BEGIN Includes. */
#include <SDL/SDL.h>

#include "Timer.h"
/* END Includes. */

Timer::Timer() {

    // Initialize the variables
    
    startTicks = 0;
    pausedTicks = 0;
    
    paused = false;
    started = false;
    
}

void Timer::start() {

    // Start the timer
    started = true;

    // Unpause the timer
    paused = false;

    // Get the current time
    startTicks = SDL_GetTicks();
    
}

void Timer::stop() {

    //Stop the timer
    started = false;

    // Unpause the timer
    paused = false;
    
}

void Timer::pause() {

    // If the timer is running and isn't already paused
    if((started == true) && (paused == false)) {
    
        // Pause the timer
        paused = true;

        // Calculate the paused ticks
        pausedTicks = SDL_GetTicks() - startTicks;
        
    }
    
}

void Timer::unpause() {

    // If the timer is paused
    if(paused == true) {
    
        // Unpause the timer
        paused = false;

        //Reset the starting ticks
        startTicks = SDL_GetTicks() - pausedTicks;

        //Reset the paused ticks
        pausedTicks = 0;
        
    }
    
}

int Timer::getTicks() {

    // If the timer is running
    if(started == true) {
    
        // If the timer is paused
        if(paused == true) {
        
            // Return the number of ticks when the the timer was paused
            return pausedTicks;
            
        } else {
        
            // Return the current time minus the start time
            return SDL_GetTicks() - startTicks;
            
        }
        
    }

    // If the timer isn't running
    return 0;
    
}

bool Timer::isStarted() {

    return started;
    
}

bool Timer::isPaused() {

    return paused;
    
}

#endif /* _Timer_CPP */
