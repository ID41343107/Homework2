# 41343107 41343109

# 作業二 Graph

# Adjacency 解題說明

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

### Adjacency

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

| 測試案例 | 輸入參數  | 輸出  |
|----------|--------------|--------------|
| 測試一   |5：共有 5 個頂點 3：共有 3 條邊 邊：1 2,2 5,1 4  |<img width="481" height="725" alt="image" src="https://github.com/user-attachments/assets/424847bb-e4f6-49c5-8e62-abe0bc1dff1e" />   |



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

# AOV and AOE 解題說明

# 解題策略

使用帶權重的有向鄰接串列（Adjacency List with Weights），將圖結構應用在拓撲排序（Topological Sort）與關鍵路徑（Critical Path）的計算：

1. AOV 網 (Activity On Vertex) 與拓撲排序

在 AOV 網中，頂點（Vertex）代表活動，有向邊（Edge）代表活動之間的先後順序。

    1. 計算圖中所有節點的入度（Indegree）。
    2. 將所有入度為 0 的節點（代表沒有任何前置任務，可以立即開始）推入佇列（queue）。
    3. 當佇列不為空時，取出一個節點 u 輸出，並將 u 所有的鄰接節點的入度減 1。
    4. 如果在減 1 後某個鄰接節點的入度變為 0，則將其推入佇列。重複此過程直到佇列清空。
    
2. AOE 網 (Activity On Edge) 與關鍵路徑

   在 AOE 網中，有向邊（Edge）代表活動，邊上的權重代表該活動所需的花費時間；頂點（Vertex）則代表事件（Event），即某些活動的結束與另一些活動的開始。

       第一步：取得拓撲序列（Topological Order）。利用上述 AOV 的邏輯排好順序，確保計算後續時間時，前置事件都已經處理完畢。

       第二步：計算事件的最早發生時間 ET (Earliest Time)。
   
       第三步：計算事件的最晚發生時間 LT (Latest Time)。
   
       第四步：找出關鍵活動（Critical Activity）。

## 程式實作

以下為主要程式碼：

### AOV and AOE

```cpp
#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>

using namespace std;

class ListGraph {
private:
    int n;
    vector<vector<pair<int, int>>> adj;

public:
    ListGraph(int n) {
        this->n = n;
        adj.resize(n);
    }

    void InsertEdge(int u, int v, int w) {
        adj[u].push_back(make_pair(v, w));
    }

    // ---------- AOV ----------
    void AOV() {
        vector<int> indegree(n, 0);

        for (int u = 0; u < n; u++) {
            for (vector<pair<int, int> >::iterator it = adj[u].begin(); it != adj[u].end(); ++it) {
                int v = it->first;
                indegree[v]++;
            }
        }

        queue<int> q;
        for (int i = 0; i < n; i++) {
            if (indegree[i] == 0) q.push(i);
        }

        cout << "AOV: ";
        while (!q.empty()) {
            int u = q.front();
            q.pop();
            cout << u << " ";

            for (vector<pair<int, int> >::iterator it = adj[u].begin(); it != adj[u].end(); ++it) {
                int v = it->first;
                if (--indegree[v] == 0) {
                    q.push(v);
                }
            }
        }
        cout << "\n";
    }

    // ---------- AOE ----------
    void AOE() {
        vector<int> indegree(n, 0);

        for (int u = 0; u < n; u++) {
            for (vector<pair<int, int> >::iterator it = adj[u].begin(); it != adj[u].end(); ++it) {
                int v = it->first;
                indegree[v]++;
            }
        }

        queue<int> q;
        vector<int> topo;

        for (int i = 0; i < n; i++) {
            if (indegree[i] == 0) q.push(i);
        }

        while (!q.empty()) {
            int u = q.front();
            q.pop();
            topo.push_back(u);

            for (vector<pair<int, int> >::iterator it = adj[u].begin(); it != adj[u].end(); ++it) {
                int v = it->first;
                if (--indegree[v] == 0) {
                    q.push(v);
                }
            }
        }

        vector<int> ET(n, 0);

        for (vector<int>::iterator it = topo.begin(); it != topo.end(); ++it) {
            int u = *it;
            for (vector<pair<int, int> >::iterator jt = adj[u].begin(); jt != adj[u].end(); ++jt) {
                int v = jt->first;
                int w = jt->second;
                ET[v] = max(ET[v], ET[u] + w);
            }
        }

        vector<int> LT(n, ET[n - 1]);
        for (int i = 0; i < n; i++) {
            LT[i] = ET[n - 1];
        }

        for (int i = (int)topo.size() - 1; i >= 0; i--) {
            int u = topo[i];
            for (vector<pair<int, int> >::iterator it = adj[u].begin(); it != adj[u].end(); ++it) {
                int v = it->first;
                int w = it->second;
                LT[u] = min(LT[u], LT[v] - w);
            }
        }

        cout << "AOE : ";
        cout << "Critical Path:\n";
        for (int u = 0; u < n; u++) {
            for (vector<pair<int, int> >::iterator it = adj[u].begin(); it != adj[u].end(); ++it) {
                int v = it->first;
                int w = it->second;
                if (ET[u] == LT[v] - w) {
                    cout << u << " -> " << v << "\n";
                }
            }
        }
    }
};

int main() {
    ListGraph g(6);

    g.InsertEdge(0, 1, 3);
    g.InsertEdge(0, 2, 2);
    g.InsertEdge(1, 3, 4);
    g.InsertEdge(2, 3, 1);
    g.InsertEdge(3, 4, 2);
    g.InsertEdge(3, 5, 3);

    g.AOV();
    g.AOE();
}
```

