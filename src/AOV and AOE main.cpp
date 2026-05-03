#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>

using namespace std;

class ListGraph {
private:
    int n;
    vector<vector<pair<int, int>>> adj;

public:
    ListGraph(int n) {
        this->n = n;
        adj.resize(n);
    }

    void InsertEdge(int u, int v, int w) {
        adj[u].push_back({ v, w });
    }

    // ---------- AOV ----------
    void AOV() {
        vector<int> indegree(n, 0);

        for (int u = 0; u < n; u++)
            for (auto& p : adj[u])
                indegree[p.first]++;

        queue<int> q;
        for (int i = 0; i < n; i++)
            if (indegree[i] == 0) q.push(i);

        cout << "AOV: ";
        while (!q.empty()) {
            int u = q.front(); q.pop();
            cout << u << " ";

            for (auto& p : adj[u])
                if (--indegree[p.first] == 0)
                    q.push(p.first);
        }
        cout << "\n";
    }

    // ---------- AOE ----------
    void AOE() {
        vector<int> indegree(n, 0);

        for (int u = 0; u < n; u++)
            for (auto& p : adj[u])
                indegree[p.first]++;

        queue<int> q;
        vector<int> topo;

        for (int i = 0; i < n; i++)
            if (indegree[i] == 0) q.push(i);

        while (!q.empty()) {
            int u = q.front(); q.pop();
            topo.push_back(u);

            for (auto& p : adj[u])
                if (--indegree[p.first] == 0)
                    q.push(p.first);
        }

        vector<int> ET(n, 0);

        for (int u : topo)
            for (auto& p : adj[u])
                ET[p.first] = max(ET[p.first], ET[u] + p.second);

        vector<int> LT(n, ET[n - 1]);
        for (int i = 0; i < n; i++) LT[i] = ET[n - 1];

        for (int i = topo.size() - 1; i >= 0; i--) {
            int u = topo[i];
            for (auto& p : adj[u])
                LT[u] = min(LT[u], LT[p.first] - p.second);
        }
        cout << "AOE : ";
        cout << "Critical Path:\n";
        for (int u = 0; u < n; u++) {
            for (auto& p : adj[u]) {
                if (ET[u] == LT[p.first] - p.second)
                    cout << u << " -> " << p.first << "\n";
            }
        }
    }
};

int main() {
    ListGraph g(6);

    g.InsertEdge(0, 1, 3);
    g.InsertEdge(0, 2, 2);
    g.InsertEdge(1, 3, 4);
    g.InsertEdge(2, 3, 1);
    g.InsertEdge(3, 4, 2);
    g.InsertEdge(3, 5, 3);

    g.AOV();
    g.AOE();
}
