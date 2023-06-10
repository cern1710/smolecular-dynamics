# Compiler
CXX = g++

# SDL paths
SDL_INCLUDE_PATH = /opt/homebrew/include/SDL2
SDL_LIB_PATH = /opt/homebrew/lib

# Compiler flags
CXXFLAGS = -std=c++11 -I $(SDL_INCLUDE_PATH) -L $(SDL_LIB_PATH) -lSDL2

# Target executable
TARGET = gravity

# Source files
SOURCES = gravity.cpp

all:
	$(CXX) $(SOURCES) $(CXXFLAGS) -o $(TARGET)

clean:
	rm -f $(TARGET)