## 效能分析

令 V 為頂點數，E 為邊數。

| 操作/方法 | 時間複雜度 | 空間複雜度 | 說明 |
|---|---|---|---|
| AOV (拓撲排序) | O(V + E) | O(V) | 統計入度需要遍歷所有邊。每個節點和邊都只進出佇列一次。 |
| AOE (最早時間 ET) | O(V + E) | O(V) | 依拓撲順序線性更新鄰接邊，複雜度與拓撲排序相同。 |
| AOE (最晚時間 LT) | O(V + E) | O(V) | 反向遍歷拓撲序列並更新前驅節點，時間複雜度同樣與邊數、點數成正比。 |
| 關鍵路徑判定 | O(V + E) | O(1) | 再次遍歷所有的邊進行條件比對。 |

## 測試與驗證

### 輸入

測試資料圖形架構在 main() 函式中建構了 6 個節點（0 ~ 5）的有向圖：

    0 -> 1 (w=3), 0 -> 2 (w=2)
    1 -> 3 (w=4), 2 -> 3 (w=1)
    3 -> 4 (w=2), 3 -> 5 (w=3)

### 輸出

AOV: 0 1 2 3 4 5

AOE : Critical Path:

0 -> 1

1 -> 3

3 -> 5

## 結論

1. 排程優化的核心工具： AOV 與 AOE 網是工程進度規劃（如甘特圖、PERT 圖）底層的核心演算法。

2. 拓撲排序的延伸： 本實作非常優雅地展現了「拓撲排序」不只能用來檢查圖形是否有環（AOV），更是後續計算動態規劃（Dynamic Programming）時間序（AOE）的基礎。

3. 程式碼潛在優化點： 在實務的 AOE 網中，專案的「終點」不一定剛好是編號 n-1 的節點。如果圖有多個終點（出度為 0 的點），一般會手動找出 ET 最大的節點作為整個專案的結束時間，並以此值初始化所有終點的 LT，這樣的程式容錯率與通用性會更高。

# DFS and BFS 解題說明

# 解題策略

本題使用鄰接串列（Adjacency List）來表達無向圖，並透過不同的圖走訪演算法來解決五大核心問題：

### 1. 深度優先搜尋 (DFS)： 

運用遞迴（Stack）。從起點出發，儘可能深地探索每條路徑，直到無法前進時再回溯（Backtrack）到上一個節點，繼續探索其他未走訪的分支。

### 2. 廣度優先搜尋 (BFS)：

運用佇列（Queue）。以「同心圓」的方式由內向外擴展，先走訪所有鄰近的節點，再依序走訪鄰近節點的鄰近節點。

### 3. 連通元件計算 (Connected Components)：

遍歷圖中所有節點，若發現某節點未被走訪，代表發現了一個全新的連通元件。以此節點為起點呼叫一次 DFS（或 BFS），將該元件內所有相連的節點標記為已走訪，並計數加1。

### 4.生成樹邊的產生 (Spanning Tree)：

在 DFS 的走訪過程中，當我們從節點 $u$ 成功走到一個尚未被走訪的節點 $v$ 時，邊 $(u, v)$ 就是該生成樹（Spanning Tree）的一條樹邊（Tree Edge）。

### 5.尋找關節點 (Articulation Points)：

Tarjan 演算法（深度優先尋找雙連通元件）。利用兩個重要的陣列：

        dfn[u]：節點 u 被 DFS 拜訪到的時間戳記（Discovery Time）。

        low[u]：節點 u 經由追溯邊（Back Edge）所能到達的最小時間戳記。

