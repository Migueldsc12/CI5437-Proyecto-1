#ifndef NODE_H
#define NODE_H

#include <vector>

struct Node {
    std::vector<std::vector<int>> state; // Estado del tablero
    int g; // Costo acumulado (g)
    int h; // Heurística (h)
    int f; // f = g + h
    Node* parent; // Puntero al nodo padre

    Node(std::vector<std::vector<int>> s, int g_cost, int h_cost, Node* p = nullptr)
        : state(s), g(g_cost), h(h_cost), f(g_cost + h_cost), parent(p) {}
};

#endif