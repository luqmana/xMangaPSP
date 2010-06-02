/*
 * $Id: TitleState.h 75 2009-12-24 04:17:52Z chaotic@luqmanrocks.co.cc $
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
 * Header for TitleState.
 * 
 * @package OneMangaPSP
 */

#ifndef _TitleState_H
#define _TitleState_H
// BEGIN Includes
#include "State.h"
#include "Timer.h"
// END Includes

class TitleState : public State {		
	public:
				
		TitleState(int which);
				
		void init();
		void shutdown();
				
		void pause();
		void resume();
	
		void handleInput();
		
		void handleLogic();
		
		void render();
		
	private:
	
		// Holds all textures
	   	std::map <std::string, OMTexture > textures;
	   	
	   	// Info on which splash to render
	   	int which;
	   	
	   	// Useful timer
	   	Timer* timer;	   	
	   	
	   	// Alpha value
	   	float alpha;
	   	
	   	// Fade in or out?
	   	int fadeMode;
		
};

#endif /* _TitleState_H */
