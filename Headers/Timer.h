/*
 * $Id: Timer.h 75 2009-12-24 04:17:52Z chaotic@luqmanrocks.co.cc $
 * 
 * This file is part of the OneMangaPSP application.
 *
 * Copyright (C) 2009  Luqman Aden <www.luqmanrocks.co.cc>.
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
 * Timer class definition.
 * 
 * @package OneMangaPSP
 */
 
#ifndef _Timer_H
#define _Timer_H

class Timer {

    private:
    
	    // The time when the timer started
	    int startTicks;

	    // The ticks stored when the timer was paused
	    int pausedTicks;

	    // The timer status
	    bool paused;
	    bool started;

    public:
    
	    // Initializes variables
	    Timer();

	    // The various clock actions
	    void start();
	    void stop();
	    void pause();
	    void unpause();

	    // Gets the timer's time
	    int getTicks();

	    // Checks the status of the timer
	    bool isStarted();
	    bool isPaused();
    
};

#endif /* _Timer_H */
