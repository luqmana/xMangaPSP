/**
 * This file is part of the xMangaPSP application.
 *
 * Copyright (C) Luqman Aden <www.luqman.ca>.
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
 * App file.
 * Header file.
 * 
 * @package xMangaPSP
 */

#ifndef _App_H
#define _App_H

// BEGIN Includes

// END Includes

// BEGIN Defines

// END Defines

namespace xM {

    namespace Engine {
    
        extern bool running;
        
        /**
         * Whether the app is currently 'running'. If false, shutdown will being.
         * 
         * @return bool
         */
        inline bool isRunning() {
        
            return running;
        
        }
                    
    }

}

#endif /* _App_H */
