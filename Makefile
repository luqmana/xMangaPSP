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
EXT_LIB_DIR = $(EXT_DIR)/lib
EXT_INC_DIR = $(EXT_DIR)/include
RES_DIR = resources

PSAR_PAK = $(BUILD_DIR)/resources.zip

# Resources to pack into psar
RESOURCES = $(wildcard $(RES_DIR)/ui/*.xml)
RESOURCES := $(RESOURCES) $(wildcard $(RES_DIR)/fonts/*.pgf)
RESOURCES := $(patsubst $(RES_DIR)/%, %, $(RESOURCES))

# Targets
TARGET = $(BUILD_DIR)/xMangaPSP
EXTRA_TARGETS = $(BUILD_DIR)/EBOOT.PBP

# Other files to be deleted when clean is called
EXTRA_CLEAN = $(PSAR_PAK)

# States
STATES += $(wildcard $(SRC_DIR)/States/*.cpp)
STATES := $(patsubst $(SRC_DIR)/States/%.cpp, %.cpp, $(STATES))

# Source files
SRC_CPP  = $(wildcard $(SRC_DIR)/*.cpp)
SRC_CPP += $(STATES)
SRC_C    = $(wildcard $(SRC_DIR)/*.c)
SRC_CPP := $(patsubst $(SRC_DIR)/%.cpp, %.cpp, $(SRC_CPP))
SRC_C   := $(patsubst $(SRC_DIR)/%.c, %.c, $(SRC_C))

# External Libs Source Files
EXT_SRC_CPP += $(wildcard $(EXT_SRC_DIR)/tinyxml/*.cpp)
EXT_SRC_C   += $(wildcard $(EXT_SRC_DIR)/intraFontG/*.c)
EXT_SRC_C   += $(wildcard $(EXT_SRC_DIR)/LodePNG/*.c)

EXT_SRC_CPP := $(patsubst $(EXT_SRC_DIR)/%.cpp, %.cpp, $(EXT_SRC_CPP))
EXT_SRC_C   := $(patsubst $(EXT_SRC_DIR)/%.c, %.c, $(EXT_SRC_C))

# Object files
OBJS += $(patsubst %.cpp, $(BUILD_DIR)/%.o, $(filter %.cpp, $(SRC_CPP)))
OBJS += $(patsubst %.c, $(BUILD_DIR)/%.o, $(filter %.c, $(SRC_C)))

# External Libs Object Files
EXT_LIB_OBJS += $(patsubst %.cpp, $(BUILD_DIR)/%.o, $(filter %.cpp, $(EXT_SRC_CPP)))
EXT_LIB_OBJS += $(patsubst %.c, $(BUILD_DIR)/%.o, $(filter %.c, $(EXT_SRC_C)))

# Defines
MAJOR_VERSION = 0
MINOR_VERSION = 1
EXTRA_VERSION = a2
VERSION_STRING= v$(MAJOR_VERSION).$(MINOR_VERSION) $(EXTRA_VERSION)
API_VERSION   = 5

# Debug mode
DEBUG = 1

COMPILER_DEFINES = -D_MAJOR_VERSION=$(MAJOR_VERSION) -D_MINOR_VERSION=$(MINOR_VERSION) -D_API_VERSION=$(API_VERSION) -D_EXTRA_VERION=$(EXTRA_VERSION) -D__xM_DEBUG=$(DEBUG)

# Include Directories
INCDIR = $(SRC_DIR) $(INC_DIR) $(EXT_SRC_DIR) $(EXT_INC_DIR) $(PSPDEV)/psp/include/

# Libraries
LIBS = -lpspssl -lpsphttp -ljpeg -lzzip -lz -lpspgum -lpspgu -lm -lpsprtc -lpspsdk -lstdc++

# Compiler Flags
CFLAGS = -Wall $(COMPILER_DEFINES) -falign-functions=64
CXXFLAGS = $(CFLAGS) -fno-rtti -fno-exceptions
ASFLAGS =

# Enable the debug options
ifeq ($(DEBUG), 1)
CFLAGS += -g -G4
else
CFLAGS += -O3 -G4 -ffast-math
endif

# Linker Flags
LDFLAGS = -L$(EXT_LIB_DIR)

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
PSP_EBOOT           = $(BUILD_DIR)/EBOOT.PBP
PSP_EBOOT_TITLE     = xMangaPSP $(VERSION_STRING)
PSP_EBOOT_SFO       = $(BUILD_DIR)/PARAM.SFO # A sort of description file, generated compile time
PSP_EBOOT_ICON      = NULL # Main Program Icon      144 x 80
PSP_EBOOT_ICON1     = #$(RES_DIR)/eboot/ICON1.PMF # Animated Program Icon  144 x 80
PSP_EBOOT_UNKPNG    = NULL # Overlay Image          310 x 180
PSP_EBOOT_PIC1      = NULL # Background             480 x 272
PSP_EBOOT_SND0      = NULL # Background music
ifeq ($(DEBUG), 1)
PSP_EBOOT_PSAR 	    = NULL
else
PSP_EBOOT_PSAR 	    = $(PSAR_PAK) # A data file. Store whatever in here.
endif
  
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
	
# Rule to build bundled LodePNG with optimization
$(BUILD_DIR)/LodePNG/LodePNG.o: $(EXT_SRC_DIR)/LodePNG/LodePNG.c | $(BUILD_DIR)/LodePNG
	$(CXX) $(CXXFLAGS) -I$(EXT_INC_DIR)/LodePNG/ -O3 -G4 -ffast-math -c -o $@ $<
$(BUILD_DIR)/LodePNG:
	mkdir $(BUILD_DIR)/LodePNG

# Rule to build image class with optimization
$(BUILD_DIR)/Image.o: $(SRC_DIR)/Image.cpp
	$(CXX) $(CXXFLAGS) -O3 -G4 -ffast-math -c -o $@ $<

# Rule to build xml parser class with optimization as well an extra definition
$(BUILD_DIR)/XMLParser.o: $(SRC_DIR)/XMLParser.cpp
	$(CXX) $(CXXFLAGS) -DTIXML_USE_STL -O3 -ffast-math -c -o $@ $<
	
# Rule to build bundled TinyXML
$(BUILD_DIR)/tinyxml/%.o: $(EXT_SRC_DIR)/tinyxml/%.cpp | $(BUILD_DIR)/tinyxml
	$(CXX) $(CXXFLAGS) -DTIXML_USE_STL -O3 -ffast-math -c -o $@ $<
$(BUILD_DIR)/tinyxml:
	mkdir $(BUILD_DIR)/tinyxml
	
# Rule to build bundled intraFont-G
$(BUILD_DIR)/intraFontG/%.o: $(EXT_SRC_DIR)/intraFontG/%.c | $(BUILD_DIR)/intraFontG
	$(CC) $(CFLAGS) -O3 -ffast-math -c -o $@ $<
$(BUILD_DIR)/intraFontG:
	mkdir $(BUILD_DIR)/intraFontG

# Override the one in the base makefile
# since we want to include the external libraries we use
$(TARGET).elf: $(OBJS) $(EXT_LIB_OBJS) $(EXPORT_OBJ)
	$(LINK.c) $^ $(LIBS) -o $@
	$(FIXUP) $@

cleanext: 
	-rm -f $(EXT_LIB_OBJS)

psar:
	-cd $(RES_DIR); rm ../$(PSAR_PAK); zip ../$(PSAR_PAK) $(RESOURCES)
