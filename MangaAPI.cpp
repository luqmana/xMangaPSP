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
 * Manga API Handler.
 * 
 * @package xMangaPSP
 */

#ifndef _MangaAPI_CPP
#define _MangaAPI_CPP

// BEGIN Includes
#include "MangaAPI.h"
#include "cJSON.h"
// END Includes

SDL_Thread* mangaApiHandlerThread = NULL;

std::vector<MangaAPIRequest> mangaApiRequestQuene;

// Local manga list.
MangaList mangaList;

// Local chapter list.
ChapterList chapterList;

// Local image list.
ImageList imageList;

// Local recent manga list.
RecentMangaList recentMangaList;

// Previous image
//MangaImage* prevImage = NULL;

// Current image
MangaImage* currImage = NULL;

// Next image
//MangaImage* nextImage = NULL;

// Local error
std::string error;

int mangaApiRequestId = 0;

/**
 * Internal function for getting raw manga list from supported APIs and parsing it thereby putting it into a usable format.
 */
int getMangaList(MangaAPIRequest request) {

	// Clear any old values
	mangaList.clear();

	// Handle different APIs
	switch (request.api) {
					
		case MangaStreamAPI:
		    
		    std::string dlBuffer;
			
			int res = downloadFile("http://omp.leonex.co.cc/api/?api=MangaStream", dlBuffer);
			
			if (res != -1) {
						
				// No error so try to parse JSON
			
				cJSON* root = cJSON_Parse(dlBuffer.c_str());
				cJSON* mangas = cJSON_GetObjectItem(root, "Manga");
				int i = 0;
				
				for (i; i < cJSON_GetArraySize(mangas); i++) {
				
					MangaListItem item;
				
					item.name = cJSON_GetObjectItem(cJSON_GetArrayItem(mangas, i), "name")->valuestring;
					item.apiHandle = cJSON_GetObjectItem(cJSON_GetArrayItem(mangas, i), "apiHandle")->valuestring;
					
					mangaList.push_back(item);
				
				}
				cJSON_Delete(root);
							
			} else {
			
				error = dlBuffer;
				
				// 1 = Net Error
				return 1;
				
			}
		    
		    break;
					
		/*case OneMangaAPI: {
			
			std::string dlBuffer;
			
			int res = downloadFile("http://omp.leonex.co.cc/api/?api=OneManga", dlBuffer);
			
			if (res != -1) {
						
				// No error so try to parse JSON
			
				cJSON* root = cJSON_Parse(dlBuffer.c_str());
				cJSON* mangas = cJSON_GetObjectItem(root, "Manga");
				int i = 0;
				
				for (i; i < cJSON_GetArraySize(mangas); i++) {
				
					MangaListItem item;
				
					item.name = cJSON_GetObjectItem(cJSON_GetArrayItem(mangas, i), "name")->valuestring;
					item.apiHandle = cJSON_GetObjectItem(cJSON_GetArrayItem(mangas, i), "apiHandle")->valuestring;
					
					mangaList.push_back(item);
				
				}
				cJSON_Delete(root);
							
			} else {
			
				error = dlBuffer;
				
				// 1 = Net Error
				return 1;
				
			}
						
			} break;*/
					
	}		
		
	// 0 -Success
	return 0;

}

/**
 * Internal function for getting raw chapter list from supported APIs and parsing it thereby putting it into a usable format.
 */
int getChapterList(MangaAPIRequest request) {

	// Clear any old values
	chapterList.manga.clear();
	chapterList.mangaApiHandle.clear();
	chapterList.list.clear();

	// Handle different APIs
	switch (request.api) {
	
	    case MangaStreamAPI:
			
			std::string dlBuffer;
			std::string url;
			
			// Format URL
			url = "http://omp.leonex.co.cc/api/";
			url.append(request.mangaApiHandle);
			url.append("/");
			url.append("?api=MangaStream");
			
			int res = downloadFile(url, dlBuffer);
			
			if (res != -1) {
						
				// Add some meta data
				chapterList.manga = request.manga;
				chapterList.mangaApiHandle = request.mangaApiHandle;
			
				// No error so try to parse JSON
			
				cJSON* root = cJSON_Parse(dlBuffer.c_str());
				cJSON* chapters = cJSON_GetObjectItem(root, "Chapters");
				int i = 0;
				
				for (i; i < cJSON_GetArraySize(chapters); i++) {
				
					ChapterListItem item;
				
					item.name = cJSON_GetObjectItem(cJSON_GetArrayItem(chapters, i), "name")->valuestring;
					item.apiHandle = cJSON_GetObjectItem(cJSON_GetArrayItem(chapters, i), "apiHandle")->valuestring;
					//item.firstImage = cJSON_GetObjectItem(cJSON_GetArrayItem(chapters, i), "firstImage")->valuestring;
					
					chapterList.list.push_back(item);
				
				}
				cJSON_Delete(root);
							
			} else {
			
				error = dlBuffer;
				
				// 1 = Net Error
				return 1;
				
			}
						
			break;
					
		/*case OneMangaAPI: {
			
			std::string dlBuffer;
			std::string url;
			
			// Format URL
			url = "http://omp.leonex.co.cc/api/";
			url.append(request.mangaApiHandle);
			url.append("/");
			url.append("?api=OneManga");
			
			int res = downloadFile(url, dlBuffer);
			
			if (res != -1) {
						
				// Add some meta data
				chapterList.manga = request.manga;
				chapterList.mangaApiHandle = request.mangaApiHandle;
			
				// No error so try to parse JSON
			
				cJSON* root = cJSON_Parse(dlBuffer.c_str());
				cJSON* chapters = cJSON_GetObjectItem(root, "Chapters");
				int i = 0;
				
				for (i; i < cJSON_GetArraySize(chapters); i++) {
				
					ChapterListItem item;
				
					item.name = cJSON_GetObjectItem(cJSON_GetArrayItem(chapters, i), "name")->valuestring;
					item.apiHandle = cJSON_GetObjectItem(cJSON_GetArrayItem(chapters, i), "apiHandle")->valuestring;
					//item.firstImage = cJSON_GetObjectItem(cJSON_GetArrayItem(chapters, i), "firstImage")->valuestring;
					
					chapterList.list.push_back(item);
				
				}
				cJSON_Delete(root);
							
			} else {
			
				error = dlBuffer;
				
				// 1 = Net Error
				return 1;
				
			}
						
			} break;*/
					
	}		
		
	// 0 -Success
	return 0;

}

