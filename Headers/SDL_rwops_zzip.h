/*
 *      Copyright (c) 2001 Guido Draheim <guidod@gmx.de>
 *      Use freely under the restrictions of the ZLIB License
 *
 * 		Modded for use with xMangaPSP.     
 *
 */

#ifndef _SDL_RWops_ZZIP_h
#define _SDL_RWops_ZZIP_h

#include <SDL/SDL_rwops.h>

#ifndef ZZIP_NO_DECLSPEC
#define ZZIP_DECLSPEC
#else /* use DECLSPEC from SDL/begin_code.h */
#define ZZIP_DECLSPEC DECLSPEC
#endif

#ifdef __cplusplus
extern "C" {
#endif

extern ZZIP_DECLSPEC
SDL_RWops *SDL_RWFromZZIP(const char* file);

#ifdef __cplusplus
} /* extern C */
#endif

#endif
