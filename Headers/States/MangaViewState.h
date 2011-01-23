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
 * Header for MangaViewState.
 * 
 * @package xMangaPSP
 */

#ifndef _MangaViewState_H
#define _MangaViewState_H

// BEGIN Includes
#include "State.h"
#include "MangaAPI.h"
// END Includes

class MangaViewState : public State {
		
	public:
				
		MangaViewState(int imageIndex);
				
		void init();
		void shutdown();
				
		void pause();
		void resume();
	
		void handleInput();
		
		void handleLogic();
		
		void render();
		
	private:
	
		// The image list
		ImageList imageList;
		
		// Holds all textures
	   	std::map <std::string, xMTexture > textures;
	   	
	   	// Manga image
	   	MangaImage* mImage;
	   	
	   	// doAction flag
	   	bool doAction;
	   	
	   	// Action
	   	int action;
	   	
	   	// Manga API Request ID
	   	int mangaApiRequestId;
	   	
	   	// Position variables
	   	int posX, posY;
	   	
	   	// Image index
	   	int imageIndex;
	   	
	   	// Scale sizes
	   	int scaleWidth, scaleHeight;
	   	
	   	// Scaled flag
	   	bool scaled;
	   	
	   	// Fullscreen flag
	   	bool fullscreen;
		
};

#endif /* _MangaViewState_H */