/**
 * Internal function for getting raw image list from supported APIs and parsing it thereby putting it into a usable format.
 */
int getImageList(MangaAPIRequest request) {

	// Clear any old values
	imageList.manga.clear();
	imageList.mangaApiHandle.clear();
	imageList.chapter.clear();
	imageList.chapterApiHandle.clear();
	imageList.list.clear();

	// Handle different APIs
	switch (request.api) {
	
	    case MangaStreamAPI:
			
			std::string dlBuffer;
			std::string url;

			// Format URL
			url = "http://omp.leonex.co.cc/api/";
			url.append(request.mangaApiHandle);
			url.append("/");
			url.append(request.chapterApiHandle);
			url.append("/");
			url.append("?api=MangaStream");
			
			int res = downloadFile(url, dlBuffer);
			
			if (res != -1) {
						
				// Add some meta data
				imageList.manga = request.manga;
				imageList.mangaApiHandle = request.mangaApiHandle;
				imageList.chapter = request.chapter;
				imageList.chapterApiHandle = request.chapterApiHandle;
			
				// No error so try to parse JSON
			
				cJSON* root = cJSON_Parse(dlBuffer.c_str());
				cJSON* images = cJSON_GetObjectItem(root, "Images");
				int i = 0;
				
				for (i; i < cJSON_GetArraySize(images); i++) {
				
					ImageListItem item;
				
					item.image = cJSON_GetObjectItem(cJSON_GetArrayItem(images, i), "image")->valuestring;
					
					imageList.list.push_back(item);
				
				}
				cJSON_Delete(root);
							
			} else {
			
				error = dlBuffer;
				
				// 1 = Net Error
				return 1;
				
			}
						
			break;
					
		/*case OneMangaAPI: {
			
			std::string dlBuffer;
			std::string url;

			// Format URL
			url = "http://omp.leonex.co.cc/api/";
			url.append(request.mangaApiHandle);
			url.append("/");
			url.append(request.chapterApiHandle);
			url.append("/");
			url.append("?api=OneManga");
			
			int res = downloadFile(url, dlBuffer);
			
			if (res != -1) {
						
				// Add some meta data
				imageList.manga = request.manga;
				imageList.mangaApiHandle = request.mangaApiHandle;
				imageList.chapter = request.chapter;
				imageList.chapterApiHandle = request.chapterApiHandle;
			
				// No error so try to parse JSON
			
				cJSON* root = cJSON_Parse(dlBuffer.c_str());
				cJSON* images = cJSON_GetObjectItem(root, "Images");
				int i = 0;
				
				for (i; i < cJSON_GetArraySize(images); i++) {
				
					ImageListItem item;
				
					item.image = cJSON_GetObjectItem(cJSON_GetArrayItem(images, i), "image")->valuestring;
					
					imageList.list.push_back(item);
				
				}
				cJSON_Delete(root);
							
			} else {
			
				error = dlBuffer;
				
				// 1 = Net Error
				return 1;
				
			}
						
			} break;*/
					
	}		
		
	// 0 -Success
	return 0;

}

/**
 * Internal function for getting raw recent manga list from supported APIs and parsing it thereby putting it into a usable format.
 */
