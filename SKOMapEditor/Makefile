CXX=g++
CXXFLAGS=-c -Wall -g -D_GLIBCXX_USE_NANOSLEEP -std=c++11 -O3
LDLIBS=-lGL -lSDL -lSDL_image
SOURCES:=$(wildcard *.cpp)
OBJECTS:=$(patsubst %.cpp,%.o,$(SOURCES))
EXECUTABLE=SKOMapEditor

all : $(EXECUTABLE)

$(EXECUTABLE) : $(OBJECTS)
	$(CXX) $(OBJECTS) -o $@ $(LDLIBS)
	
$(OBJECTS) : %.o : %.cpp
	$(CXX) $(CXXFLAGS) $< -o $@

clean : 
	$(RM) $(OBJECTS) $(EXECUTABLE)