判定條件：

        若 u 為 DFS 樹的根節點：擁有超過一個獨立的子分支（Children $> 1$），則 u 為關節點。

        若 u 不為根節點：若其某個子節點 v 滿足 low[v] >= dfn[u]，代表 v 無法透過非 u 的路徑回到 u 的祖先，則 u 為關節點。
 
## 程式實作

以下為主要程式碼：

### DFS and BFS

```cpp
#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
using namespace std;

class ListGraph {
private:
    int n;
    vector<vector<int>> adj;
    vector<bool> visited;

    vector<int> dfn;
    vector<int> low;
    int dfnCounter;
    vector<bool> isArticulation;

    // DFS
    void DFSUtil(int u) {
        visited[u] = true;
        cout << u << " ";
        for (int v : adj[u]) {
            if (!visited[v]) {
                DFSUtil(v);
            }
        }
    }

    // BFS
    void BFSUtil(int start) {
        queue<int> q;
        visited[start] = true;
        q.push(start);
        while (!q.empty()) {
            int u = q.front();
            q.pop();
            cout << u << " ";
            for (int v : adj[u]) {
                if (!visited[v]) {
                    visited[v] = true;
                    q.push(v);
                }
            }
        }
    }

    // Spanning Tree
    void SpanningTreeUtil(int u) {
        visited[u] = true;
        for (int v : adj[u]) {
            if (!visited[v]) {
                cout << "(" << u << ", " << v << ") ";
                SpanningTreeUtil(v);
            }
        }
    }

    // Biconnected
    void BiconnectedUtil(int u, int parent) {
        dfn[u] = low[u] = ++dfnCounter;
        int children = 0;

        for (int v : adj[u]) {
            if (v == parent) continue;

            if (dfn[v] > 0) {
                low[u] = min(low[u], dfn[v]);
            }
            else {
                children++;
                BiconnectedUtil(v, u);
                low[u] = min(low[u], low[v]);
                if (parent != -1 && low[v] >= dfn[u]) {
                    isArticulation[u] = true;
                }
            }
        }

        if (parent == -1 && children > 1) {
            isArticulation[u] = true;
        }
    }

public:
    ListGraph(int n) {
        this->n = n;
        adj.resize(n);
        visited.resize(n, false);
    }

    void InsertEdge(int u, int v) {
        if (u >= 0 && u < n && v >= 0 && v < n) {
            adj[u].push_back(v);
            adj[v].push_back(u);
        }
    }

    void DFS() {
        fill(visited.begin(), visited.end(), false);
        for (int i = 0; i < n; i++) {
            if (!visited[i]) {
                DFSUtil(i);
            }
        }
    }

    void BFS() {
        fill(visited.begin(), visited.end(), false);
        for (int i = 0; i < n; i++) {
            if (!visited[i]) {
                BFSUtil(i);
            }
        }
    }

    // 計算連通元件
    void ConnectedComponents() {
        fill(visited.begin(), visited.end(), false);
        int count = 0;

        for (int i = 0; i < n; i++) {
            if (!visited[i]) {
                count++;
                cout << "元件 " << count << ": ";
                DFSUtil(i);
                cout << "\n";
            }
        }
        cout << "圖中共有 " << count << " 個連通元件。\n";
    }

    // 產生生成樹的邊
    void SpanningTree() {
        fill(visited.begin(), visited.end(), false);
        cout << "生成樹的邊有: ";
        for (int i = 0; i < n; i++) {
            if (!visited[i]) {
                SpanningTreeUtil(i);
            }
        }
        cout << "\n";
    }

    // 尋找關節點
    void FindArticulationPoints() {
        dfn.assign(n, 0);
        low.assign(n, 0);
        isArticulation.assign(n, false);
        dfnCounter = 0;

        for (int i = 0; i < n; i++) {
            if (dfn[i] == 0) {
                BiconnectedUtil(i, -1);
            }
        }

        cout << "圖中的關節點有: ";
        bool found = false;
        for (int i = 0; i < n; i++) {
            if (isArticulation[i]) {
                cout << i << " ";
                found = true;
            }
        }
        if (!found) cout << "無";
        cout << "\n";
    }
};

int main() {
    ListGraph g(6);

    g.InsertEdge(0, 1);
    g.InsertEdge(0, 2);
    g.InsertEdge(1, 3);
    g.InsertEdge(2, 3);
    g.InsertEdge(3, 4);

    cout << "DFS 走訪: ";
    g.DFS();
    cout << "\n";

    cout << "BFS 走訪: ";
    g.BFS();
    cout << "\n\n";

    g.ConnectedComponents();
    cout << "\n";

    g.SpanningTree();
    cout << "\n";

    g.FindArticulationPoints();

    return 0;
}
```
## 效能分析

