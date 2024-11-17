#include <iostream>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <queue>
#include <bitset>
using namespace std;

class GraphPathChecker {
private:
    int V;  // Number of vertices
    unordered_map<int, vector<int>> graph;

    // Helper function for DFS traversal
    void dfs(int vertex, unordered_set<int>& visited) {
        visited.insert(vertex);
        for (int neighbor : graph[vertex]) {
            if (visited.find(neighbor) == visited.end()) {
                dfs(neighbor, visited);
            }
        }
    }

    // Check if graph is connected
    bool isConnected() {
        if (graph.empty()) return true;

        unordered_set<int> visited;
        int start_vertex = -1;
        for (const auto& pair : graph) {
            if (!pair.second.empty()) {
                start_vertex = pair.first;
                break;
            }
        }
        if (start_vertex == -1) return true;  // No edges, so it's trivially connected

        dfs(start_vertex, visited);

        for (const auto& pair : graph) {
            if (!pair.second.empty() && visited.find(pair.first) == visited.end()) {
                return false;
            }
        }
        return true;
    }

    // Helper function for Hamilton path using DP and bitmask
    bool hamiltonDP() {
        const int FULL_MASK = (1 << V) - 1;
        vector<vector<bool>> dp(1 << V, vector<bool>(V, false));

        for (int i = 0; i < V; ++i) {
            if (graph.find(i) != graph.end()) {
                dp[1 << i][i] = true;  // Start at each vertex
            }
        }

        for (int mask = 0; mask <= FULL_MASK; ++mask) {
            for (int u = 0; u < V; ++u) {
                if ((mask & (1 << u)) && dp[mask][u]) {
                    for (int v : graph[u]) {
                        if (!(mask & (1 << v))) {
                            dp[mask | (1 << v)][v] = true;
                        }
                    }
                }
            }
        }

        for (int u = 0; u < V; ++u) {
            if (dp[FULL_MASK][u]) {
                return true;
            }
        }
        return false;
    }

public:
    GraphPathChecker(int vertices) : V(vertices) {}

    // Add edge to the graph
    void addEdge(int u, int v) {
        graph[u].push_back(v);
        graph[v].push_back(u);  // For undirected graph
    }

    // Check for Euler path or circuit
    int checkEuler() {
        if (!isConnected()) {
            return 0;
        }

        int odd_degree = 0;
        for (const auto& pair : graph) {
            if (pair.second.size() % 2 != 0) {
                odd_degree++;
            }
        }

        if (odd_degree == 0) {
            return 2;  // Euler Circuit
        } else if (odd_degree == 2) {
            return 1;  // Euler Path
        }
        return 0;  // Neither
    }

    // Check for Hamilton path or circuit
    int checkHamilton() {
        if (hamiltonDP()) {
            for (int u = 0; u < V; ++u) {
                for (int v : graph[u]) {
                    if (u < V && v < V && graph[u].size() > 0 && graph[v].size() > 0) {
                        return 2;  // Hamiltonian Circuit exists
                    }
                }
            }
            return 1;  // Hamiltonian Path exists
        }
        return 0;  // Neither
    }

    // Print the adjacency list representation of graph
    void printGraph() {
        for (const auto& pair : graph) {
            cout << "Vertex " << pair.first << ": ";
            for (int neighbor : pair.second) {
                cout << neighbor << " ";
            }
            cout << endl;
        }
    }
};

// Helper function to print results
void printResult(int result, const string& pathType) {
    if (result == 0) {
        cout << "No " << pathType << " path or circuit exists" << endl;
    } else if (result == 1) {
        cout << pathType << " path exists" << endl;
    } else {
        cout << pathType << " circuit and path exists" << endl;
    }
}

int main() {
    GraphPathChecker g(10);

    // Add edges for testing
    vector<pair<int, int>> edges = {
        {0, 1}, {0, 2}, {0, 3}, {0, 4},
        {1, 2}, {1, 5}, {2, 3}, {2, 6},
        {3, 4}, {3, 7}, {4, 8}, {5, 6},
        {6, 7}, {7, 8}, {8, 9}, {9, 0},
        {1, 9}, {5, 9}, {2, 8}
    };

    for (const auto& edge : edges) {
        g.addEdge(edge.first, edge.second);
    }

    cout << "Graph representation:" << endl;
    g.printGraph();
    cout << endl;

    // Check for Euler path/circuit
    int eulerResult = g.checkEuler();
    printResult(eulerResult, "Euler");

    // Check for Hamilton path/circuit
    int hamiltonResult = g.checkHamilton();
    printResult(hamiltonResult, "Hamilton");

    return 0;
}
