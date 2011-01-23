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
 * Header for AboutState.
 * 
 * @package xMangaPSP
 */

#ifndef _AboutState_H
#define _AboutState_H

// BEGIN Includes
#include "State.h"
// END Includes

class AboutState : public State {
		
	public:
				
		void init();
		void shutdown();
		
		void cleanUpMusic();
				
		void pause();
		void resume();
	
		void handleInput();
		
		void handleLogic();
		
		void render();
		
	private:

		// Hold all credit items
	    std::vector <xMStrArray> creditItems;

	    // Holds all textures
	   	std::map <std::string, xMTexture > textures;
	   	
	   	// Counter for displaying menu
	   	int minList, maxList, selected;
		
};

#endif /* _AboutState_H */
