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
 * Header file.
 * 
 * @package xMangaPSP
 */

#ifndef _UTILS_H
#define _UTILS_H

// BEGIN Includes
#include <sstream>
#include <string>
#include <vector>
// END Includes

// BEGIN Defines
#define MAX(x, y) ((x) > (y) ? (x) : (y))
#define pspPrintf pspDebugScreenPrintf 		// Cause we're lazy :P
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
        void tokenize(const std::string& str, std::vector<std::string>& tokens, const std::string& delimiters);

        /**
         * Calculates the next power of two for a given number.
         *
         * @param int j Number to 'convert'
         *
         * @return int The next power of 2.
         */
        inline int nextPow2(int j) {

            int k = 1;

            while (k < j)
                k <<= 1;

            return k;

        }
        
        /**
         * Calculates the next power of two for a given number.
         * 
         * @param unsigned int j Number to 'convert'
         * 
         * @return unsigned int The next power of 2.
         */
        inline unsigned int nextPow2(unsigned int j) {

            unsigned int k = 1;

            while (k < j)
                k <<= 1;

            return k;

        }

        template <class T>
        inline std::string toString(const T& t) {

            std::stringstream sS;
            sS << t;
            return sS.str();

        }

    }

}

#endif /* _UTILS_H */