int getRecentMangaList(MangaAPIRequest request) {

	// Clear any old values
	recentMangaList.clear();

	// Handle different APIs
	switch (request.api) {
					
	    case MangaStreamAPI:
			
			std::string dlBuffer;
			
			int res = downloadFile("http://omp.leonex.co.cc/api/Recent-Manga/?api=MangaStream", dlBuffer);
			
			if (res != -1) {
						
				// No error so try to parse JSON
			
				cJSON* root = cJSON_Parse(dlBuffer.c_str());
				cJSON* mangas = cJSON_GetObjectItem(root, "Manga");
				int i = 0;
				
				for (i; i < cJSON_GetArraySize(mangas); i++) {
				
					RecentMangaListItem item;
				
					item.name = cJSON_GetObjectItem(cJSON_GetArrayItem(mangas, i), "name")->valuestring;
					item.date = cJSON_GetObjectItem(cJSON_GetArrayItem(mangas, i), "date")->valuestring;
					item.manga = cJSON_GetObjectItem(cJSON_GetArrayItem(mangas, i), "manga")->valuestring;
					item.mangaApiHandle = cJSON_GetObjectItem(cJSON_GetArrayItem(mangas, i), "mangaApiHandle")->valuestring;
					item.chapter = cJSON_GetObjectItem(cJSON_GetArrayItem(mangas, i), "chapter")->valuestring;
					item.chapterApiHandle = cJSON_GetObjectItem(cJSON_GetArrayItem(mangas, i), "chapterApiHandle")->valuestring;
					
					recentMangaList.push_back(item);
				
				}
				cJSON_Delete(root);
							
			} else {
			
				error = dlBuffer;
				
				// 1 = Net Error
				return 1;
				
			}
						
			break;
					
		/*case OneMangaAPI: {
			
			std::string dlBuffer;
			
			int res = downloadFile("http://omp.leonex.co.cc/api/Recent-Manga/?api=OneManga", dlBuffer);
			
			if (res != -1) {
						
				// No error so try to parse JSON
			
				cJSON* root = cJSON_Parse(dlBuffer.c_str());
				cJSON* mangas = cJSON_GetObjectItem(root, "Manga");
				int i = 0;
				
				for (i; i < cJSON_GetArraySize(mangas); i++) {
				
					RecentMangaListItem item;
				
					item.name = cJSON_GetObjectItem(cJSON_GetArrayItem(mangas, i), "name")->valuestring;
					item.date = cJSON_GetObjectItem(cJSON_GetArrayItem(mangas, i), "date")->valuestring;
					item.manga = cJSON_GetObjectItem(cJSON_GetArrayItem(mangas, i), "manga")->valuestring;
					item.mangaApiHandle = cJSON_GetObjectItem(cJSON_GetArrayItem(mangas, i), "mangaApiHandle")->valuestring;
					item.chapter = cJSON_GetObjectItem(cJSON_GetArrayItem(mangas, i), "chapter")->valuestring;
					item.chapterApiHandle = cJSON_GetObjectItem(cJSON_GetArrayItem(mangas, i), "chapterApiHandle")->valuestring;
					
					recentMangaList.push_back(item);
				
				}
				cJSON_Delete(root);
							
			} else {
			
				error = dlBuffer;
				
				// 1 = Net Error
				return 1;
				
			}
						
			} break;*/
					
	}		
		
	// 0 -Success
	return 0;

}

/**
 * Internal function for getting raw image from supported APIs and parsing it thereby putting it into a usable format.
 */
