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
 * Manga selection state class.
 * Header file.
 * 
 * @package xMangaPSP
 */

#ifndef _MangaSelectState_H
#define _MangaSelectState_H

// BEGIN Includes
#include "xM/States/Base.h"
#include "xM/Manga/MangaAPI.h"
#include "xM/Ui/ExtraElements.h"
#include "xM/Ui/XMLParser.h"
// END Includes

// BEGIN Defines

// END Defines

namespace xM {

    namespace States {

        class MangaSelect : public Base {
        public:

            /**
             * Start up code.
             */
            void init(void);

            /**
             * Clean up code.
             */
            void cleanUp(void);

            /**
             * Pause state.
             */
            void pause(void);

            /**
             * Resume state.
             */
            void resume(void);

            /**
             * Poll for input, read event state etc
             */
            void handleEvents(void);

            /**
             * Now do something with the data we got from events and what not.
             */
            void handleLogic(void);

            /**
             * Done with the logic? Draw what's needed then.
             */
            void draw(void);

        private:

            Ui::XMLParser parser;
            Ui::ExtraElements extraElements;
            Ui::ListInfo lInfo;
                        
            bool doAction;
            
            int activeDialog;
            
            unsigned int selected;
            Manga::MangaList mangaList;
            
            xM::Manga::APIMessage msg;
            SceUID localBox;

        };

    }

}

#endif /* _MangaSelectState_H */
