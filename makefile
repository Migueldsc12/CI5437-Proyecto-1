CXX = g++
CXXFLAGS = -std=c++11 -Wall -Wextra -O2
SRCS = main.cpp puzzle.cpp HH.cpp
TARGET = puzzle_solver
PRECOMPUTE_TARGET = precompute
DB_FILE = dbStates.txt

ifeq ($(OS),Windows_NT)
    RM = del
    CHECK_FILE_EXISTS = if not exist
else
    RM = rm -f
    CHECK_FILE_EXISTS = test -f
endif

all: $(DB_FILE) $(TARGET)

$(TARGET): $(SRCS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) main.cpp puzzle.cpp HH.cpp

$(DB_FILE): precompute_states.cpp
	@$(CHECK_FILE_EXISTS) $(DB_FILE) && echo "El archivo $(DB_FILE) ya existe. No se requiere crear nuevamente." || ( \
		echo "El archivo $(DB_FILE) no existe. Ejecutando precompute_states.cpp..."; \
		$(CXX) $(CXXFLAGS) -o $(PRECOMPUTE_TARGET) precompute_states.cpp; \
		./$(PRECOMPUTE_TARGET); \
		echo "Archivo $(DB_FILE) creado exitosamente." \
	)

clean:
	$(RM) $(TARGET) $(PRECOMPUTE_TARGET) $(DB_FILE)

.PHONY: all clean
