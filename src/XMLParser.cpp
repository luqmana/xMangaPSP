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
 * The XML UI parser.
 * 
 * @package xMangaPSP
 */

#ifndef _XMLParser_CPP
#define _XMLParser_CPP

// BEGIN Includes
#include "xM/Ui/XMLParser.h"
#include "xM/Util/Log.h"
#include "xM/Util/Utils.h"

#include <algorithm>
// END Includes

// BEGIN Defines

// END Defines

namespace xM {

    namespace Ui {
    
        /**
         * Default constructor.
         */
        XMLParser::XMLParser() {
        
            this->uiXMLFile = new TiXmlDocument();
        
        }
        
        /**
         * Destructor.
         */
        XMLParser::~XMLParser() {
        
            delete this->uiXMLFile;
        
        }
        
        /**
         * Loads an XML ui file to parse.
         * 
         * @param const std::string& file The file to parse.
         */
        void XMLParser::loadFile(const std::string& xFile) {
        
            this->file = xFile;
            
            if (!this->uiXMLFile->LoadFile(file.c_str(), TIXML_ENCODING_UTF8)) {
            
                if (__xM_DEBUG)
                    Util::logMsg("XMLParser::loadFile — Unable to load UI XML file [%s][%s].", file.c_str(), uiXMLFile->ErrorDesc());
                    
                return;
            
            }
            
            // Get the root node
	        TiXmlNode* root = this->uiXMLFile->RootElement();   
	        if (root == NULL) {
	        
	            if (__xM_DEBUG)
                    Util::logMsg("XMLParser::draw — Unable to load UI XML root node [%s].", file.c_str());
	        
	            return;
	        
	        }
	        	        
	        // Loop over children
	        TiXmlElement* child;
	        for(child = TiXmlHandle(root).FirstChild().ToElement(); child; child = child->NextSiblingElement()) {
                
	            Element* uiElement = new Element;
	            uiElement->type = NOOP;
	            uiElement->x = uiElement->y = uiElement->colour = uiElement->shadowColour = uiElement->width = uiElement->height = 0;
	            uiElement->text = "";
	            uiElement->size = 1.0;
	            
	            // Some common attributes that must be included for all elements
	            if (child->QueryDoubleAttribute("x", &uiElement->x) != TIXML_SUCCESS) {
	                continue;
	            }
	            if (child->QueryDoubleAttribute("y", &uiElement->y) != TIXML_SUCCESS) {
	                continue;
	            }
	            
	            // Specific elements
	            	            
	            //------QUAD
	            if (strcmp(child->Value(), "quad") == 0) {
	            
	                uiElement->type = QUAD;
	                
	                if (child->QueryDoubleAttribute("width", &uiElement->width) != TIXML_SUCCESS) {
	                    continue;
	                }
	                if (child->QueryDoubleAttribute("height", &uiElement->height) != TIXML_SUCCESS) {
	                    continue;
	                }
	                
	                if (child->Attribute("colour") == NULL) {
	                    uiElement->colour = 0;
	                } else {
	                
	                    std::string tempColour = child->Attribute("colour");
	                
	                    std::vector<std::string> colourParts;
	                    
	                    // Split string to RGBA parts
	                    Util::tokenize(tempColour, colourParts, ",");
	                    
	                    if (colourParts.size() == 3) {
	                    
	                        std::remove(colourParts[0].begin(), colourParts[0].end(), ' ');
	                        std::remove(colourParts[1].begin(), colourParts[1].end(), ' ');
	                        std::remove(colourParts[2].begin(), colourParts[2].end(), ' ');
	                    
	                        unsigned int r, g, b;
	                        r = atoi(colourParts[0].c_str());
	                        g = atoi(colourParts[1].c_str());
	                        b = atoi(colourParts[2].c_str());
	                    
	                        uiElement->colour = GU_COLOR((float)r/256, (float)g/256, (float)b/256, 1.0f);
	                    
	                    } else if (colourParts.size() == 4) {
	                    
	                        std::remove(colourParts[0].begin(), colourParts[0].end(), ' ');
	                        std::remove(colourParts[1].begin(), colourParts[1].end(), ' ');
	                        std::remove(colourParts[2].begin(), colourParts[2].end(), ' ');
	                        std::remove(colourParts[3].begin(), colourParts[3].end(), ' ');
	                    
	                        unsigned int r, g, b, a;
	                        r = atoi(colourParts[0].c_str());
	                        g = atoi(colourParts[1].c_str());
	                        b = atoi(colourParts[2].c_str());
	                        a = atoi(colourParts[3].c_str());
	                    
	                        uiElement->colour = GU_COLOR((float)r/256, (float)g/256, (float)b/256, (float)a/256);
	                    
	                    } else {
	                    
	                        uiElement->colour = 0;
	                        
	                    }
	                
	                }
	                	            
	            //------TEXT
	            } else if (strcmp(child->Value(), "text") == 0) {
	            
	                uiElement->type = TEXT;
	                
	                if (child->Attribute("value") == NULL) {
	                    continue;
	                } else {
	                    uiElement->text = child->Attribute("value");
	                }
	                
	                if (child->QueryDoubleAttribute("width", &uiElement->width) != TIXML_SUCCESS) {
	                    uiElement->width = 0;
                    }
                    
                    if (child->QueryDoubleAttribute("size", &uiElement->size) != TIXML_SUCCESS) {
	                    uiElement->size = 0;
                    }
                                        
	                if (child->Attribute("colour") == NULL) {
	                    uiElement->colour = 0;
	                } else {
	                
	                    std::string tempColour = child->Attribute("colour");
	                
	                    std::vector<std::string> colourParts;
	                    
	                    // Split string to RGBA parts
	                    Util::tokenize(tempColour, colourParts, ",");
	                    
	                    if (colourParts.size() == 3) {
	                    
	                        std::remove(colourParts[0].begin(), colourParts[0].end(), ' ');
	                        std::remove(colourParts[1].begin(), colourParts[1].end(), ' ');
	                        std::remove(colourParts[2].begin(), colourParts[2].end(), ' ');
	                    
	                        unsigned int r, g, b;
	                        r = atoi(colourParts[0].c_str());
	                        g = atoi(colourParts[1].c_str());
	                        b = atoi(colourParts[2].c_str());
	                    
	                        uiElement->colour = GU_COLOR((float)r/256, (float)g/256, (float)b/256, 1.0f);
	                    
	                    } else if (colourParts.size() == 4) {
	                    
	                        std::remove(colourParts[0].begin(), colourParts[0].end(), ' ');
	                        std::remove(colourParts[1].begin(), colourParts[1].end(), ' ');
	                        std::remove(colourParts[2].begin(), colourParts[2].end(), ' ');
	                        std::remove(colourParts[3].begin(), colourParts[3].end(), ' ');
	                    
	                        unsigned int r, g, b, a;
	                        r = atoi(colourParts[0].c_str());
	                        g = atoi(colourParts[1].c_str());
	                        b = atoi(colourParts[2].c_str());
	                        a = atoi(colourParts[3].c_str());
	                    
	                        uiElement->colour = GU_COLOR((float)r/256, (float)g/256, (float)b/256, (float)a/256);
	                    
	                    } else {
	                    
	                        uiElement->colour = 0;
	                        
	                    }
	                
	                }
	                
	                if (child->Attribute("shadowColour") == NULL) {
	                    uiElement->shadowColour = 0;
	                } else {
	                
	                    std::string tempColour = child->Attribute("shadowColour");
	                
	                    std::vector<std::string> colourParts;
	                    
	                    // Split string to RGBA parts
	                    Util::tokenize(tempColour, colourParts, ",");
	                    
	                    if (colourParts.size() == 3) {
	                    
	                        std::remove(colourParts[0].begin(), colourParts[0].end(), ' ');
	                        std::remove(colourParts[1].begin(), colourParts[1].end(), ' ');
	                        std::remove(colourParts[2].begin(), colourParts[2].end(), ' ');
	                    
	                        unsigned int r, g, b;
	                        r = atoi(colourParts[0].c_str());
	                        g = atoi(colourParts[1].c_str());
	                        b = atoi(colourParts[2].c_str());
	                    
	                        uiElement->shadowColour = GU_COLOR((float)r/256, (float)g/256, (float)b/256, 1.0f);
	                    
	                    } else if (colourParts.size() == 4) {
	                    
	                        std::remove(colourParts[0].begin(), colourParts[0].end(), ' ');
	                        std::remove(colourParts[1].begin(), colourParts[1].end(), ' ');
	                        std::remove(colourParts[2].begin(), colourParts[2].end(), ' ');
	                        std::remove(colourParts[3].begin(), colourParts[3].end(), ' ');
	                    
	                        unsigned int r, g, b, a;
	                        r = atoi(colourParts[0].c_str());
	                        g = atoi(colourParts[1].c_str());
	                        b = atoi(colourParts[2].c_str());
	                        a = atoi(colourParts[3].c_str());
	                    
	                        uiElement->shadowColour = GU_COLOR((float)r/256, (float)g/256, (float)b/256, (float)a/256);
	                    
	                    } else {
	                    
	                        uiElement->shadowColour = 0;
	                        
	                    }
	                
	                }
	                
	                if (child->Attribute("font") == NULL) {
	                    continue;
	                } else {
	                    
	                    std::string font = child->Attribute("font");
	                    
	                    if (font == "{LATIN_SANS_SERIF_REGULAR}") {

	                        uiElement->font.loadFont(Gfx::Font::LATIN_SANS_SERIF_REGULAR, uiElement->size, uiElement->colour, uiElement->shadowColour, 0, 0);
	                    
	                    } else if (font == "{LATIN_SERIF_REGULAR}") {

	                        uiElement->font.loadFont(Gfx::Font::LATIN_SERIF_REGULAR, uiElement->size, uiElement->colour, uiElement->shadowColour, 0, 0);
	                    
	                    } else if (font == "{LATIN_SANS_SERIF_ITALIC}") {

	                        uiElement->font.loadFont(Gfx::Font::LATIN_SANS_SERIF_ITALIC, uiElement->size, uiElement->colour, uiElement->shadowColour, 0, 0);
	                    
	                    } else if (font == "{LATIN_SERIF_ITALIC}") {

	                        uiElement->font.loadFont(Gfx::Font::LATIN_SERIF_ITALIC, uiElement->size, uiElement->colour, uiElement->shadowColour, 0, 0);
	                    
	                    } else if (font == "{LATIN_SANS_SERIF_BOLD}") {

	                        uiElement->font.loadFont(Gfx::Font::LATIN_SANS_SERIF_BOLD, uiElement->size, uiElement->colour, uiElement->shadowColour, 0, 0);
	                    
	                    } else if (font == "{LATIN_SERIF_BOLD}") {

	                        uiElement->font.loadFont(Gfx::Font::LATIN_SERIF_BOLD, uiElement->size, uiElement->colour, uiElement->shadowColour, 0, 0);
	                    
	                    } else if (font == "{LATIN_SANS_SERIF_ITALIC_BOLD}") {

	                        uiElement->font.loadFont(Gfx::Font::LATIN_SANS_SERIF_ITALIC_BOLD, uiElement->size, uiElement->colour, uiElement->shadowColour, 0, 0);
	                    
	                    } else if (font == "{LATIN_SERIF_ITALIC_BOLD}") {

	                        uiElement->font.loadFont(Gfx::Font::LATIN_SERIF_ITALIC_BOLD, uiElement->size, uiElement->colour, uiElement->shadowColour, 0, 0);
	                    
	                    } else if (font == "{LATIN_SANS_SERIF_REGULAR_SMALL}") {

	                        uiElement->font.loadFont(Gfx::Font::LATIN_SANS_SERIF_REGULAR_SMALL, uiElement->size, uiElement->colour, uiElement->shadowColour, 0, 0);
	                    
	                    } else if (font == "{LATIN_SERIF_REGULAR_SMALL}") {

	                        uiElement->font.loadFont(Gfx::Font::LATIN_SERIF_REGULAR_SMALL, uiElement->size, uiElement->colour, uiElement->shadowColour, 0, 0);
	                    
	                    } else if (font == "{LATIN_SANS_SERIF_ITALIC_SMALL}") {

	                        uiElement->font.loadFont(Gfx::Font::LATIN_SANS_SERIF_ITALIC_SMALL, uiElement->size, uiElement->colour, uiElement->shadowColour, 0, 0);
	                    
	                    } else if (font == "{LATIN_SERIF_ITALIC_SMALL}") {

	                        uiElement->font.loadFont(Gfx::Font::LATIN_SERIF_ITALIC_SMALL, uiElement->size, uiElement->colour, uiElement->shadowColour, 0, 0);
	                    
	                    } else if (font == "{LATIN_SANS_SERIF_BOLD_SMALL}") {

	                        uiElement->font.loadFont(Gfx::Font::LATIN_SANS_SERIF_BOLD_SMALL, uiElement->size, uiElement->colour, uiElement->shadowColour, 0, 0);
	                    
	                    } else if (font == "{LATIN_SERIF_BOLD_SMALL}") {

	                        uiElement->font.loadFont(Gfx::Font::LATIN_SERIF_BOLD_SMALL, uiElement->size, uiElement->colour, uiElement->shadowColour, 0, 0);
	                    
	                    } else if (font == "{LATIN_SANS_SERIF_ITALIC_BOLD_SMALL}") {

	                        uiElement->font.loadFont(Gfx::Font::LATIN_SANS_SERIF_ITALIC_BOLD_SMALL, uiElement->size, uiElement->colour, uiElement->shadowColour, 0, 0);
	                    
	                    } else if (font == "{LATIN_SERIF_ITALIC_BOLD_SMALL}") {

	                        uiElement->font.loadFont(Gfx::Font::LATIN_SERIF_ITALIC_BOLD_SMALL, uiElement->size, uiElement->colour, uiElement->shadowColour, 0, 0);
	                    
	                    } else if (font == "{JAPANESE_SJIS}") {

	                        uiElement->font.loadFont(Gfx::Font::JAPANESE_SJIS, uiElement->size, uiElement->colour, uiElement->shadowColour, 0, 0);
	                    
	                    } else if (font == "{JAPANESE_UTF8}") {

	                        uiElement->font.loadFont(Gfx::Font::JAPANESE_UTF8, uiElement->size, uiElement->colour, uiElement->shadowColour, 0, 0);
	                    
	                    } else if (font == "{KOREAN_UTF8}") {

	                        uiElement->font.loadFont(Gfx::Font::KOREAN_UTF8, uiElement->size, uiElement->colour, uiElement->shadowColour, 0, 0);
	                    
	                    } else if (font == "{SYMBOLS}") {

	                        uiElement->font.loadFont(Gfx::Font::SYMBOLS, uiElement->size, uiElement->colour, uiElement->shadowColour, 0, 0);
	                    
	                    } else if (font == "{CHINESE}") {

	                        uiElement->font.loadFont(Gfx::Font::CHINESE, uiElement->size, uiElement->colour, uiElement->shadowColour, 0, 0);
	                    
	                    } else {
	                    
	                        // @TODO: Attempt to load via path
	                    
	                    }	                    
	                    
	                }
	            
	            } else {
	            
	                continue;
	                
	            }
	            
	            if (uiElement->type == NOOP)
	                continue;
	            
	            // Add
	            this->uiElements.push_back(uiElement);
	            	        
	        }
                        
        }
        
        /**
         * Draws the UI described by the XML file.
         */
        void XMLParser::draw() {
        
            for (unsigned int i = 0; i < this->uiElements.size(); ++i) {
            
                switch(this->uiElements[i]->type) {
                
                    case QUAD:
                        
                        Gfx::drawQuad(this->uiElements[i]->x, this->uiElements[i]->y, this->uiElements[i]->width, this->uiElements[i]->height, this->uiElements[i]->colour, 0);
                    
                        break;
                        
                    case TEXT:
                    
                        break;
                        
                    case IMAGE:
                    
                        break;
                
                }
            
            }
	                        
        }
        
    }

}

#endif /* _XMLParser_CPP */
