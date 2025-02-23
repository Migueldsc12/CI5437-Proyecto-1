CXX = g++
CXXFLAGS = -std=c++11 -Wall -Wextra -O2
SRCS = main.cpp puzzle.cpp HH.cpp precompute_states.cpp
TARGET = puzzle_solver
PRECOMPUTE_TARGET = precompute
DB_FILE = states_db.txt

all: $(DB_FILE) $(TARGET)

$(TARGET): $(SRCS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) main.cpp puzzle.cpp HH.cpp

$(DB_FILE): precompute_states.cpp
	@echo "Creando archivo $(DB_FILE)..."
	$(CXX) $(CXXFLAGS) -o $(PRECOMPUTE_TARGET) precompute_states.cpp
	./$(PRECOMPUTE_TARGET)
	@echo "Archivo $(DB_FILE) creado exitosamente."

clean:
	rm -f $(TARGET) $(PRECOMPUTE_TARGET) $(DB_FILE)

.PHONY: all clean
