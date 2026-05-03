# Graph

## 解題說明
本作業依照 src 內的程式碼，實作圖形走訪、最小生成樹、最短路徑、以及 AOV/AOE 的
拓樸排序與關鍵路徑。各功能分別在四個 main.cpp 中示範，直接以程式內建的範例圖執行
並輸出結果。

## 解題想法
1. DFS / BFS：以 adjacency list 儲存圖，DFS 使用遞迴走訪，BFS 使用佇列並補足非連通圖。
2. 最小生成樹：同時保存 edge list 與 adjacency list；Kruskal/Sollin 透過 disjoint set
   避免成環，Prim 使用 min-heap 擴張。
3. 最短路徑：Dijkstra 使用 priority queue；Bellman-Ford 反覆鬆弛並檢查負權迴圈；
   Floyd-Warshall 以矩陣動態規劃更新所有點對距離。
4. AOV/AOE：拓樸排序以 indegree + queue 完成；AOE 依拓樸序計算 ve/vl，輸出關鍵路徑
   與專案完成時間。

## 程式實作

以下為主程式

```cpp
// DFS / BFS
Graph g(6);
g.addEdge(0, 1);
g.addEdge(0, 2);
g.addEdge(1, 3);
g.addEdge(1, 4);
g.addEdge(2, 5);
g.DFS();
g.BFS(0);

// Minimum Spanning Tree
Graph g2(6);
g2.addEdge(0, 1, 4);
g2.addEdge(0, 2, 3);
g2.addEdge(1, 2, 1);
g2.addEdge(1, 3, 2);
g2.addEdge(2, 3, 4);
g2.addEdge(3, 4, 2);
g2.addEdge(4, 5, 6);
g2.kruskalMST();
g2.primMST();
g2.sollinMST();

// Shortest Paths
DijkstraGraph g3(5);
g3.addEdge(0, 1, 6);
g3.addEdge(0, 3, 7);
g3.addEdge(1, 2, 5);
g3.addEdge(1, 4, 2);
g3.addEdge(2, 3, 3);
g3.addEdge(3, 4, 1);
g3.dijkstra(0);

BellmanGraph g4(5);
g4.addEdge(0, 1, 6);
g4.addEdge(0, 3, 7);
g4.addEdge(1, 2, 5);
g4.addEdge(1, 4, 2);
g4.addEdge(2, 3, 3);
g4.addEdge(3, 4, 1);
g4.bellmanFord(0);

FloydGraph g5(5);
g5.addEdge(0, 1, 6);
g5.addEdge(0, 3, 7);
g5.addEdge(1, 2, 5);
g5.addEdge(1, 4, 2);
g5.addEdge(2, 3, 3);
g5.addEdge(3, 4, 1);
g5.floydWarshall();

// AOV / AOE
Graph g6(6);
g6.addEdge(0, 1, 3);
g6.addEdge(0, 2, 2);
g6.addEdge(1, 3, 2);
g6.addEdge(2, 3, 1);
g6.addEdge(3, 4, 4);
g6.addEdge(4, 5, 2);
vector<int> topo = g6.topologicalSort();
cout << "拓樸排序：";
for (int x : topo) cout << x << " ";
cout << "\n";
g6.criticalPath();
```

## 效能分析

### 時間複雜度
- DFS / BFS：O(V + E)
- Kruskal：O(E log E)
- Prim（priority queue）：O((V + E) log V)
- Sollin：O(E log V)
- Dijkstra（priority queue）：O((V + E) log V)
- Bellman-Ford：O(VE)
- Floyd-Warshall：O(V^3)
- Topological Sort / Critical Path：O(V + E)

### 空間複雜度
- adjacency list 與 edge list：O(V + E)
- Floyd-Warshall dist matrix：O(V^2)
- 其餘輔助結構（queue、priority_queue、disjoint set、dist）：O(V)

### 效能量測
本次以固定小型範例圖進行示範，未額外進行大規模效能量測。

## 測試與驗證

### 輸入
各程式皆在 main 中直接建構範例圖：
- DFS/BFS：6 個節點的無向樹狀圖（0-1-3/4、0-2-5）。
- MST：6 個節點、7 條帶權無向邊。
- Shortest Paths：5 個節點、6 條帶權邊。
- AOV/AOE：6 個節點、6 條帶權有向邊。

### 輸出
#### DFS / BFS
```
DFS traversal: 0 1 3 4 2 5 
BFS traversal: 0 1 2 3 4 5 
```

#### Minimum Spanning Tree
```
========== Kruskal 最小生成樹 ==========
1 - 2 : 1
1 - 3 : 2
3 - 4 : 2
0 - 2 : 3
4 - 5 : 6
總成本 = 14

========== Prim 最小生成樹 ==========
2 - 1
0 - 2
1 - 3
3 - 4
4 - 5
總成本 = 14

========== Sollin 最小生成樹 ==========
0 - 2 : 3
1 - 2 : 1
1 - 3 : 2
3 - 4 : 2
4 - 5 : 6
總成本 = 14
```

#### Shortest Paths
```
========== Dijkstra 最短路 ==========
0 = 0
1 = 6
2 = 10
3 = 7
4 = 8

========== Bellman-Ford ==========
0 = 0
1 = 6
2 = 11
3 = 7
4 = 8

========== Floyd-Warshall ==========
0 6 11 7 8 
INF 0 5 8 2 
INF INF 0 3 4 
INF INF INF 0 1 
INF INF INF INF 0 
```

#### AOV / AOE
```
拓樸排序：0 1 2 3 4 5 

========== AOE 關鍵路徑 ==========
0 -> 1 (time=3)
1 -> 3 (time=2)
3 -> 4 (time=4)
4 -> 5 (time=2)
專案完成時間 = 11
```

## 申論及開發報告

### 心得討論
本次一次實作多種經典圖論演算法，體會到資料結構選擇（adjacency list、edge list、
矩陣）對演算法流程的影響；另外也注意到 DFS/BFS 與 MST 皆需處理非連通圖或避免成環。
藉由執行範例圖輸出結果，可以快速驗證演算法正確性。

### 總結
程式完整涵蓋 DFS/BFS、最小生成樹、最短路徑、AOV/AOE 等核心題目，並透過固定範例
驗證結果。後續若需要擴充，只要替換輸入圖資料即可套用同一套邏輯。
