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
 * Various utilities.
 * 
 * @package xMangaPSP
 */

#ifndef _UTILS_CPP
#define _UTILS_CPP

// BEGIN Includes
#include "xM/Util/Utils.h"
// END Includes

// BEGIN Defines

// END Defines

namespace xM {

    namespace Util {
    
        /**
         * Tokenize string.
         * 
         * @param const std::string& str The string to tokenize.
         * @param std::vector<std::string>& tokens The vector to fill.
         * @param const std::string& delimiters The delimiters.
         */
        void tokenize(const std::string& str, std::vector<std::string>& tokens, const std::string& delimiters) {

            // Skip delimiters at beginning.
            std::string::size_type lastPos = str.find_first_not_of(delimiters, 0);
            
            // Find first "non-delimiter".
            std::string::size_type pos = str.find_first_of(delimiters, lastPos);

            while (std::string::npos != pos || std::string::npos != lastPos) {
            
                // Found a token, add it to the vector.
                tokens.push_back(str.substr(lastPos, pos - lastPos));
                
                // Skip delimiters.  Note the "not_of"
                lastPos = str.find_first_not_of(delimiters, pos);
                
                // Find next "non-delimiter"
                pos = str.find_first_of(delimiters, lastPos);
                
            }
            
        }

    }

}

#endif /* _UTILS_CPP */
