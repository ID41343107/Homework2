# Graph

## 一. 圖形的基本定義 (Basic Definitions)

#### 組成：
    
  圖 G = (V, E)，由頂點集合 V（有限非空集合）與邊集合 E 組成。

### 分類：

#### 1. 有向 / 無向
  
    有向：A → B（單向）
    無向：A — B（雙向）
    
#### 2. 有權重 / 無權重

    有權重：邊有數字（距離、成本）
    無權重：只有連或不連
   
#### 3. 限制：不允許自環 (Self-loop) 與 重邊 (Multigraph)。

## 二. 圖形的表示法 (Representations)

#### 🔹相鄰矩陣 (Adjacency Matrix)：
  
  使用 $n \times n$ 的二維陣列。優點是能快速判斷兩點是否有邊，缺點是對於很少邊的圖會浪費空間 $O(n^2)$。
  
#### 🔹相鄰串列 (Adjacency List)：
  
  使用陣列搭配鏈結串列。空間複雜度為 $O(n+e)$，適合表示邊數較少的稀疏圖。

#### 🔹相鄰多重串列 (Adjacency Multi-lists)：
  
  常用於無向圖，每條邊只記錄一次，但同時連結到兩個頂點的串列中。

## 三. 搜尋演算法 (Graph Operations)

#### 🔹深度搜尋 (DFS)：

利用「回溯」機制探索，通常用遞迴或堆疊 (Stack) 實作。

#### 🔹廣度搜尋 (BFS)：

按層級往外擴散搜尋，利用佇列 (Queue) 實作。

#### 🔹關節點 (Articulation Points)：

若移除該頂點會導致圖不連通，則稱之為關節點。

## 四. 最小成本生成樹 (Minimum Cost Spanning Trees)

這類演算法目標是在連通圖中找到包含所有頂點且權重總和最小的樹：

🔹Kruskal：每次挑選權重最小的邊，並確保不形成環。

🔹Prim：從單一頂點開始，每次加入一條連接「已選頂點」與「未選頂點」中權重最小的邊。

🔹Sollin：每一輪讓森林中的每棵樹同時選擇最小邊進行合併。

## 五. 最短路徑演算法 (Shortest Paths)

🔹Dijkstra：解決單一源點到其餘所有點的最短路徑（邊權重須為非負）。

🔹Bellman-Ford：可處理包含負權重的邊。

🔹Floyd-Warshall：計算所有頂點對之間的最短路徑 $O(n^3)$。

## 六. 活動網路 (Activity Networks)

### 1. AOV 網路 (Activity on Vertex)：

頂點代表活動，邊代表先後順序。透過拓撲排序 (Topological Sort) 可判斷計畫是否有環（矛盾）並排定執行順序。

### 2. AOE 網路 (Activity on Edge)：

邊代表活動且帶有工期。用於計算關鍵路徑 (Critical Path)，找出哪些活動若延遲會影響整個工程的完工時間。
