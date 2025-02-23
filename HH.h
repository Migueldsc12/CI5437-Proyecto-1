#ifndef HH_H
#define HH_H

#include <map>
#include <vector>
#include <string>

using namespace std;

extern map<string, int> db;

string matrixToString(const vector<vector<int>>& matrix);
void loadDatabase(const string& filename);
int findStateInFile(const vector<vector<int>>& targetMatrix);
int walking_distance(const vector<vector<int>>& node);
int HH(const vector<vector<int>>& node);

#endif // HH_H
