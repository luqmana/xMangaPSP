/*
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
 * Full License: LICENSE.txt
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

/**
 * Header for Manga API Handler.
 * 
 * @package xMangaPSP
 */

#ifndef _MangaAPI_H
#define _MangaAPI_H

// BEGIN Includes
#include "xMPStandard.h"
#include "xMangaPSP.h"
#include "xMPNetwork.h"
#include <vector>
// END Includes

// Event flags
#define SDL_xMANGAAPIEVENT (SDL_USEREVENT + 2)

// Event codes
enum {

	xMangaApiError,
	xMangaApiSuccess,
	xMangaApiNoNextChapter,
	xMangaApiNoPrevChapter

};

// Request types
enum MangaAPIRequestType {

	MangaAPIMangaListRequest,
	MangaAPIChapterListRequest,
	MangaAPIImageListRequest,
	MangaAPIRecentMangaListRequest,
	MangaAPIImageRequest,
	MangaAPINextImageRequest,
	MangaAPIPrevImageRequest

};

// Supported APIs
enum SupportedMangaAPI {

    MangaStreamAPI//,
	//OneMangaAPI

};

// Manga list item
typedef struct {

	std::string name;
	std::string apiHandle;

} MangaListItem;

// Manga list
typedef std::vector<MangaListItem> MangaList;

// Chapter list item
typedef struct {

	std::string name;
	std::string apiHandle;
	//std::string firstImage;

} ChapterListItem;

// Chapter list
typedef struct {

	std::string manga;
	std::string mangaApiHandle;
	std::vector<ChapterListItem> list;
	
} ChapterList;

// Image list item
typedef struct {

	std::string image;

} ImageListItem;

// Image list
typedef struct {

	std::string manga;
	std::string mangaApiHandle;
	std::string chapter;
	std::string chapterApiHandle;
	std::vector<ImageListItem> list;
	
} ImageList;

// Recent Manga Item
typedef struct {

	std::string name;
	std::string date;
	std::string manga;
	std::string mangaApiHandle;
	std::string chapter;
	std::string chapterApiHandle;

} RecentMangaListItem;

// Request structure
typedef struct {

	int id;
	MangaAPIRequestType type;
	SupportedMangaAPI api;
	std::string manga;
	std::string mangaApiHandle;
	std::string chapter;
	std::string chapterApiHandle;
	int imageIndex;
	ImageList imageList;

} MangaAPIRequest;

// Recent Manga List
typedef std::vector<RecentMangaListItem> RecentMangaList;

// Manga Image Part
typedef struct {

	int x, y;
	int w, h;
	xMTexture texture;
	
} MangaImageSection;

// Manga Image
typedef struct {

	int w, h;
	std::vector<MangaImageSection> sections;

} MangaImage;

namespace MangaAPI {

	/**
	 * Create a new request for the manga list from specified api.
	 * 
	 * @param SupportedMangaAPI api Chosen API.
	 * 
	 * @returns int Comparision id.
	 */
	int requestMangaList(SupportedMangaAPI api);
	
	/**
	 * Create a new request for the chapter list from specified api.
	 * 
	 * @param MangaAPI api Chosen API.
	 * @param std::string manga Associated manga name.
	 * @param std::string mangaApiHandle Associated manga handle.
	 * 
	 * @returns int Comparision id.
	 */
	int requestChapterList(SupportedMangaAPI api, std::string manga, std::string mangaApiHandle);
	
	/**
	 * Create a new request for the image list from specified api.
	 * 
	 * @param MangaAPI api Chosen API.
	 * @param std::string manga Associated manga name.
	 * @param std::string mangaApiHandle Associated manga handle.
	 * @param std::string chapter Associated chapter name.
	 * @param std::string chapterApiHandle Associated chapter handle.
	 * 
	 * @returns int Comparision id.
	 */
	int requestImageList(SupportedMangaAPI api, std::string manga, std::string mangaApiHandle, std::string chapter, std::string chapterApiHandle);
	
	/**
	 * Create new request for the manga list from specified api.
	 * 
	 * @param SupportedMangaAPI api Chosen API.
	 * 
	 * @areturns int Comparision id.
	 */
	int requestRecentMangaList(SupportedMangaAPI api);
	
	/**
	 * Create a new request for the image from specified api.
	 * 
	 * @param MangaAPI api Chosen API.
	 * @param ImageList imageList Image list.
	 * @param int imageIndex Index for selected image.
	 * 
	 * @returns int Comparision id.
	 */
	int requestImage(SupportedMangaAPI api, ImageList imageList, int imageIndex);
	
	/**
	 * Create a new request for the next image from specified api.
	 * 
	 * @param MangaAPI api Chosen API.
	 * @param ImageList imageList Image list.
	 * @param int imageIndex Index for selected image.
	 * 
	 * @returns int Comparision id.
	 */
	int requestNextImage(SupportedMangaAPI api, ImageList imageList, int imageIndex);
	
	/**
	 * Create a new request for the prev image from specified api.
	 * 
	 * @param MangaAPI api Chosen API.
	 * @param ImageList imageList Image list.
	 * @param int imageIndex Index for selected image.
	 * 
	 * @returns int Comparision id.
	 */
	int requestPrevImage(SupportedMangaAPI api, ImageList imageList, int imageIndex);
	
	/**
	 * Get the manga list.
	 * 
	 * @returns MangaList The manga list.
	 */
	MangaList getMangaList();
	
	/**
	 * Get the chapter list.
	 * 
	 * @returns ChapterList The chapter list.
	 */
	ChapterList getChapterList();
	
	/**
	 * Get the image list.
	 * 
	 * @returns ImageList The image list.
	 */
	ImageList getImageList();
	
	/**
	 * Get the recent manga list.
	 * 
	 * @returns RecentMangaList The recent manga list.
	 */
	RecentMangaList getRecentMangaList();
	
	/**
	 * Get current manga image.
	 * 
	 * @returns MangaImage* The manga image.
	 */
	MangaImage* getMangaImage();
	
	/**
 	* Get latest error.
 	* 
 	* @returns std::string The error.
 	*/
	std::string getError();

};

/**
 * Creates a thread to handle manga api events/request.
 */
void startMangaAPIHandlerThread();

/**
 * Stop manga api events/request handler thread.
 */
void stopMangaAPIHandlerThread();

#endif /* _MangaAPI_H */
