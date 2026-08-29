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
      source/compiler/Asset/SpriteCompiler.cpp \
      source/compiler/Variables/HandleVariables.cpp

OBJ = $(patsubst %.cpp,build/%.o,$(SRC))

all: $(TARGET)
	cp -r Runtime output

$(TARGET): $(OBJ)
	@echo "Linking $(TARGET)..."
	$(CXX) $(OBJ) -o "$(TARGET)" $(LDFLAGS) $(LDLIBS)

build/%.o: %.cpp
	@echo "Compiling $<"
	@mkdir -p "$(dir $@)"
	$(CXX) $(CXXFLAGS) -c "$<" -o "$@"

run: all
	./"$(TARGET)"

clean:
	rm -f $(TARGET)
	rm -rf build
	rm -rf output/Runtime