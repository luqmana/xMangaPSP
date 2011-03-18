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
 * The file manager.
 * 
 * @package xMangaPSP
 */

#ifndef _FileManager_CPP
#define _FileManager_CPP

// BEGIN Includes
#include "xM/Engine/FileManager.h"
#include "xM/Util/Log.h"
#include "xM/Util/Utils.h"

#include <pspiofilemgr.h>

#include <sstream>
#include <string.h>

#include <cstdlib>

#include <zzip/zzip.h>
#include <zzip/plugin.h>

#if defined ZZIP_HAVE_UNISTD_H
#include <unistd.h>
#elif defined ZZIP_HAVE_IO_H
#include <io.h>
#else
#error need posix io
#endif
// END Includes

// BEGIN Defines

// For backward compatibility
#ifndef _zzip_plugin_io_handlers
#define _zzip_plugin_io_handlers struct zzip_plugin_io
#endif

// END Defines

namespace xM {

    namespace Engine {
    
        static unsigned int psarOffset;
        static zzip_plugin_io_handlers psarZipHandlers = { };
    
        static int psarZipOpen(zzip_char_t* name, int flags, ...) {
                
            // in this case name should always be EBOOT.PBP…
            SceUID fD = sceIoOpen(name, PSP_O_RDONLY, 0777);
            
            if (fD < 0) {
            
                // Compare name to make sure to ignore magic attempts by zziplib
                if (__xM_DEBUG && (strstr(name, "EBOOT.PBP/") == NULL))
                    Util::logMsg("FileManager::readFromPSAR - Unable to open EBOOT [%s].", name);
                    
                return -1;
            
            }
            
            // Seek to header to find the position of the PSAR
            sceIoLseek(fD, 0x24, PSP_SEEK_SET);
                        
            // Read in offset
            sceIoRead(fD, &psarOffset, sizeof(unsigned int));
                        
            // Now seek to PSAR offset
            sceIoLseek(fD, psarOffset, PSP_SEEK_SET);
            
            return fD;
        
        }
        
        static int psarZipClose(int fD) {
                
            return sceIoClose(fD);
        
        }
        
        static zzip_ssize_t psarZipRead(int fD, void* buf, zzip_size_t len) {
                
            return sceIoRead(fD, buf, len);
        
        }
        
        static zzip_off_t psarZipSeek(int fD, zzip_off_t offset, int whence) {
                
            if (whence == PSP_SEEK_SET)
                return sceIoLseek(fD, (psarOffset + offset), PSP_SEEK_SET) - psarOffset;
            else if (whence == PSP_SEEK_CUR)
                return sceIoLseek(fD, offset, PSP_SEEK_CUR) - psarOffset;
            else if (whence == PSP_SEEK_END) {
            
                if (offset < 0)
                    return sceIoLseek(fD, offset, PSP_SEEK_END) - psarOffset;
                
            }
            
            return 0;
                    
        }
        
        static zzip_off_t psarZipFilesize(int fD) {
                
            // Calculate PSAR section size
            long totalSize = sceIoLseek32(fD, 0, PSP_SEEK_END);
            long size = totalSize - psarOffset;
                    
            return size;
        
        }
        
        static zzip_ssize_t psarZipWrite(int fD, _zzip_const void* buf, zzip_size_t len) {
        
            return sceIoWrite(fD, buf, len);
        
        }

        FileManager* FileManager::fMInstance = NULL;

        /**
         * Get an instance of the class.
         * 
         * @return FileManager* Singleton instance.
         */
        FileManager* FileManager::getInstance(void) {

            // Create instance if necessary
            if (fMInstance == NULL)
                fMInstance = new FileManager;

            return fMInstance;

        }
        
        /**
         * Set up some stuff.
         */
        void FileManager::init(void) {
        
            zzip_init_io (&psarZipHandlers, 0);
            psarZipHandlers.fd.open = &psarZipOpen;
            psarZipHandlers.fd.close = &psarZipClose;
            psarZipHandlers.fd.read = &psarZipRead;
            psarZipHandlers.fd.seeks = &psarZipSeek;
            psarZipHandlers.fd.filesize = &psarZipFilesize;
            psarZipHandlers.fd.write = &psarZipWrite;
        
        }
        
        /**
         * Magic read function!
         * 
         * Reads the specified file from a multitude of possible locations choosing based on input.
         * 
         * "PSAR@afile.ext" => would load 'afile.ext' from the PSAR section of the EBOOT
         * "resources.zip@afile.ext" => would load 'afile.ext' from a zip file name 'resources.zip'
         * "afile.ext" or "FS@afile.ext" => would simply be loaded from the filesystem.
         * 
         * @param const std::string& file The file.
         * 
         * @return std::string The contents of the file.
         */
        std::string FileManager::read(const std::string& file) {
        
            std::vector<std::string> fileParts;
            
            Util::tokenize(file, fileParts, "@");
            
            if (fileParts.size() == 1) {
            
                // the easy case :)
                return this->readFromFS(file);
            
            } else if (fileParts.size() == 2) {
            
                if (fileParts[0] == "PSAR") {
                
                    return this->readFromPSAR(fileParts[1]);
                    
                } else if (fileParts[0] == "FS") {
                
                    // The explicit case
                
                    return this->readFromFS(fileParts[1]);
                
                } else {
                
                    return this->readFromZIP(fileParts[0], fileParts[1]);
                
                }
            
            }
            
            return "";
        
        }

