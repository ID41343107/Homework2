#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
using namespace std;

class ListGraph {
private:
    int n;
    vector<vector<int>> adj;
    vector<bool> visited;

    // DFS
    void DFSUtil(int u) {
        visited[u] = true;
        cout << u << " ";
        for (int v : adj[u]) {
            if (!visited[v]) {
                DFSUtil(v);
            }
        }
    }

    // BFS
    void BFSUtil(int start) {
        queue<int> q;
        visited[start] = true;
        q.push(start);

        while (!q.empty()) {
            int u = q.front(); 
            q.pop();
            cout << u << " ";

            for (int v : adj[u]) {
                if (!visited[v]) {
                    visited[v] = true;
                    q.push(v);
                }
            }
        }
    }

public:
    ListGraph(int n) {
        this->n = n;
        adj.resize(n);
    }

    void InsertEdge(int u, int v) {
        if (u >= 0 && u < n && v >= 0 && v < n) {
            adj[u].push_back(v);
            adj[v].push_back(u);
        }
    }
    
    void DFS() {
        fill(visited.begin(), visited.end(), false);
        visited.resize(n, false);
        
        for (int i = 0; i < n; i++) {
            if (!visited[i]) {
                DFSUtil(i);
            }
        }
    }

    void BFS() {
        fill(visited.begin(), visited.end(), false);
        visited.resize(n, false);

        for (int i = 0; i < n; i++) {
            if (!visited[i]) {
                BFSUtil(i);
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

    cout << "DFS 走訪: ";
    g.DFS();
    cout << "\n";

    cout << "BFS 走訪: ";
    g.BFS();
    cout << "\n";

    return 0;
}
