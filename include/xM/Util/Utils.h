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
#define pspPrintf pspDebugScreenPrintf      // Cause we're lazy :P
// END Defines

namespace xM {

    namespace Util {
    
        /**
         * Get the system language.
         * 
         * @return int Number specifying the language.
         */
        int getSystemLanguage();
        
        /**
         * Whether the X/O buttons are swapped.
         *
         * @return int
         */
        int areButtonsSwapped();
    
        /**
         * Tokenize string.
         * 
         * @param const std::string& str The string to tokenize.
         * @param std::vector<std::string>& tokens The vector to fill.
         * @param const std::string& delimiters The delimiters.
         */
        void tokenize(const std::string& str, std::vector<std::string>& tokens, const std::string& delimiters);

        /**
         * Check whether one string has a specified ending.
         * From: http://stackoverflow.com/questions/874134/find-if-string-endswith-another-string-c
         * 
         * @param const std::string& fullString The string to check.
         * @param const std::string& ending The ending to check with.
         *
         * @return bool
         */
        inline bool hasEnding(const std::string& fullString, const std::string& ending) {

            if (fullString.length() >= ending.length())
                return (0 == fullString.compare(fullString.length() - ending.length(), ending.length(), ending));
            else
                return false;

        }


        /**
         * Calculates the next power of two for a given number.
         *
         * @param int j Number to 'convert'
         *
         * @return int The next power of 2.
         */
        inline int nextPow2(int j) {

            uint32_t n = j > 0 ? j - 1 : 0;

            n |= n >> 1;
            n |= n >> 2;
            n |= n >> 4;
            n |= n >> 8;
            n |= n >> 16;
            n++;

            return n;

        }
        
        /**
         * Calculates the next power of two for a given number.
         * 
         * @param unsigned int j Number to 'convert'
         * 
         * @return unsigned int The next power of 2.
         */
        inline unsigned int nextPow2(unsigned int j) {

            uint32_t n = j > 0 ? j - 1 : 0;

            n |= n >> 1;
            n |= n >> 2;
            n |= n >> 4;
            n |= n >> 8;
            n |= n >> 16;
            n++;

            return n;

        }

        template <class T>
        inline std::string toString(const T& t) {

            std::stringstream sS;
            sS << t;
            return sS.str();

        }
        
        inline int stringToInt(const std::string& str) {

            std::stringstream buffer(str);
            int out;
            buffer >> out;
            return out;

        }

    }

}

#endif /* _UTILS_H */
