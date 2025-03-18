#include <iostream>
#include <vector>
#include <queue>
#include <stack>
#include <omp.h>
#include <chrono>

using namespace std;
using namespace std::chrono;

class Graph {
public:
    int V; // Number of vertices
    vector<vector<int>> adj; // Adjacency list

    Graph(int V) {
        this->V = V;
        adj.resize(V);
    }

    void addEdge(int u, int v) {
        adj[u].push_back(v);
        adj[v].push_back(u); // Undirected graph
    }

    void parallelBFS(int start) {
        vector<bool> visited(V, false);
        queue<int> q;
        q.push(start);
        visited[start] = true;

        cout << "Parallel BFS Traversal: ";
        auto start_time = high_resolution_clock::now();

        while (!q.empty()) {
            int size = q.size();
            vector<int> nextLevel;

            #pragma omp parallel for
            for (int i = 0; i < size; i++) {
                int node;
                #pragma omp critical
                {
                    node = q.front();
                    q.pop();
                }
                cout << node << " ";

                for (int neighbor : adj[node]) {
                    if (!visited[neighbor]) {
                        visited[neighbor] = true;
                        nextLevel.push_back(neighbor);
                    }
                }
            }

            for (int n : nextLevel) {
                q.push(n);
            }
        }
        auto end_time = high_resolution_clock::now();
        cout << "\nExecution Time: " << duration_cast<microseconds>(end_time - start_time).count() << " microseconds\n";
    }

    void parallelDFS(int start) {
        vector<bool> visited(V, false);
        stack<int> s;
        s.push(start);
        visited[start] = true;

        cout << "Parallel DFS Traversal: ";
        auto start_time = high_resolution_clock::now();

        while (!s.empty()) {
            int node;
            #pragma omp critical
            {
                node = s.top();
                s.pop();
            }
            cout << node << " ";

            vector<int> neighbors;
            for (int neighbor : adj[node]) {
                if (!visited[neighbor]) {
                    visited[neighbor] = true;
                    neighbors.push_back(neighbor);
                }
            }

            #pragma omp parallel for
            for (int i = neighbors.size() - 1; i >= 0; i--) {
                s.push(neighbors[i]);
            }
        }
        auto end_time = high_resolution_clock::now();
        cout << "\nExecution Time: " << duration_cast<microseconds>(end_time - start_time).count() << " microseconds\n";
    }
};

int main() {
    Graph g(7);
    g.addEdge(0, 1);
    g.addEdge(0, 2);
    g.addEdge(1, 3);
    g.addEdge(1, 4);
    g.addEdge(2, 5);
    g.addEdge(2, 6);

    g.parallelBFS(0);
    g.parallelDFS(0);
    
    return 0;
}
