#include <iostream>
#include <vector>
#include "puzzle.h"

int main() {
    std::vector<std::vector<int>> initial_state(4, std::vector<int>(4)); // Matriz 4x4
    std::vector<int> numbers;

    std::cout << "Ingrese los 16 números del tablero (separados por espacios): ";

    // Leer los 16 números desde la terminal
    for (int i = 0; i < 16; i++) {
        int num;
        std::cin >> num;
        numbers.push_back(num);
    }

    // Verificar que se ingresaron exactamente 16 números
    if (numbers.size() != 16) {
        std::cerr << "Error: Debe ingresar exactamente 16 números." << std::endl;
        return 1;
    }

    // Convertir el vector de números en una matriz 4x4
    int index = 0;
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            initial_state[i][j] = numbers[index++];
        }
    }

    int states_generated = 0; // Contador de estados generados

    // Resolver el 15-Puzzle con IDA*
    auto result = IDA_star(initial_state, states_generated);
    std::vector<std::vector<std::vector<int>>> path = result.first;
    int path_length = result.second;

    // Mostrar la solución
    if (path.empty()) {
        std::cout << "No hay solucion" << std::endl;
    } else {
        std::cout << "Solucion encontrada" << std::endl;
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