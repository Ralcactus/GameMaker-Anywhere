TARGET = Runner.elf
OBJS = source/main.o source/cJSON.o source/cross_platform.o source/gml_functions.o source/gml_runner.o source/shortcut_functions.o romdisk.o
KOS_ROMDISK_DIR = romdisk

KOS_CFLAGS += -I${KOS_PORTS}/include/raylib -D__RAYLIB__ -D__DC__

all: rm-elf $(TARGET)

include $(KOS_BASE)/Makefile.rules

clean: rm-elf
	-rm -f $(OBJS)

rm-elf:
	-rm -f $(TARGET) romdisk.*

$(TARGET): $(OBJS)
	kos-cc -o $(TARGET) $(OBJS) -lraylib -lGL -lkosutils 

run: $(TARGET)
	$(KOS_LOADER) $(TARGET)

dist: $(TARGET)
	-rm -f $(OBJS) romdisk.img
	$(KOS_STRIP) $(TARGET)

