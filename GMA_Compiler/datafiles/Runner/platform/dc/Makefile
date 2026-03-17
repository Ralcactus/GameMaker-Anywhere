#---------------------------------------------------------------------------------
.SUFFIXES:
#---------------------------------------------------------------------------------

TOPDIR      ?= $(CURDIR)

TARGET      := $(notdir $(CURDIR))
BUILD       := build
SOURCES     := source
DATA        := data
INCLUDES    := include
OUTPUT      := output
ROMFS       := romfs
KOS_ROMDISK_DIR := romdisk

#---------------------------------------------------------------------------------
# Toolchain
#---------------------------------------------------------------------------------

CC      := kos-cc
CXX     := kos-c++
LD      := kos-c++

#---------------------------------------------------------------------------------
# Flags
#---------------------------------------------------------------------------------

CFLAGS      += -Wall -O2 -std=gnu11 -D_DEFAULT_SOURCE -D__RAYLIB__
CXXFLAGS    += -Wall -O2 -std=gnu++14 -fno-exceptions -fno-rtti -D_DEFAULT_SOURCE -D__RAYLIB__

INCLUDE     := $(foreach dir,$(INCLUDES),-I$(CURDIR)/$(dir)) \
               -I$(KOS_PORTS)/include/raylib

LIBS        := -lraylib -lGL -lm -lkosutils -lADX

#---------------------------------------------------------------------------------
# Source discovery
#---------------------------------------------------------------------------------

CFILES      := $(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.c)))
CPPFILES    := $(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.cpp)))

OFILES      := $(CFILES:.c=.o) $(CPPFILES:.cpp=.o)

#---------------------------------------------------------------------------------
# Build logic
#---------------------------------------------------------------------------------

ifneq ($(BUILD),$(notdir $(CURDIR)))

export TOPDIR      := $(CURDIR)
export OUTPUT_DIR  := $(TOPDIR)/$(OUTPUT)
export OUTPUT_FILE := $(OUTPUT_DIR)/$(TARGET)
export VPATH       := $(foreach dir,$(SOURCES),$(CURDIR)/$(dir))
export DEPSDIR     := $(CURDIR)/$(BUILD)

.PHONY: all clean run

all: bootstrap
	@$(MAKE) --no-print-directory -C $(BUILD) -f $(CURDIR)/Makefile

dc: all

bootstrap:
	@[ -d $(BUILD) ] || mkdir -p $(BUILD)
	@[ -d $(OUTPUT) ] || mkdir -p $(OUTPUT)

clean:
	@echo clean ...
	@rm -rf $(BUILD) $(OUTPUT)

run: all
	$(KOS_LOADER) $(OUTPUT_FILE).elf

#---------------------------------------------------------------------------------
else

#---------------------------------------------------------------------------------
# Inside build directory
#---------------------------------------------------------------------------------

$(OUTPUT_FILE).elf: $(OFILES) $(KOS_ROMDISK_OBJ)
	$(LD) -o $@ $(OFILES) $(KOS_ROMDISK_OBJ) $(LIBS)
	@echo built ... $(notdir $@)

%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDE) -c $< -o $@

%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDE) -c $< -o $@

clean:
	@rm -f *.o *.elf

#---------------------------------------------------------------------------------
endif
#---------------------------------------------------------------------------------
