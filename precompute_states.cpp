#include <iostream>
#include <vector>
#include <algorithm>
#include <map>
#include <queue>
#include <numeric>
#include <fstream>

using namespace std;

// Structure to represent the state of the game
struct State {
    vector<vector<int>> matrix;
    int depth;
    vector<State> children;
};

// Function to create the children nodes of the current state
void createChildren(State& state) {
    vector<vector<int>> node = state.matrix;
    int size = node.size();
    int index = -1;
    int current_depth = state.depth;

    // Find the row with the sum of elements equal to size - 1
    for (int i = 0; i < size; i++) {
        int rowSum = accumulate(node[i].begin(), node[i].end(), 0);
        if (rowSum == size - 1) {
            index = i;
            break;
        }
    }
    // If index is greater than 0, then we can modify the row above
    if (index > 0) {
        for (int j = 0; j < size; j++) {
            if (node[index - 1][j] > 0) {
                vector<vector<int>> child = node;
                child[index - 1][j]--; // Move the marble tile the row above to the row below
                child[index][j]++;// Move the tile from the row below to the row above

                State child_state;
                child_state.matrix = child;
                child_state.depth = current_depth + 1;
                state.children.push_back(child_state); // Add the child to the children vector
            }
        }
    }
    // If index is less than size - 1, then we can modify the row below
    if (index < size - 1) {
        for (int j = 0; j < size; j++) {
            if (node[index + 1][j] > 0) {
                vector<vector<int>> child = node;
                child[index + 1][j]--; // Move the marble tile the row below to the row above
                child[index][j]++; // Move the tile from the row above to the row below
                
                State child_state;
                child_state.matrix = child;
                child_state.depth = current_depth + 1;
                state.children.push_back(child_state); // Add the child to the children vector
            }
        }
    }
}

// Function to save the state to a file with the distance from the goal state
// The format of the file is:
// 4 0 0 0;0 4 0 0;0 0 4 0;0 0 0 3:distance

void save_state_to_file(const vector<vector<int>>& state, int distance, ofstream& file) {
    for (const auto& row : state) {
        for (int val : row) {
            file << val << " ";
        }
        file << ";";
    }
    file.seekp(-1, ios_base::cur);
    file << ":";
    file << distance << endl;
}

// Function to perform the BFS search to find all the possible states, using the
// goal state as the initial state
int bfs(){
    vector<vector<int>> goal = {{4,0,0,0},
                                {0,4,0,0},
                                {0,0,4,0},
                                {0,0,0,3}}; // Goal state
    State initial_state;
    initial_state.matrix = goal;
    initial_state.depth = 0;

    map<vector<vector<int>>, int> visited;
    queue<State> q;

    q.push(initial_state);
    visited[goal] = 0;

    // Save the goal state to the file
    ofstream file("dbStates.txt");
    if (!file.is_open()) {
        cerr << "Can't open the file." << endl;
        return -1;
    }
    save_state_to_file(goal, 0, file);

    // Perform the BFS search
    while(!q.empty()){
        State current = q.front(); // Get the current state
        q.pop(); // Remove the current state from the queue
        createChildren(current); // Create the children of the current state

        for(const auto& child : current.children){
            if(visited.find(child.matrix) == visited.end()){ // If the child has not been visited
                visited[child.matrix] = current.depth + 1; // Mark the child as visited and save the distance from the goal state
                q.push(child); // Add the child to the queue
                save_state_to_file(child.matrix, current.depth + 1, file); // Save the child to the file
            }
        }
    }file.close();
    return 0;
}

int main(){
    bfs();
    return 0;
}