令 V 為圖中的頂點數（Vertices），E 為圖中的邊數（Edges）。

| 操作/方法 | 時間複雜度 | 空間複雜度 | 說明 |
|---|---|---|---|
| DFS 走訪 | O(V + E) | O(V) | 每個頂點拜訪一次，每條邊掃描兩次（無向圖）。空間主要為遞迴系統堆疊。 |
| BFS 走訪 | O(V + E) | O(V) | 時間與 DFS 相同。空間主要為儲存節點的 queue。 |
| 連通元件 | O(V + E) | O(V) | 本質上是完整的圖走訪，因此複雜度不變。 |
| 生成樹 | O(V + E) | O(V) | 同 DFS 走訪。 |
| 關節點 (Tarjan) | O(V + E) | O(V) | 僅需一次 DFS 即可計算出所有 dfn 與 low 值，效率極高。 |

## 測試與驗證

### 輸入

6 個節點（0到5）的圖，並加入了以下邊：

        (0, 1), (0, 2), (1, 3), (2, 3), (3, 4)。
        
節點5沒有與任何邊相連。

### 輸出

DFS 走訪: 0 1 3 2 4 5

BFS 走訪: 0 1 2 3 4 5

元件 1: 0 1 3 2 4

元件 2: 5

圖中共有 2 個連通元件。

生成樹的邊有: (0, 1) (1, 3) (3, 2) (3, 4)

圖中的關節點有: 3

## 結論

1. 基礎走訪的應用： DFS 與 BFS 是圖論演算法的基石。透過維護一個簡單的 visited 陣列，就能夠延伸出計算連通元件與建立生成樹的功能。

2. Tarjan 演算法的優雅： 傳統若要尋找關節點，必須暴力嘗試移除每個節點再做 DFS，時間複雜度高達 O(V ㄨ (V + E))。而本實作採用的 Tarjan 演算法，巧妙利用了 DFS 樹的時間戳記特性，將難題優化至單次走訪 O(V + E) 即可完成，是圖論中非常經典且高效的進階演生技巧。

# MST 解題說明

# 解題策略

最小生成樹的目標是在一個帶權重的無向連通圖中，找到一個包含所有頂點且邊權重總和最小的樹狀子圖。本題採用了兩種不同的「貪心法」（Greedy Approach）來達成：

### 1. Prim's 演算法（以「頂點」為核心擴展）

類似於 BFS，它從一個起點出發，每次都選擇與當前生成樹相連、且權重最小的邊，並將該邊對應的未知頂點拉進樹中。
    
    1. 使用一個 key 陣列記錄每個頂點到當前 MST 的最短距離，初始化為無限大（1e9），起點（節點 0）設為 0。
    2. 使用最小優先佇列（priority_queue，搭配 greater<>）來動態維護並提取目前權重最小的邊。
    3. 每次從 pq 彈出距離最小的頂點 u（並標記 inMST[u] = true），接著遍歷其鄰接節點 v。若 v 尚未進入 MST 且邊權重 w < key[v]，則更新 key[v]、修改 parent[v] 指向 u，並將新的權重與頂點推入 pq。

### 2. Kruskal's 演算法（以「邊」為核心擴展）

拋開頂點的概念，直接將圖中所有的邊依權重由小到大排序。每次挑選權重最小的邊，只要這條邊不會與已選的邊形成環（Cycle），就將它納入 MST 中。

    1. 收集圖中所有的邊（透過 u < p.first 避免重複收集無向邊），並依權重排序（sort）。
    2. 使用互斥集合資料結構來高效率偵測環：
        find(x)：尋找節點 x 所屬集合的根節點，並使用路徑壓縮（Path Compression）將結構扁平化。
        unite(a, b)：若兩節點的根節點不同（代表不連通，不會形成環），則利用按秩合併（Union by Rank）將小樹掛在大樹下，合併兩個集合。
    3. 依序檢查排序後的邊 (u, v)，若 find(u) != find(v)，代表加入此邊不會形成環，隨即呼叫 unite(u, v) 並將此邊印出。

## 程式實作

以下為主要程式碼：

```cpp
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
```

## 效能分析

令 V 為頂點數，E 為邊數。

