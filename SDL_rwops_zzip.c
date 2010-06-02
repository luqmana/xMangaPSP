/*
 * $Id: SDL_rwops_zzip.c 81 2009-12-29 02:58:45Z chaotic@luqmanrocks.co.cc $
 *      Copyright (c) 2001 Guido Draheim <guidod@gmx.de>
 *      Use freely under the restrictions of the ZLIB License
 *
 *      (this example uses errno which might not be multithreaded everywhere)
 * 
 * 		Modded for use with OneMangaPSP.
 */

#include <SDL_rwops_zzip.h>
#include <zzip/zzip.h>
#include <string.h> /* strchr */

/* MSVC can not take a casted variable as an lvalue ! */
#define SDL_RWOPS_ZZIP_DATA(_context) \
             ((_context)->hidden.unknown.data1)
#define SDL_RWOPS_ZZIP_FILE(_context)  (ZZIP_FILE*) \
             ((_context)->hidden.unknown.data1)

static int _zzip_seek(SDL_RWops *context, int offset, int whence) {

	return zzip_seek(SDL_RWOPS_ZZIP_FILE(context), offset, whence);
    
}

static int _zzip_read(SDL_RWops *context, void *ptr, int size, int maxnum) {

	return zzip_read(SDL_RWOPS_ZZIP_FILE(context), ptr, size*maxnum) / size;
	
}

static int _zzip_write(SDL_RWops *context, const void *ptr, int size, int num) {

	/* Writing not supported. */
	return 0;
	
}

static int _zzip_close(SDL_RWops *context) {

	/* Context may have already been freed. */
    if (!context)
    	return 0;

	// Close ZIP file
	zzip_close(SDL_RWOPS_ZZIP_FILE(context));
    
    // Free SDL_RWops
	SDL_FreeRW(context);
	
	return 0;
	
}

SDL_RWops *SDL_RWFromZZIP(const char* file) {

	register SDL_RWops* rwops;
	register ZZIP_FILE* zzip_file;
    
    // Extensions
	zzip_strings_t ext[] = {"", 0};

	// Get ZZIP_FILE handle
    zzip_file = zzip_open_ext_io(file, O_RDONLY, ZZIP_PREFERZIP | ZZIP_CASELESS, ext, 0);
    
    // Error check
    if (!zzip_file){
    
    	return 0;
    	
    }

	// Alloc
    rwops = SDL_AllocRW();
    
    // Error check
    if (!rwops) {
    
    	errno = ENOMEM;
    	
    	// Close handle
    	zzip_close(zzip_file);
    	    	
    	return 0;
    	
    }

	
    SDL_RWOPS_ZZIP_DATA(rwops) = zzip_file;
    rwops->read = _zzip_read;
    rwops->write = _zzip_write;
    rwops->seek = _zzip_seek;
    rwops->close = _zzip_close;
    
    return rwops;
    
}
