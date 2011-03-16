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
         * Parse a single XML element.
         * 
         * @param TiXmlElement* xmlElement Element to parse.
         */
        Element* XMLParser::parseElement(TiXmlElement* xmlElement) {
        
            TiXmlNode* xmlNode = (TiXmlNode*)xmlElement;
        
            Element* uiElement = new Element;
            
            uiElement->type = NOOP;
            uiElement->name = xmlElement->Value();
            uiElement->x = uiElement->y = uiElement->offsetX = uiElement->offsetY = uiElement->colour = uiElement->shadowColour = uiElement->width = uiElement->height = uiElement->paddingLeft = uiElement->paddingRight = 0;
            uiElement->text = "";
            uiElement->size = 1.0;

            // Collect all the attributes
            std::map<std::string, std::string> attributes;
            
            TiXmlAttribute* attr = xmlElement->FirstAttribute();
            
            while (attr != NULL) {
            
                attributes.insert(std::pair<std::string, std::string>(attr->Name(), attr->Value()));
                
                attr = attr->Next();
            
            }
            
            uiElement->attributes = attributes;

            // Some common attributes
            if (xmlElement->QueryDoubleAttribute("x", &uiElement->x) != TIXML_SUCCESS) {
                uiElement->x = 0;
            }
            if (xmlElement->QueryDoubleAttribute("y", &uiElement->y) != TIXML_SUCCESS) {
                uiElement->y = 0;
            }
            if (xmlElement->QueryDoubleAttribute("offsetX", &uiElement->offsetX) != TIXML_SUCCESS) {
                uiElement->offsetX = 0;
            }
            if (xmlElement->QueryDoubleAttribute("offsetY", &uiElement->offsetY) != TIXML_SUCCESS) {
                uiElement->offsetY = 0;
            }
            if (xmlElement->QueryDoubleAttribute("paddingLeft", &uiElement->paddingLeft) != TIXML_SUCCESS) {
                uiElement->paddingLeft = 0;
            }
            if (xmlElement->QueryDoubleAttribute("paddingRight", &uiElement->paddingRight) != TIXML_SUCCESS) {
                uiElement->paddingRight = 0;
            }

            // Specific elements

            //------QUAD
            if (strcmp(xmlElement->Value(), "quad") == 0) {

                uiElement->type = QUAD;

                if (xmlElement->QueryDoubleAttribute("width", &uiElement->width) != TIXML_SUCCESS) {
                    
                    delete uiElement;
                    return NULL;
                    
                }
                if (xmlElement->QueryDoubleAttribute("height", &uiElement->height) != TIXML_SUCCESS) {
                    
                    delete uiElement;
                    return NULL;
                    
                }

                if (xmlElement->Attribute("colour") == NULL) {
                    uiElement->colour = 0;
                } else {

                    std::string tempColour = xmlElement->Attribute("colour");

                    std::vector<std::string> colourParts;

                    // Split string to RGBA parts
                    Util::tokenize(tempColour, colourParts, ",");

                    if (colourParts.size() == 1) {
                    
                        if (colourParts[0] == "red")
                            uiElement->colour = Gfx::Colour::RED;
                        else if (colourParts[0] == "green")
                            uiElement->colour = Gfx::Colour::GREEN;
                        else if (colourParts[0] == "blue")
                            uiElement->colour = Gfx::Colour::BLUE;
                        else if (colourParts[0] == "black")
                            uiElement->colour = Gfx::Colour::BLACK;
                        else if (colourParts[0] == "white")
                            uiElement->colour = Gfx::Colour::WHITE;
                        else if (colourParts[0] == "gray")
                            uiElement->colour = Gfx::Colour::GRAY;

                    } else if (colourParts.size() == 3) {

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
            } else if (strcmp(xmlElement->Value(), "text") == 0) {

                uiElement->type = TEXT;

                if (xmlElement->Attribute("value") == NULL) {
                    
                    delete uiElement;
                    return NULL;
                    
                } else {
                    uiElement->text = xmlElement->Attribute("value");
                }

                if (xmlElement->QueryDoubleAttribute("width", &uiElement->width) != TIXML_SUCCESS) {
                    uiElement->width = 0;
                }

                if (xmlElement->QueryDoubleAttribute("size", &uiElement->size) != TIXML_SUCCESS) {
                    uiElement->size = 0;
                }
                
                if (xmlElement->Attribute("colour") == NULL) {
                    uiElement->colour = 0;
                } else {

                    std::string tempColour = xmlElement->Attribute("colour");

                    std::vector<std::string> colourParts;

                    // Split string to RGBA parts
                    Util::tokenize(tempColour, colourParts, ",");

                    if (colourParts.size() == 1) {
                    
                        if (colourParts[0] == "red")
                            uiElement->colour = Gfx::Colour::RED;
                        else if (colourParts[0] == "green")
                            uiElement->colour = Gfx::Colour::GREEN;
                        else if (colourParts[0] == "blue")
                            uiElement->colour = Gfx::Colour::BLUE;
                        else if (colourParts[0] == "black")
                            uiElement->colour = Gfx::Colour::BLACK;
                        else if (colourParts[0] == "white")
                            uiElement->colour = Gfx::Colour::WHITE;
                        else if (colourParts[0] == "gray")
                            uiElement->colour = Gfx::Colour::GRAY;

                    } else if (colourParts.size() == 3) {

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

                if (xmlElement->Attribute("shadowColour") == NULL) {
                    uiElement->shadowColour = 0;
                } else {

                    std::string tempColour = xmlElement->Attribute("shadowColour");

                    std::vector<std::string> colourParts;

                    // Split string to RGBA parts
                    Util::tokenize(tempColour, colourParts, ",");

                    if (colourParts.size() == 1) {
                    
                        if (colourParts[0] == "red")
                            uiElement->shadowColour = Gfx::Colour::RED;
                        else if (colourParts[0] == "green")
                            uiElement->shadowColour = Gfx::Colour::GREEN;
                        else if (colourParts[0] == "blue")
                            uiElement->shadowColour = Gfx::Colour::BLUE;
                        else if (colourParts[0] == "black")
                            uiElement->shadowColour = Gfx::Colour::BLACK;
                        else if (colourParts[0] == "white")
                            uiElement->shadowColour = Gfx::Colour::WHITE;
                        else if (colourParts[0] == "gray")
                            uiElement->shadowColour = Gfx::Colour::GRAY;

                    } else if (colourParts.size() == 3) {

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
                
                unsigned int fontStyleOps = 0;
                
                if (xmlElement->Attribute("align") == NULL) {
                    fontStyleOps |= INTRAFONT_ALIGN_LEFT;
                } else {
                
                    std::string align = xmlElement->Attribute("align");
                    
                    if (align == "left")
                        fontStyleOps |= INTRAFONT_ALIGN_LEFT;
                    else if (align == "right")
                        fontStyleOps |= INTRAFONT_ALIGN_RIGHT;
                    else if (align == "center")
                        fontStyleOps |= INTRAFONT_ALIGN_CENTER;
                    else if (align == "full")
                        fontStyleOps |= INTRAFONT_ALIGN_FULL;
                
                }
                
                if (xmlElement->Attribute("font") == NULL) {
                    
                    delete uiElement;
                    return NULL;
                    
                } else {

                    std::string font = xmlElement->Attribute("font");

                    if (font == "{LATIN_SANS_SERIF_REGULAR}") {

                        uiElement->font.loadFont(Gfx::Font::LATIN_SANS_SERIF_REGULAR, uiElement->size, uiElement->colour, uiElement->shadowColour, 0, fontStyleOps);

                    } else if (font == "{LATIN_SERIF_REGULAR}") {

                        uiElement->font.loadFont(Gfx::Font::LATIN_SERIF_REGULAR, uiElement->size, uiElement->colour, uiElement->shadowColour, 0, fontStyleOps);

                    } else if (font == "{LATIN_SANS_SERIF_ITALIC}") {

                        uiElement->font.loadFont(Gfx::Font::LATIN_SANS_SERIF_ITALIC, uiElement->size, uiElement->colour, uiElement->shadowColour, 0, fontStyleOps);

                    } else if (font == "{LATIN_SERIF_ITALIC}") {

                        uiElement->font.loadFont(Gfx::Font::LATIN_SERIF_ITALIC, uiElement->size, uiElement->colour, uiElement->shadowColour, 0, fontStyleOps);

                    } else if (font == "{LATIN_SANS_SERIF_BOLD}") {

                        uiElement->font.loadFont(Gfx::Font::LATIN_SANS_SERIF_BOLD, uiElement->size, uiElement->colour, uiElement->shadowColour, 0, fontStyleOps);

                    } else if (font == "{LATIN_SERIF_BOLD}") {

                        uiElement->font.loadFont(Gfx::Font::LATIN_SERIF_BOLD, uiElement->size, uiElement->colour, uiElement->shadowColour, 0, fontStyleOps);

                    } else if (font == "{LATIN_SANS_SERIF_ITALIC_BOLD}") {

                        uiElement->font.loadFont(Gfx::Font::LATIN_SANS_SERIF_ITALIC_BOLD, uiElement->size, uiElement->colour, uiElement->shadowColour, 0, fontStyleOps);

                    } else if (font == "{LATIN_SERIF_ITALIC_BOLD}") {

                        uiElement->font.loadFont(Gfx::Font::LATIN_SERIF_ITALIC_BOLD, uiElement->size, uiElement->colour, uiElement->shadowColour, 0, fontStyleOps);

                    } else if (font == "{LATIN_SANS_SERIF_REGULAR_SMALL}") {

                        uiElement->font.loadFont(Gfx::Font::LATIN_SANS_SERIF_REGULAR_SMALL, uiElement->size, uiElement->colour, uiElement->shadowColour, 0, fontStyleOps);

                    } else if (font == "{LATIN_SERIF_REGULAR_SMALL}") {

                        uiElement->font.loadFont(Gfx::Font::LATIN_SERIF_REGULAR_SMALL, uiElement->size, uiElement->colour, uiElement->shadowColour, 0, fontStyleOps);

                    } else if (font == "{LATIN_SANS_SERIF_ITALIC_SMALL}") {

                        uiElement->font.loadFont(Gfx::Font::LATIN_SANS_SERIF_ITALIC_SMALL, uiElement->size, uiElement->colour, uiElement->shadowColour, 0, fontStyleOps);

                    } else if (font == "{LATIN_SERIF_ITALIC_SMALL}") {

                        uiElement->font.loadFont(Gfx::Font::LATIN_SERIF_ITALIC_SMALL, uiElement->size, uiElement->colour, uiElement->shadowColour, 0, fontStyleOps);

                    } else if (font == "{LATIN_SANS_SERIF_BOLD_SMALL}") {

                        uiElement->font.loadFont(Gfx::Font::LATIN_SANS_SERIF_BOLD_SMALL, uiElement->size, uiElement->colour, uiElement->shadowColour, 0, fontStyleOps);

                    } else if (font == "{LATIN_SERIF_BOLD_SMALL}") {

                        uiElement->font.loadFont(Gfx::Font::LATIN_SERIF_BOLD_SMALL, uiElement->size, uiElement->colour, uiElement->shadowColour, 0, fontStyleOps);

                    } else if (font == "{LATIN_SANS_SERIF_ITALIC_BOLD_SMALL}") {

                        uiElement->font.loadFont(Gfx::Font::LATIN_SANS_SERIF_ITALIC_BOLD_SMALL, uiElement->size, uiElement->colour, uiElement->shadowColour, 0, fontStyleOps);

                    } else if (font == "{LATIN_SERIF_ITALIC_BOLD_SMALL}") {

                        uiElement->font.loadFont(Gfx::Font::LATIN_SERIF_ITALIC_BOLD_SMALL, uiElement->size, uiElement->colour, uiElement->shadowColour, 0, fontStyleOps);

                    } else if (font == "{JAPANESE_SJIS}") {

                        uiElement->font.loadFont(Gfx::Font::JAPANESE_SJIS, uiElement->size, uiElement->colour, uiElement->shadowColour, 0, fontStyleOps);

                    } else if (font == "{JAPANESE_UTF8}") {

                        uiElement->font.loadFont(Gfx::Font::JAPANESE_UTF8, uiElement->size, uiElement->colour, uiElement->shadowColour, 0, fontStyleOps);

                    } else if (font == "{KOREAN_UTF8}") {

                        uiElement->font.loadFont(Gfx::Font::KOREAN_UTF8, uiElement->size, uiElement->colour, uiElement->shadowColour, 0, fontStyleOps);

                    } else if (font == "{SYMBOLS}") {

                        uiElement->font.loadFont(Gfx::Font::SYMBOLS, uiElement->size, uiElement->colour, uiElement->shadowColour, 0, fontStyleOps);

                    } else if (font == "{CHINESE}") {

                        uiElement->font.loadFont(Gfx::Font::CHINESE, uiElement->size, uiElement->colour, uiElement->shadowColour, 0, fontStyleOps);

                    } else {

                        // @TODO: Attempt to load via path

                    }
                    
                }
                
                if (xmlElement->Attribute("altFonts") != NULL) {
                
                    std::string otherFonts = xmlElement->Attribute("altFonts");
                    std::vector<std::string> altFonts;
                    
                    Util::tokenize(otherFonts, altFonts, ",");
                    
                    for (unsigned int i = 0; i < altFonts.size(); ++i) {
                    
                        std::string altFont = altFonts[i];
                        std::remove(altFont.begin(), altFont.end(), ' ');
                                        
                        if (altFont == "{LATIN_SANS_SERIF_REGULAR}") {

                            uiElement->font.loadAltFont(Gfx::Font::LATIN_SANS_SERIF_REGULAR, uiElement->size, uiElement->colour, uiElement->shadowColour, 0, fontStyleOps);

                        } else if (altFont == "{LATIN_SERIF_REGULAR}") {

                            uiElement->font.loadAltFont(Gfx::Font::LATIN_SERIF_REGULAR, uiElement->size, uiElement->colour, uiElement->shadowColour, 0, fontStyleOps);

                        } else if (altFont == "{LATIN_SANS_SERIF_ITALIC}") {

                            uiElement->font.loadAltFont(Gfx::Font::LATIN_SANS_SERIF_ITALIC, uiElement->size, uiElement->colour, uiElement->shadowColour, 0, fontStyleOps);

                        } else if (altFont == "{LATIN_SERIF_ITALIC}") {

                            uiElement->font.loadAltFont(Gfx::Font::LATIN_SERIF_ITALIC, uiElement->size, uiElement->colour, uiElement->shadowColour, 0, fontStyleOps);

                        } else if (altFont == "{LATIN_SANS_SERIF_BOLD}") {

                            uiElement->font.loadAltFont(Gfx::Font::LATIN_SANS_SERIF_BOLD, uiElement->size, uiElement->colour, uiElement->shadowColour, 0, fontStyleOps);

                        } else if (altFont == "{LATIN_SERIF_BOLD}") {

                            uiElement->font.loadAltFont(Gfx::Font::LATIN_SERIF_BOLD, uiElement->size, uiElement->colour, uiElement->shadowColour, 0, fontStyleOps);

                        } else if (altFont == "{LATIN_SANS_SERIF_ITALIC_BOLD}") {

                            uiElement->font.loadAltFont(Gfx::Font::LATIN_SANS_SERIF_ITALIC_BOLD, uiElement->size, uiElement->colour, uiElement->shadowColour, 0, fontStyleOps);

                        } else if (altFont == "{LATIN_SERIF_ITALIC_BOLD}") {

                            uiElement->font.loadAltFont(Gfx::Font::LATIN_SERIF_ITALIC_BOLD, uiElement->size, uiElement->colour, uiElement->shadowColour, 0, fontStyleOps);

                        } else if (altFont == "{LATIN_SANS_SERIF_REGULAR_SMALL}") {

                            uiElement->font.loadAltFont(Gfx::Font::LATIN_SANS_SERIF_REGULAR_SMALL, uiElement->size, uiElement->colour, uiElement->shadowColour, 0, fontStyleOps);

                        } else if (altFont == "{LATIN_SERIF_REGULAR_SMALL}") {

                            uiElement->font.loadAltFont(Gfx::Font::LATIN_SERIF_REGULAR_SMALL, uiElement->size, uiElement->colour, uiElement->shadowColour, 0, fontStyleOps);

                        } else if (altFont == "{LATIN_SANS_SERIF_ITALIC_SMALL}") {

                            uiElement->font.loadAltFont(Gfx::Font::LATIN_SANS_SERIF_ITALIC_SMALL, uiElement->size, uiElement->colour, uiElement->shadowColour, 0, fontStyleOps);

                        } else if (altFont == "{LATIN_SERIF_ITALIC_SMALL}") {

                            uiElement->font.loadAltFont(Gfx::Font::LATIN_SERIF_ITALIC_SMALL, uiElement->size, uiElement->colour, uiElement->shadowColour, 0, fontStyleOps);

                        } else if (altFont == "{LATIN_SANS_SERIF_BOLD_SMALL}") {

                            uiElement->font.loadAltFont(Gfx::Font::LATIN_SANS_SERIF_BOLD_SMALL, uiElement->size, uiElement->colour, uiElement->shadowColour, 0, fontStyleOps);

                        } else if (altFont == "{LATIN_SERIF_BOLD_SMALL}") {

                            uiElement->font.loadAltFont(Gfx::Font::LATIN_SERIF_BOLD_SMALL, uiElement->size, uiElement->colour, uiElement->shadowColour, 0, fontStyleOps);

                        } else if (altFont == "{LATIN_SANS_SERIF_ITALIC_BOLD_SMALL}") {

                            uiElement->font.loadAltFont(Gfx::Font::LATIN_SANS_SERIF_ITALIC_BOLD_SMALL, uiElement->size, uiElement->colour, uiElement->shadowColour, 0, fontStyleOps);

                        } else if (altFont == "{LATIN_SERIF_ITALIC_BOLD_SMALL}") {

                            uiElement->font.loadAltFont(Gfx::Font::LATIN_SERIF_ITALIC_BOLD_SMALL, uiElement->size, uiElement->colour, uiElement->shadowColour, 0, fontStyleOps);

                        } else if (altFont == "{JAPANESE_SJIS}") {

                            uiElement->font.loadAltFont(Gfx::Font::JAPANESE_SJIS, uiElement->size, uiElement->colour, uiElement->shadowColour, 0, fontStyleOps);

                        } else if (altFont == "{JAPANESE_UTF8}") {

                            uiElement->font.loadAltFont(Gfx::Font::JAPANESE_UTF8, uiElement->size, uiElement->colour, uiElement->shadowColour, 0, fontStyleOps);

                        } else if (altFont == "{KOREAN_UTF8}") {

                            uiElement->font.loadAltFont(Gfx::Font::KOREAN_UTF8, uiElement->size, uiElement->colour, uiElement->shadowColour, 0, fontStyleOps);

                        } else if (altFont == "{SYMBOLS}") {

                            uiElement->font.loadAltFont(Gfx::Font::SYMBOLS, uiElement->size, uiElement->colour, uiElement->shadowColour, 0, fontStyleOps);

                        } else if (altFont == "{CHINESE}") {

                            uiElement->font.loadAltFont(Gfx::Font::CHINESE, uiElement->size, uiElement->colour, uiElement->shadowColour, 0, fontStyleOps);

                        } else {

                            // @TODO: Attempt to load via path

                        }
                                                
                    }
                
                }
                
            //------IMAGE
            } else if (strcmp(xmlElement->Value(), "image") == 0) {
            
                uiElement->type = IMAGE;
                
                if (xmlElement->QueryDoubleAttribute("width", &uiElement->width) != TIXML_SUCCESS) {
                    uiElement->width = 0;
                }
                if (xmlElement->QueryDoubleAttribute("height", &uiElement->height) != TIXML_SUCCESS) {
                    uiElement->height = 0;
                }                    
                
                if (xmlElement->Attribute("src") == NULL) {
                    
                    delete uiElement;
                    return NULL;
                    
                } else {
                
                    std::string src = xmlElement->Attribute("src");
                    
                    uiElement->image.loadFile(src);
                    
                    if (strcmp(xmlElement->Attribute("swizzle"), "false") != 0)
                        uiElement->image.swizzle();
                                            
                }

            //------CUSTOM [possibly]
            } else {
                    
                // Try to find a handler        
                std::map<std::string, CustomElementHandler*>::const_iterator customElementHandler = this->customElementHandlers.find(xmlElement->Value());

                if (customElementHandler != this->customElementHandlers.end()) {
                
                    uiElement->type = CUSTOM;
                                        
                    // Collect all child elements
                    TiXmlNode* innerChild = xmlNode->FirstChild();    
                    while (innerChild != NULL) {
                    
                        uiElement->children.push_back(this->parseElement(innerChild->ToElement()));
                    
                        innerChild = innerChild->NextSibling();    
                            
                    }
                                                                    
                    // Call the back
                    customElementHandler->second->initElement(this, uiElement);
                
                } else {
                    
                    delete uiElement;
                    return NULL;
                    
                }

            } 
            
            return uiElement;
        
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
                
                // Parse the element
                Element* e = this->parseElement(child);

                if (e != NULL && e->type != NOOP)
                    this->uiElements.push_back(e);

            }

        }
        
        /**
         * Render a specific element.
         * 
         * @param Element* e Element to render.
         */
        void XMLParser::renderElement(Element* e) {
        
            switch (e->type) {

                case QUAD:

                    Gfx::drawQuad(e->x, e->y, e->width, e->height, e->colour, 0);

                    break;

                case TEXT:

                    e->font.draw(e->x + e->paddingLeft, e->y, e->text.c_str());

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

        /**
         * Draws the UI described by the XML file.
         */
        void XMLParser::draw() {

            for (unsigned int i = 0; i < this->uiElements.size(); ++i) {

                this->renderElement(this->uiElements[i]);

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
