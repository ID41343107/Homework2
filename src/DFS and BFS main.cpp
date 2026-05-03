#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
using namespace std;

class Graph {
private:
    int n;
    vector<vector<int>> adj;
    vector<bool> visited;

public:
    Graph(int vertices);
    void addEdge(int u, int v);

    void DFS();
    void DFS(int v);

    void BFS(int v);
};

Graph::Graph(int vertices)
{
    n = vertices;
    adj.resize(n);
    visited.resize(n, false);
}

void Graph::addEdge(int u, int v)
{
    if (u >= n || v >= n) return;
    adj[u].push_back(v);
    adj[v].push_back(u);
}

// ===== DFS =====
void Graph::DFS()
{
    fill(visited.begin(), visited.end(), false);

    cout << "DFS traversal: ";

    for (int i = 0; i < n; i++) // 處理非連通圖
    {
        if (!visited[i])
            DFS(i);
    }

    cout << endl;
}

void Graph::DFS(int v)
{
    visited[v] = true;
    cout << v << " ";

    for (int w : adj[v])
    {
        if (!visited[w])
            DFS(w);
    }
}

// ===== BFS =====
void Graph::BFS(int start)
{
    fill(visited.begin(), visited.end(), false);

    queue<int> q;

    cout << "BFS traversal: ";

    visited[start] = true;
    q.push(start);

    while (!q.empty())
    {
        int v = q.front();
        q.pop();

        cout << v << " ";

        for (int w : adj[v])
        {
            if (!visited[w])
            {
                visited[w] = true;
                q.push(w);
            }
        }
    }

    // 處理非連通圖
    for (int i = 0; i < n; i++)
    {
        if (!visited[i])
        {
            visited[i] = true;
            q.push(i);

            while (!q.empty())
            {
                int v = q.front();
                q.pop();

                cout << v << " ";

                for (int w : adj[v])
                {
                    if (!visited[w])
                    {
                        visited[w] = true;
                        q.push(w);
                    }
                }
            }
        }
    }

    cout << endl;
}

int main()
{
    Graph g(6);

    g.addEdge(0, 1);
    g.addEdge(0, 2);
    g.addEdge(1, 3);
    g.addEdge(1, 4);
    g.addEdge(2, 5);

    g.DFS();
    g.BFS(0);

    return 0;
}
