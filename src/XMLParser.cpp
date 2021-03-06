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
 * The XML UI parser.
 * 
 * @package xMangaPSP
 */

#ifndef _XMLParser_CPP
#define _XMLParser_CPP

// BEGIN Includes
#include "xM/Engine/ResourceManager.h"
#include "xM/Net/Net.h"
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
            uiElement->x = uiElement->y = uiElement->offsetX = uiElement->offsetY = uiElement->colour = uiElement->shadowColour = uiElement->width = uiElement->height = uiElement->paddingLeft = uiElement->paddingTop = 0;
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
            if (xmlElement->QueryFloatAttribute("x", &uiElement->x) != TIXML_SUCCESS) {
                uiElement->x = 0;
            }
            if (xmlElement->QueryFloatAttribute("y", &uiElement->y) != TIXML_SUCCESS) {
                uiElement->y = 0;
            }
            if (xmlElement->QueryIntAttribute("width", (int*)&uiElement->width) != TIXML_SUCCESS) {
                uiElement->width = 0;
            }
            if (xmlElement->QueryIntAttribute("height",(int*) &uiElement->height) != TIXML_SUCCESS) {
                uiElement->height = 0;
            }
            if (xmlElement->QueryFloatAttribute("offsetX", &uiElement->offsetX) != TIXML_SUCCESS) {
                uiElement->offsetX = 0;
            }
            if (xmlElement->QueryFloatAttribute("offsetY", &uiElement->offsetY) != TIXML_SUCCESS) {
                uiElement->offsetY = 0;
            }
            if (xmlElement->QueryFloatAttribute("paddingLeft", &uiElement->paddingLeft) != TIXML_SUCCESS) {
                uiElement->paddingLeft = 0;
            }
            if (xmlElement->QueryFloatAttribute("paddingTop", &uiElement->paddingTop) != TIXML_SUCCESS) {
                uiElement->paddingTop = 0;
            }
            if (xmlElement->QueryDoubleAttribute("rotate", &uiElement->rotate) != TIXML_SUCCESS) {
                uiElement->rotate = 0;
            }
            if (xmlElement->Attribute("align") == NULL) {
                uiElement->align = ALIGN_LEFT;
            } else {
            
                std::string align = xmlElement->Attribute("align");
                
                if (align == "left")
                    uiElement->align = ALIGN_LEFT;
                else if (align == "right")
                    uiElement->align = ALIGN_RIGHT;
                else if (align == "center")
                    uiElement->align = ALIGN_CENTER;
                else if (align == "full")
                    uiElement->align = ALIGN_FULL;
                else
                    uiElement->align = ALIGN_LEFT;
            
            }           
            
            uiElement->whence = (xmlElement->Attribute("whence") != NULL) ? xmlElement->Attribute("whence") : ""; 

            // Specific elements

            //------QUAD
            if (strcmp(xmlElement->Value(), "quad") == 0) {

                uiElement->type = QUAD;

                if (uiElement->width == 0) {
                    
                    delete uiElement;
                    return NULL;
                    
                }
                if (uiElement->height == 0) {
                    
                    delete uiElement;
                    return NULL;
                    
                }

                if (xmlElement->Attribute("colour") == NULL) {
                    uiElement->colour = 0;
                } else {

                    uiElement->colour = Gfx::colourFromString(xmlElement->Attribute("colour"));

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
                
                if (xmlElement->QueryDoubleAttribute("size", &uiElement->size) != TIXML_SUCCESS) {
                    uiElement->size = 0;
                }
                
                // handle newline characters
                // replace \n with 0x0A
                size_t newline = uiElement->text.find("\\n");
                while (newline != std::string::npos) {
                    
                    uiElement->text.replace(newline, 2, "\n");
                    
                    newline = uiElement->text.find("\\n");
                }
                
                // handle var substitutions
                for (std::map<std::string, std::string>::const_iterator iter = this->textSubstitutes.begin(); iter != this->textSubstitutes.end(); ++iter) {
                
                    std::string tag = "{" + iter->first + "}";
                    
                    size_t pos = uiElement->text.find(tag);
                    
                    if (pos != std::string::npos) {
                    
                        uiElement->text.replace(pos, tag.size(), iter->second);
                    
                    } else
                        continue;
                
                }
                
                if (xmlElement->Attribute("colour") == NULL) {
                    uiElement->colour = 0;
                } else {

                    uiElement->colour = Gfx::colourFromString(xmlElement->Attribute("colour"));

                }

                if (xmlElement->Attribute("shadowColour") == NULL) {
                    uiElement->shadowColour = 0;
                } else {

                    uiElement->shadowColour = Gfx::colourFromString(xmlElement->Attribute("shadowColour"));

                }
                                                
                unsigned int fontStyleOps = 0;

                switch (uiElement->align) {
                
                    case ALIGN_LEFT:
                    
                        fontStyleOps |= INTRAFONT_ALIGN_LEFT;
                    
                        break;
                        
                    case ALIGN_RIGHT:
                    
                        fontStyleOps |= INTRAFONT_ALIGN_RIGHT;
                    
                        break;
                        
                    case ALIGN_CENTER:
                    
                        fontStyleOps |= INTRAFONT_ALIGN_CENTER;
                    
                        break;
                        
                    case ALIGN_FULL:
                    
                        fontStyleOps |= INTRAFONT_ALIGN_FULL;
                    
                        break;
                
                }
                
                if (xmlElement->Attribute("scroll") != NULL) {
                
                    std::string scroll = xmlElement->Attribute("scroll");
                    
                    if (scroll == "left") {
                    
                        uiElement->scroll = SCROLL_LEFT;
                        fontStyleOps |= INTRAFONT_SCROLL_LEFT;
                        
                    } else if (scroll == "through") {
                    
                        uiElement->scroll = SCROLL_THROUGH;
                        fontStyleOps |= INTRAFONT_SCROLL_THROUGH;
                        
                    } else if (scroll == "seesaw") {
                    
                        uiElement->scroll = SCROLL_SEESAW;
                        fontStyleOps |= INTRAFONT_SCROLL_SEESAW;
                        
                    }
                    
                } else
                    uiElement->scroll = SCROLL_NONE;
                
                if (xmlElement->Attribute("font") == NULL) {
                    
                    delete uiElement;
                    return NULL;
                    
                } else {

                    std::string font = xmlElement->Attribute("font");

                    if (font == "{LATIN_SANS_SERIF_REGULAR}") {

                        uiElement->font.loadFont(Gfx::Font::LATIN_SANS_SERIF_REGULAR, uiElement->size, uiElement->colour, uiElement->shadowColour, 0, fontStyleOps, uiElement->rotate);

                    } else if (font == "{LATIN_SERIF_REGULAR}") {

                        uiElement->font.loadFont(Gfx::Font::LATIN_SERIF_REGULAR, uiElement->size, uiElement->colour, uiElement->shadowColour, 0, fontStyleOps, uiElement->rotate);

                    } else if (font == "{LATIN_SANS_SERIF_ITALIC}") {

                        uiElement->font.loadFont(Gfx::Font::LATIN_SANS_SERIF_ITALIC, uiElement->size, uiElement->colour, uiElement->shadowColour, 0, fontStyleOps, uiElement->rotate);

                    } else if (font == "{LATIN_SERIF_ITALIC}") {

                        uiElement->font.loadFont(Gfx::Font::LATIN_SERIF_ITALIC, uiElement->size, uiElement->colour, uiElement->shadowColour, 0, fontStyleOps, uiElement->rotate);

                    } else if (font == "{LATIN_SANS_SERIF_BOLD}") {

                        uiElement->font.loadFont(Gfx::Font::LATIN_SANS_SERIF_BOLD, uiElement->size, uiElement->colour, uiElement->shadowColour, 0, fontStyleOps, uiElement->rotate);

                    } else if (font == "{LATIN_SERIF_BOLD}") {

                        uiElement->font.loadFont(Gfx::Font::LATIN_SERIF_BOLD, uiElement->size, uiElement->colour, uiElement->shadowColour, 0, fontStyleOps, uiElement->rotate);

                    } else if (font == "{LATIN_SANS_SERIF_ITALIC_BOLD}") {

                        uiElement->font.loadFont(Gfx::Font::LATIN_SANS_SERIF_ITALIC_BOLD, uiElement->size, uiElement->colour, uiElement->shadowColour, 0, fontStyleOps, uiElement->rotate);

                    } else if (font == "{LATIN_SERIF_ITALIC_BOLD}") {

                        uiElement->font.loadFont(Gfx::Font::LATIN_SERIF_ITALIC_BOLD, uiElement->size, uiElement->colour, uiElement->shadowColour, 0, fontStyleOps, uiElement->rotate);

                    } else if (font == "{LATIN_SANS_SERIF_REGULAR_SMALL}") {

                        uiElement->font.loadFont(Gfx::Font::LATIN_SANS_SERIF_REGULAR_SMALL, uiElement->size, uiElement->colour, uiElement->shadowColour, 0, fontStyleOps, uiElement->rotate);

                    } else if (font == "{LATIN_SERIF_REGULAR_SMALL}") {

                        uiElement->font.loadFont(Gfx::Font::LATIN_SERIF_REGULAR_SMALL, uiElement->size, uiElement->colour, uiElement->shadowColour, 0, fontStyleOps, uiElement->rotate);

                    } else if (font == "{LATIN_SANS_SERIF_ITALIC_SMALL}") {

                        uiElement->font.loadFont(Gfx::Font::LATIN_SANS_SERIF_ITALIC_SMALL, uiElement->size, uiElement->colour, uiElement->shadowColour, 0, fontStyleOps, uiElement->rotate);

                    } else if (font == "{LATIN_SERIF_ITALIC_SMALL}") {

                        uiElement->font.loadFont(Gfx::Font::LATIN_SERIF_ITALIC_SMALL, uiElement->size, uiElement->colour, uiElement->shadowColour, 0, fontStyleOps, uiElement->rotate);

                    } else if (font == "{LATIN_SANS_SERIF_BOLD_SMALL}") {

                        uiElement->font.loadFont(Gfx::Font::LATIN_SANS_SERIF_BOLD_SMALL, uiElement->size, uiElement->colour, uiElement->shadowColour, 0, fontStyleOps, uiElement->rotate);

                    } else if (font == "{LATIN_SERIF_BOLD_SMALL}") {

                        uiElement->font.loadFont(Gfx::Font::LATIN_SERIF_BOLD_SMALL, uiElement->size, uiElement->colour, uiElement->shadowColour, 0, fontStyleOps, uiElement->rotate);

                    } else if (font == "{LATIN_SANS_SERIF_ITALIC_BOLD_SMALL}") {

                        uiElement->font.loadFont(Gfx::Font::LATIN_SANS_SERIF_ITALIC_BOLD_SMALL, uiElement->size, uiElement->colour, uiElement->shadowColour, 0, fontStyleOps, uiElement->rotate);

                    } else if (font == "{LATIN_SERIF_ITALIC_BOLD_SMALL}") {

                        uiElement->font.loadFont(Gfx::Font::LATIN_SERIF_ITALIC_BOLD_SMALL, uiElement->size, uiElement->colour, uiElement->shadowColour, 0, fontStyleOps, uiElement->rotate);

                    } else if (font == "{JAPANESE_SJIS}") {

                        uiElement->font.loadFont(Gfx::Font::JAPANESE_SJIS, uiElement->size, uiElement->colour, uiElement->shadowColour, 0, fontStyleOps, uiElement->rotate);

                    } else if (font == "{JAPANESE_UTF8}") {

                        uiElement->font.loadFont(Gfx::Font::JAPANESE_UTF8, uiElement->size, uiElement->colour, uiElement->shadowColour, 0, fontStyleOps, uiElement->rotate);

                    } else if (font == "{KOREAN_UTF8}") {

                        uiElement->font.loadFont(Gfx::Font::KOREAN_UTF8, uiElement->size, uiElement->colour, uiElement->shadowColour, 0, fontStyleOps, uiElement->rotate);

                    } else if (font == "{SYMBOLS}") {

                        uiElement->font.loadFont(Gfx::Font::SYMBOLS, uiElement->size, uiElement->colour, uiElement->shadowColour, 0, fontStyleOps, uiElement->rotate);

                    } else if (font == "{CHINESE}") {

                        uiElement->font.loadFont(Gfx::Font::CHINESE, uiElement->size, uiElement->colour, uiElement->shadowColour, 0, fontStyleOps, uiElement->rotate);

                    } else {

                        uiElement->font.loadFont(font, uiElement->size, uiElement->colour, uiElement->shadowColour, 0, fontStyleOps, uiElement->rotate);

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

                            uiElement->font.loadAltFont(Gfx::Font::LATIN_SANS_SERIF_REGULAR, uiElement->size, uiElement->colour, uiElement->shadowColour, 0, fontStyleOps, uiElement->rotate);

                        } else if (altFont == "{LATIN_SERIF_REGULAR}") {

                            uiElement->font.loadAltFont(Gfx::Font::LATIN_SERIF_REGULAR, uiElement->size, uiElement->colour, uiElement->shadowColour, 0, fontStyleOps, uiElement->rotate);

                        } else if (altFont == "{LATIN_SANS_SERIF_ITALIC}") {

                            uiElement->font.loadAltFont(Gfx::Font::LATIN_SANS_SERIF_ITALIC, uiElement->size, uiElement->colour, uiElement->shadowColour, 0, fontStyleOps, uiElement->rotate);

                        } else if (altFont == "{LATIN_SERIF_ITALIC}") {

                            uiElement->font.loadAltFont(Gfx::Font::LATIN_SERIF_ITALIC, uiElement->size, uiElement->colour, uiElement->shadowColour, 0, fontStyleOps, uiElement->rotate);

                        } else if (altFont == "{LATIN_SANS_SERIF_BOLD}") {

                            uiElement->font.loadAltFont(Gfx::Font::LATIN_SANS_SERIF_BOLD, uiElement->size, uiElement->colour, uiElement->shadowColour, 0, fontStyleOps, uiElement->rotate);

                        } else if (altFont == "{LATIN_SERIF_BOLD}") {

                            uiElement->font.loadAltFont(Gfx::Font::LATIN_SERIF_BOLD, uiElement->size, uiElement->colour, uiElement->shadowColour, 0, fontStyleOps, uiElement->rotate);

                        } else if (altFont == "{LATIN_SANS_SERIF_ITALIC_BOLD}") {

                            uiElement->font.loadAltFont(Gfx::Font::LATIN_SANS_SERIF_ITALIC_BOLD, uiElement->size, uiElement->colour, uiElement->shadowColour, 0, fontStyleOps, uiElement->rotate);

                        } else if (altFont == "{LATIN_SERIF_ITALIC_BOLD}") {

                            uiElement->font.loadAltFont(Gfx::Font::LATIN_SERIF_ITALIC_BOLD, uiElement->size, uiElement->colour, uiElement->shadowColour, 0, fontStyleOps, uiElement->rotate);

                        } else if (altFont == "{LATIN_SANS_SERIF_REGULAR_SMALL}") {

                            uiElement->font.loadAltFont(Gfx::Font::LATIN_SANS_SERIF_REGULAR_SMALL, uiElement->size, uiElement->colour, uiElement->shadowColour, 0, fontStyleOps, uiElement->rotate);

                        } else if (altFont == "{LATIN_SERIF_REGULAR_SMALL}") {

                            uiElement->font.loadAltFont(Gfx::Font::LATIN_SERIF_REGULAR_SMALL, uiElement->size, uiElement->colour, uiElement->shadowColour, 0, fontStyleOps, uiElement->rotate);

                        } else if (altFont == "{LATIN_SANS_SERIF_ITALIC_SMALL}") {

                            uiElement->font.loadAltFont(Gfx::Font::LATIN_SANS_SERIF_ITALIC_SMALL, uiElement->size, uiElement->colour, uiElement->shadowColour, 0, fontStyleOps, uiElement->rotate);

                        } else if (altFont == "{LATIN_SERIF_ITALIC_SMALL}") {

                            uiElement->font.loadAltFont(Gfx::Font::LATIN_SERIF_ITALIC_SMALL, uiElement->size, uiElement->colour, uiElement->shadowColour, 0, fontStyleOps, uiElement->rotate);

                        } else if (altFont == "{LATIN_SANS_SERIF_BOLD_SMALL}") {

                            uiElement->font.loadAltFont(Gfx::Font::LATIN_SANS_SERIF_BOLD_SMALL, uiElement->size, uiElement->colour, uiElement->shadowColour, 0, fontStyleOps, uiElement->rotate);

                        } else if (altFont == "{LATIN_SERIF_BOLD_SMALL}") {

                            uiElement->font.loadAltFont(Gfx::Font::LATIN_SERIF_BOLD_SMALL, uiElement->size, uiElement->colour, uiElement->shadowColour, 0, fontStyleOps, uiElement->rotate);

                        } else if (altFont == "{LATIN_SANS_SERIF_ITALIC_BOLD_SMALL}") {

                            uiElement->font.loadAltFont(Gfx::Font::LATIN_SANS_SERIF_ITALIC_BOLD_SMALL, uiElement->size, uiElement->colour, uiElement->shadowColour, 0, fontStyleOps, uiElement->rotate);

                        } else if (altFont == "{LATIN_SERIF_ITALIC_BOLD_SMALL}") {

                            uiElement->font.loadAltFont(Gfx::Font::LATIN_SERIF_ITALIC_BOLD_SMALL, uiElement->size, uiElement->colour, uiElement->shadowColour, 0, fontStyleOps, uiElement->rotate);

                        } else if (altFont == "{JAPANESE_SJIS}") {

                            uiElement->font.loadAltFont(Gfx::Font::JAPANESE_SJIS, uiElement->size, uiElement->colour, uiElement->shadowColour, 0, fontStyleOps, uiElement->rotate);

                        } else if (altFont == "{JAPANESE_UTF8}") {

                            uiElement->font.loadAltFont(Gfx::Font::JAPANESE_UTF8, uiElement->size, uiElement->colour, uiElement->shadowColour, 0, fontStyleOps, uiElement->rotate);

                        } else if (altFont == "{KOREAN_UTF8}") {

                            uiElement->font.loadAltFont(Gfx::Font::KOREAN_UTF8, uiElement->size, uiElement->colour, uiElement->shadowColour, 0, fontStyleOps, uiElement->rotate);

                        } else if (altFont == "{SYMBOLS}") {

                            uiElement->font.loadAltFont(Gfx::Font::SYMBOLS, uiElement->size, uiElement->colour, uiElement->shadowColour, 0, fontStyleOps, uiElement->rotate);

                        } else if (altFont == "{CHINESE}") {

                            uiElement->font.loadAltFont(Gfx::Font::CHINESE, uiElement->size, uiElement->colour, uiElement->shadowColour, 0, fontStyleOps, uiElement->rotate);

                        } else {

                            uiElement->font.loadAltFont(altFont, uiElement->size, uiElement->colour, uiElement->shadowColour, 0, fontStyleOps, uiElement->rotate);

                        }
                                                
                    }
                
                }
                
            //------IMAGE
            } else if (strcmp(xmlElement->Value(), "image") == 0) {
            
                uiElement->type = IMAGE;
                
                if (xmlElement->QueryIntAttribute("width", (int*)&uiElement->width) != TIXML_SUCCESS) {
                    uiElement->width = 0;
                }
                if (xmlElement->QueryIntAttribute("height", (int*)&uiElement->height) != TIXML_SUCCESS) {
                    uiElement->height = 0;
                }                    
                
                if (xmlElement->Attribute("src") == NULL) {
                    
                    delete uiElement;
                    return NULL;
                    
                } else {
                
                    if (xmlElement->Attribute("cache") != NULL && strcmp(xmlElement->Attribute("cache"), "false") == 0) {
                        
                        uiElement->image = new Gfx::Image();
            
                        if (!uiElement->image->loadFile(xmlElement->Attribute("src")))
                            uiElement->image = NULL;

                    } else
                        uiElement->image = Engine::ResourceManager::getInstance()->getImage(xmlElement->Attribute("src"));
                    
                    if (uiElement->image == NULL) {
                    
                        delete uiElement;
                        return NULL;
                    
                    }
                    
                    if (xmlElement->Attribute("swizzle") != NULL && strcmp(xmlElement->Attribute("swizzle"), "true") == 0)
                        uiElement->image->swizzle();
                      
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
                    
                        if (innerChild->Type() == TiXmlNode::TINYXML_ELEMENT) {
                            Element *e = this->parseElement(innerChild->ToElement());
                            if (e != NULL)
                                uiElement->children.push_back(e);
                        }
                    
                        innerChild = innerChild->NextSibling();    
                            
                    }
                    
                    void* data = NULL;
                    
                    if (customElementHandlersData.find(xmlElement->Value()) != customElementHandlersData.end())
                        data = customElementHandlersData.find(xmlElement->Value())->second;
                                                                    
                    // Call the back
                    customElementHandler->second->initElement(this, uiElement, data);
                                    
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
            
            if (!this->uiXMLFile->Parse(Engine::ResourceManager::getInstance()->getRes(this->file).c_str())) {

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
            
                if (childNode->Type() != TiXmlNode::TINYXML_ELEMENT)
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
        
            if (e->whence == "online" && !Net::isConnected())
                return;
            else if (e->whence == "offline" && Net::isConnected())
                return;
        
            switch (e->type) {

                case QUAD:

                    switch (e->align) {
                                                
                        case ALIGN_RIGHT:
                        
                            Gfx::drawQuad(e->x - e->width + e->paddingLeft, e->y + e->paddingTop, e->width, e->height, e->colour, e->rotate);
                        
                            break;
                            
                        case ALIGN_CENTER:
                        case ALIGN_FULL:
                        
                            Gfx::drawQuad(e->x - (e->width / 2) + e->paddingLeft, e->y + e->paddingTop, e->width, e->height, e->colour, e->rotate);
                        
                            break;
                            
                        case ALIGN_LEFT:
                        default:
                        
                            Gfx::drawQuad(e->x + e->paddingLeft, e->y + e->paddingTop, e->width, e->height, e->colour, e->rotate);
                        
                            break;
                    
                    }

                    break;

                case TEXT:
                    
                    if (e->scroll == SCROLL_NONE) {
                        
                        if (e->width == 0)
                            e->font.draw(e->x + e->paddingLeft, e->y + e->paddingTop, e->text.c_str());
                        else
                            e->font.drawColumn(e->x + e->paddingLeft, e->y + e->paddingTop, e->width, e->text.c_str());
                        
                    } else {
                    
                        if (e->width == 0)
                            e->x = e->font.drawColumn(e->x + e->paddingLeft, e->y + e->paddingTop, -1, e->text.c_str());
                        else
                            e->x = e->font.drawColumn(e->x + e->paddingLeft, e->y + e->paddingTop, e->width, e->text.c_str());
                        
                    }
                    
                    break;

                case IMAGE:
                                    
                    Gfx::ImageClip clip;
                    clip.x = e->offsetX;
                    clip.y = e->offsetY;
                    clip.width = e->width;
                    clip.height = e->height;
                                        
                    switch (e->align) {
                                                
                        case ALIGN_RIGHT:
                        
                            e->image->draw(e->x - e->image->width + e->paddingLeft, e->y + e->paddingTop, &clip);
                        
                            break;
                            
                        case ALIGN_CENTER:
                        case ALIGN_FULL:
                        
                            e->image->draw(e->x - (e->image->width / 2) + e->paddingLeft, e->y + e->paddingTop, &clip);
                        
                            break;
                            
                        case ALIGN_LEFT:
                        default:
                        
                            e->image->draw(e->x + e->paddingLeft, e->y + e->paddingTop, &clip);
                        
                            break;
                    
                    }

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
         * @param void* data[optional] Some extra data to pass to handler on init.
         */
        void XMLParser::registerCustomElementHandler(const std::string& element, CustomElementHandler* handler, void* data) {
        
            if (this->customElementHandlers.find(element) != this->customElementHandlers.end()) {
              
                if (__xM_DEBUG)
                    Util::logMsg("XMLParser::registerCustomElementHandler — Custom element handler already registered for element '%s'.", element.c_str());
                
            } else {
                        
                this->customElementHandlers.insert(std::pair<std::string, CustomElementHandler*>(element, handler));
                
                if (data != NULL)
                    this->customElementHandlersData.insert(std::pair<std::string, void*>(element, data));
                
            }
        
        }
        
        /**
         * Deregister a custom element handler.
         * 
         * @param const std::string& element The custom element.
         */
        void XMLParser::deRegisterCustomElementHandler(const std::string& element) {
        
            if (this->customElementHandlers.find(element) != this->customElementHandlers.end()) {
            
                this->customElementHandlers.erase(element);
                
                if (this->customElementHandlersData.find(element) != this->customElementHandlersData.end())
                    this->customElementHandlersData.erase(element);
                
            }
        
        }
        
        /**
         * Add a token to be replaced in text elements.
         * 
         * @param const std::string& key The token to replace.
         * @param const std::string& replace What to replace with.
         */
        void XMLParser::addTextSubstitute(const std::string& key, const std::string& replace) {
        
            this->textSubstitutes[key] = replace;
        
        }

    }

}

#endif /* _XMLParser_CPP */
