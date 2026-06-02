# 41343107 41343109

# 作業二 Graph

## 解題說明(第一題)

Adjacency Matrix（鄰接矩陣）
Adjacency List（鄰接串列）
Adjacency Multilist (鄰接多重串列）

# 解題策略

## 1. class Graph

圖形（Graph）的抽象基底類別，定義所有圖形結構共同擁有的功能。

包含：

- 頂點數量 `n`
- 邊數量 `e`

提供：

- 判斷是否為空圖
- 取得頂點數量
- 取得邊數量

並宣告以下純虛擬函式：

- Degree()
- ExistsEdge()
- InsertVertex()
- InsertEdge()
- DeleteVertex()
- DeleteEdge()

讓各種圖形表示法繼承後自行實作。

---

## 2. class AMatrix

鄰接矩陣（Adjacency Matrix）實作。

利用：

```cpp
vector<vector<int>>
```

建立二維矩陣儲存圖形。

矩陣中：

- 1 表示有邊
- 0 表示無邊

例如：

0 1 0
1 0 1
0 1 0

表示：

0 ── 1 ── 2

---

## 3. AMatrix::InsertVertex()

新增頂點。

作法：

1. 頂點數量 n++
2. 所有既有列新增一個 0
3. 新增一列 n 個 0

使矩陣維持 n × n 大小。

---

## 4. AMatrix::InsertEdge()

新增邊。

由於本程式為無向圖：

```cpp
mx[u][v] = 1;
mx[v][u] = 1;
```

同時更新兩個方向。

若原本不存在該邊：

- 建立連結
- 邊數 e++

---

## 5. AMatrix::DeleteEdge()

刪除邊。

將：

```cpp
mx[u][v]
mx[v][u]
```

設為 0。

並更新邊數。

---

## 6. AMatrix::Degree()

計算頂點度數。

逐一檢查：

```cpp
mx[u][i]
```

統計值為 1 的數量。

即為該頂點連接的邊數。

---

## 7. AMatrix::Display()

輸出鄰接矩陣。

格式：

```text
Adjacency Matrix
0 1 1
1 0 0
1 0 0
```

方便觀察圖形結構。

---

## 8. class AList

鄰接串列（Adjacency List）實作。

利用：

```cpp
vector<list<int>>
```

儲存每個頂點相鄰的頂點。

例如：

```text
0 → 1 → 2
1 → 0
2 → 0
```

表示：

0 同時連接 1 與 2。

---

## 9. AList::InsertVertex()

新增頂點。

建立新的空串列：

```cpp
adj.push_back(list<int>());
```

表示新的頂點尚未與任何頂點連接。

---

## 10. AList::InsertEdge()

新增邊。

將：

```cpp
v
```

加入：

```cpp
adj[u]
```

同時將：

```cpp
u
```

加入：

```cpp
adj[v]
```

形成無向圖連結。

---

## 11. AList::ExistsEdge()

判斷邊是否存在。

逐一搜尋：

```cpp
adj[u]
```

若找到 v：

```cpp
return true;
```

否則回傳 false。

---

## 12. AList::Degree()

計算頂點度數。

由於每個鄰接頂點都存於串列中：

```cpp
adj[u].size()
```

即可得到度數。

---

## 13. AList::DeleteEdge()

刪除邊。

利用：

```cpp
list.remove()
```

同時移除：

- u → v
- v → u

並更新邊數。

---

## 14. class AMatrixList

鄰接多重串列（Adjacency Multilist）實作。

適用於無向圖。

特色：

每條邊只建立一個 EdgeNode。

可同時被兩個頂點共享。

避免重複儲存邊資訊。

---

## 15. struct EdgeNode

表示圖中的一條邊。

包含：

```cpp
c0
```

邊編號

```cpp
c1
```

第一個頂點

```cpp
c2
```

第二個頂點

```cpp
c1_link
```

同頂點 c1 的下一條邊

```cpp
c2_link
```

同頂點 c2 的下一條邊

---

## 16. F_edge

頂點表頭陣列。

```cpp
vector<EdgeNode*> F_edge
```

用途：

紀錄每個頂點的第一條邊。

例如：

```text
vertex 0 → N2
vertex 1 → N1
vertex 2 → N0
```

---

## 17. A_edges

邊節點陣列。

```cpp
vector<EdgeNode*> A_edges
```

保存所有邊節點。

方便：

- 顯示資料
- 搜尋邊
- 刪除邊

---

## 18. AMatrixList::InsertEdge()

新增邊。

建立：

```cpp
EdgeNode
```

後利用頭插法：

```cpp
edge->c1_link = F_edge[u];
edge->c2_link = F_edge[v];
```

加入兩個頂點的邊串列。

最後：

```cpp
e++
```

更新邊數。

---

## 19. AMatrixList::ExistsEdge()

判斷邊是否存在。

從：

```cpp
F_edge[u]
```

開始走訪。

若找到：

```cpp
(u,v)
```

或

```cpp
(v,u)
```

則回傳 true。

---

## 20. AMatrixList::Degree()

計算頂點度數。

沿著：

```cpp
nextFrom()
```

不斷走訪與該頂點相連的邊。

統計總數即為 Degree。

---

## 21. AMatrixList::DeleteEdge()

刪除邊。

步驟：

1. 找出目標 EdgeNode
2. 從兩個頂點串列中解除連結
3. 從 A_edges 中移除
4. delete 記憶體
5. e--

完成邊的刪除。

---

## 22. AMatrixList::Display()

輸出鄰接多重串列。

包含：

### Edge Nodes

```text
N0 [0 1 N2 N1]
N1 [1 2 0 N0]
```

顯示：

- 邊編號
- 連接頂點
- 下一條邊

---

### Vertex Lists

```text
vertex 0 : N0 -> N2
vertex 1 : N1
vertex 2 : N2
```

顯示每個頂點所連接的邊。

---

## 23. main()

主程式。

先輸入：

```cpp
op
```

選擇圖形表示法：

```text
1 → Adjacency Matrix
2 → Adjacency List
3 → Adjacency Multilist
```

再輸入：

```cpp
vc
```

頂點數量。

---

## 24. 建立圖形

根據使用者選擇：

```cpp
AMatrix
AList
AMatrixList
```

建立對應圖形物件。

並新增：

```cpp
vc
```

個頂點。

---

## 25. 輸入邊資訊

讀入三組：

```cpp
u v
```

代表：

```text
u ── v
```

並呼叫：

```cpp
InsertEdge()
```

建立邊。

---

## 26. 輸出結果

若：

### op = 1

輸出鄰接矩陣。

---

### op = 2

輸出：

```text
Degree = ...
Edge exists.
```

測試：

- Degree()
- ExistsEdge()

功能。

---

### op = 3

輸出：

```text
Edge Nodes
Vertex Lists
```

展示鄰接多重串列結構。

---



- Graph 為抽象基底類別
- AMatrix 使用鄰接矩陣表示圖
- AList 使用鄰接串列表示圖
- AMatrixList 使用鄰接多重串列表示圖
- EdgeNode 負責儲存每條邊的資訊

本程式利用三種不同圖形表示法實作相同的圖形操作，展示了圖論中常見的資料結構設計與應用。

## 程式實作

以下為主要程式碼：

```cpp
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

/* ===================== Main ===================== */
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

```

各檔案皆以 `ListGraph` 類別封裝圖形資料與演算法：
- `InsertEdge`：插入邊（依題目為有向或無向）。
- `DFS` / `BFS`：走訪輸出節點順序。
- `Prim` / `Kruskal`：輸出最小生成樹的邊集合。
- `Dijkstra`：輸出起點到各節點最短距離。
- `AOV` / `AOE`：輸出拓撲序與關鍵路徑。

## 效能分析
設頂點數為 V、邊數為 E：

| 演算法 | 時間複雜度 | 空間複雜度 |
|:---:|:---:|:---:|
| DFS | O(V + E) | O(V + E) |
| BFS | O(V + E) | O(V + E) |
| Prim（priority queue） | O(E log V) | O(V + E) |
| Kruskal（sort + DSU） | O(E log E) | O(V + E) |
| Dijkstra（priority queue） | O(E log V) | O(V + E) |
| AOV（Topological Sort） | O(V + E) | O(V + E) |
| AOE（Critical Path） | O(V + E) | O(V + E) |

## 測試與驗證

各主程式檔案內的 `main()` 皆建立小型圖並輸出結果，可直接編譯執行檢查：
- DFS / BFS 是否依序走訪所有可達節點
- MST 是否輸出合法的樹邊
- Dijkstra 是否輸出合理的最短距離
- AOV / AOE 是否輸出拓撲序與關鍵路徑

## 結論
本作業完成多種圖形演算法的基本實作，採用鄰接串列作為底層資料結構，並能輸出走訪順序、最小生成樹、最短距離與關鍵路徑等結果。

## 心得討論
透過將各演算法拆成獨立檔案，能清楚對照每個演算法的核心流程。實作時需特別注意圖的方向性與權重設定，才能確保輸出符合題目需求。

