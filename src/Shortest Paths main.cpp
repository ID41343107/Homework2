#include <iostream>
#include <vector>
#include <queue>
#include <climits>
using namespace std;

const int INF = INT_MAX;

struct Edge {
    int u, v, w;
};

class DijkstraGraph {
private:
    int V;
    vector<vector<pair<int,int>>> adj;

public:
    DijkstraGraph(int V) {
        this->V = V;
        adj.resize(V);
    }

    void addEdge(int u, int v, int w) {
        adj[u].push_back({v, w});
        adj[v].push_back({u, w}); // 無向圖
    }

    void dijkstra(int start) {
        cout << "\n========== Dijkstra 最短路 ==========\n";

        vector<int> dist(V, INF);
        priority_queue<pair<int,int>,
                       vector<pair<int,int>>,
                       greater<pair<int,int>>> pq;

        dist[start] = 0;
        pq.push({0, start});

        while (!pq.empty()) {
            int u = pq.top().second;
            int d = pq.top().first;
            pq.pop();

            if (d > dist[u]) continue;

            for (auto [v, w] : adj[u]) {
                if (dist[u] != INF && dist[u] + w < dist[v]) {
                    dist[v] = dist[u] + w;
                    pq.push({dist[v], v});
                }
            }
        }

        for (int i = 0; i < V; i++) {
            cout << i << " = ";
            if (dist[i] == INF) cout << "INF";
            else cout << dist[i];
            cout << "\n";
        }
    }
};

class BellmanGraph {
private:
    int V;
    vector<Edge> edges;

public:
    BellmanGraph(int V) {
        this->V = V;
    }

    void addEdge(int u, int v, int w) {
        edges.push_back({u, v, w});
    }

    void bellmanFord(int start) {
        cout << "\n========== Bellman-Ford ==========\n";

        vector<int> dist(V, INF);
        dist[start] = 0;

        for (int i = 0; i < V - 1; i++) {
            bool updated = false;

            for (auto e : edges) {
                if (dist[e.u] != INF && dist[e.u] + e.w < dist[e.v]) {
                    dist[e.v] = dist[e.u] + e.w;
                    updated = true;
                }
            }

            if (!updated) break;
        }

        for (auto e : edges) {
            if (dist[e.u] != INF && dist[e.u] + e.w < dist[e.v]) {
                cout << "偵測到負權迴圈\n";
                return;
            }
        }

        for (int i = 0; i < V; i++) {
            cout << i << " = ";
            if (dist[i] == INF) cout << "INF";
            else cout << dist[i];
            cout << "\n";
        }
    }
};

class FloydGraph {
private:
    int V;
    vector<vector<int>> dist;

public:
    FloydGraph(int V) {
        this->V = V;
        dist.assign(V, vector<int>(V, INF));

        for (int i = 0; i < V; i++)
            dist[i][i] = 0;
    }

    void addEdge(int u, int v, int w) {
        dist[u][v] = min(dist[u][v], w); // 正確初始化
    }

    void floydWarshall() {
        cout << "\n========== Floyd-Warshall ==========\n";

        for (int k = 0; k < V; k++) {
            for (int i = 0; i < V; i++) {
                for (int j = 0; j < V; j++) {
                    if (dist[i][k] == INF || dist[k][j] == INF)
                        continue;

                    dist[i][j] = min(dist[i][j],
                                     dist[i][k] + dist[k][j]);
                }
            }
        }

        for (int i = 0; i < V; i++) {
            for (int j = 0; j < V; j++) {
                if (dist[i][j] == INF) cout << "INF ";
                else cout << dist[i][j] << " ";
            }
            cout << "\n";
        }
    }
};

int main() {

    // ---------------- Dijkstra ----------------
    DijkstraGraph g1(5);
    g1.addEdge(0, 1, 6);
    g1.addEdge(0, 3, 7);
    g1.addEdge(1, 2, 5);
    g1.addEdge(1, 4, 2);
    g1.addEdge(2, 3, 3);
    g1.addEdge(3, 4, 1);

    g1.dijkstra(0);

    // ---------------- Bellman-Ford ----------------
    BellmanGraph g2(5);
    g2.addEdge(0, 1, 6);
    g2.addEdge(0, 3, 7);
    g2.addEdge(1, 2, 5);
    g2.addEdge(1, 4, 2);
    g2.addEdge(2, 3, 3);
    g2.addEdge(3, 4, 1);

    g2.bellmanFord(0);

    // ---------------- Floyd-Warshall ----------------
    FloydGraph g3(5);
    g3.addEdge(0, 1, 6);
    g3.addEdge(0, 3, 7);
    g3.addEdge(1, 2, 5);
    g3.addEdge(1, 4, 2);
    g3.addEdge(2, 3, 3);
    g3.addEdge(3, 4, 1);

    g3.floydWarshall();

    return 0;
}
