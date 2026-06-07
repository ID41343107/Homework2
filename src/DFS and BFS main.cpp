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

    vector<int> dfn;
    vector<int> low;
    int dfnCounter;
    vector<bool> isArticulation;

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

    // Spanning Tree
    void SpanningTreeUtil(int u) {
        visited[u] = true;
        for (int v : adj[u]) {
            if (!visited[v]) {
                cout << "(" << u << ", " << v << ") ";
                SpanningTreeUtil(v);
            }
        }
    }

    // 4. Biconnected
    void BiconnectedUtil(int u, int parent) {
        dfn[u] = low[u] = ++dfnCounter;
        int children = 0;

        for (int v : adj[u]) {
            if (v == parent) continue;

            if (dfn[v] > 0) {
                low[u] = min(low[u], dfn[v]);
            } else {
                children++;
                BiconnectedUtil(v, u);
                low[u] = min(low[u], low[v]);
                if (parent != -1 && low[v] >= dfn[u]) {
                    isArticulation[u] = true;
                }
            }
        }
        
        if (parent == -1 && children > 1) {
            isArticulation[u] = true;
        }
    }

public:
    ListGraph(int n) {
        this->n = n;
        adj.resize(n);
        visited.resize(n, false);
    }

    void InsertEdge(int u, int v) {
        if (u >= 0 && u < n && v >= 0 && v < n) {
            adj[u].push_back(v);
            adj[v].push_back(u);
        }
    }
    
    void DFS() {
        fill(visited.begin(), visited.end(), false);
        for (int i = 0; i < n; i++) {
            if (!visited[i]) {
                DFSUtil(i);
            }
        }
    }

    void BFS() {
        fill(visited.begin(), visited.end(), false);
        for (int i = 0; i < n; i++) {
            if (!visited[i]) {
                BFSUtil(i);
            }
        }
    }

    // 計算連通元件
    void ConnectedComponents() {
        fill(visited.begin(), visited.end(), false);
        int count = 0;
        
        for (int i = 0; i < n; i++) {
            if (!visited[i]) {
                count++;
                cout << "元件 " << count << ": ";
                DFSUtil(i);
                cout << "\n";
            }
        }
        cout << "圖中共有 " << count << " 個連通元件。\n";
    }

    // 產生生成樹的邊
    void SpanningTree() {
        fill(visited.begin(), visited.end(), false);
        cout << "生成樹的邊有: ";
        for (int i = 0; i < n; i++) {
            if (!visited[i]) {
                SpanningTreeUtil(i);
            }
        }
        cout << "\n";
    }

    // 尋找關節點
    void FindArticulationPoints() {
        dfn.assign(n, 0);
        low.assign(n, 0);
        isArticulation.assign(n, false);
        dfnCounter = 0;

        for (int i = 0; i < n; i++) {
            if (dfn[i] == 0) {
                BiconnectedUtil(i, -1);
            }
        }

        cout << "圖中的關節點有: ";
        bool found = false;
        for (int i = 0; i < n; i++) {
            if (isArticulation[i]) {
                cout << i << " ";
                found = true;
            }
        }
        if (!found) cout << "無";
        cout << "\n";
    }
};

int main() {
    ListGraph g(6);

    g.InsertEdge(0, 1);
    g.InsertEdge(0, 2);
    g.InsertEdge(1, 3);
    g.InsertEdge(2, 3);
    g.InsertEdge(3, 4);

    cout << "DFS 走訪: ";
    g.DFS();
    cout << "\n";

    cout << "BFS 走訪: ";
    g.BFS();
    cout << "\n\n";

    g.ConnectedComponents();
    cout << "\n";

    g.SpanningTree();
    cout << "\n";

    g.FindArticulationPoints();

    return 0;
}
