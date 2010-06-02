/*
 * $Id: OMPUtil.h 86 2010-01-03 04:12:17Z chaotic@luqmanrocks.co.cc $
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
 * Header for OMP Utils.
 * 
 * @package OneMangaPSP
 */

#ifndef _OMPUtil_H
#define _OMPUtil_H

// BEGIN Includes
#include "OMPStandard.h"
#include "OneMangaPSP.h"
// END Includes

namespace OMPUtil {

	/**
	 * Convenience function to load resources needed by main ui.
	 */
	void loadMainUI();

	/**
	 * Convenience function to unload resources needed by main ui.
	 */
	void unloadMainUI();

	/**
	 * Convenience function to load some common resources.
	 */
	void loadCommonResources();
	
	/**
	 * Draw the loading indicator.
	 * 
	 * @param int x[optional] X position.
	 * @param int y[optional] Y position.
	 * @param bool withBG Draw BG?
	 */
	void drawLoadingIcon(int x = 208, int y = 104, bool withBG = true);
	
	/**
	 * Convenience function to draw battery icon.
	 */
	void drawBatteryIcon();

};

#endif /* _OMPUtil_H */