int getImage(MangaAPIRequest request, MangaImage*& mangaImage) {

	if (mangaImage != NULL) {
	
		// Clear old stuff
		for (unsigned int i = 0; i < mangaImage->sections.size(); i++) {
		
			if (glIsTexture(mangaImage->sections[i].texture.texture) == GL_TRUE) {
			
				glDeleteTextures(1, &mangaImage->sections[i].texture.texture);
				
			}
				
		}
		mangaImage->sections.clear();
		delete mangaImage;
		mangaImage = new MangaImage;
		
	} else
		mangaImage = new MangaImage;
	
	// Handle different APIs
	switch (request.api) {
	
	    case MangaStreamAPI:
			
			std::string dlBuffer;
			std::string url;
			
			// Format URL
			url = "http://omp.leonex.co.cc/api/";
			url.append(request.mangaApiHandle);
			url.append("/");
			url.append(request.chapterApiHandle);
			url.append("/");
			url.append(request.imageList.list[request.imageIndex].image);
			url.append("/");
			url.append("?api=MangaStream");
						
			int res = downloadFile(url, dlBuffer);
			
			if (res != -1) {
			
				// Create temp surface for images
				SDL_Surface* image = engine->sdlSurfaceFromImageRaw(dlBuffer);
				
				if (image == NULL) {
				
					// 2 = SDL Conversion error
					error = "MangaAPI: Unable to create surface from image data.";
					return 2;
				
				}
				
				mangaImage->w = image->w;
				mangaImage->h = image->h;
				
				// Calculating and parsing different parts of image
				// Here we split the image into segments of 512x512
				int wFit = ceil((float)image->w / 512);
				int hFit = ceil((float)image->h / 512);
				int i = 0, j = 0;
				SDL_Rect clip;
								
				for (i = 0; i < hFit; i++) {
				
					for (j = 0; j < wFit; j++) {
					
						clip.x = 512 * j; clip.y = 512 * i;
						
						if (j == wFit - 1)
							clip.w = image->w - (512 * (wFit - 1));
						else
							clip.w = 512;
							
						if (i == hFit - 1)
							clip.h = image->h - (512 * (hFit - 1));
						else
							clip.h = 512;
							
						MangaImageSection sect;
							
						//printf("Making texture\n"); // <-- DEBUG
						
						// Convert to texture
						sect.texture = engine->glTextureFromSDLSurface(image, &clip, true);

						// Error check
						if (glIsTexture(sect.texture.texture) == GL_FALSE) {
						
							// 3 = OpenGL Conversion error
							error = "MangaAPI: Unable to convert surface segment to OpenGL texture.";
							return 3;
						
						}
						
						sect.x = clip.x;
						sect.y = clip.y;
						sect.w = clip.w;
						sect.h = clip.h;
						
						mangaImage->sections.push_back(sect);
						
					
					}
				
				}
				
				// Free temp surface
				SDL_FreeSurface(image);
							
			} else {
			
				error = dlBuffer;
				
				// 1 = Net Error
				return 1;
				
			}
						
			break;
					
		/*case OneMangaAPI: {
			
			std::string dlBuffer;
			std::string url;
			
			// Format URL
			url = "http://omp.leonex.co.cc/api2/";
			url.append(request.mangaApiHandle);
			url.append("/");
			url.append(request.chapterApiHandle);
			url.append("/");
			url.append(request.imageList.list[request.imageIndex].image);
			url.append("/");
			url.append("?api=OneManga");
						
			int res = downloadFile(url, dlBuffer);
			
			if (res != -1) {
			
				// Create temp surface for images
				SDL_Surface* image = engine->sdlSurfaceFromImageRaw(dlBuffer);
				
				if (image == NULL) {
				
					// 2 = SDL Conversion error
					error = "MangaAPI: Unable to create surface from image data.";
					return 2;
				
				}
				
				mangaImage->w = image->w;
				mangaImage->h = image->h;
				
				// Calculating and parsing different parts of image
				// Here we split the image into segments of 512x512
				int wFit = ceil((float)image->w / 512);
				int hFit = ceil((float)image->h / 512);
				int i = 0, j = 0;
				SDL_Rect clip;
								
				for (i = 0; i < hFit; i++) {
				
					for (j = 0; j < wFit; j++) {
					
						clip.x = 512 * j; clip.y = 512 * i;
						
						if (j == wFit - 1)
							clip.w = image->w - (512 * (wFit - 1));
						else
							clip.w = 512;
							
						if (i == hFit - 1)
							clip.h = image->h - (512 * (hFit - 1));
						else
							clip.h = 512;
							
						MangaImageSection sect;
							
						//printf("Making texture\n"); // <-- DEBUG
						
						// Convert to texture
						sect.texture = engine->glTextureFromSDLSurface(image, &clip, true);

						// Error check
						if (glIsTexture(sect.texture.texture) == GL_FALSE) {
						
							// 3 = OpenGL Conversion error
							error = "MangaAPI: Unable to convert surface segment to OpenGL texture.";
							return 3;
						
						}
						
						sect.x = clip.x;
						sect.y = clip.y;
						sect.w = clip.w;
						sect.h = clip.h;
						
						mangaImage->sections.push_back(sect);
						
					
					}
				
				}
				
				// Free temp surface
				SDL_FreeSurface(image);
							
			} else {
			
				error = dlBuffer;
				
				// 1 = Net Error
				return 1;
				
			}
						
			} break;*/
					
	}		
		
	// 0 -Success
	return 0;

}

/**
 * Thread function.
 */
