#include <iostream>
#include <vector>
#include <list>
using namespace std;
//gragh 副
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

/* ===================== 1) AdjacencyMatrix ===================== */
class AMatrix : public Graph {
private:
    vector< vector<int> > mx;

public:
    AMatrix() : Graph() {}

    int Degree(int u) const override {
        int t = 0;
        for (int i = 0; i < n; i++) if (mx[u][i] == 1) t++;
        return t;
    }

    bool ExistsEdge(int u, int v) const override {
        return mx[u][v] == 1;
    }

    void InsertVertex(int /*v*/) override {
        n++;
        for (int i = 0; i < (int)mx.size(); i++) mx[i].push_back(0);
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
        // (這份作業展示用，其實不一定會用到)
        for (int j = 0; j < n; j++) if (mx[v][j] == 1) e--;
        mx.erase(mx.begin() + v);
        for (int i = 0; i < (int)mx.size(); i++) mx[i].erase(mx[i].begin() + v);
        n--;
    }

    // display exactly like image 6 (no index header)
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

/* ===================== 2) AdjacencyList ===================== */
class AList : public Graph {
private:
    vector< list<int> > adj;

public:
    AList() : Graph() {}

    int Degree(int u) const override {
        return (int)adj[u].size();
    }

    bool ExistsEdge(int u, int v) const override {
        list<int>::const_iterator it;
        for (it = adj[u].begin(); it != adj[u].end(); ++it) {
            if (*it == v) return true;
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
        // (展示用)
        list<int>::iterator it;
        for (it = adj[v].begin(); it != adj[v].end(); ++it) {
            int u = *it;
            adj[u].remove(v);
            e--;
        }
        adj.erase(adj.begin() + v);
        n--;
        for (int i = 0; i < n; i++) {
            for (it = adj[i].begin(); it != adj[i].end(); ++it) {
                if (*it > v) (*it)--;
            }
        }
    }
};

/* ===================== 3) AdjacencyMatrixList (Adjacency Multilist) ===================== */
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
        for (int i = 0; i < (int)A_edges.size(); i++) delete A_edges[i];
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
        // for multilist, we need a head pointer per vertex
        F_edge.push_back(NULL);
        n++;
    }

    void InsertEdge(int u, int v) override {
        if (u == v) return;
        if (ExistsEdge(u, v)) return;

        EdgeNode* edge = new EdgeNode(e, u, v);

        // head insert into u / v lists
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
            if (isTarget(p, u, v)) { target = p; break; }
            p = nextFrom(p, u);
        }
        if (target == NULL) return;

        unlinkFromVertex(u, target);
        unlinkFromVertex(v, target);

        // remove from A_edges
        vector<EdgeNode*>::iterator it;
        for (it = A_edges.begin(); it != A_edges.end(); ++it) {
            if (*it == target) { A_edges.erase(it); break; }
        }

        delete target;
        e--;
    }

    void DeleteVertex(int v) override {
        // (展示用，不一定會用到)
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

    // Display exactly like image 6
    void Display() const {
        cout << "---------- Edge Nodes ----------\n";
        for (int i = 0; i < (int)A_edges.size(); i++) {
            EdgeNode* edge = A_edges[i];
            cout << "N" << edge->c0 << " [ "
                << edge->c1 << " " << edge->c2 << " ";

            if (edge->c1_link != NULL) cout << "N" << edge->c1_link->c0 << " ";
            else cout << "0 ";

            if (edge->c2_link != NULL) cout << "N" << edge->c2_link->c0 << " ";
            else cout << "0 ";

            cout << "] edge(" << edge->c1 << "," << edge->c2 << ")\n";
        }

        cout << "\n---------- Vertex Lists ----------\n";
        for (int i = 0; i < n; i++) {
            cout << "vertex " << i << " : ";
            EdgeNode* p = F_edge[i];
            while (p != NULL) {
                cout << "N" << p->c0;

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

/* ===================== Main - match image 6 test style ===================== */
int main() {
    int op,vc;

    cin >> op;      // choose 1/2/3
    cin >> vc;      // Vertex count

    if (op == 1) {
        AMatrix g;
        for (int i = 0; i < vc; i++) g.InsertVertex(i);

        // read 3 edges (like your sample)
        int u, v;
        for (int k = 0; k < 3; k++) {
            cin >> u >> v;
            g.InsertEdge(u, v);
        }
        g.Display();
    }
    else if (op == 2) {
        AList g;
        for (int i = 0; i < vc; i++) g.InsertVertex(i);

        // read 3 edges
        int u, v;
        for (int k = 0; k < 3; k++) {
            cin >> u >> v;
            g.InsertEdge(u, v);
        }

        // then do Degree(0) and CheckEdge(0,2) like sample
        cout << "Degree = " << g.Degree(0) << "\n";
        if (g.ExistsEdge(0, 2)) cout << "Edge exists.\n";
        else cout << "Edge does not exist.\n";
    }
    else if (op == 3) {
        AMatrixList g;
        for (int i = 0; i < vc; i++) g.InsertVertex(i);

        // read 3 edges
        int u, v;
        for (int k = 0; k < 3; k++) {
            cin >> u >> v;
            g.InsertEdge(u, v);
        }
        g.Display();
    }

    return 0;
}
