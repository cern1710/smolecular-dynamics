CXX = g++

SDL_INCLUDE_PATH = /opt/homebrew/include/
SDL_LIB_PATH = /opt/homebrew/lib
CXXFLAGS = -std=c++11 -I$(SDL_INCLUDE_PATH)
LDFLAGS = -L$(SDL_LIB_PATH) -lSDL2
TARGET = particle
SOURCES = particle.cpp

all:
	$(CXX) $(CXXFLAGS) $(SOURCES) $(LDFLAGS) -o $(TARGET)

clean:
	rm -f $(TARGET)