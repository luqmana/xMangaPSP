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

        /**
         * Start up code.
         */
        void Menu::init(void) {

            rotate = 0.0f;
            timer.start();

            // A Non-power-of-two image (<512x512)
            //testImg.loadFile("test2.png");
            //testImg.swizzle();

            // A power-of-two image (<512x512)
            testImg2.loadFile("test.png");
            testImg2.swizzle();

            // A non-power-of-two image (>512x512)
            //testImg3.loadFile("test3.png");
            //testImg3.swizzle();

            // A power-of-two image (>512x512)
            //testImg4.loadFile("test4.png");
            //testImg4.swizzle();
            
            textFont.loadFont(Gfx::Font::LATIN_SANS_SERIF_REGULAR, 0.8f, Gfx::Colour::WHITE, 0, 0, 0);
            
            textFont.setEncodingToUtf8();
            
        }

        /**
         * Clean up code.
         */
        void Menu::cleanUp(void) {



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

            Gfx::ImageClip clip = {32, 32, 64, 64};

            //testImg.draw(0, 0);
            testImg2.draw(120, 100, &clip);
            //testImg3.draw(0, 0);
            //testImg4.draw(0, 0);

            // Draw the first quad rotating
            Gfx::drawQuad(240.0f - (100 / 2), 160.0f - (100 / 2), 100, 100, Gfx::Colour::WHITE, GU_COLOR(1.0f, 0.0f, 0.0f, 0.75f),
                    GU_COLOR(0.0f, 1.0f, 0.0f, 0.50f), GU_COLOR(0.0f, 0.0f, 1.0f, 0.25f), rotate);

            // Draw the second quad rotating in the opposite direction 10 times as fast
            Gfx::drawQuad(240.0f - (50 / 2), 160.0f - (50 / 2), 50.0f, 50.0f, GU_COLOR(0.0f, 0.0f, 0.0f, 0.1f), -rotate * 10.0f);
            
            textFont.draw(180, 50, "Hello with intraFont! åèíôüñ.");
            
        }

    }

}

#endif /* _State_CPP */
