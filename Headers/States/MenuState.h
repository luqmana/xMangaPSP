/*
 * $Id: MenuState.h 85 2010-01-02 16:57:15Z chaotic@luqmanrocks.co.cc $
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
 * Header for MenuState.
 * 
 * @package OneMangaPSP
 */

#ifndef _MenuState_H
#define _MenuState_H
// BEGIN Includes
#include "State.h"
#include "OMPNetwork.h"
#include "MangaAPI.h"

#include <iostream>
#include <vector>
// END Includes

class MenuState : public State {		
	public:
		
		MenuState();
		
		void init();
		void shutdown();
				
		void pause();
		void resume();
	
		void handleInput();
		
		void handleLogic();
		
		void render();
		
	private:
	    
	    // Hold all menu items
	    std::vector <OMStrArray> menuItems;
	    	    	    	    
	    // Holds all textures
	   	std::map <std::string, OMTexture > textures;
	   	
	   	// Counter for displaying menu
	   	int minList, maxList, selected;
	   	
	   	// doAction flag
	   	bool doAction;
	   	
	   	// Manga List API Request ID
	   	int mangaListApiRequestId;
	   	
	   	// Recent Manga List API Request ID
	   	int recentMangaListApiRequestId;
		
};

#endif /* _MenuState_H */
