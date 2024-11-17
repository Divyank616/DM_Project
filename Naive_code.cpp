#include <iostream>
#include <vector>
#include <algorithm>
#include <stack>
using namespace std;

// Check if the graph is connected using DFS
bool isConnected(const vector<vector<int>>& graph, int V) {
    vector<bool> visited(V, false);
    stack<int> s;
    
    // Start DFS from vertex 0
    s.push(0);
    visited[0] = true;
    
    while (!s.empty()) {
        int v = s.top();
        s.pop();
        
        for (int i = 0; i < V; i++) {
            if (graph[v][i] == 1 && !visited[i]) {
                visited[i] = true;
                s.push(i);
            }
        }
    }
    
    // Check if all vertices are visited
    for (int i = 0; i < V; i++) {
        if (!visited[i]) return false;
    }
    return true;
}

// Check if the graph has an Eulerian Path or Circuit
bool isEulerian(const vector<vector<int>>& graph, int V) {
    if (!isConnected(graph, V)) return false; // The graph must be connected
    
    int oddDegreeCount = 0;
    
    for (int i = 0; i < V; i++) {
        int degree = 0;
        for (int j = 0; j < V; j++) {
            if (graph[i][j] == 1) degree++;
        }
        if (degree % 2 != 0) oddDegreeCount++;
    }
    
    // Eulerian Circuit: All vertices must have even degree
    // Eulerian Path: Exactly 0 or 2 vertices must have odd degree
    return (oddDegreeCount == 0 || oddDegreeCount == 2);
}

// Utility function to check if there is a Hamiltonian Path in the graph
bool isHamiltonianPath(const vector<vector<int>>& graph, vector<int>& path, int V, int pos) {
    if (pos == V) return true; // All vertices are visited
    
    for (int v = 0; v < V; v++) {
        bool canVisit = true;
        
        // Check if v is adjacent to the last vertex in the path and not yet visited
        if (graph[path[pos-1]][v] == 1) {
            for (int i = 0; i < pos; i++) {
                if (path[i] == v) {
                    canVisit = false;
                    break;
                }
            }
            if (canVisit) {
                path[pos] = v;
                if (isHamiltonianPath(graph, path, V, pos + 1)) return true;
                path[pos] = -1; // Backtrack
            }
        }
    }
    
    return false;
}

// Check if the graph has a Hamiltonian Cycle
bool isHamiltonianCycle(const vector<vector<int>>& graph, vector<int>& path, int V) {
    // Start from the first vertex (0)
    path[0] = 0;
    
    if (isHamiltonianPath(graph, path, V, 1)) {
        // Check if the last vertex is connected to the first vertex
        if (graph[path[V-1]][path[0]] == 1) return true;
    }
    return false;
}

// Main function
int main() {
    int V = 5;  // Number of vertices
    vector<vector<int>> graph = {
        {0, 1, 1, 1, 0},
        {1, 0, 1, 0, 1},
        {1, 1, 0, 1, 1},
        {1, 0, 1, 0, 1},
        {0, 1, 1, 1, 0}
    };
    
    if (isEulerian(graph, V)) {
        cout << "The graph has an Eulerian path or circuit.\n";
    } else {
        cout << "The graph does not have an Eulerian path or circuit.\n";
    }
    
    vector<int> path(V, -1);
    if (isHamiltonianCycle(graph, path, V)) {
        cout << "The graph has a Hamiltonian cycle.\n";
    } else if (isHamiltonianPath(graph, path, V, 1)) {
        cout << "The graph has a Hamiltonian path.\n";
    } else {
        cout << "The graph does not have a Hamiltonian path or cycle.\n";
    }
    
    return 0;
}