int mangaApiHandler(void*) {

	while (true) {
	
		// Only act if request quene is not empty
		if (!mangaApiRequestQuene.empty()) {
		
			// Get first request
			MangaAPIRequest request = mangaApiRequestQuene.front();
			
			switch (request.type) {
			
				case MangaAPIMangaListRequest:
				
					{
	
						// Actually get the list
						int retValue = getMangaList(request);
										
						SDL_Event mangaApiEvent;
							
						mangaApiEvent.type = SDL_xMANGAAPIEVENT;
						mangaApiEvent.user.data1 = (void*)request.id; // So user can compare ids
						
						if (retValue == 0)
							mangaApiEvent.user.code = xMangaApiSuccess;
						else if (retValue == 1)
							mangaApiEvent.user.code = xMangaApiError;
							
						// Push event onto quene
						FE_PushEvent(&mangaApiEvent);
										
					}
					
					break;
					
				case MangaAPIChapterListRequest:
				
					{
		
						// Actually get the list
						int retValue = getChapterList(request);
																
						SDL_Event mangaApiEvent;
							
						mangaApiEvent.type = SDL_xMANGAAPIEVENT;
						mangaApiEvent.user.data1 = (void*)request.id; // So user can compare ids
						
						if (retValue == 0)
							mangaApiEvent.user.code = xMangaApiSuccess;
						else if (retValue == 1)
							mangaApiEvent.user.code = xMangaApiError;
							
						// Push event onto quene
						FE_PushEvent(&mangaApiEvent);
										
					}
				
					break;
					
				case MangaAPIImageListRequest:
				
					{
		
						// Actually get the list
						int retValue = getImageList(request);
																
						SDL_Event mangaApiEvent;
							
						mangaApiEvent.type = SDL_xMANGAAPIEVENT;
						mangaApiEvent.user.data1 = (void*)request.id; // So user can compare ids
						
						if (retValue == 0)
							mangaApiEvent.user.code = xMangaApiSuccess;
						else if (retValue == 1)
							mangaApiEvent.user.code = xMangaApiError;
							
						// Push event onto quene
						FE_PushEvent(&mangaApiEvent);
										
					}
				
					break;
					
				case MangaAPIRecentMangaListRequest:
				
					{
	
						// Actually get the list
						int retValue = getRecentMangaList(request);
										
						SDL_Event mangaApiEvent;
							
						mangaApiEvent.type = SDL_xMANGAAPIEVENT;
						mangaApiEvent.user.data1 = (void*)request.id; // So user can compare ids
						
						if (retValue == 0)
							mangaApiEvent.user.code = xMangaApiSuccess;
						else if (retValue == 1)
							mangaApiEvent.user.code = xMangaApiError;
							
						// Push event onto quene
						FE_PushEvent(&mangaApiEvent);
										
					}
				
					break;
					
				case MangaAPIImageRequest:
				
					{
	
						int retValue = -1;
						int imageIndex = request.imageIndex;
						
						// Need to work on cache support, so disabled for now
						/*// Load previous (same chapter)
						if (request.imageIndex > 0) {
						
							request.imageIndex -= 1; // Set to previous image
							retValue = getImage(request, prevImage);
							
						}
						
						// Don't continue if error
						if (retValue == 1) {
						
							SDL_Event mangaApiEvent;
							mangaApiEvent.type = SDL_xMANGAAPIEVENT;
							mangaApiEvent.user.data1 = (void*)request.id; // So user can compare ids
							mangaApiEvent.user.code = xMangaApiError;
							// Push event onto quene
							FE_PushEvent(&mangaApiEvent);
						
						} else {*/
							
							// Load current
							request.imageIndex = imageIndex; // Just to make sure we get the right image
							retValue = getImage(request, currImage);
							
							SDL_Event mangaApiEvent;
							
							mangaApiEvent.type = SDL_xMANGAAPIEVENT;
							mangaApiEvent.user.data1 = (void*)request.id; // So user can compare ids
							mangaApiEvent.user.data2 = (void*)imageIndex;
							
							if (retValue == 0)
								mangaApiEvent.user.code = xMangaApiSuccess;
							else if (retValue == 1)
								mangaApiEvent.user.code = xMangaApiError;
								
							// Push event onto quene
							FE_PushEvent(&mangaApiEvent);
							
							// Need to work on cache support, so disabled for now
							// Don't continue if error
							/*if (retValue == 1) {
							
								SDL_Event mangaApiEvent;
								mangaApiEvent.type = SDL_xMANGAAPIEVENT;
								mangaApiEvent.user.data1 = (void*)request.id; // So user can compare ids
								mangaApiEvent.user.code = xMangaApiError;
								// Push event onto quene
								FE_PushEvent(&mangaApiEvent);
							
							} else {
							
								// Load next (same chapter)
								if ((unsigned int)request.imageIndex < (request.imageList.list.size() - 1)) {
								
									request.imageIndex += 1; // Set to next image
									retValue = getImage(request, nextImage);
									
									SDL_Event mangaApiEvent;
							
									mangaApiEvent.type = SDL_xMANGAAPIEVENT;
									mangaApiEvent.user.data1 = (void*)request.id; // So user can compare ids
									mangaApiEvent.user.data2 = (void*)imageIndex;
									
									if (retValue == 0)
										mangaApiEvent.user.code = xMangaApiSuccess;
									else if (retValue == 1)
										mangaApiEvent.user.code = xMangaApiError;
										
									// Push event onto quene
									FE_PushEvent(&mangaApiEvent);
																	
								}
							
							}
						
						}*/
																
					}
				
					break;
					
				case MangaAPINextImageRequest:
				
					{
	
						int retValue = -1;
						bool delOld = false;
						
						// Save value
						int imageIndex = request.imageIndex;
						
						// Save current image temporarily
						MangaImage* tempCopy = currImage;
						currImage = NULL;
						
						// Load next image [same chapter]
						if ((unsigned int)request.imageIndex + 1 <= (request.imageList.list.size() - 1)) {
						
							// Set to next image
							request.imageIndex += 1;
						
							retValue = getImage(request, currImage);
							
							SDL_Event mangaApiEvent;
					
							mangaApiEvent.type = SDL_xMANGAAPIEVENT;
							mangaApiEvent.user.data1 = (void*)request.id; // So user can compare ids
							mangaApiEvent.user.data2 = (void*)(imageIndex + 1);
							
							if (retValue == 0) {
							
								mangaApiEvent.user.code = xMangaApiSuccess;
								delOld = true;
								
							} else if (retValue == 1) {
							
								mangaApiEvent.user.code = xMangaApiError;
								delOld = false;
								
							}
								
							// Push event onto quene
							FE_PushEvent(&mangaApiEvent);
															
						} else {
						
							// Get chapter index
							int chapterIndex = 0;
							unsigned int i = 0;
							ChapterList cList = MangaAPI::getChapterList();
							
							for (i; i < cList.list.size(); i++) {
							
								if (cList.list[i].apiHandle == request.chapterApiHandle) {
								
									chapterIndex = i;
									break;
								
								}
							
							}
							
							if ((unsigned int)chapterIndex + 1  <= (cList.list.size() - 1)) {
							
								// Next chapter available
								
								request.chapterApiHandle = cList.list[chapterIndex + 1].apiHandle;
								request.chapter = cList.list[chapterIndex + 1].name;
								
								// Load next chapter image list
								retValue = getImageList(request);
								
								if (retValue == 1) {
								
									// Error
									
									SDL_Event mangaApiEvent;
					
									mangaApiEvent.type = SDL_xMANGAAPIEVENT;
									mangaApiEvent.user.data1 = (void*)request.id; // So user can compare ids
									mangaApiEvent.user.code = xMangaApiError;
										
									// Push event onto quene
									FE_PushEvent(&mangaApiEvent);
																		
									delOld = false;
									
								} else {
								
									// Load first image
									request.imageList = MangaAPI::getImageList();
									request.imageIndex = 0; // Just to make sure we get the right image
									retValue = getImage(request, currImage);
									
									SDL_Event mangaApiEvent;
									
									mangaApiEvent.type = SDL_xMANGAAPIEVENT;
									mangaApiEvent.user.data1 = (void*)request.id; // So user can compare ids
									mangaApiEvent.user.data2 = (void*)0;
									
									if (retValue == 0)
										mangaApiEvent.user.code = xMangaApiSuccess;
									else if (retValue == 1)
										mangaApiEvent.user.code = xMangaApiError;
										
									// Push event onto quene
									FE_PushEvent(&mangaApiEvent);
									
									delOld = true;
								
								}								
							
							} else {
							
								// Next chapter not available
								
								SDL_Event mangaApiEvent;
					
								mangaApiEvent.type = SDL_xMANGAAPIEVENT;
								mangaApiEvent.user.data1 = (void*)request.id; // So user can compare ids
								mangaApiEvent.user.data2 = (void*)xMangaApiNoNextChapter;
								mangaApiEvent.user.code = xMangaApiError;
									
								// Push event onto quene
								FE_PushEvent(&mangaApiEvent);
								
								error = "WARN: Next chapter not available.";
								
								delOld = false;
							
							}
						
						}
						
						if (delOld) {
						
							// Free temp image
							for (unsigned int i = 0; i < tempCopy->sections.size(); i++) {
						
								if (glIsTexture(tempCopy->sections[i].texture.texture) == GL_TRUE) {
							
									glDeleteTextures(1, &tempCopy->sections[i].texture.texture);
									
								}
									
							}
							tempCopy->sections.clear();
							delete tempCopy;
							
						}
																	
					}
				
					break;
					
				case MangaAPIPrevImageRequest:
				
					{
	
						int retValue = -1;
						bool delOld = false;
						
						// Save value
						int imageIndex = request.imageIndex;
						
						// Save current image temporarily
						MangaImage* tempCopy = currImage;
						currImage = NULL;
						
						// Load prev image [same chapter]
						if (request.imageIndex - 1 >= 0) {
						
							// Set to prev image
							request.imageIndex -= 1;
						
							retValue = getImage(request, currImage);
							
							SDL_Event mangaApiEvent;
					
							mangaApiEvent.type = SDL_xMANGAAPIEVENT;
							mangaApiEvent.user.data1 = (void*)request.id; // So user can compare ids
							mangaApiEvent.user.data2 = (void*)(imageIndex - 1);
							
							if (retValue == 0) {
							
								mangaApiEvent.user.code = xMangaApiSuccess;
								delOld = true;
								
							} else if (retValue == 1) {
							
								mangaApiEvent.user.code = xMangaApiError;
								delOld = false;
								
							}
								
							// Push event onto quene
							FE_PushEvent(&mangaApiEvent);
																						
						} else {
						
							// Get chapter index
							int chapterIndex = 0;
							unsigned int i = 0;
							ChapterList cList = MangaAPI::getChapterList();
							
							for (i; i < cList.list.size(); i++) {
							
								if (cList.list[i].apiHandle == request.chapterApiHandle) {
								
									chapterIndex = i;
									break;
								
								}
							
							}

							if ((chapterIndex - 1)  >= 0) {

								// Prev chapter available
								
								request.chapterApiHandle = cList.list[chapterIndex - 1].apiHandle;
								request.chapter = cList.list[chapterIndex - 1].name;
								
								// Load prev chapter image list
								retValue = getImageList(request);
								
								if (retValue == 1) {
								
									// Error
									
									SDL_Event mangaApiEvent;
					
									mangaApiEvent.type = SDL_xMANGAAPIEVENT;
									mangaApiEvent.user.data1 = (void*)request.id; // So user can compare ids
									mangaApiEvent.user.code = xMangaApiError;
										
									// Push event onto quene
									FE_PushEvent(&mangaApiEvent);
																		
									delOld = false;
									
								} else {
									
									// Load last image
									request.imageIndex = (imageList.list.size() - 1);
									request.imageList = MangaAPI::getImageList();
									retValue = getImage(request, currImage);
									
									SDL_Event mangaApiEvent;
									
									mangaApiEvent.type = SDL_xMANGAAPIEVENT;
									mangaApiEvent.user.data1 = (void*)request.id; // So user can compare ids
									mangaApiEvent.user.data2 = (void*)(imageList.list.size() - 1);
									
									if (retValue == 0) {
									
										mangaApiEvent.user.code = xMangaApiSuccess;
										delOld = true;
										
									} else if (retValue == 1) {
									
										mangaApiEvent.user.code = xMangaApiError;
										delOld = false;
										
									}
										
									// Push event onto quene
									FE_PushEvent(&mangaApiEvent);
																	
								}								
							
							} else {
							
								// Prev chapter not available
								
								SDL_Event mangaApiEvent;
					
								mangaApiEvent.type = SDL_xMANGAAPIEVENT;
								mangaApiEvent.user.data1 = (void*)request.id; // So user can compare ids
								mangaApiEvent.user.data2 = (void*)xMangaApiNoPrevChapter;
								mangaApiEvent.user.code = xMangaApiError;
									
								// Push event onto quene
								FE_PushEvent(&mangaApiEvent);
								
								error = "WARN: Prev chapter not available.";
								
								delOld = false;
							
							}
						
						}
						
						if (delOld) {
						
							// Free temp image
							for (unsigned int i = 0; i < tempCopy->sections.size(); i++) {
							
								if (glIsTexture(tempCopy->sections[i].texture.texture) == GL_TRUE) {
								
									glDeleteTextures(1, &tempCopy->sections[i].texture.texture);
									
								}
									
							}
							tempCopy->sections.clear();
							delete tempCopy;
							
						}
																	
					}
				
					break;
			
			}
			
			// Remove from quene
			mangaApiRequestQuene.erase(mangaApiRequestQuene.begin());
		
		// Sleep 250 ms
		} else
			SDL_Delay(250);
	
	}

	return 0;

}

