#include <iostream>
#include <vector>
#include <queue>
#include <climits>
#include <tuple>
#include <algorithm>

using namespace std;

class Graph {
public:
    virtual ~Graph() {}

    virtual void InsertEdge(int u, int v, int w = 1) = 0;

protected:
    int n = 0;
    int e = 0;
};
class TraversalGraph : public Graph {
private:
    vector<vector<int>> adj;

public:
    TraversalGraph(int n) {
        this->n = n;
        adj.resize(n);
    }

    void InsertEdge(int u, int v, int w = 1) override {
        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    // DFS
    void DFSUtil(int u, vector<bool>& visited) {
        visited[u] = true;
        cout << u << " ";

        for (int v : adj[u]) {
            if (!visited[v]) DFSUtil(v, visited);
        }
    }

    void DFS(int start) {
        vector<bool> visited(n, false);
        cout << "DFS: ";
        DFSUtil(start, visited);
        cout << endl;
    }

    // BFS
    void BFS(int start) {
        vector<bool> visited(n, false);
        queue<int> q;

        visited[start] = true;
        q.push(start);

        cout << "BFS: ";

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
        cout << endl;
    }
};
