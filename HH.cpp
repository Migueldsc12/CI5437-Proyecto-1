#include <iostream>
#include <vector>
#include <algorithm>
#include <map>
#include <numeric>
#include <fstream>
#include<sstream>
#include <unordered_map>

using namespace std;

// ------ Helper functions -------

// Converts the puzzle matrix into a new matrix that represents the number of elements
// that are part of the goal state in each row or column of the puzzle.
// If the direction is 0, the conversion is horizontal; otherwise, it is vertical.
//
// For example: If the direction is horizontal and the first row of the puzzle is
// [1, 2, 3, 6] the first row of the new matrix will be [3, 1, 0, 0]
// because the first row has 3 elements that are part of the first row of the goal state
// and 1 element that is part of the second row of the goal state. If the second row is
// [4, 8, 0, 7], the second row of the new matrix will be [1, 2, 0, 0],
// because there is 1 element that is part of the first row of the goal state and 2 elements that are
// part of the second one. The zero isn't counted.

vector<vector<int>> associatedMatrix(const vector<vector<int>>& state, int direction) {
    int size = state.size();
    vector<vector<int>> map(size, vector<int>(size, 0));
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            if (state[i][j] != 0) {
                int n = (direction == 0) ? (state[i][j] - 1) / size : (state[i][j] - 1) % size;
                map[i][n]++;
            }
        }
    }
    return map;
}

// Finds the traspose of a matrix to get the vertical walking distance
vector<vector<int>> traspose(const vector<vector<int>>& node) {
    int size = node.size();
    vector<vector<int>> traspose(size, vector<int>(size));
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            traspose[j][i] = node[i][j];
        }
    }
    return traspose;
}

// Converts a matrix to a string for comparison with the database
string matrixToString(const vector<vector<int>>& matrix) {
    stringstream ss;
    for (const auto& row : matrix) {
        for (int val : row) {
            ss << val << " ";
        }
        ss << ";";
    }
    string result = ss.str();
    result.pop_back();
    return result;
}

// Finds the state in the database

unordered_map<string, int> loadDatabase(const string& filename) {
    ifstream file(filename);
    unordered_map<string, int> db;

    if (!file.is_open()) {
        cerr << "Can't open the file " << filename << endl;
        return db;
    }

    string line;
    while (getline(file, line)) {
        size_t pos = line.find(':');
        if (pos != string::npos) {
            string matrixPart = line.substr(0, pos);
            int distance = stoi(line.substr(pos + 1));
            db[matrixPart] = distance;
        }
    }

    file.close();
    return db;
}

int findStateInFile(const vector<vector<int>>& targetMatrix, const unordered_map<string, int>& db) {
    string targetMatrixStr = matrixToString(targetMatrix);
    auto it = db.find(targetMatrixStr);
    return (it != db.end()) ? it->second : -1;

}
// ------ Heuristics -------

// Walking distance
int walking_distance(const vector<vector<int>>& node, const unordered_map<string, int>& db) {
    vector<vector<int>> horizontal = associatedMatrix(node, 0);
    vector<vector<int>> vertical = associatedMatrix(node, 1);
    vertical = traspose(vertical);

    int horizontalWD = findStateInFile(horizontal, db);
    int verticalWD = findStateInFile(vertical, db);

    return horizontalWD + verticalWD;
}

// Manhattan distance
int manhattanDistance(vector<vector<int>> node){
    int distance = 0; 
    for(int i = 0; i < static_cast<int>(node.size()); i++){
        for(int j = 0; j < static_cast<int>(node[i].size()); j++){
            if(node[i][j] != 0){
                int val = node[i][j];
                int goalX = (val - 1) / node.size();
                int goalY = (val - 1) % node.size();
                distance += abs(i-goalX) + abs(j-goalY);

                    }
            };
        };
    return distance;
}

// Linear Conflict
int linearConflict(const vector<vector<int>>& puzzle) {
    int conflict = 0;
    int N = puzzle.size();

    // Horizontal conflict
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N - 1; ++j) {
            for (int k = j + 1; k < N; ++k) {
                int pj = puzzle[i][j];
                int pk = puzzle[i][k];
                if (pj != 0 && pk != 0 && (pj - 1) / N == i && (pk - 1) / N == i && pj > pk) {
                    conflict += 2;
                }
            }
        }
    }
    // Vertical Conflict
    for (int j = 0; j < N; ++j) {
        for (int i = 0; i < N - 1; ++i) {
            for (int k = i + 1; k < N; ++k) {
                int pi = puzzle[i][j];
                int pk = puzzle[k][j];
                if (pi != 0 && pk != 0 && pi % N == j + 1 && pk % N == j + 1 && pi > pk) {
                    conflict += 2;
                }
            }
        }
    }

    return conflict;
}

// Hybrid heuristic (HH)
int HH(const vector<vector<int>>& node) {
    string filename = "dbStates.txt";
    unordered_map<string, int> db = loadDatabase(filename);

    int distance = walking_distance(node, db);
    int manhattan = manhattanDistance(node) / 3;
    int linear = linearConflict(node);

    return distance + manhattan + linear;
}
