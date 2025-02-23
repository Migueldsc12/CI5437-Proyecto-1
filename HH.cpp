#include <iostream>
#include <vector>
#include <algorithm>
#include <map>
#include <numeric>
#include <fstream>
#include<sstream>

using namespace std;


vector<vector<int>> convertMap(vector<vector<int>> node, int direction){
    vector<vector<int>> map(node.size(), vector<int>(node.size(), 0));
    int size = node.size();
    if (direction == 0){
        for(int i = 0; i < size ; i++){
            for(int j = 0 ; j < size ; j++){
                if(node[i][j]!=0){
                    int n = (node[i][j] - 1) /size;
                    map[i][n]++;
                }
            }
        }
    }else if (direction==1){
        for(int i = 0; i < size ; i++){
            for(int j = 0 ; j < size ; j++){
                if(node[i][j]!=0){
                    int n = (node[i][j] - 1) % size;
                    map[n][j]++;
                }
            }
        }
    }
    
    return map;
};

// Find the traspose of a matrix to get the vertical walking distance
vector<vector<int>> traspose(vector<vector<int>> node){
    vector<vector<int>> traspose;
    for(int i = 0; i < node.size(); i++){
        vector<int> row;
        for(int j = 0; j < node.size(); j++){
            row.push_back(node[j][i]);
        }
        traspose.push_back(row);
    }
    return traspose;
};

// Convert a matrix to a string to compare it with the database
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

// Find the state in the database
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
    vector<vector<int>> horizontal = convertMap(node,0);
    vector<vector<int>> vertical = convertMap(node,1);
    vertical = traspose(vertical);
    int wd = 0;

    int horizontalWD= findStateInFile(horizontal, "dbStates.txt");
    int verticalWD = findStateInFile(vertical, "dbStates.txt");

    return horizontalWD + verticalWD;

}

// Manhattan distance and linear conflict
int md_lc(vector<vector<int>> node){
    int distance = 0; 
    int conflict = 0;
    for(int i = 0; i < node.size(); i++){
        for(int j = 0; j < node[i].size(); j++){
            if(node[i][j] != 0){
                int val = node[i][j];
                int goalX = (val - 1) / node.size();
                int goalY = (val - 1) % node.size();
                distance += abs(i-goalX) + abs(j-goalY); // Manhattan distance

                // Linear Conflict
                // Check if the tile is in the same row as its goal position
                if (i == goalX){
                    for(int k = j+1; k < node.size(); k++){
                        if(node[i][k] != 0){
                            int val2 = node[i][k];
                            int goalY2 = (val2 - 1) % node.size();
                            if(goalY > goalY2){
                                conflict++;
                                
                            };
                        };
                    };
                // Check if the tile is in the same column as its goal position
                } if(j == goalY){
                    for(int k = i+1; k < node.size(); k++){
                        if(node[k][j] != 0){
                            int val2 = node[k][j];
                            int goalX2 = (val2 - 1) / node.size();
                            if(goalX > goalX2){
                                conflict++;
                            };
                        };
                    };
                };
            };
        };
    };
    return distance/3 + conflict; // md/3 + lc
}

int HH(vector<vector<int>> node){
    return walking_distance(node) + md_lc(node);
}