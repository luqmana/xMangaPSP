/*
 * $Id: OMPNetwork.h 80 2009-12-28 02:39:01Z chaotic@luqmanrocks.co.cc $
 * 
 * This file is part of the OneMangaPSP application.
 *
 * Copyright (C) 2009  Luqman Aden <www.luqmanrocks.co.cc>.
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
 * Full License: LICENSE.txt
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

/**
 * Header for network stuff.
 * 
 * @package OneMangaPSP
 */

#ifndef _OMPNetwork_H
#define _OMPNetwork_H

// BEGIN Includes
#include "OMPStandard.h"
#include "OneMangaPSP.h"
// END Includes

/**
 * Download a file using cURL.
 * 
 * @param std::string url The url to download from.
 * @param std::string &response Pointer to string object to populate with response. (Which can also be error message.)
 * 
 * @return int If -1 then error, other wise your good.
 */ 
int downloadFile(std::string url, std::string &response);

#endif /* _OMPNetwork_H */
