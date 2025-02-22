CXX = g++
CXXFLAGS = -std=c++11 -Wall -Wextra -O2
SRCS = main.cpp puzzle.cpp HH.cpp
TARGET = puzzle_solver

all: $(TARGET)

$(TARGET): $(SRCS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRCS)

clean:
	rm -f $(TARGET)

.PHONY: all clean