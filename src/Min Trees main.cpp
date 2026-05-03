#include <iostream>
#include <vector>
#include <algorithm>
#include <queue>
#include <climits>
using namespace std;

struct Edge {
    int u, v, w;
};

class DisjointSet {
private:
    vector<int> parent, rank;

public:
    DisjointSet(int n) {
        parent.resize(n);
        rank.resize(n, 0);

        for (int i = 0; i < n; i++)
            parent[i] = i;
    }

    int find(int x) {
        if (parent[x] != x)
            parent[x] = find(parent[x]);
        return parent[x];
    }

    void unite(int x, int y) {
        int rx = find(x);
        int ry = find(y);

        if (rx == ry) return;

        if (rank[rx] < rank[ry])
            parent[rx] = ry;
        else if (rank[rx] > rank[ry])
            parent[ry] = rx;
        else {
            parent[ry] = rx;
            rank[rx]++;
        }
    }
};

class Graph {
private:
    int V;
    vector<Edge> edges;
    vector<vector<pair<int,int>>> adj;

public:
    Graph(int V) {
        this->V = V;
        adj.resize(V);
    }

    void addEdge(int u, int v, int w) {
        edges.push_back({u, v, w});
        adj[u].push_back({v, w});
        adj[v].push_back({u, w});
    }

    void kruskalMST() {
        cout << "\n========== Kruskal 最小生成樹 ==========\n";

        sort(edges.begin(), edges.end(),
             [](Edge a, Edge b) { return a.w < b.w; });

        DisjointSet ds(V);
        vector<Edge> MST;
        int cost = 0;

        for (auto e : edges) {
            if (ds.find(e.u) != ds.find(e.v)) {
                ds.unite(e.u, e.v);
                MST.push_back(e);
                cost += e.w;
            }
        }

        if (MST.size() != V - 1) {
            cout << "沒有生成樹\n";
            return;
        }

        for (auto e : MST)
            cout << e.u << " - " << e.v << " : " << e.w << endl;

        cout << "總成本 = " << cost << endl;
    }

    void primMST() {
        cout << "\n========== Prim 最小生成樹 ==========\n";

        vector<int> key(V, INT_MAX);
        vector<int> parent(V, -1);
        vector<bool> inMST(V, false);

        priority_queue<pair<int,int>,
                       vector<pair<int,int>>,
                       greater<pair<int,int>>> pq;

        key[0] = 0;
        pq.push({0, 0});

        int cost = 0;

        while (!pq.empty()) {
            int u = pq.top().second;
            int w = pq.top().first;
            pq.pop();

            if (inMST[u]) continue;

            inMST[u] = true;
            cost += w;

            for (auto [v, weight] : adj[u]) {
                if (!inMST[v] && weight < key[v]) {
                    key[v] = weight;
                    parent[v] = u;
                    pq.push({key[v], v});
                }
            }
        }

        for (int i = 1; i < V; i++)
            cout << parent[i] << " - " << i << endl;

        cout << "總成本 = " << cost << endl;
    }

    void sollinMST() {
        cout << "\n========== Sollin 最小生成樹 ==========\n";

        DisjointSet ds(V);
        vector<Edge> MST;
        int components = V;

        while (components > 1) {

            vector<Edge> cheapest(V, {-1, -1, INT_MAX});
            
            for (auto e : edges) {
                int uSet = ds.find(e.u);
                int vSet = ds.find(e.v);

                if (uSet != vSet) {

                    if (e.w < cheapest[uSet].w)
                        cheapest[uSet] = e;

                    if (e.w < cheapest[vSet].w)
                        cheapest[vSet] = e;
                }
            }

            bool merged = false;

            for (int i = 0; i < V; i++) {
                Edge e = cheapest[i];

                if (e.u != -1) {
                    int uSet = ds.find(e.u);
                    int vSet = ds.find(e.v);

                    if (uSet != vSet) {
                        ds.unite(uSet, vSet);
                        MST.push_back(e);
                        components--;
                        merged = true;
                    }
                }
            }

            if (!merged) break;
        }

        int cost = 0;

        for (auto e : MST) {
            cout << e.u << " - " << e.v << " : " << e.w << endl;
            cost += e.w;
        }

        cout << "總成本 = " << cost << endl;
    }
};

int main() {
    Graph g(6);

    g.addEdge(0, 1, 4);
    g.addEdge(0, 2, 3);
    g.addEdge(1, 2, 1);
    g.addEdge(1, 3, 2);
    g.addEdge(2, 3, 4);
    g.addEdge(3, 4, 2);
    g.addEdge(4, 5, 6);

    g.kruskalMST();
    g.primMST();
    g.sollinMST();

    return 0;
}
