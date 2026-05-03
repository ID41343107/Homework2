
# 41343109 41343107

作業二

## 解題說明

完成一個多項式的加法乘法。

### 解題策略

1. class Term
儲存單一項的資料結構：float coef; int exp;。

2. Polynomial::Polynomial()
建構子：初始化動態陣列（初始 capacity = 2）與項數為 0。

3. Polynomial::~Polynomial()
解構子：釋放動態配置的 termArray。

4. void Polynomial::look(int n)
確保內部陣列至少有容量 n，不足時擴增並複製既有項目。

5. void Polynomial::newTerm(const float newcoef, int newexp)
新增一項（跳過係數為 0），若容量不足自動擴增並將項附加到陣列末端。

6. Polynomial Polynomial::Add(const Polynomial &poly)
回傳兩多項式相加的結果：先複製目前物件的項，再把 poly 的項合併（相同指數則累加係數）。

7. Polynomial Polynomial::mult(const Polynomial &poly)
回傳兩多項式相乘的結果：對每對項做相乘（係數相乘、指數相加），合併相同指數的項。

8. float Polynomial::Eval(float f)
在 x = f 評值多項式，對每項計算 coef * pow(f, exp) 並累加回傳總和。

9. istream& operator>>(istream& in, Polynomial& poly)
從輸入讀入多項式：先讀入項數 n，接著讀 n 組 coef exp 並呼叫 newTerm 插入。

10. ostream& operator<<(ostream& out, const Polynomial& poly)
輸出多項式為可讀字串：處理正負號與指數為 0 的情況，依陣列順序輸出每一項。



## 程式實作

以下為主要程式碼：

```cpp

#include<iostream>
#include <cmath>   
#include <algorithm> 
using namespace std;
class Polynomial; //為了讓Term類別可以找到Polynomial類別
class Term {
	friend Polynomial;
public:
	float coef;
	int exp;
};
class Polynomial {
private:
	Term* termArray; //宣告一個名為 termArray 的指標，型別是 Term*
	int capacity;
	int terms;
public:
	Polynomial() :capacity(2), terms(0) {
		termArray = new Term[capacity]; //配置 capacity 個連續的 Term 物件給 termArray
	}

	~Polynomial() { 
		delete[] termArray; //清除
	}
	void look(int n) {
		if (capacity >= n)return;//足夠容量就直接離開
		int c = max(capacity * 2, n);//足夠容量就直接離開
		Term* temp = new Term[c];
		copy(termArray, termArray + terms, temp);//現有 terms 個元素複製到新陣列
		delete[]termArray;
		termArray = temp;//指向新陣列
		capacity = c;//更新容量
	}
	Polynomial Add(const Polynomial &poly) {
		Polynomial result;
		for (int i = 0; i < terms; i++) {
			result.newTerm(termArray[i].coef, termArray[i].exp);//目前物件中所有的項逐一加入到 result。
		}
		for (int j = 0; j < poly.terms; j++) {
			int exp = poly.termArray[j].exp;
			double coef = poly.termArray[j].coef;
			int find = 0;
			for (int i = 0; i < result.terms; i++) {
				if (result.termArray[i].exp == exp) //如果指數相同
				{
					result.termArray[i].coef += coef; //相加
					find = 1;
				}
			}
			if (find == 0 && coef != 0) { //如果沒找到並且係數不為0
				result.look(result.terms + 1);//新增一項
				result.termArray[result.terms].coef = coef;//把值放進去
				result.termArray[result.terms].exp = exp;
				result.terms++;
			}
		}
		return result;
	}
	Polynomial mult(const Polynomial &poly) {
		Polynomial result;
		for (int i = 0; i < terms; i++) {
			for (int j = 0; j < poly.terms; j++) {
				float coef = termArray[i].coef * poly.termArray[j].coef;
				int exp = termArray[i].exp + poly.termArray[j].exp;
				int find = 0;
				for (int k = 0; k < result.terms; k++) {
					if (result.termArray[k].exp == exp) {
						result.termArray[k].coef += coef;
						find = 1;
					}
				}
				if (find==0 && coef != 0) {
					result.look(result.terms + 1);//新增一項
					result.termArray[result.terms].coef = coef;//把值放進去
					result.termArray[result.terms].exp = exp;
					result.terms++;
				}
			}
		}
		return result;
	}
	float  Eval(float f) {
		float sum = 0;
		for (int i = 0; i < terms; i++) {
			sum += termArray[i].coef * pow(f, termArray[i].exp);
		}
		return sum;
	}
	void newTerm(const float newcoef, int newexp) {
		if (newcoef == 0)return;
		if (terms == capacity) {
			capacity *= 2;//把容量變為原來的兩倍
			Term* temp = new Term[capacity];//新的動態記憶
			copy(termArray, termArray + terms, temp);//將舊的資料放去新的
			delete[]termArray;//把舊資料刪除
			termArray = temp;//新資料放到舊資料的位址
		}
		termArray[terms].coef = newcoef;
		termArray[terms].exp = newexp;
		terms++;
	}
	friend istream& operator>>(istream& in, Polynomial& poly);
	friend ostream& operator<<(ostream& out, const Polynomial& poly);
};

istream& operator>>(istream& in, Polynomial& poly) {
	float coef;
	int exp;
	int n;
	cin >> n;
	poly.terms = 0;
	while (n--) {
		in >> coef >> exp;
		poly.newTerm(coef, exp);
	}
	return in;
}
ostream& operator<<(ostream& out, const Polynomial& poly) {
	for (int i = 0; i < poly.terms; i++) {
		if (poly.termArray[i].coef < 0 && i>0)out << "-";
		else if (poly.termArray[i].coef > 0 && i > 0)out << "+";
		out << poly.termArray[i].coef;
		if (poly.termArray[i].exp != 0)out << "X^" << poly.termArray[i].exp;
	}
	return out;
}
int main() {
	Polynomial p1, p2;
	cin >> p1 >> p2;
	Polynomial sum = p1.Add(p2);
	Polynomial p = p1.mult(p2);
	float x;
	cin >> x;
	cout << "P1+P2=" << sum << endl;
	cout << "P1*P2=" << p << endl;
	cout << "P1(x)=" << p1 << endl;
	cout << "P1(" << x << ")=" << p1.Eval(x) << endl;
	return 0;
}

```

