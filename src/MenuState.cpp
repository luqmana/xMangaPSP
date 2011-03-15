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
#include "xM/Engine/FileManager.h"
#include "xM/Engine/InputManager.h"
#include "xM/Ui/Dialogs.h"
#include "xM/Util/Log.h"
// END Includes

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
            //testImg2.loadFile("test.png");
            //testImg2.swizzle();

            // A non-power-of-two image (>512x512)
            //testImg3.loadFile("test3.png");
            //testImg3.swizzle();

            // A power-of-two image (>512x512)
            //testImg4.loadFile("test4.png");
            //testImg4.swizzle();
            
            textFont.loadFont(Gfx::Font::LATIN_SANS_SERIF_REGULAR_SMALL, 1.0f, Gfx::Colour::GREEN, Gfx::Colour::RED, 0, 0);
                        
            // Register before loading the file
            parser.registerCustomElementHandler("psarText", this);
            parser.loadFile("ui/home.xml");
            
            // Which dialog
            dialog = 0;
                                    
        }

        /**
         * Clean up code.
         */
        void Menu::cleanUp(void) {

            parser.deRegisterCustomElementHandler("psarText");

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

            Engine::InputManager* iM = Engine::InputManager::getInstance();

            if (__xM_DEBUG && iM->pressed(PSP_CTRL_LTRIGGER)) {
            
                Util::logMsg("Reloading XML ui file.");
            
                parser.loadFile("ui/home.xml");    
            
            } else if (iM->pressed(PSP_CTRL_TRIANGLE)) {
            
                Ui::Dialog::msg("Hello!");
                
                dialog = 1;
            
            } else if (iM->pressed(PSP_CTRL_SQUARE)) {
            
                Ui::Dialog::msg("Do I work?", true);
                
                dialog = 2;
            
            } else if (iM->pressed(PSP_CTRL_CIRCLE)) {
            
                Ui::Dialog::net();
            
            }
            
        }

        /**
         * Now do something with the data we got from events and what not.
         */
        void Menu::handleLogic(void) {
        
            rotate -= (1.0f * timer.getDeltaTicks(true));
            
            if (dialog == 1 && Ui::Dialog::getMsgDialogResult() == Ui::Dialog::RESPONSE_BACK) {

                Ui::Dialog::msg("I knew you'd press 'Back'");
                
                dialog = 0;
            
            } else if (dialog == 2) {
            
                if (Ui::Dialog::getMsgDialogResult() == Ui::Dialog::RESPONSE_YES) {
                
                    Ui::Dialog::msg("Smart you are.");
                    dialog = 0;
                    
                } else if (Ui::Dialog::getMsgDialogResult() == Ui::Dialog::RESPONSE_NO) {
                
                    Ui::Dialog::msg("o.O\nChallenged are you?");
                    dialog = 0;
                
                } else if (Ui::Dialog::getMsgDialogResult() == Ui::Dialog::RESPONSE_BACK) {
                
                    Ui::Dialog::msg("Don't avoid the question!\nDo I work?", true);
                    
                    dialog = 2;
                
                }
            
            } else {
            
                dialog = 0;
                
            }

        }

        /**
         * Done with the logic? Draw what's needed then.
         */
        void Menu::draw(void) {
            
            parser.draw();
            
        }
        
        /**
         * A callback function definition to handle the setup of a custom element read from XML UI file.
         * 
         * @param XMLParser* parser Pointer to the current XML parser.
         * @param Element* customElement The custom element to be setup.
         */
        void Menu::initElement(Ui::XMLParser* parser, Ui::Element* customElement) {
        
            Engine::FileManager* fM = Engine::FileManager::getInstance();
        
            customElement->text = fM->readFromPSAR(customElement->attributes["psarFile"]);
                    
        }
        
        /**
         * A callback function definition to handle custom elements in an XML UI file.
         * 
         * @param XMLParser* parser Pointer to the current XML parser.
         * @param Element* customElement The custom element to be rendered.
         */
        void Menu::renderElement(Ui::XMLParser* parser, Ui::Element* customElement) {
                
            textFont.draw(customElement->x, customElement->y, customElement->text.c_str());
        
        }

    }

}

#endif /* _State_CPP */
