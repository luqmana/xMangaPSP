/*
 * $Id: ChapterSelectState.h 76 2009-12-25 02:42:26Z chaotic@luqmanrocks.co.cc $
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
 * Header for ChapterSelectState.
 * 
 * @package OneMangaPSP
 */

#ifndef _ChapterSelectState_H
#define _ChapterSelectState_H
// BEGIN Includes
#include "State.h"
#include "MangaAPI.h"
// END Includes

class ChapterSelectState : public State {		
	public:
				
		void init();
		void shutdown();
				
		void pause();
		void resume();
	
		void handleInput();
		
		void handleLogic();
		
		void render();
		
	private:
	
		// The chapter list
		ChapterList chapterList;
		
		// Holds all textures
	   	std::map <std::string, OMTexture > textures;
	   	
	   	// Counter for displaying menu
	   	int minList, maxList, selected;
	   	
	   	// doAction flag
	   	bool doAction;
	   	
	   	// Manga API Request ID
	   	int mangaApiRequestId;
		
};

#endif /* _ChapterSelectState_H */
