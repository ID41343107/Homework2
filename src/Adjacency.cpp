#include <iostream>
#include <vector>
#include <list>
#include <utility>
using namespace std;

/* ===================== Graph ADT ===================== */
class Graph {
public:
    Graph() : n(0), e(0) {}
    virtual ~Graph() {}

    bool IsEmpty() const { return n == 0; }
    int NumberOfVertices() const { return n; }
    int NumberOfEdges() const { return e; }

    virtual int Degree(int u) const = 0;
    virtual bool ExistsEdge(int u, int v) const = 0;

    virtual void InsertVertex(int v) = 0;
    virtual void InsertEdge(int u, int v) = 0;

    virtual void DeleteVertex(int v) = 0;
    virtual void DeleteEdge(int u, int v) = 0;

protected:
    int n;
    int e;
};

/* ===================== 1) Adjacency Matrix ===================== */
class AMatrix : public Graph {
private:
    vector<vector<int>> mx;

public:
    AMatrix() : Graph() {}

    int Degree(int u) const override {
        int t = 0;
        for (int i = 0; i < n; i++) {
            if (mx[u][i] == 1) t++;
        }
        return t;
    }

    bool ExistsEdge(int u, int v) const override {
        return mx[u][v] == 1;
    }

    void InsertVertex(int /*v*/) override {
        n++;
        for (int i = 0; i < (int)mx.size(); i++) {
            mx[i].push_back(0);
        }
        mx.push_back(vector<int>(n, 0));
    }

    void InsertEdge(int u, int v) override {
        if (u == v) return;
        if (mx[u][v] == 0) {
            mx[u][v] = 1;
            mx[v][u] = 1;
            e++;
        }
    }

    void DeleteEdge(int u, int v) override {
        if (mx[u][v] == 1) {
            mx[u][v] = 0;
            mx[v][u] = 0;
            e--;
        }
    }

    void DeleteVertex(int v) override {
        for (int j = 0; j < n; j++) {
            if (mx[v][j] == 1) e--;
        }
        mx.erase(mx.begin() + v);
        for (int i = 0; i < (int)mx.size(); i++) {
            mx[i].erase(mx[i].begin() + v);
        }
        n--;
    }

    void Display() const {
        cout << "Adjacency Matrix\n";
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                cout << mx[i][j] << " ";
            }
            cout << "\n";
        }
    }
};

/* ===================== 2) Adjacency List ===================== */
class AList : public Graph {
private:
    vector< list<int> > adj;

public:
    AList() : Graph() {}

    int Degree(int u) const override {
        return (int)adj[u].size();
    }

    bool ExistsEdge(int u, int v) const override {
        for (int x : adj[u]) {
            if (x == v) return true;
        }
        return false;
    }

    void InsertVertex(int /*v*/) override {
        adj.push_back(list<int>());
        n++;
    }

    void InsertEdge(int u, int v) override {
        if (u == v) return;
        if (ExistsEdge(u, v)) return;
        adj[u].push_back(v);
        adj[v].push_back(u);
        e++;
    }

    void DeleteEdge(int u, int v) override {
        if (!ExistsEdge(u, v)) return;
        adj[u].remove(v);
        adj[v].remove(u);
        e--;
    }

    void DeleteVertex(int v) override {
        for (int u : adj[v]) {
            adj[u].remove(v);
            e--;
        }
        adj.erase(adj.begin() + v);
        n--;

        for (int i = 0; i < n; i++) {
            for (int& x : adj[i]) {
                if (x > v) x--;
            }
        }
    }

   
    void Display() const {
        for (int i = 0; i < n; i++) {
            cout << "aList[" << (i + 1) << "] ";
            for (int x : adj[i]) {
                cout << (x + 1) << " ";
            }
            cout << "\n";
        }
    }
};

/* ===================== 3) Adjacency Multilist ===================== */
class AMatrixList : public Graph {
private:
    struct EdgeNode {
        int c0;
        int c1, c2;
        EdgeNode* c1_link;
        EdgeNode* c2_link;

        EdgeNode(int id, int i, int j) {
            c0 = id;
            c1 = i;
            c2 = j;
            c1_link = NULL;
            c2_link = NULL;
        }
    };

    vector<EdgeNode*> F_edge;
    vector<EdgeNode*> A_edges;

    EdgeNode* nextFrom(EdgeNode* p, int u) const {
        if (p == NULL) return NULL;
        if (p->c1 == u) return p->c1_link;
        return p->c2_link;
    }

    bool isTarget(EdgeNode* p, int u, int v) const {
        if (p == NULL) return false;
        return (p->c1 == u && p->c2 == v) || (p->c1 == v && p->c2 == u);
    }

    void unlinkFromVertex(int u, EdgeNode* target) {
        EdgeNode* cur = F_edge[u];
        EdgeNode* prev = NULL;

        while (cur != NULL) {
            if (cur == target) break;
            prev = cur;
            cur = nextFrom(cur, u);
        }
        if (cur == NULL) return;

        EdgeNode* nxt = nextFrom(cur, u);

        if (prev == NULL) {
            F_edge[u] = nxt;
        }
        else {
            if (prev->c1 == u) prev->c1_link = nxt;
            else prev->c2_link = nxt;
        }

        if (target->c1 == u) target->c1_link = NULL;
        else target->c2_link = NULL;
    }

public:
    AMatrixList() : Graph() {}

