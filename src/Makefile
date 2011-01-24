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
 
 BUILD_DIR = ../build
 
 # Targets
 TARGET = xMangaPSP
 EXTRA_TARGETS = $(BUILD_DIR)/EBOOT.PBP
 
 # Other files to be deleted when clean is called
 EXTRA_CLEAN = 
 
 # States
 STATES = MenuState.o
 
 # Objects
 OBJS = xMangaPSP.o Callbacks.o Graphics.o Stats.o Timer.o StateManager.o $(STATES)
  
 # Defines
 MAJOR_VERSION = 1
 MINOR_VERSION = 0
 EXTRA_VERSION = 
 API_VERSION   = 5
 
 COMPILER_DEFINES = -D_MAJOR_VERSION=$(MAJOR_VERSION) -D_MINOR_VERSION=$(MINOR_VERSION) -D_API_VERSION=$(API_VERSION) -D_EXTRA_VERION=$(EXTRA_VERSION)
 
 # Include Directories
 INCDIR = ../include
 
 # Library Directories
 LIBS = -lpspgum -lpspgu -lm -lpsprtc -lpspsdk -lstdc++
 
 # Compiler Flags
 CFLAGS = -O3 -G4 -Wall -falign-functions=64 $(COMPILER_DEFINES)
 CXXFLAGS = $(CFLAGS) -fno-exceptions -fno-rtti
 ASFLAGS = 
 
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
 PSP_EBOOT_TITLE = xMangaPSP v$(MAJOR_VERSION).$(MINOR_VERSION)$(EXTRA_VERSION)
 #PSP_EBOOT_SFO = PARAM.SFO
 #PSP_EBOOT_ICON = Resources/ICON0.png
 #PSP_EBOOT_ICON1 = Resources/ICON1.pmf
 #PSP_EBOOT_UNKPNG = NULL
 #PSP_EBOOT_PIC1 = Resources/PIC1.png
 #PSP_EBOOT_SND0 = NULL
 #PSP_EBOOT_PSAR = NULL
 #PSP_EBOOT = EBOOT.PBP
 
 # Get the base makefile
 include $(PSPSDK)/lib/build.mak
 
 $(BUILD_DIR)/EBOOT.PBP: EBOOT.PBP
