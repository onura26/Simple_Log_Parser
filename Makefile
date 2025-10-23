CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra
TARGET = logparser
SRCS = main.cpp src/arg_parser.cpp src/file_processor.cpp src/utils.cpp

all: $(TARGET)

$(TARGET): $(SRCS)
	$(CXX) $(CXXFLAGS) $(SRCS) -o $(TARGET)

clean:
	rm -f $(TARGET)

.PHONY: all clean