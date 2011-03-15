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

namespace xM {

    namespace Ui {

        /**
         * Default constructor.
         */
        XMLParser::XMLParser() {

            this->uiXMLFile = NULL;

        }

        /**
         * Destructor.
         */
        XMLParser::~XMLParser() {

            delete this->uiXMLFile;
            
            for (unsigned int i = 0; i < this->uiElements.size(); ++i) {
            
                for (unsigned int j = 0; j < this->uiElements[i]->children.size(); ++j) {
            
                    delete this->uiElements[i]->children[j];
            
                }
            
                delete this->uiElements[i];
            
            }
            
            this->uiElements.clear();
            this->customElementHandlers.clear();

        }

        /**
         * Loads an XML ui file to parse.
         * 
         * @param const std::string& file The file to parse.
         */
        void XMLParser::parseFile(const std::string& xFile) {
        
            if (this->uiXMLFile != NULL) {
            
                delete this->uiXMLFile;
                
                for (unsigned int i = 0; i < this->uiElements.size(); ++i) {
                
                    for (unsigned int j = 0; j < this->uiElements[i]->children.size(); ++j) {
            
                        delete this->uiElements[i]->children[j];
            
                    }
                
                    delete this->uiElements[i];
                
                }
                
                this->uiElements.clear();
            
            }
            
            this->uiXMLFile = new TiXmlDocument();

            this->file = xFile;

            if (!this->uiXMLFile->LoadFile(file.c_str(), TIXML_ENCODING_UTF8)) {

                if (__xM_DEBUG)
                    Util::logMsg("XMLParser::parseFile — Unable to load UI XML file [%s][%s].", file.c_str(), uiXMLFile->ErrorDesc());

                return;

            }

            // Get the root node
            TiXmlNode* root = this->uiXMLFile->RootElement();
            if (root == NULL) {

                if (__xM_DEBUG)
                    Util::logMsg("XMLParser::parseFile — Unable to load UI XML root node [%s].", file.c_str());

                return;

            }

            // Loop over children
            TiXmlNode* childNode;
            for (childNode = TiXmlHandle(root).FirstChild().ToNode(); childNode; childNode = childNode->NextSibling()) {
            
                if (childNode->Type() != TiXmlNode::ELEMENT)
                    continue;
                    
                TiXmlElement* child = childNode->ToElement();

                Element* uiElement = new Element;
                uiElement->type = NOOP;
                uiElement->name = child->Value();
                uiElement->x = uiElement->y = uiElement->offsetX = uiElement->offsetY = uiElement->colour = uiElement->shadowColour = uiElement->width = uiElement->height = 0;
                uiElement->text = "";
                uiElement->size = 1.0;

                // Some common attributes
                if (child->QueryDoubleAttribute("x", &uiElement->x) != TIXML_SUCCESS) {
                    uiElement->x = 0;
                }
                if (child->QueryDoubleAttribute("y", &uiElement->y) != TIXML_SUCCESS) {
                    uiElement->y = 0;
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

                            unsigned int r, g, b;
                            r = Util::stringToInt(colourParts[0]);
                            g = Util::stringToInt(colourParts[1]);
                            b = Util::stringToInt(colourParts[2]);

                            uiElement->colour = GU_RGBA(r, g, b, 255);

                        } else if (colourParts.size() == 4) {

                            unsigned int r, g, b, a;
                            r = Util::stringToInt(colourParts[0]);
                            g = Util::stringToInt(colourParts[1]);
                            b = Util::stringToInt(colourParts[2]);
                            a = Util::stringToInt(colourParts[3]);

                            uiElement->colour = GU_RGBA(r, g, b, a);
                            
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

                            unsigned int r, g, b;
                            r = Util::stringToInt(colourParts[0]);
                            g = Util::stringToInt(colourParts[1]);
                            b = Util::stringToInt(colourParts[2]);
                            
                            uiElement->colour = GU_RGBA(r, g, b, 255);

                        } else if (colourParts.size() == 4) {

                            unsigned int r, g, b, a;
                            r = Util::stringToInt(colourParts[0]);
                            g = Util::stringToInt(colourParts[1]);
                            b = Util::stringToInt(colourParts[2]);
                            a = Util::stringToInt(colourParts[3]);

                            uiElement->colour = GU_RGBA(r, g, b, a);

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

                            unsigned int r, g, b;
                            r = Util::stringToInt(colourParts[0]);
                            g = Util::stringToInt(colourParts[1]);
                            b = Util::stringToInt(colourParts[2]);

                            uiElement->shadowColour = GU_RGBA(r, g, b, 255);

                        } else if (colourParts.size() == 4) {

                            unsigned int r, g, b, a;
                            r = Util::stringToInt(colourParts[0]);
                            g = Util::stringToInt(colourParts[1]);
                            b = Util::stringToInt(colourParts[2]);
                            a = Util::stringToInt(colourParts[3]);

                            uiElement->shadowColour = GU_RGBA(r, g, b, a);

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
                    
                    if (child->Attribute("altFonts") != NULL) {
                    
                        std::string otherFonts = child->Attribute("altFonts");
                        std::vector<std::string> altFonts;
                        
                        Util::tokenize(otherFonts, altFonts, ",");
                        
                        for (unsigned int i = 0; i < altFonts.size(); ++i) {
                        
                            std::string altFont = altFonts[i];
                            std::remove(altFont.begin(), altFont.end(), ' ');
                                            
                            if (altFont == "{LATIN_SANS_SERIF_REGULAR}") {

                                uiElement->font.loadAltFont(Gfx::Font::LATIN_SANS_SERIF_REGULAR, uiElement->size, uiElement->colour, uiElement->shadowColour, 0, 0);

                            } else if (altFont == "{LATIN_SERIF_REGULAR}") {

                                uiElement->font.loadAltFont(Gfx::Font::LATIN_SERIF_REGULAR, uiElement->size, uiElement->colour, uiElement->shadowColour, 0, 0);

                            } else if (altFont == "{LATIN_SANS_SERIF_ITALIC}") {

                                uiElement->font.loadAltFont(Gfx::Font::LATIN_SANS_SERIF_ITALIC, uiElement->size, uiElement->colour, uiElement->shadowColour, 0, 0);

                            } else if (altFont == "{LATIN_SERIF_ITALIC}") {

                                uiElement->font.loadAltFont(Gfx::Font::LATIN_SERIF_ITALIC, uiElement->size, uiElement->colour, uiElement->shadowColour, 0, 0);

                            } else if (altFont == "{LATIN_SANS_SERIF_BOLD}") {

                                uiElement->font.loadAltFont(Gfx::Font::LATIN_SANS_SERIF_BOLD, uiElement->size, uiElement->colour, uiElement->shadowColour, 0, 0);

                            } else if (altFont == "{LATIN_SERIF_BOLD}") {

                                uiElement->font.loadAltFont(Gfx::Font::LATIN_SERIF_BOLD, uiElement->size, uiElement->colour, uiElement->shadowColour, 0, 0);

                            } else if (altFont == "{LATIN_SANS_SERIF_ITALIC_BOLD}") {

                                uiElement->font.loadAltFont(Gfx::Font::LATIN_SANS_SERIF_ITALIC_BOLD, uiElement->size, uiElement->colour, uiElement->shadowColour, 0, 0);

                            } else if (altFont == "{LATIN_SERIF_ITALIC_BOLD}") {

                                uiElement->font.loadAltFont(Gfx::Font::LATIN_SERIF_ITALIC_BOLD, uiElement->size, uiElement->colour, uiElement->shadowColour, 0, 0);

                            } else if (altFont == "{LATIN_SANS_SERIF_REGULAR_SMALL}") {

                                uiElement->font.loadAltFont(Gfx::Font::LATIN_SANS_SERIF_REGULAR_SMALL, uiElement->size, uiElement->colour, uiElement->shadowColour, 0, 0);

                            } else if (altFont == "{LATIN_SERIF_REGULAR_SMALL}") {

                                uiElement->font.loadAltFont(Gfx::Font::LATIN_SERIF_REGULAR_SMALL, uiElement->size, uiElement->colour, uiElement->shadowColour, 0, 0);

                            } else if (altFont == "{LATIN_SANS_SERIF_ITALIC_SMALL}") {

                                uiElement->font.loadAltFont(Gfx::Font::LATIN_SANS_SERIF_ITALIC_SMALL, uiElement->size, uiElement->colour, uiElement->shadowColour, 0, 0);

                            } else if (altFont == "{LATIN_SERIF_ITALIC_SMALL}") {

                                uiElement->font.loadAltFont(Gfx::Font::LATIN_SERIF_ITALIC_SMALL, uiElement->size, uiElement->colour, uiElement->shadowColour, 0, 0);

                            } else if (altFont == "{LATIN_SANS_SERIF_BOLD_SMALL}") {

                                uiElement->font.loadAltFont(Gfx::Font::LATIN_SANS_SERIF_BOLD_SMALL, uiElement->size, uiElement->colour, uiElement->shadowColour, 0, 0);

                            } else if (altFont == "{LATIN_SERIF_BOLD_SMALL}") {

                                uiElement->font.loadAltFont(Gfx::Font::LATIN_SERIF_BOLD_SMALL, uiElement->size, uiElement->colour, uiElement->shadowColour, 0, 0);

                            } else if (altFont == "{LATIN_SANS_SERIF_ITALIC_BOLD_SMALL}") {

                                uiElement->font.loadAltFont(Gfx::Font::LATIN_SANS_SERIF_ITALIC_BOLD_SMALL, uiElement->size, uiElement->colour, uiElement->shadowColour, 0, 0);

                            } else if (altFont == "{LATIN_SERIF_ITALIC_BOLD_SMALL}") {

                                uiElement->font.loadAltFont(Gfx::Font::LATIN_SERIF_ITALIC_BOLD_SMALL, uiElement->size, uiElement->colour, uiElement->shadowColour, 0, 0);

                            } else if (altFont == "{JAPANESE_SJIS}") {

                                uiElement->font.loadAltFont(Gfx::Font::JAPANESE_SJIS, uiElement->size, uiElement->colour, uiElement->shadowColour, 0, 0);

                            } else if (altFont == "{JAPANESE_UTF8}") {

                                uiElement->font.loadAltFont(Gfx::Font::JAPANESE_UTF8, uiElement->size, uiElement->colour, uiElement->shadowColour, 0, 0);

                            } else if (altFont == "{KOREAN_UTF8}") {

                                uiElement->font.loadAltFont(Gfx::Font::KOREAN_UTF8, uiElement->size, uiElement->colour, uiElement->shadowColour, 0, 0);

                            } else if (altFont == "{SYMBOLS}") {

                                uiElement->font.loadAltFont(Gfx::Font::SYMBOLS, uiElement->size, uiElement->colour, uiElement->shadowColour, 0, 0);

                            } else if (altFont == "{CHINESE}") {

                                uiElement->font.loadAltFont(Gfx::Font::CHINESE, uiElement->size, uiElement->colour, uiElement->shadowColour, 0, 0);

                            } else {

                                // @TODO: Attempt to load via path

                            }
                                                    
                        }
                    
                    }

                //------IMAGE
                } else if (strcmp(child->Value(), "image") == 0) {
                
                    uiElement->type = IMAGE;
                    
                    if (child->QueryDoubleAttribute("width", &uiElement->width) != TIXML_SUCCESS) {
                        uiElement->width = 0;
                    }
                    if (child->QueryDoubleAttribute("height", &uiElement->height) != TIXML_SUCCESS) {
                        uiElement->height = 0;
                    }                    
                    if (child->QueryDoubleAttribute("offsetX", &uiElement->offsetX) != TIXML_SUCCESS) {
                        uiElement->offsetX = 0;
                    }
                    if (child->QueryDoubleAttribute("offsetY", &uiElement->offsetY) != TIXML_SUCCESS) {
                        uiElement->offsetY = 0;
                    }
                    
                    if (child->Attribute("src") == NULL) {
                        continue;
                    } else {
                    
                        std::string src = child->Attribute("src");
                        
                        uiElement->image.loadFile(src);
                        
                        if (strcmp(child->Attribute("swizzle"), "false") != 0)
                            uiElement->image.swizzle();
                                                
                    }

                //------CUSTOM [possibly]
                } else {
                        
                    // Try to find a handler        
                    std::map<std::string, CustomElementHandler*>::const_iterator customElementHandler = this->customElementHandlers.find(child->Value());

                    if (customElementHandler != this->customElementHandlers.end()) {
                    
                        uiElement->type = CUSTOM;
                        
                        // Collect the attributes for the custom element to pass on to the handler
                        std::map<std::string, std::string> attributes;
                        
                        TiXmlAttribute* attr = child->FirstAttribute();
                        
                        while (attr != NULL) {
                        
                            attributes.insert(std::pair<std::string, std::string>(attr->Name(), attr->Value()));
                            
                            attr = attr->Next();
                        
                        }
                        
                        uiElement->attributes = attributes;
                                                
                        // Call the back
                        customElementHandler->second->initElement(this, uiElement);
                    
                    } else
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

                Element* e = this->uiElements[i];

                switch (this->uiElements[i]->type) {

                    case QUAD:

                        Gfx::drawQuad(e->x, e->y, e->width, e->height, e->colour, 0);

                        break;

                    case TEXT:

                        e->font.draw(e->x, e->y, e->text.c_str());

                        break;

                    case IMAGE:
                                        
                        Gfx::ImageClip clip;
                        clip.x = e->offsetX;
                        clip.y = e->offsetY;
                        clip.width = e->width;
                        clip.height = e->height;
                    
                        e->image.draw(e->x, e->y, &clip);

                        break;
                        
                    case CUSTOM:
                    
                        // Let the callback render
                        this->customElementHandlers[e->name]->renderElement(this, e);
                    
                        break;

                    case NOOP:
                    default:
                        break;

                }

            }

        }
        
        /**
         * Register a custom element handler.
         * 
         * @param const std::string& element The custom element.
         * @param CustomElementHandler* handler Pointer to the handling class.
         */
        void XMLParser::registerCustomElementHandler(const std::string& element, CustomElementHandler* handler) {
        
            if (this->customElementHandlers.find(element) != this->customElementHandlers.end()) {
              
                if (__xM_DEBUG)
                    Util::logMsg("XMLParser::registerCustomElementHandler — Custom element handler already registered for element '%s'.", element.c_str());
                
            } else
                this->customElementHandlers.insert(std::pair<std::string, CustomElementHandler*>(element, handler));
        
        }
        
        /**
         * Deregister a custom element handler.
         * 
         * @param const std::string& element The custom element.
         */
        void XMLParser::deRegisterCustomElementHandler(const std::string& element) {
        
            if (this->customElementHandlers.find(element) != this->customElementHandlers.end())
                this->customElementHandlers.erase(element);
        
        }

    }

}

#endif /* _XMLParser_CPP */
