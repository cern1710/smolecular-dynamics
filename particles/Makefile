CXX = g++

SDL_INCLUDE_PATH = /opt/homebrew/include/SDL2
SDL_LIB_PATH = /opt/homebrew/lib
CXXFLAGS = -std=c++11 -I $(SDL_INCLUDE_PATH) -L $(SDL_LIB_PATH) -lSDL2
TARGET = particle
SOURCES = particle.cpp

all:
	$(CXX) $(SOURCES) $(CXXFLAGS) -o $(TARGET)

clean:
	rm -f $(TARGET)
