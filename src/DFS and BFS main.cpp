#include <iostream>
#include <vector>
#include <queue>

using namespace std;

class ListGraph {
private:
    int n;
    vector<vector<int>> adj;

public:
    ListGraph(int n) {
        this->n = n;
        adj.resize(n);
    }

    void InsertEdge(int u, int v) {
        adj[u].push_back(v);
        adj[v].push_back(u); // undirected
    }

    void DFSUtil(int u, vector<bool>& visited) {
        visited[u] = true;
        cout << u << " ";
        for (int v : adj[u]) {
            if (!visited[v]) DFSUtil(v, visited);
        }
    }

    void DFS(int start) {
        vector<bool> visited(n, false);
        DFSUtil(start, visited);
    }

    void BFS(int start) {
        vector<bool> visited(n, false);
        queue<int> q;

        visited[start] = true;
        q.push(start);

        while (!q.empty()) {
            int u = q.front(); q.pop();
            cout << u << " ";

            for (int v : adj[u]) {
                if (!visited[v]) {
                    visited[v] = true;
                    q.push(v);
                }
            }
        }
    }
};

int main() {
    ListGraph g(5);

    g.InsertEdge(0, 1);
    g.InsertEdge(0, 2);
    g.InsertEdge(1, 3);
    g.InsertEdge(2, 4);

    cout << "DFS: ";
    g.DFS(0);
    cout << "\n";

    cout << "BFS: ";
    g.BFS(0);
    cout << "\n";
}