        /**
         * Read a file in from the PSAR archive (zip file) in the EBOOT.
         * 
         * @param const std::string& file The file within the PSAR .
         * 
         * @return std::string The contents of the file.
         */
        std::string FileManager::readFromPSAR(const std::string& file) {
        
            std::string out;
            
            zzip_strings_t ext[] = {"", 0};
            
            // Construct the path to the resource
            std::stringstream path;
            path << "EBOOT.PBP" << "/" << file;
        
            ZZIP_FILE *fP = zzip_open_ext_io(path.str().c_str(), O_RDONLY | O_BINARY, ZZIP_ONLYZIP, ext, &psarZipHandlers);
            
            if (!fP) {
            
                if (__xM_DEBUG)
                    Util::logMsg("FileManager::readFromPSAR — Unable to open file [%s].", file.c_str());
                    
                return "";
            
            }
            
            // Temporary buffer
            char *buffer;
	
            // Obtain filesize
            // Seek to end
            zzip_seek(fP, 0, SEEK_END);
	
            // Get position [so since we're at the end means filesize]
            long filesize = zzip_tell(fP);
            
            // Go back to start of the file
            zzip_rewind(fP);
	
            // Alloc enough memory for file
            buffer = (char *) malloc(sizeof(char) * filesize);
            
            if (buffer == NULL) {

                if (__xM_DEBUG)
                    Util::logMsg("FileManager::readFromPSAR — Unable to allocate memory.");

            }
	
            // Read file
            int read = zzip_fread(buffer, 1, filesize, fP);
	
            if (read != filesize) {
	
	            if (__xM_DEBUG)
                    Util::logMsg("FileManager::readFromPSAR - Unable to completely read resource from psar [%s][R: %d - F: %d].", file.c_str(), read, filesize);
	
            }
				
            // Alloc to std::string		
            out.assign(buffer, filesize);
	
            // Close handle
            zzip_fclose(fP);
	
            // Free memory
            free(buffer);
                                
            return out;
        
        }
        
        /**
         * Read a file in from a zip file.
         * 
         * @param const std::string& zip The zip file.
         * @param const std::string& file The file within the zip.
         * 
         * @return std::string The contents of the file.
         */
        std::string FileManager::readFromZIP(const std::string& zip, const std::string& file) {
        
            std::string out;
            
            zzip_strings_t ext[] = {"", 0};
            
            // Construct the path to the resource
            std::stringstream path;
            path << zip << "/" << file;
        
            ZZIP_FILE *fP = zzip_open_ext_io(path.str().c_str(), O_RDONLY | O_BINARY, ZZIP_ONLYZIP, ext, 0);
            
            if (!fP) {
            
                if (__xM_DEBUG)
                    Util::logMsg("FileManager::readFromZIP — Unable to open file in zip [%s@%s].", zip.c_str(), file.c_str());
                    
                return "";
            
            }
            
            // Temporary buffer
            char *buffer;
	
            // Obtain filesize
            // Seek to end
            zzip_seek(fP, 0, SEEK_END);
	
            // Get position [so since we're at the end means filesize]
            long filesize = zzip_tell(fP);
            
            // Go back to start of the file
            zzip_rewind(fP);
	
            // Alloc enough memory for file
            buffer = (char *) malloc(sizeof(char) * filesize);
            
            if (buffer == NULL) {

                if (__xM_DEBUG)
                    Util::logMsg("FileManager::readFromZIP — Unable to allocate memory.");

            }
	
            // Read file
            int read = zzip_fread(buffer, 1, filesize, fP);
	
            if (read != filesize) {
	
	            if (__xM_DEBUG)
                    Util::logMsg("FileManager::readFromZIP - Unable to completely read resource from zip [%s@%s][R: %d - F: %d].", zip.c_str(), file.c_str(), read, filesize);
	
            }
				
            // Alloc to std::string		
            out.assign(buffer, filesize);
	
            // Close handle
            zzip_fclose(fP);
	
            // Free memory
            free(buffer);
            
            return out;
        
        }
        
        /**
         * Read a file in from the filesystem. (ms0 or host0 usually)
         * 
         * @param const std::string& file The file.
         * 
         * @return std::string The contents of the file.
         */
        std::string FileManager::readFromFS(const std::string& file) {
        
            std::string out;
            
            SceUID fD = sceIoOpen(file.c_str(), PSP_O_RDONLY, 0777);

            if (fD < 0) {

                if (__xM_DEBUG)
                    Util::logMsg("FileManager::readFromFS — Unable to open file. [%s]", file.c_str());

                return false;

            }

            // Seek to end of file
            long filesize = sceIoLseek32(fD, 0, PSP_SEEK_END);

            // Back to the beginning
            sceIoLseek(fD, 0, 0);

            // Alloc some memory for file contents
            char* buffer = (char*) malloc(filesize);

            if (buffer == NULL) {

                if (__xM_DEBUG)
                    Util::logMsg("FileManager::readFromFS — Unable to allocate memory.");

            }

            // Read in the file
            int read = sceIoRead(fD, buffer, filesize);
            
            if (read != filesize) {
	
	            if (__xM_DEBUG)
                    Util::logMsg("FileManager::readFromFS - Unable to completely read file [%s][R: %d - F: %d].", file.c_str(), read, filesize);
	
            }
            
            // Alloc to std::string		
            out.assign(buffer, filesize);
	
            // Close handle
            sceIoClose(fD);
	
            // Free memory
            free(buffer);
            
            return out;
        
        }

    }

}

#endif /* _FileManager_CPP */
