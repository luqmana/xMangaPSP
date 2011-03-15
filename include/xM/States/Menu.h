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
 * Header file.
 * 
 * @package xMangaPSP
 */

#ifndef _MenuState_H
#define _MenuState_H

// BEGIN Includes
#include "xM/States/Base.h"
#include "xM/Gfx/Graphics.h"
#include "xM/Gfx/Image.h"
#include "xM/Gfx/Text.h"
#include "xM/Ui/Element.h"
#include "xM/Ui/CustomElementHandler.h"
#include "xM/Util/Timer.h"
#include "xM/Ui/XMLParser.h"
// END Includes

// BEGIN Defines

// END Defines

namespace xM {

    namespace States {

        class Menu : public Base, public Ui::CustomElementHandler {
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
            
            /**
             * A callback function definition to handle the setup of a custom element read from XML UI file.
             * 
             * @param XMLParser* parser Pointer to the current XML parser.
             * @param Element* customElement The custom element to be setup.
             */
            virtual void initElement(Ui::XMLParser* parser, Ui::Element* customElement);
            
            /**
             * A callback function definition to handle rendering custom elements in an XML UI file.
             * 
             * @param XMLParser* parser Pointer to the current XML parser.
             * @param Element* customElement The custom element to be rendered.
             */
            virtual void renderElement(Ui::XMLParser* parser, Ui::Element* customElement);

        private:

            Util::Timer timer;

            float rotate;
            
            Gfx::Text textFont;
            
            Gfx::Image testImg;
            Gfx::Image testImg2;
            Gfx::Image testImg3;
            Gfx::Image testImg4;
            
            int dialog;
            
            Ui::XMLParser parser;
            
            std::string readData;
            std::string otherReadData;

        };

    }

}

#endif /* _State_H */
