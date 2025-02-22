#include <iostream>
#include "puzzle.h"

int main() {
    // Ejemplo de tablero inicial
    std::vector<std::vector<int>> initial_state = {
        {1, 2, 3, 4},
        {5, 6, 7, 8},
        {9, 10, 11, 12},
        {13, 14, 0, 15}
    };

    int states_generated = 0; // Contador de estados generados

    // Resolver el 15-Puzzle con IDA*
    auto result = IDA_star(initial_state, states_generated);
    std::vector<std::vector<std::vector<int>>> path = result.first;
    int path_length = result.second;

    // Mostrar la solución
    if (path.empty()) {
        std::cout << "No hay solucion" << std::endl;
    } else {
        // (1) Secuencia de tableros
        std::cout << "Secuencia de tableros:" << std::endl;
        for (auto state : path) {
            for (auto row : state) {
                for (auto val : row) {
                    std::cout << val << " ";
                }
                std::cout << std::endl;
            }
            std::cout << std::endl;
        }

        // (2) Longitud del camino
        std::cout << "Longitud del camino: " << path_length << std::endl;

        // (3) Número de estados generados
        std::cout << "Estados generados: " << states_generated << std::endl;
    }

    return 0;
}