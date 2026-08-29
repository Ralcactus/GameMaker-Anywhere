# This was made with slight help with ai, need to rewrite and learn to make makefiles lol

# TODO
# MAKE THE SRC VAR CHECK A FOLDER RECURSIVELY AND CHECK FOR CPP FILES

CXX = g++

CXXFLAGS = -I./lib/SDL3/include
LDFLAGS = -L./lib
LDLIBS = -lSDL3 -lSDL3_image -lcomdlg32 -lole32 -luuid -ljsoncpp

TARGET = output/GameMaker Anywhere.exe

SRC = source/main.cpp \
      source/compiler/compiler_main.cpp \
      source/helpers/renderer.cpp \
      source/helpers/meta.cpp \
      source/compiler/Asset/RoomCompiler.cpp \
      source/compiler/Variables/HandleVariables.cpp

OBJ = $(SRC:.cpp=.o)

all: $(TARGET)
	cp -r Runtime output

$(TARGET): $(OBJ)
	@echo "Linking $(TARGET)..."
	$(CXX) $(OBJ) -o "$(TARGET)" $(LDFLAGS) $(LDLIBS)

%.o: %.cpp
	@echo "Compiling $<"
	$(CXX) $(CXXFLAGS) -c "$<" -o "$@"

run: all
	./"$(TARGET)"

clean:
	rm -f $(TARGET) $(OBJ)
	rm -rf output/Runtime