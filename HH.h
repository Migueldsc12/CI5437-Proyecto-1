#ifndef HH_H
#define HH_H

#include <unordered_map>
#include <vector>
#include <string>

using namespace std;

extern unordered_map<string, int> db; // Declarar la base de datos como externa

string matrixToString(const vector<vector<int>>& matrix);
unordered_map<string, int> loadDatabase(const string& filename);
int findStateInFile(const vector<vector<int>>& targetMatrix);
int walking_distance(const vector<vector<int>>& node);
int HH(const vector<vector<int>>& node);

#endif // HH_H
