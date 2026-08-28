# This was made with slight help with ai, need to rewrite and learn to make makefiles lol

# TODO
# MAKE THE SRC VAR CHECK A FOLDER RECURSIVELY AND CHECK FOR CPP FILES

CXX = g++

CXXFLAGS = -I./lib/SDL3/include
LDFLAGS = -L./lib
LDLIBS = -lSDL3 -lSDL3_image -lcomdlg32 -lole32 -luuid

TARGET = "output/GameMaker Anywhere.exe"
SRC = source/main.cpp\
	  source/compiler/compiler_main.cpp\
	  source/helpers/renderer.cpp\
	  source/helpers/meta.cpp

all:
	$(CXX) $(CURDIR)/$(SRC) -o $(TARGET) $(CXXFLAGS) $(LDFLAGS) $(LDLIBS)
	cp -r Runtime output # COPY RUNTIME TO THE OUTPUT

run: all
	./$(TARGET)

clean:
	rm -f $(TARGET) 
	rm -rf output/Runtime