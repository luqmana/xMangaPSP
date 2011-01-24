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
 * Menu state class.
 * 
 * @package xMangaPSP
 */

#ifndef _MenuState_CPP
#define _MenuState_CPP

// BEGIN Includes
#include "xM/States/Menu.h"
// END Includes

// BEGIN Defines

// END Defines

namespace xM {

	namespace States {
	
	    /*xM::Gfx::Vertex __attribute__((aligned(16))) quad[4] = {
	        {GU_COLOR(1.0f, 1.0f, 1.0f, 1.0f), -50.0f, -50.0f, 0.0f}, // Top-Left point
	        {GU_COLOR(1.0f, 0.0f, 0.0f, 1.0f), 50.0f,  -50.0f, 0.0f}, // Top-Right point
	        {GU_COLOR(0.0f, 1.0f, 0.0f, 1.0f), -50.0f, 50.0f, 0.0f}, // Bottom-Left point
	        {GU_COLOR(0.0f, 0.0f, 1.0f, 1.0f), 50.0f, 50.0f, 0.0f} // Bottom-Right point
        };
        	        
        xM::Gfx::Vertex __attribute__((aligned(16))) quad2[4] = {
	        {GU_COLOR(0.0f, 0.0f, 0.0f, 0.5f), -25.0f, -25.0f, 0.0f}, // Top-Left point
	        {GU_COLOR(0.0f, 0.0f, 0.0f, 0.5f), 25.0f,  -25.0f, 0.0f}, // Top-Right point
	        {GU_COLOR(0.0f, 0.0f, 0.0f, 0.5f), -25.0f, 25.0f, 0.0f}, // Bottom-Left point
	        {GU_COLOR(0.0f, 0.0f, 0.0f, 0.5f), 25.0f, 25.0f, 0.0f} // Bottom-Right point
        };*/
        
        /**
         * Start up code.
         */
        void Menu::init(void) {
                
            quad = new xM::Gfx::Vertex[4];
            
            // Top-Left point
            quad[0].colour  = GU_COLOR(1.0f, 1.0f, 1.0f, 1.0f);
            quad[0].x       = -50.0f;
            quad[0].y       = -50.0f;
            quad[0].z       =   0.0f;
            
            // Top-Right point
            quad[1].colour  = GU_COLOR(1.0f, 0.0f, 0.0f, 1.0f);
            quad[1].x       =  50.0f;
            quad[1].y       = -50.0f;
            quad[1].z       =   0.0f;
            
            // Bottom-Left point
            quad[2].colour  = GU_COLOR(0.0f, 1.0f, 0.0f, 1.0f);
            quad[2].x       = -50.0f;
            quad[2].y       =  50.0f;
            quad[2].z       =   0.0f;
            
            // Bottom-Right point
            quad[3].colour  = GU_COLOR(0.0f, 0.0f, 1.0f, 1.0f);
            quad[3].x       =  50.0f;
            quad[3].y       =  50.0f;
            quad[3].z       =   0.0f;
	            
            rotate = 0.0f;
            timer.start();
        
        }
        
        /**
         * Clean up code.
         */
        void Menu::cleanUp(void) {
        
            delete [] quad;
            //delete [] quad2;
            
            quad = NULL;
            //quad2 = NULL;
        
        }
        
        /**
         * Pause state.
         */
        void Menu::pause(void) {
        
            
        
        }
        
        /**
         * Resume state.
         */
        void Menu::resume(void) {
        
            
        
        }
        
        /**
         * Poll for input, read event state etc
         */
        void Menu::handleEvents(void) {
        
            
        
        }
        
        /**
         * Now do something with the data we got from events and what not.
         */
        void Menu::handleLogic(void) {
        
            rotate -= (1.0f * timer.getDeltaTicks(true));
        
        }
        
        /**
         * Done with the logic? Draw what's needed then.
         */
        void Menu::draw(void) {
                
            sceGumMatrixMode(GU_MODEL);
		    sceGumLoadIdentity(); // Reset
		    {
			
			    ScePspFVector3 pos = {240.0f, 160.0f, 0.0f};
			
			    // Move
			    sceGumTranslate(&pos);
			
			    // Rotate
			    sceGumRotateZ(-rotate);
		
		    }
	
		    // Draw the quad
		    sceGumDrawArray(GU_TRIANGLE_STRIP, GU_COLOR_8888 | GU_VERTEX_32BITF | GU_TRANSFORM_3D, 4, 0, quad);
		
		    // Draw the second quad
		    Gfx::drawQuad(240.0f, 160.0f, 0.0f, 50.0f, 50.0f, GU_COLOR(0.0f, 0.0f, 0.0f, 0.5f), rotate);
				        
        }
		        					
	}

}

#endif /* _State_CPP */