    ~AMatrixList() override {
        for (int i = 0; i < (int)A_edges.size(); i++) {
            delete A_edges[i];
        }
    }

    int Degree(int u) const override {
        int t = 0;
        EdgeNode* p = F_edge[u];
        while (p != NULL) {
            t++;
            p = nextFrom(p, u);
        }
        return t;
    }

    bool ExistsEdge(int u, int v) const override {
        EdgeNode* p = F_edge[u];
        while (p != NULL) {
            if (isTarget(p, u, v)) return true;
            p = nextFrom(p, u);
        }
        return false;
    }

    void InsertVertex(int /*v*/) override {
        F_edge.push_back(NULL);
        n++;
    }

    void InsertEdge(int u, int v) override {
        if (u == v) return;
        if (ExistsEdge(u, v)) return;

        EdgeNode* edge = new EdgeNode(e, u, v);

        edge->c1_link = F_edge[u];
        edge->c2_link = F_edge[v];
        F_edge[u] = edge;
        F_edge[v] = edge;

        A_edges.push_back(edge);
        e++;
    }

    void DeleteEdge(int u, int v) override {
        if (!ExistsEdge(u, v)) return;

        EdgeNode* target = NULL;
        EdgeNode* p = F_edge[u];
        while (p != NULL) {
            if (isTarget(p, u, v)) {
                target = p;
                break;
            }
            p = nextFrom(p, u);
        }
        if (target == NULL) return;

        unlinkFromVertex(u, target);
        unlinkFromVertex(v, target);

        for (auto it = A_edges.begin(); it != A_edges.end(); ++it) {
            if (*it == target) {
                A_edges.erase(it);
                break;
            }
        }

        delete target;
        e--;
    }

    void DeleteVertex(int v) override {
        while (F_edge[v] != NULL) {
            EdgeNode* p = F_edge[v];
            int other = (p->c1 == v) ? p->c2 : p->c1;
            DeleteEdge(v, other);
        }

        F_edge.erase(F_edge.begin() + v);
        n--;

        for (int i = 0; i < (int)A_edges.size(); i++) {
            if (A_edges[i]->c1 > v) A_edges[i]->c1--;
            if (A_edges[i]->c2 > v) A_edges[i]->c2--;
        }
    }

    void Display() const {
        for (int i = 0; i < (int)A_edges.size(); i++) {
            EdgeNode* edge = A_edges[i];
            cout << "Node" << edge->c0 << " [ "
                << (edge->c1 + 1) << " " << (edge->c2 + 1) << " ";

            if (edge->c1_link != NULL) cout << "Node" << edge->c1_link->c0 << " ";
            else cout << "0 ";

            if (edge->c2_link != NULL) cout << "Node" << edge->c2_link->c0 << " ";
            else cout << "0 ";

            cout << "] edge[" << (edge->c1 + 1) << "," << (edge->c2 + 1) << "]\n";
        }

        cout << "\nthe lists are\n";
        for (int i = 0; i < n; i++) {
            cout << "vertex " << (i + 1) << " : ";
            EdgeNode* p = F_edge[i];
            while (p != NULL) {
                cout << "Node" << p->c0;
                bool hasNext = false;
                if (p->c1 == i && p->c1_link != NULL) hasNext = true;
                if (p->c2 == i && p->c2_link != NULL) hasNext = true;

                if (hasNext) cout << " -> ";

                if (p->c1 == i) p = p->c1_link;
                else p = p->c2_link;
            }
            cout << "\n";
        }
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int vc, ec;
    cin >> vc >> ec;
	

    vector<pair<int, int>> edges(ec);
    for (int i = 0; i < ec; i++) {
        cin >> edges[i].first >> edges[i].second;
    }
    cout << "have" << vc << " vertex" << ",have" << ec << " edge\n";
    AMatrix g1;
    AList g2;
    AMatrixList g3;

    for (int i = 0; i < vc; i++) {
        g1.InsertVertex(i);
        g2.InsertVertex(i);
        g3.InsertVertex(i);
    }

    for (int i = 0; i < ec; i++) {
        int u = edges[i].first - 1;  // 1-based -> 0-based
        int v = edges[i].second - 1; // 1-based -> 0-based

        if (u < 0 || u >= vc || v < 0 || v >= vc) {
            cout << "Invalid edge input: " << edges[i].first << " " << edges[i].second << "\n";
            return 0;
        }

        g1.InsertEdge(u, v);
        g2.InsertEdge(u, v);
        g3.InsertEdge(u, v);
    }

    cout << "  Adjacency Matrix \n";
    g1.Display();

    cout << "\n  Adjacency List \n";
    g2.Display();

    cout << "\n  Adjacency Multilist \n";
    g3.Display();

    return 0;
}
