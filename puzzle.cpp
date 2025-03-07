#include "puzzle.h"
#include <algorithm>
#include <vector>
#include <climits> 
#include <unordered_set>
#include <iostream>

// Verifica si el estado actual es la meta
bool is_goal(std::vector<std::vector<int>> state) {
    size_t n = state.size();
    int count = 1;
    for (size_t i = 0; i < n; i++) {
        for (size_t j = 0; j < n; j++) {
            if (i == n - 1 && j == n - 1) {
                if (state[i][j] != 0) return false;
            } else {
                if (state[i][j] != count) return false;
                count++;
            }
        }
    }
    return true;
}

// Genera todos los posibles estados sucesores a partir de un estado dado del tablero
std::vector<std::vector<std::vector<int>>> get_successors(std::vector<std::vector<int>> state) {
    std::vector<std::vector<std::vector<int>>> successors;
    size_t n = state.size();
    size_t x = n, y = n; // Inicializar con un valor inválido (n)

    // Encontrar la posición del espacio vacío (0)
    for (size_t i = 0; i < n; i++) {
        for (size_t j = 0; j < n; j++) {
            if (state[i][j] == 0) {
                x = i;
                y = j;
                break;
            }
        }
        if (x != n) break; // Usar n como valor inválido
    }

    // Movimientos posibles: arriba, abajo, izquierda, derecha
    int dx[] = {-1, 1, 0, 0};
    int dy[] = {0, 0, -1, 1};

    for (int i = 0; i < 4; i++) {
        int nx = x + dx[i];
        int ny = y + dy[i];

        if (nx >= 0 && nx < static_cast<int>(n) && ny >= 0 && ny < static_cast<int>(n)) {
            std::vector<std::vector<int>> new_state = state;
            std::swap(new_state[x][y], new_state[nx][ny]);
            successors.push_back(new_state);
        }
    }

    return successors;
}

// Función para contar el número de inversiones en una permutación
int count_inversions(const std::vector<std::vector<int>>& state) {
    size_t n = state.size();
    std::vector<int> flattened;
    for (size_t i = 0; i < n; i++) {
        for (size_t j = 0; j < n; j++) {
            if (state[i][j] != 0) {
                flattened.push_back(state[i][j]);
            }
        }
    }

    int inversions = 0;
    for (size_t i = 0; i < flattened.size(); i++) {
        for (size_t j = i + 1; j < flattened.size(); j++) {
            if (flattened[i] > flattened[j]) {
                inversions++;
            }
        }
    }
    return inversions;
}

// Función para determinar la paridad de la distancia del espacio vacío
int get_blank_parity(const std::vector<std::vector<int>>& state) {
    size_t n = state.size();
    size_t blank_row = n; // Inicializar con un valor inválido (n)

    // Encontrar la fila del espacio vacío (0)
    for (size_t i = 0; i < n; i++) {
        for (size_t j = 0; j < n; j++) {
            if (state[i][j] == 0) {
                blank_row = i;
                break;
            }
        }
        if (blank_row != n) break; // Usar n como valor inválido
    }

    // La paridad es la distancia desde la fila inferior (n-1)
    return (n - 1 - blank_row) % 2;
}

// Función para verificar si el estado tiene solución
bool has_solution(const std::vector<std::vector<int>>& state) {
    int inversions = count_inversions(state);
    int blank_parity = get_blank_parity(state);

    // El estado tiene solución si las paridades coinciden
    return (inversions % 2) == blank_parity;
}

// Estructura para comparar estados en el hash set
struct StateHash {
    size_t operator()(const std::vector<std::vector<int>>& state) const {
        size_t hash = 0;
        for (const auto& row : state) {
            for (int val : row) {
                hash = hash * 31 + val;
            }
        }
        return hash;
    }
};

// Algoritmo DFS_CONTOUR
std::pair<Node*, int> DFS_CONTOUR(Node* node, int f_limit, int& states_generated, std::unordered_set<std::vector<std::vector<int>>, StateHash>& visited) {
    if (node->f > f_limit) {
        return {nullptr, node->f};
    }
    if (is_goal(node->state)) {
        return {node, f_limit};
    }

    visited.insert(node->state);
    int next_f = INT_MAX;
    std::vector<Node*> successors;

    for (auto& successor_state : get_successors(node->state)) {
        if (visited.count(successor_state)) continue;
        if (node->parent != nullptr && successor_state == node->parent->state) continue;

        states_generated++; // Contar el estado generado
        int h_cost = HH(successor_state);
        Node* successor_node = new Node(successor_state, node->g + 1, h_cost, node);
        successors.push_back(successor_node);
    }

    std::sort(successors.begin(), successors.end(), [](Node* a, Node* b) {
        return a->f < b->f;
    });

    for (Node* successor : successors) {
        auto result = DFS_CONTOUR(successor, f_limit, states_generated, visited);
        Node* solution = result.first;
        int new_f = result.second;

        if (solution != nullptr) {
            return {solution, f_limit};
        }
        next_f = std::min(next_f, new_f);
    }

    visited.erase(node->state);
    return {nullptr, next_f};
}

// Algoritmo IDA*
std::pair<std::vector<std::vector<std::vector<int>>>, int> IDA_star(std::vector<std::vector<int>> initial_state, int& states_generated) {
    
    // Verificar si el estado inicial tiene solución
    if (!has_solution(initial_state)) {
        return {{}, 0}; // No hay solución
    }

    int f_limit = HH(initial_state);
    Node* root = new Node(initial_state, 0, f_limit);

    while (true) {
        std::unordered_set<std::vector<std::vector<int>>, StateHash> visited;
        auto result = DFS_CONTOUR(root, f_limit, states_generated, visited);
        Node* solution = result.first;
        int new_f_limit = result.second;

        // Imprimir el nuevo límite y los estados generados
        std::cout << "Nuevo límite: " << f_limit << " | Estados generados: " << states_generated << std::endl;

        if (solution != nullptr) {
            // Reconstruir el camino desde la solución hasta la raíz
            std::vector<std::vector<std::vector<int>>> path;
            Node* current = solution;
            while (current != nullptr) {
                path.push_back(current->state);
                current = current->parent;
            }
            std::reverse(path.begin(), path.end()); // Invertir para obtener el camino desde el inicio hasta la meta
            return {path, solution->g}; // Devolver el camino y la longitud del camino
        }
        if (new_f_limit == INT_MAX) {
            return {{}, 0}; // No hay solución
        }
        f_limit = new_f_limit;
    }
}