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
 * Base state class.
 * Header file.
 * 
 * @package xMangaPSP
 */

#ifndef _BaseState_H
#define _BaseState_H

// BEGIN Includes

// END Includes

// BEGIN Defines

// END Defines

namespace xM {

    namespace States {

        class Base {
        public:

            /**
             * Start up code.
             */
            virtual void init(void) = 0;

            /**
             * Clean up code.
             */
            virtual void cleanUp(void) = 0;

            /**
             * Pause state.
             */
            virtual void pause(void) = 0;

            /**
             * Resume state.
             */
            virtual void resume(void) = 0;

            /**
             * Poll for input, read event state etc
             */
            virtual void handleEvents(void) = 0;

            /**
             * Now do something with the data we got from events and what not.
             */
            virtual void handleLogic(void) = 0;

            /**
             * Done with the logic? Draw what's needed then.
             */
            virtual void draw(void) = 0;

        };

    }

}

#endif /* _BaseState_H */
