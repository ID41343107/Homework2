#include <iostream>
#include <vector>
#include <queue>
#include <climits>

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
    }

    void Dijkstra(int start) {
        vector<int> dist(n, INT_MAX);
        priority_queue<pair<int,int>, vector<pair<int,int>>, greater<>> pq;

        dist[start] = 0;
        pq.push({0, start});

        while (!pq.empty()) {
            auto [d, u] = pq.top(); pq.pop();

            for (auto &p : adj[u]) {
                int v = p.first;
                int w = p.second;

                if (dist[v] > dist[u] + w) {
                    dist[v] = dist[u] + w;
                    pq.push({dist[v], v});
                }
            }
        }

        cout << "Shortest Paths:\n";
        for (int i = 0; i < n; i++)
            cout << i << " = " << dist[i] << "\n";
    }
};

int main() {
    ListGraph g(5);

    g.InsertEdge(0,1,2);
    g.InsertEdge(0,2,4);
    g.InsertEdge(1,2,1);
    g.InsertEdge(1,3,7);
    g.InsertEdge(2,4,3);

    g.Dijkstra(0);
}