/**
 * Create a new request for the manga list from specified api.
 * 
 * @param MangaAPI api Chosen API.
 * 
 * @returns int Comparision id.
 */
int MangaAPI::requestMangaList(SupportedMangaAPI api) {
	
	MangaAPIRequest request;
		
	request.type = MangaAPIMangaListRequest;
	request.api = api;
	request.id = mangaApiRequestId;
		
	// Add new request to end of quene
	mangaApiRequestQuene.push_back(request);
	
	mangaApiRequestId++;
	
	return request.id;
	
}

/**
 * Create a new request for the chapter list from specified api.
 * 
 * @param MangaAPI api Chosen API.
 * @param std::string manga Associated manga name.
 * @param std::string mangaApiHandle Associated manga handle.
 * 
 * @returns int Comparision id.
 */
int MangaAPI::requestChapterList(SupportedMangaAPI api, std::string manga, std::string mangaApiHandle) {

	MangaAPIRequest request;
		
	request.type = MangaAPIChapterListRequest;
	request.api = api;
	request.id = mangaApiRequestId;
	request.manga = manga;
	request.mangaApiHandle = mangaApiHandle;
		
	// Add new request to end of quene
	mangaApiRequestQuene.push_back(request);
	
	mangaApiRequestId++;
		
	return request.id;

}

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
int MangaAPI::requestImageList(SupportedMangaAPI api, std::string manga, std::string mangaApiHandle, std::string chapter, std::string chapterApiHandle) {

	MangaAPIRequest request;
		
	request.type = MangaAPIImageListRequest;
	request.api = api;
	request.id = mangaApiRequestId;
	request.manga = manga;
	request.mangaApiHandle = mangaApiHandle;
	request.chapter = chapter;
	request.chapterApiHandle = chapterApiHandle;
		
	// Add new request to end of quene
	mangaApiRequestQuene.push_back(request);
	
	mangaApiRequestId++;
		
	return request.id;

}

