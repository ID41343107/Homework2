# 41343107 41343109

作業二 graph

## 解題說明
本作業以鄰接串列實作圖形結構，分別完成：
1. DFS / BFS 走訪
2. 最小生成樹（Prim / Kruskal）
3. 最短路徑（Dijkstra）
4. AOV / AOE 活動網路（拓撲排序與關鍵路徑）

對應程式碼位於 `src/` 內的各個主程式檔案。

## 解題策略
1. `DFS and BFS main.cpp`
   - 使用無向圖的鄰接串列。
   - DFS 使用遞迴；BFS 使用佇列。
2. `MST main.cpp`
   - 使用加權無向圖。
   - Prim 以最小優先佇列選邊；Kruskal 以排序邊集合與 Union-Find 合併集合。
3. `Shortest Paths main.cpp`
   - 使用加權有向圖。
   - Dijkstra 以最小優先佇列更新最短距離。
4. `AOV and AOE main.cpp`
   - 以加權有向圖完成 AOV 拓撲排序。
   - AOE 依拓撲序計算最早事件時間（ET）與最遲事件時間（LT），找出關鍵路徑。
5. `header.h`
   - 集中常用標頭，供各檔案引用。

## 程式實作
各檔案皆以 `ListGraph` 類別封裝圖形資料與演算法：
- `InsertEdge`：插入邊（依題目為有向或無向）。
- `DFS` / `BFS`：走訪輸出節點順序。
- `Prim` / `Kruskal`：輸出最小生成樹的邊集合。
- `Dijkstra`：輸出起點到各節點最短距離。
- `AOV` / `AOE`：輸出拓撲序與關鍵路徑。

## 效能分析（AI 分析）
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

## 分工
41343107
