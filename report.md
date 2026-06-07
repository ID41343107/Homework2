# 41343107 41343109

# 作業二 Graph

## 解題說明(第一題)

Adjacency Matrix（鄰接矩陣）
Adjacency List（鄰接串列）
Adjacency Multilist (鄰接多重串列）

# 解題策略

本程式使用 **繼承 + 多型** 實作三種 Graph 表示法，並共用同一套操作介面，如 `InsertVertex`、`InsertEdge`、`DeleteEdge`、`Degree` 與 `Display`。

首先建立抽象父類別 `Graph`，將三種圖形表示法共通的屬性與操作統一管理，並宣告純虛擬函式，讓子類別必須各自實作對應功能。

接著建立三個子類別：

- `AMatrix`：使用 **Adjacency Matrix**
- `AList`：使用 **Adjacency List**
- `AMatrixList`：使用 **Adjacency Multilist**

雖然三者功能相同，但因為底層資料結構不同，所以 `Degree`、`ExistsEdge`、`InsertEdge` 等操作的實作方式也不同，例如：
- Matrix 透過走訪矩陣列/欄完成操作
- List 透過串列長度與節點搜尋完成操作
- Multilist 透過 edge chain 完成操作

最後在主程式中建立各種 Graph 物件並插入資料，再呼叫各自的 `Display()`，即可比較三種表示法的差異。
## 程式實作

以下為主要程式碼：

```cpp
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

    // 輸出格式改成圖片中的 aList[1], aList[2] ...
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
        cout << "---------- Edge Nodes ----------\n";
        for (int i = 0; i < (int)A_edges.size(); i++) {
            EdgeNode* edge = A_edges[i];
            cout << "N" << edge->c0 << " [ "
                << (edge->c1 + 1) << " " << (edge->c2 + 1) << " ";

            if (edge->c1_link != NULL) cout << "N" << edge->c1_link->c0 << " ";
            else cout << "0 ";

            if (edge->c2_link != NULL) cout << "N" << edge->c2_link->c0 << " ";
            else cout << "0 ";

            cout << "] edge(" << (edge->c1 + 1) << "," << (edge->c2 + 1) << ")\n";
        }

        cout << "\n---------- Vertex Lists ----------\n";
        for (int i = 0; i < n; i++) {
            cout << "vertex " << (i + 1) << " : ";
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

/* ===================== Main ===================== */
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int vc, ec;
    cin >> vc >> ec;

    vector<pair<int, int>> edges(ec);
    for (int i = 0; i < ec; i++) {
        cin >> edges[i].first >> edges[i].second;
    }

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

    cout << "===== 1) Adjacency Matrix =====\n";
    g1.Display();

    cout << "\n===== 2) Adjacency List =====\n";
    g2.Display();

    cout << "\n===== 3) Adjacency Multilist =====\n";
    g3.Display();

    return 0;
}
```

## 效能分析

設頂點數為 `V`、邊數為 `E`。

| 操作/方法 | AMatrix | AList | AMatrixList |
|:---|:---:|:---:|:---:|
| `Degree(u)` | `O(V)` | `O(deg(u))` | `O(deg(u))` |
| `ExistsEdge(u, v)` | `O(1)` | `O(deg(u))` | `O(deg(u))` |
| `InsertVertex(v)` | `O(V)` | `O(1)` | `O(1)` |
| `InsertEdge(u, v)` | `O(1)` | `O(deg(u))` | `O(1)` |
| `DeleteEdge(u, v)` | `O(1)` | `O(deg(u))` | `O(deg(u))` |
| `DeleteVertex(v)` | `O(V^2)` | `O(V + E)` | `O(V + E)` |
| `Display()` | `O(V^2)` | `O(V + E)` | `O(V + E)` |

## 測試與驗證

本程式可透過 `main()` 建立圖並呼叫三種表示法的操作函式來驗證結果，例如：

- `InsertVertex()` 是否正確建立頂點
- `InsertEdge()` 是否正確加入邊
- `Degree()` 是否回傳正確度數
- `ExistsEdge()` 是否能正確判斷邊是否存在
- `Display()` 是否能輸出符合預期的圖形表示

測試時可輸入小型圖，並比對 `Adjacency Matrix`、`Adjacency List`、`Adjacency Multilist` 的輸出是否正確，以確認三種資料結構的實作都能正常運作。

## 結論

本程式透過 **繼承與多型**，成功將三種不同的 Graph 表示法整合在同一套操作介面下，使程式架構更清楚，也更容易維護與擴充。

`Adjacency Matrix`、`Adjacency List` 與 `Adjacency Multilist` 雖然都能表示圖，但在時間與空間效率上各有優缺點，因此可依不同需求選擇合適的資料結構。藉由本次實作，可以更清楚了解不同圖形表示法之間的差異，以及物件導向設計在資料結構實作上的優勢。

## 心得討論
透過將各演算法拆成獨立檔案，能清楚對照每個演算法的核心流程。實作時需特別注意圖的方向性與權重設定，才能確保輸出符合題目需求。

