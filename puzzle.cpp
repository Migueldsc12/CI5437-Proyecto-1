#include "puzzle.h"
#include <algorithm>

// Verifica si el estado actual es la meta
bool is_goal(std::vector<std::vector<int>> state) {
    int n = state.size();
    int count = 1;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
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
    int n = state.size();
    int x = -1, y = -1;

    // Encontrar la posición del espacio vacío (0)
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (state[i][j] == 0) {
                x = i;
                y = j;
                break;
            }
        }
        if (x != -1) break;
    }

    // Movimientos posibles: arriba, abajo, izquierda, derecha
    int dx[] = {-1, 1, 0, 0};
    int dy[] = {0, 0, -1, 1};

    for (int i = 0; i < 4; i++) {
        int nx = x + dx[i];
        int ny = y + dy[i];

        if (nx >= 0 && nx < n && ny >= 0 && ny < n) {
            std::vector<std::vector<int>> new_state = state;
            std::swap(new_state[x][y], new_state[nx][ny]);
            successors.push_back(new_state);
        }
    }

    return successors;
}

// Algoritmo DFS_CONTOUR
std::pair<Node*, int> DFS_CONTOUR(Node* node, int f_limit, int& states_generated) {
    if (node->f > f_limit) {
        return {nullptr, node->f};
    }
    if (is_goal(node->state)) {
        return {node, f_limit};
    }

    int next_f = INT_MAX;
    for (auto successor_state : get_successors(node->state)) {
        states_generated++; // Contar el estado generado
        int h_cost = md_lc(successor_state);
        Node* successor_node = new Node(successor_state, node->g + 1, h_cost, node);

        auto result = DFS_CONTOUR(successor_node, f_limit, states_generated);
        Node* solution = result.first;
        int new_f = result.second;

        if (solution != nullptr) {
            return {solution, f_limit};
        }
        next_f = std::min(next_f, new_f);
    }

    return {nullptr, next_f};
}

// // Algoritmo IDA*
std::pair<std::vector<std::vector<std::vector<int>>>, int> IDA_star(std::vector<std::vector<int>> initial_state, int& states_generated) {
    int f_limit = md_lc(initial_state);
    Node* root = new Node(initial_state, 0, f_limit);

    while (true) {
        auto result = DFS_CONTOUR(root, f_limit, states_generated);
        Node* solution = result.first;
        int new_f_limit = result.second;

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