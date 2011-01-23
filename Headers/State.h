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
 * Header for State.
 * 
 * @package xMangaPSP
 */

#ifndef _State_H
#define _State_H

// BEGIN Includes
#include "LeXEngine.h"
#include "ResourceManager.h"
// END Includes

class State {
		
	public:
	
		LeXEngine *engine;
			
		virtual void init() = 0;
		virtual void shutdown() = 0;
		
		virtual void pause() = 0;
		virtual void resume() = 0;
	
		virtual void handleInput() = 0;
		
		virtual void handleLogic() = 0;
		
		virtual void render() = 0;
		
		void changeState(State *state) {
		
			this->engine->changeState(state);			
		
		}
		
		void assignEngine(LeXEngine *engine) {
		
			this->engine = engine;
		
		}		
		
};

#endif /* _State_H */
