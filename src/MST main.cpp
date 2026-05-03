#include <iostream>
#include <vector>
#include <queue>
#include <tuple>
#include <algorithm>

using namespace std;

class ListGraph {
private:
    int n;
    vector<vector<pair<int,int>>> adj;

public:
    ListGraph(int n) {
        this->n = n;
        adj.resize(n);
    }

    void InsertEdge(int u, int v, int w) {
        adj[u].push_back({v, w});
        adj[v].push_back({u, w});
    }

    // ---------- Prim ----------
    void Prim() {
        vector<int> key(n, 1e9);
        vector<bool> inMST(n, false);
        vector<int> parent(n, -1);

        priority_queue<pair<int,int>, vector<pair<int,int>>, greater<>> pq;

        key[0] = 0;
        pq.push({0, 0});

        while (!pq.empty()) {
            int u = pq.top().second;
            pq.pop();

            inMST[u] = true;

            for (auto &p : adj[u]) {
                int v = p.first;
                int w = p.second;

                if (!inMST[v] && w < key[v]) {
                    key[v] = w;
                    parent[v] = u;
                    pq.push({key[v], v});
                }
            }
        }

        cout << "Prim MST:\n";
        for (int i = 1; i < n; i++)
            cout << parent[i] << " - " << i << "\n";
    }

    // ---------- Kruskal ----------
    vector<int> parent, rnk;

    int find(int x) {
        if (parent[x] != x)
            parent[x] = find(parent[x]);
        return parent[x];
    }

    void unite(int a, int b) {
        a = find(a);
        b = find(b);
        if (a != b) {
            if (rnk[a] < rnk[b]) swap(a, b);
            parent[b] = a;
            if (rnk[a] == rnk[b]) rnk[a]++;
        }
    }

    void Kruskal() {
        vector<tuple<int,int,int>> edges;

        for (int u = 0; u < n; u++)
            for (auto &p : adj[u])
                if (u < p.first)
                    edges.push_back({p.second, u, p.first});

        sort(edges.begin(), edges.end());

        parent.resize(n);
        rnk.assign(n, 0);
        for (int i = 0; i < n; i++) parent[i] = i;

        cout << "Kruskal MST:\n";

        for (auto &[w,u,v] : edges) {
            if (find(u) != find(v)) {
                unite(u,v);
                cout << u << " - " << v << "\n";
            }
        }
    }
};

int main() {
    ListGraph g(5);

    g.InsertEdge(0,1,2);
    g.InsertEdge(0,2,4);
    g.InsertEdge(1,2,1);
    g.InsertEdge(1,3,7);
    g.InsertEdge(2,4,3);

    g.Prim();
    cout << "\n";
    g.Kruskal();
}
