CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra
TARGET = logparser
SOURCES = main.cpp $(wildcard src/*.cpp)
OBJECTS = $(SOURCES:.cpp=.o)

all: $(TARGET)

$(TARGET): $(SOURCES)
	$(CXX) $(CXXFLAGS) $(SOURCES) -o $(TARGET)

clean:
	rm -f $(TARGET) $(OBJECTS)