## 效能分析(AI分析)
| 函式 | 時間複雜度 | 空間複雜度 |
|:---:|:---:|:---:|
| Constructor / Destructor | O(1) | O(1) |
| look(int n) |  O(1) |  O(capacity) | 
| newTerm(const float, int) | O(1) | O(1) | 
| Add(const Polynomial&) | O(m*n + m^2) | O(n + m) |
| mult(const Polynomial&) | O((n*m)^2) | O(n * m) |
| Eval(float) | O(n)（若 pow 當 O(1)） | O(1) | 
| operator>>(istream&, Polynomial&) | O(k) | O(k) | 
| operator<<(ostream&, const Polynomial&) | O(n) | O(1) | 
| main（整體） | O(m*n + m^2 + (n*m)^2 + n) | O(n*m + n + m) | 


## 測試與驗證

### 測試案例





### 結論
1. 功能正確：程式實作了 Polynomial 類別，新增項（newTerm）、相加（Add）、相乘（mult）、附值（Eval）以及輸入/輸出運算子，能完成多項式的基本運算與顯示。
2. 記憶體管理：使用動態陣列（look/newTerm），可正確保留既有項目並避免儲存係數為 0 的項。
3. 複雜度摘要（設 n = this.terms, m = poly.terms）：
Add：O(m*n + m^2)，空間 O(n + m)。
mult： O((nm)^2)，空間 O(nm)。
Eval：O(n)，空間 O(1)。

## 心得討論
在寫add 和 mult函式時出現了"block 0x000001e5087e9dd0"這個錯誤導致無法成功執行 上網查資料說C++ 預設是「淺拷貝」，容易導致記憶體重複刪除。
因此改成用「指標參考」傳遞，而不是「值傳遞」這樣就能解決問題;

