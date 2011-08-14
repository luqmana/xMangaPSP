 # This file is part of the xMangaPSP application.
 #
 # Copyright (C) Luqman Aden <www.luqmanrocks.co.cc>.
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
 # Full License: GNU-LICENSE
 #
 # You should have received a copy of the GNU General Public License
 # along with this program.  If not, see <http://www.gnu.org/licenses/>.

 # PSPSDK Variables
 PSPSDK = $(shell psp-config --pspsdk-path)
 PSPBIN = $(PSPSDK)/../bin
 
 BUILD_DIR = build
 SRC_DIR = src
 INC_DIR = include
 EXT_DIR = ext
 EXT_SRC_DIR = $(EXT_DIR)/src
 EXT_INC_DIR = $(EXT_DIR)/include
 RES_DIR = resources
 
 # Targets
 TARGET		= $(BUILD_DIR)/xMangaPSP
 EXTRA_TARGETS 	= $(BUILD_DIR)/EBOOT.PBP
 
 # Other files to be deleted when clean is called
 EXTRA_CLEAN = 
 
 # States
 STATES += $(wildcard $(SRC_DIR)/States/*.cpp)
 STATES := $(patsubst $(SRC_DIR)/States/%.cpp, %.cpp, $(STATES))
 
 # Source files
 SRC_CPP   =  xMangaPSP.cpp Callbacks.cpp Graphics.cpp Dialogs.cpp Stats.cpp Utils.cpp Timer.cpp FileManager.cpp InputManager.cpp ResourceManager.cpp StateManager.cpp Image.cpp Log.cpp PicoPNG.cpp Text.cpp XMLParser.cpp Net.cpp ExtraElements.cpp MangaAPI.cpp MAP.cpp
 SRC_CPP  +=  $(STATES)
 SRC_C	   = cJSON.c
 
 # External Libs Source Files
 EXT_SRC_CPP += $(wildcard $(EXT_SRC_DIR)/tinyxml/*.cpp)
 EXT_SRC_CPP += $(wildcard $(EXT_SRC_DIR)/imagexx/*.cpp)
 EXT_SRC_CPP += $(wildcard $(EXT_SRC_DIR)/jpegxx/*.cpp)
 EXT_SRC_C 	 += $(wildcard $(EXT_SRC_DIR)/intraFontG/*.c)
 
 EXT_SRC_CPP := $(patsubst $(EXT_SRC_DIR)/%.cpp, %.cpp, $(EXT_SRC_CPP))
 EXT_SRC_C   := $(patsubst $(EXT_SRC_DIR)/%.c, %.c, $(EXT_SRC_C))
  
 # Object files
 OBJS    += $(patsubst %.cpp, $(BUILD_DIR)/%.o, $(filter %.cpp, $(SRC_CPP)))
 OBJS    += $(patsubst %.c, $(BUILD_DIR)/%.o, $(filter %.c, $(SRC_C)))
 
 # External Libs Object Files
 OBJS	+= $(patsubst %.cpp, $(BUILD_DIR)/%.o, $(filter %.cpp, $(EXT_SRC_CPP)))
 OBJS	+= $(patsubst %.c, $(BUILD_DIR)/%.o, $(filter %.c, $(EXT_SRC_C)))
 
 # Defines
 MAJOR_VERSION = 1
 MINOR_VERSION = 0
 EXTRA_VERSION = 
 API_VERSION   = 5
 
 # Debug mode
 DEBUG = 1
 
 COMPILER_DEFINES = -D_MAJOR_VERSION=$(MAJOR_VERSION) -D_MINOR_VERSION=$(MINOR_VERSION) -D_API_VERSION=$(API_VERSION) -D_EXTRA_VERION=$(EXTRA_VERSION) -D__xM_DEBUG=$(DEBUG) -DTIXML_USE_STL
 
 # Include Directories
 INCDIR = $(SRC_DIR) $(INC_DIR) $(EXT_SRC_DIR) $(EXT_INC_DIR) $(PSPDEV)/psp/include/
 
 # Library Directories
 LIBS = -lcurl -ljpeg -lzzip -lz -lpspgum -lpspgu -lm -lpsprtc -lpspsdk -lstdc++
 
 # Compiler Flags
 CFLAGS = -Wall $(COMPILER_DEFINES) -falign-functions=64
 CXXFLAGS = $(CFLAGS) #-fno-rtti
 ASFLAGS =

 # Enable the debug options
 ifeq ($(DEBUG), 1)
 CFLAGS += -g -G4
 else
 CFLAGS += -O3 -G4 -ffast-math
 endif
 
 # Linker Flags
 LDFLAGS = 
 
 # FW Version
 PSP_FW_VERSION = 600
 
 # Extra Memory on SLIM and Lite
 # This option combined with the SIGN option below doesn't seem to work
 #PSP_LARGE_MEMORY = 1
 
 # Build as prx
 BUILD_PRX = 1
 
 # [Fake] Sign
 SIGN = 1
 
 # User Libraries
 USE_USER_LIBS = 1
  
 # EBOOT.PBP Variables
 PSP_EBOOT 			= $(BUILD_DIR)/EBOOT.PBP
 PSP_EBOOT_TITLE 	= xMangaPSP v$(MAJOR_VERSION).$(MINOR_VERSION)$(EXTRA_VERSION)
 PSP_EBOOT_SFO 		= $(BUILD_DIR)/PARAM.SFO# A sort of description file, generated compile time
 PSP_EBOOT_ICON 	= NULL# Main Program Icon		144 x 80
 PSP_EBOOT_ICON1 	= NULL# Animated Program Icon	144 x 80
 PSP_EBOOT_UNKPNG 	= NULL# Overlay Image			310 x 180
 PSP_EBOOT_PIC1 	= NULL# Background				480 x 272
 PSP_EBOOT_SND0 	= NULL# Background music
 #PSP_EBOOT_PSAR 	= $(RES_DIR)/psar.zip# A data file. Store whatever in here.
  
 # Get the base makefile
 include $(PSPSDK)/lib/build.mak

 # Rule to keep src dir clean while building
 $(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<
	
 # Rule to keep src dir clean while building
 $(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c -o $@ $<

 # Rule to build state classes
 $(BUILD_DIR)/%.o: $(SRC_DIR)/States/%.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<
	
 # Rule to build PicoPNG with optimization
 $(BUILD_DIR)/PicoPNG.o: $(SRC_DIR)/PicoPNG.cpp
	$(CXX) $(CXXFLAGS) -O3 -G4 -ffast-math -c -o $@ $<

 # Rule to build image class with optimization
 $(BUILD_DIR)/Image.o: $(SRC_DIR)/Image.cpp
	$(CXX) $(CXXFLAGS) -O3 -G4 -ffast-math -c -o $@ $<
	
 # Rule to build bundled TinyXML
 $(BUILD_DIR)/tinyxml/%.o: $(EXT_SRC_DIR)/tinyxml/%.cpp | $(BUILD_DIR)/tinyxml
	$(CXX) $(CXXFLAGS) -c -o $@ $<
 $(BUILD_DIR)/tinyxml:
	mkdir $(BUILD_DIR)/tinyxml
	
 # Rule to build bundled intraFont-G
 $(BUILD_DIR)/intraFontG/%.o: $(EXT_SRC_DIR)/intraFontG/%.c | $(BUILD_DIR)/intraFontG
	$(CC) $(CFLAGS) -O3 -ffast-math -c -o $@ $<
 $(BUILD_DIR)/intraFontG:
	mkdir $(BUILD_DIR)/intraFontG

 # Rule to build bundled imagexx
 $(BUILD_DIR)/imagexx/%.o: $(EXT_SRC_DIR)/imagexx/%.cpp | $(BUILD_DIR)/imagexx
	$(CXX) $(CXXFLAGS) -O3 -ffast-math -c -o $@ $<
 $(BUILD_DIR)/imagexx:
	mkdir $(BUILD_DIR)/imagexx

 # Rule to build bundled jpegxx
 $(BUILD_DIR)/jpegxx/%.o: $(EXT_SRC_DIR)/jpegxx/%.cpp | $(BUILD_DIR)/jpegxx
	$(CXX) $(CXXFLAGS) -I$(EXT_SRC_DIR)/jpegxx -I$(EXT_INC_DIR)/jpegxx -O3 -ffast-math -c -o $@ $<
 $(BUILD_DIR)/jpegxx:
	mkdir $(BUILD_DIR)/jpegxx