/**
 * Create new request for the manga list from specified api.
 * 
 * @param SupportedMangaAPI api Chosen API.
 * 
 * @areturns int Comparision id.
 */
int MangaAPI::requestRecentMangaList(SupportedMangaAPI api) {

	MangaAPIRequest request;
		
	request.type = MangaAPIRecentMangaListRequest;
	request.api = api;
	request.id = mangaApiRequestId;
		
	// Add new request to end of quene
	mangaApiRequestQuene.push_back(request);
	
	mangaApiRequestId++;
	
	return request.id;

}

/**
 * Create a new request for the image from specified api.
 * 
 * @param MangaAPI api Chosen API.
 * @param ImageList imageList Image list.
 * @param int imageIndex Index for selected image.
 * 
 * @returns int Comparision id.
 */
int MangaAPI::requestImage(SupportedMangaAPI api, ImageList imageList, int imageIndex) {
	
	MangaAPIRequest request;
		
	request.type = MangaAPIImageRequest;
	request.api = api;
	request.id = mangaApiRequestId;
	request.manga = imageList.manga;
	request.mangaApiHandle = imageList.mangaApiHandle;
	request.chapter = imageList.chapter;
	request.chapterApiHandle = imageList.chapterApiHandle;
	request.imageIndex = imageIndex;
	request.imageList = imageList;
	
	// Add new request to end of quene
	mangaApiRequestQuene.push_back(request);
	
	mangaApiRequestId++;
	
	return request.id;

}

