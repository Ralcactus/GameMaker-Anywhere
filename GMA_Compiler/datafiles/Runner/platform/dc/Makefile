TOPDIR ?= $(CURDIR)

#---------------------------------------------------------------------------------
# TARGET is the name of the output
# BUILD is the directory where object files & intermediate files will be placed
# SOURCES is a list of directories containing source code
# DATA is a list of directories containing data files
# INCLUDES is a list of directories containing header files
# RESOURCES is the directory where AppInfo template.rsf etc can be found
# OUTPUT is the directory where final executables will be placed
# GRAPHICS is a list of directories containing graphics files
# GFXBUILD is the directory where converted graphics files will be placed
#   If set to $(BUILD), it will statically link in the converted
#   files as if they were data files.
#
# NO_SMDH: if set to anything, no SMDH file is generated.
# ROMFS is the directory which contains the RomFS, relative to the Makefile (Optional)
# APP_TITLE is the name of the app stored in the SMDH file (Optional)
# APP_DESCRIPTION is the description of the app stored in the SMDH file (Optional)
# APP_AUTHOR is the author of the app stored in the SMDH file (Optional)
# ICON is the filename of the icon (.png), relative to the project folder.
#   If not set, it attempts to use one of the following (in this order):
#     - <Project name>.png
#     - icon.png
#     - <libctru folder>/default_icon.png
#---------------------------------------------------------------------------------

TARGET      := $(notdir $(CURDIR))
BUILD       := build
SOURCES     := source
DATA        := data
INCLUDES    := include
GRAPHICS    := gfx
OUTPUT      := output
RESOURCES   := resources
ROMFS       := romfs
GFXBUILD    := $(ROMFS)/gfx


OBJS = source/*.c romdisk.o
KOS_ROMDISK_DIR = romdisk

#---------------------------------------------------------------------------------
# list of directories containing libraries, this must be the top level containing
# include and lib
#---------------------------------------------------------------------------------

KOS_CFLAGS += -I${KOS_PORTS}/include/raylib

#---------------------------------------------------------------------------------
# options for code generation
#---------------------------------------------------------------------------------

CXX = kos-c++
CC = kos-cc

CFLAGS += -Wall -std=c++14 -D_DEFAULT_SOURCE -Wno-missing-braces -D__RAYLIB__

ifneq ($(BUILD),$(notdir $(CURDIR)))

export TOPDIR      := $(CURDIR)
export OUTPUT_DIR  := $(TOPDIR)/$(OUTPUT)
export OUTPUT_FILE := $(OUTPUT_DIR)/$(TARGET)
export VPATH       := $(foreach dir,$(SOURCES),$(CURDIR)/$(dir)) \
                      $(foreach dir,$(GRAPHICS),$(CURDIR)/$(dir)) \
                      $(foreach dir,$(DATA),$(CURDIR)/$(dir))

export DEPSDIR     := $(CURDIR)/$(BUILD)

all: rm-elf $(TARGET)

include $(KOS_BASE)/Makefile.rules

clean: rm-elf
	-rm -f $(OBJS)

rm-elf:
	-rm -f $(TARGET) romdisk.*

#---------------------------------------------------------------------------------
# Libraries needed to link into the executable.
#---------------------------------------------------------------------------------

$(TARGET): $(OBJS)
	$(CXX) -o $(TARGET) $(OBJS) -lraylib -lGL -lm -lkosutils -lADX

run: $(TARGET)
	$(KOS_LOADER) $(TARGET)

dist: $(TARGET)
	-rm -f $(OBJS) romdisk.img
	$(KOS_STRIP) $(TARGET)