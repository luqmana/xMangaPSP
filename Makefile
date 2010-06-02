 # $Id: Makefile 86 2010-01-03 04:12:17Z chaotic@luqmanrocks.co.cc $
 # 
 # This file is part of the OneMangaPSP application.
 #
 # Copyright (C) 2009  Luqman Aden <www.luqmanrocks.co.cc>.
 #
 # This program is free software: you can redistribute it and/or modify
 # it under the terms of the GNU General Public License as published by
 # the Free Software Foundation, either version 3 of the License, or
 # (at your option) any later version.
 #
 # This program is distributed in the hope that it will be useful,
 # but WITHOUT ANY WARRANTY; without even the implied warranty of
 # MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 # GNU General Public License for more details.
 #
 # Full License: LICENSE.txt
 #
 # You should have received a copy of the GNU General Public License
 # along with this program.  If not, see <http://www.gnu.org/licenses/>.

 # PSPSDK Variables
 PSPSDK = $(shell psp-config --pspsdk-path)
 PSPBIN = $(PSPSDK)/../bin
 
 # Targets
 TARGET = OneMangaPSP
 EXTRA_TARGETS = EBOOT.PBP
 
 # Other files to be deleted when clean is called
 EXTRA_CLEAN = Resources.omle OneMangaPSP.zip
 
 # Files to be put into resource file
 RESOURCE_FILES = Resources/BatteryIcons.png Resources/LoadingIcons.png Resources/MainUI.png Resources/Splash1.png Resources/Splash2.png
 
 # Objects
 OBJS = OneMangaPSP.o OMPStandard.o OMPNetwork.o LeXEngine.o MangaAPI.o Timer.o ResourceManager.o cJSON.o TitleState.o MenuState.o MangaSelectState.o BookmarkState.o RecentMangaState.o MusicSelectState.o AboutState.o ChapterSelectState.o ImageSelectState.o MangaViewState.o OMText.o OMPUtil.o FastEvents.o SDL_rwops_zzip.o BookmarkManager.o pge/pgeFile.o pge/pgeMp3.o
 
 # Defines
 MAJOR_VERSION = 1
 MINOR_VERSION = 0
 API_VERSION = 4
 
 COMPILER_DEFINES = -DTIXML_USE_STL -D_MAJOR_VERSION=$(MAJOR_VERSION) -D_MINOR_VERSION=$(MINOR_VERSION) -D_API_VERSION=$(API_VERSION)
 
 # Include Directories
 INCDIR = Headers/ pge/ #$(shell psp-config --pspdev-path)/psp/include/SDL
 
 # Library Directories
 LIBS = -ltinyxml -lcurl -lpspaudiocodec -lSDL_ttf -lfreetype -lSDL_gfx -lSDL_image -lpng -ljpeg $(shell sdl-config --libs) -lpspirkeyb -lpsppower -lzzip -lz -lpspgu -lpspsdk -lstdc++
 
 # Compiler Flags
 #CFLAGS = -g -O3 -G4 -Wall -falign-functions=64 $(COMPILER_DEFINES) $(shell freetype-config --cflags)
 CFLAGS = -O3 -G4 -Wall -falign-functions=64 $(COMPILER_DEFINES) $(shell freetype-config --cflags)
 #LDFLAGS = -g
 CXXFLAGS = $(CFLAGS) -fno-exceptions -fno-rtti
 ASFLAGS = 
 
 # FW Version
 PSP_FW_VERSION = 500
 
 # Extra Memory on SLIM and Lite
 PSP_LARGE_MEMORY = 1
 
 # Build as prx for dynamic memory allocation
 BUILD_PRX = 1
 
 # User Libraries
 USE_USER_LIBS = 1
  
 # EBOOT.PBP Variables
 PSP_EBOOT_TITLE = OneMangaPSP v$(MAJOR_VERSION).$(MINOR_VERSION)
 PSP_EBOOT_SFO = PARAM.SFO
 PSP_EBOOT_ICON = Resources/ICON0.png
 PSP_EBOOT_ICON1 = Resources/ICON1.pmf
 PSP_EBOOT_UNKPNG = NULL
 PSP_EBOOT_PIC1 = Resources/PIC1.png
 PSP_EBOOT_SND0 = NULL
 PSP_EBOOT_PSAR = NULL
 PSP_EBOOT = EBOOT.PBP
 
 # Get the base makefile
 include $(PSPSDK)/lib/build.mak
  
 psplink:
 
	@rm -rf PSPLinkBuild
	@mkdir PSPLinkBuild
	@cp Resources.omle PSPLinkBuild
	@cp Resources/credits.mp3 PSPLinkBuild
	@cp Resources/SansBold.ttf PSPLinkBuild
	+make
	@cp -R OneMangaPSP.prx PSPLinkBuild
	@cp -R EBOOT.PBP PSPLinkBuild
	
 pak:
 
	-rm -f Resources.omle
	-zip -0 -j Resources.omle $(RESOURCE_FILES)
	
 release:
 
	@rm -rf OneMangaPSP
	@mkdir OneMangaPSP
	+make
	+make pak
	@cp Resources.omle OneMangaPSP
	@cp Resources/SansBold.ttf OneMangaPSP
	@cp Resources/credits.mp3 OneMangaPSP
	@cp README.txt OneMangaPSP
	@cp LICENSE.txt OneMangaPSP
	@cp EBOOT.PBP OneMangaPSP
	-zip -r OneMangaPSP.zip OneMangaPSP