/**
 * Create a new request for the next image from specified api.
 * 
 * @param MangaAPI api Chosen API.
 * @param ImageList imageList Image list.
 * @param int imageIndex Index for selected image.
 * 
 * @returns int Comparision id.
 */
int MangaAPI::requestNextImage(SupportedMangaAPI api, ImageList imageList, int imageIndex) {

	MangaAPIRequest request;
		
	request.type = MangaAPINextImageRequest;
	request.api = api;
	request.id = mangaApiRequestId;
	request.manga = imageList.manga;
	request.mangaApiHandle = imageList.mangaApiHandle;
	request.chapter = imageList.chapter;
	request.chapterApiHandle = imageList.chapterApiHandle;
	request.imageIndex = imageIndex;
	request.imageList = imageList;
	
	// Add new request to end of quene
	mangaApiRequestQuene.push_back(request);
	
	mangaApiRequestId++;
	
	return request.id;

}

/**
 * Create a new request for the prev image from specified api.
 * 
 * @param MangaAPI api Chosen API.
 * @param ImageList imageList Image list.
 * @param int imageIndex Index for selected image.
 * 
 * @returns int Comparision id.
 */
int MangaAPI::requestPrevImage(SupportedMangaAPI api, ImageList imageList, int imageIndex) {

	MangaAPIRequest request;
		
	request.type = MangaAPIPrevImageRequest;
	request.api = api;
	request.id = mangaApiRequestId;
	request.manga = imageList.manga;
	request.mangaApiHandle = imageList.mangaApiHandle;
	request.chapter = imageList.chapter;
	request.chapterApiHandle = imageList.chapterApiHandle;
	request.imageIndex = imageIndex;
	request.imageList = imageList;
	
	// Add new request to end of quene
	mangaApiRequestQuene.push_back(request);
	
	mangaApiRequestId++;
	
	return request.id;

}

/**
 * Get the manga list.
 * 
 * @returns MangaList The manga list.
 */
MangaList MangaAPI::getMangaList() {

	return mangaList;

}

/**
 * Get the chapter list.
 * 
 * @returns ChapterList The chapter list.
 */
ChapterList MangaAPI::getChapterList() {

	return chapterList;

}

/**
 * Get the image list.
 * 
 * @returns ImageList The image list.
 */
ImageList MangaAPI::getImageList() {

	return imageList;

}

/**
 * Get the recent manga list.
 * 
 * @returns RecentMangaList The recent manga list.
 */
RecentMangaList MangaAPI::getRecentMangaList() {

	return recentMangaList;

}

/**
 * Get current manga image.
 * 
 * @returns MangaImage* The manga image.
 */
MangaImage* MangaAPI::getMangaImage() {

	return currImage;

}

/**
 * Get latest error.
 * 
 * @returns std::string The error.
 */
std::string MangaAPI::getError() {

	return error;

}

/**
 * Creates a thread to handle manga api events/request.
 */
void startMangaAPIHandlerThread() {

	// Create the thread
	mangaApiHandlerThread = SDL_CreateThread(mangaApiHandler, NULL);
	
	// Error checking
	if (mangaApiHandlerThread == NULL)
		engine->logMsg("Unable to create Manga API Event/Request Handling thread [%s].", SDL_GetError());

}

/**
 * Stop manga api events/request handler thread.
 */
void stopMangaAPIHandlerThread() {

	// Stop the thread
	SDL_KillThread(mangaApiHandlerThread);

}

#endif /* _MangaAPI_CPP */