| 操作/方法 | 時間複雜度 | 空間複雜度 | 適用場景與說明 |
|---|---|---|---|
| Prim (Min-Heap) | O(E log V) | O(V + E) | 稠密圖（Dense Graph，邊很多）。因為它以點為中心向外擴展，pq 的操作次數與邊數相關，但頂點處理比較集中。 |
| Kruskal (Disjoint-Set) | O(E log V) | O(V + E) | 稀疏圖（Sparse Graph，邊很少）。時間瓶頸主要卡在最一開始對所有邊進行排序（O(E log E)），隨後的 Union-Find 操作近乎線性 O(E x /alpha(V))。 |

## 測試與驗證

### 輸入

測試資料圖形架構在 main() 函式中建構了一個包含 5 個節點（0 ~ 4）的無向帶權圖：

    (0, 1, w=2)
    (0, 2, w=4)
    (1, 2, w=1)
    (1, 3, w=7)
    (2, 4, w=3)

### 輸出

Prim MST:

0 - 1

1 - 2

1 - 3

2 - 4

Kruskal MST:

1 - 2

0 - 1

2 - 4

1 - 3

## 結論

Greedy 的示範： 兩個演算法都體現了「每一步都選當前最好」的貪心特質，且皆能保證求得全域最佳解（Global Optimum）。

# Shortest Paths 解題說明

# 解題策略

Dijkstra 演算法的核心思想是貪心法（Greedy）與路徑鬆弛（Relaxation）。它從指定的起點出發，每次都挑選目前距離起點最近的節點，並以此節點為跳板去更新其他鄰接節點的距離。

核心實作三步驟：

    1. 初始化與防禦設定：
    使用 dist 陣列記錄起點到各點的最短距離，初始值皆設為無限大（INT_MAX），唯獨起點 dist[start] = 0。
    使用 priority_queue<pair<int,int>, ..., greater<>>（最小堆疊）來儲存 {當前總距離, 節點編號}。這樣做可以保證每次 pq.top() 彈出的都是目前路徑最短的節點。
    2. 核心鬆弛操作 (Relaxation)：
    當彈出節點 u 後，遍歷 u 的所有有向邊鄰居 v（邊權重為 w）。
    檢查條件： 如果「起點到 u 的距離 + u 到 v 的權重」小於「目前起點到 v 的已知距離」，即： if (dist[v] > dist[u] + w)
    若條件成立，代表找到了一條更短的白老鼠路徑，隨即更新 dist[v] 並將新型態的 {dist[v], v} 推入 pq 中。
    3. 無效路徑剪枝（優化建議）：當一個節點被多次更新並推入 pq 時，舊的、較長的距離仍會殘留在 pq 中。雖然你的程式碼能正確跑出結果，但在大型圖形中，建議在 while 迴圈剛彈出 [d, u] 時，加上一行剪枝判斷：if (d > dist[u]) continue;，這能避免重複處理已經過期的舊節點，大幅提升執行效率。
    
## 程式實作

以下為主要程式碼：

```cpp
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
```

## 效能分析

令 V 為頂點數，E 為邊數。

| 項目 | 複雜度 | 說明 |
|---|---|---|
| 時間複雜度 | O(E log V) | 每個頂點與其鄰邊都會被檢查。在最壞情況下，每條邊都可能觸發一次 pq 的插入操作，而每次 pq 調整的時間為 O(log V)。 |
| 空間複雜度 | O(V + E)  | 鄰接串列 adj 佔用 O(V + E) 空間；dist 陣列與 pq 最多佔用 O(V) 空間。 |

## 測試與驗證

### 輸入

在 main() 函式中，你建立了一個有 5 個節點的有向圖（Directed Graph），其結構與權重如下：

    0 -> 1 (權重 2)
    0 -> 2 (權重 4)
    1 -> 3 (權重 7)
    1 -> 2 (權重 1)
    2 -> 4 (權重 3)

### 輸出

Shortest Paths:

0 = 0

1 = 2

2 = 3

3 = 9

4 = 6

## 結論

1. 動態規劃與貪心法的結合： Dijkstra 演算法利用 Min-Heap 的特質，每次都鎖定當前「已確定是最短路徑」的點向外延伸，是計算單源最短路徑。

2. 致命盲點： 本演算法的大前提是「圖中不能包含負權重的邊」。如果圖中存在負權邊，Dijkstra 的貪心策略將會失效（因為提早被標記確定最短的點，後面可能會因為負權邊而變得更短）。若遇到負權邊，必須改用 Bellman-Ford 或 SPFA 演算法。

