#include <iostream>
#include <vector>
#include <algorithm>
#include <map>
#include <numeric>
#include <fstream>
#include<sstream>

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

vector<vector<int>> associatedMatrix(vector<vector<int>> state, int direction){
    vector<vector<int>> map(state.size(), vector<int>(state.size(), 0));
    int size = state.size();
        for(int i = 0; i < size ; i++){
            for(int j = 0 ; j < size ; j++){
                if(state[i][j]!= 0 && direction == 0)  {
                    int n = (state[i][j] - 1) /size;
                    map[i][n]++;
                }else if(state[i][j]!= 0 && direction == 1){
                    int n = (state[i][j] - 1) % size;
                    map[n][j]++;
                }
        }
    }
    return map;
};

// Finds the traspose of a matrix to get the vertical walking distance
vector<vector<int>> traspose(vector<vector<int>> node){
    vector<vector<int>> traspose;
    int size = node.size();
    for(int i = 0; i < size; i++){
        vector<int> row;
        for(int j = 0; j < size; j++){
            row.push_back(node[j][i]);
        }
        traspose.push_back(row);
    }
    return traspose;
};

// Converts a matrix to a string for comparison with the database
string matrixToString(const vector<vector<int>>& matrix) {
    string matrix_str;
    for (const auto& row : matrix) {
        for (int val : row) {
            matrix_str += to_string(val) + " ";
        }
        matrix_str += ";";
    }
    matrix_str.pop_back();
    return matrix_str;
}

// Finds the state in the database
int findStateInFile(const vector<vector<int>>& targetMatrix, const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Cant open the file" << filename << endl;
        return -1;
    }

    string targetMatrixStr = matrixToString(targetMatrix);

    string line;
    while (getline(file, line)) {
        size_t pos = line.find(':');
        if (pos != string::npos) {
            string matrixPart = line.substr(0, pos); //Extract the matrix
            string distancePart = line.substr(pos + 1); // Extract the distance

            if (matrixPart == targetMatrixStr) {
                return stoi(distancePart); // Convert the distance to int
            }
        }
    }

    file.close();
    return -1; // Return -1 if the state isn't found
}

// ------ Heuristics -------

// Walking distance
int walking_distance(vector<vector<int>> node){
    vector<vector<int>> horizontal = associatedMatrix(node,0);
    vector<vector<int>> vertical = associatedMatrix(node,1);
    vertical = traspose(vertical);

    int horizontalWD= findStateInFile(horizontal, "dbStates.txt");
    int verticalWD = findStateInFile(vertical, "dbStates.txt");

    return horizontalWD + verticalWD;

}

// Manhattan distance
int manhattanDistance(vector<vector<int>> node){
    int distance = 0;
    int size = node.size();
    for(int i = 0; i < size ; i++){
        for(int j = 0; j < size ; j++){
            if(node[i][j] != 0){
                int val = node[i][j];
                int goalX = (val - 1) / size;
                int goalY = (val - 1) % size;
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
int HH(vector<vector<int>> node){
    return walking_distance(node) + (manhattanDistance(node) /3)+ linearConflict(node);
}
