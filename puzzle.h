#ifndef PUZZLE_H
#define PUZZLE_H

#include <vector>
#include <climits>
#include "node.h"
#include "HH.h"

bool is_goal(std::vector<std::vector<int>> state);
std::vector<std::vector<std::vector<int>>> get_successors(std::vector<std::vector<int>> state);
std::pair<Node*, int> DFS_CONTOUR(Node* node, int f_limit, int& states_generated);
std::pair<std::vector<std::vector<std::vector<int>>>, int> IDA_star(std::vector<std::vector<int>> initial_state, int& states_generated);

#endif // PUZZLE_H