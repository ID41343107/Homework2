#include <iostream>
#include <vector>
#include <queue>
#include <climits>
using namespace std;

struct Edge {
    int to, weight;
};

class Graph {
private:
    int V;
    vector<vector<Edge>> adj;

public:
    Graph(int V) {
        this->V = V;
        adj.resize(V);
    }

    void addEdge(int u, int v, int w = 0) {
        adj[u].push_back({v, w});
    }

    vector<int> topologicalSort() {
        vector<int> indegree(V, 0);

        for (int u = 0; u < V; u++)
            for (auto e : adj[u])
                indegree[e.to]++;

        queue<int> q;

        for (int i = 0; i < V; i++)
            if (indegree[i] == 0)
                q.push(i);

        vector<int> topo;

        while (!q.empty()) {
            int u = q.front();
            q.pop();
            topo.push_back(u);

            for (auto e : adj[u]) {
                if (--indegree[e.to] == 0)
                    q.push(e.to);
            }
        }

        if (topo.size() != V) {
            cout << "偵測到迴圈（不是 DAG）\n";
            return {};
        }

        return topo;
    }

    void criticalPath() {
        cout << "\n========== AOE 關鍵路徑 ==========\n";

        vector<int> topo = topologicalSort();
        if (topo.empty()) return;

        vector<int> ve(V, 0);

        for (int u : topo) {
            for (auto e : adj[u]) {
                ve[e.to] = max(ve[e.to], ve[u] + e.weight);
            }
        }

        int projectTime = 0;
        for (int i = 0; i < V; i++)
            projectTime = max(projectTime, ve[i]);

        vector<int> vl(V, projectTime);

        for (int i = V - 1; i >= 0; i--) {
            int u = topo[i];

            for (auto e : adj[u]) {
                vl[u] = min(vl[u], vl[e.to] - e.weight);
            }
        }
      
        for (int u = 0; u < V; u++) {
            for (auto e : adj[u]) {

                int v = e.to;
                int w = e.weight;

                int ee = ve[u];
                int ll = vl[v] - w;

                if (ee == ll) {
                    cout << u << " -> " << v
                         << " (time=" << w << ")\n";
                }
            }
        }

        cout << "專案完成時間 = " << projectTime << endl;
    }
};

int main() {

    Graph g(6);

    g.addEdge(0, 1, 3);
    g.addEdge(0, 2, 2);
    g.addEdge(1, 3, 2);
    g.addEdge(2, 3, 1);
    g.addEdge(3, 4, 4);
    g.addEdge(4, 5, 2);

    vector<int> topo = g.topologicalSort();

    cout << "拓樸排序：";
    for (int x : topo) cout << x << " ";
    cout << "\n";

    g.criticalPath();

    return 0;
}
