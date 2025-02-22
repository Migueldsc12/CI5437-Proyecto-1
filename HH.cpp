#include <iostream>
#include "HH.h"
using namespace std;

// Heuristics

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
                            int goalX2 = (val2 - 1) / node.size();
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
                            int goalY2 = (val2 - 1) % node.size();
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
};

// Pa probar
// int main(){
//     vector<vector<int>> node;
//     node = {
//         {1,4,2,3},
//         {13,6,7,8},
//         {5,10,11,0},
//         {9,14,15,12}
//     };
//     cout << md_lc(node) << endl;
// };