/*
Created by Jeonghan Kim

Binary search with array-type heap sort
Heap sort load : O(nlogn) when pushing, O(logn) when popping
*/

#include <iostream>
#include <algorithm>
#include <cstdlib>
#include <ctime>
using namespace std;

#define rt register
#define rint rt int
#define f(i,a,b) for(rint i=a;i<b;++i)
const int maxN = 100000;

struct Data {
	int ts;
	int id;

	bool operator<(const Data& r) const {
		if (ts != r.ts) return ts < r.ts;
		return id > r.id;
	}

	bool operator==(const Data& r) const {
		return ts == r.ts && id == r.id;
	}

	void print() {
		cout << ts << "," << id << endl;
	}
};

Data gt[maxN];
Data sorted[maxN];
Data* arr[maxN];

// Return index immediate after possible match found
template <typename T>
int binsearch(rint s, rint e, T* arr[], T *v) {
	rint ret = 0;
	while (s <= e) {
		rint m = (s + e) >> 1;
		if (*arr[m] == *v)  return m;
		else if (*arr[m] < *v) s = m + 1;
		else e = m - 1;
	}
	return ret;
}

// Return lowest index when duplicated
template <typename T>
int binsearchL(rint s, rint e, T* arr[], T* v) {
	rint ret = 0;
	while (s <= e) {
		rint m = (s + e) >> 1;
		if (*arr[m] == *v) ret = m, e = m - 1;
		else if (*arr[m] < *v) s = m + 1;
		else e = m - 1;
	}
	return ret;
}

// Return highest index when duplicated
template <typename T>
int binsearchH(rint s, rint e, T* arr[], T* v) {
	rint ret = 0;
	while (s <= e) {
		rint m = (s + e) >> 1;
		if (*arr[m] == *v) ret = m, s = m + 1;
		else if (*arr[m] < *v) s = m + 1;
		else e = m - 1;
	}
	return ret;
}

template <typename T>
struct Heap {
	T* heap[maxN];
	int sz;

	void push(T* value) {
		heap[sz] = value;

		int current = sz;
		while (current > 0 && *heap[current] < *heap[(current - 1) >> 1]){
			int parent = (current - 1) >> 1;
			T* temp(heap[parent]);
			heap[parent] = heap[current]; 
			heap[current] = temp;
			current = parent;
		}
		sz++;
	}

	void pop() {
		if (sz == 0) return;
		sz--;
		heap[0] = heap[sz];

		int current = 0;
		int lchild = 1;
		int rchild = 2;

		while (lchild < sz) {
			int child;
			if (rchild == sz)	child = lchild;
			else child = *heap[lchild] < *heap[rchild] ? lchild : rchild;
			if (*heap[current] < *heap[child]) break;

			T* temp(heap[child]);
			heap[child] = heap[current];
			heap[current] = temp;
			current = child;
			lchild = (current << 1) + 1;
			rchild = lchild + 1;
		}
	}
};

Heap<Data> hp;
void setup(int tc) {
	srand(tc);

	f(i, 0, maxN) {
		rint k = i / maxN;
		gt[i] = { rand(),rand()%100 };
		sorted[i] = gt[i];
	}

	sort(&sorted[0], &sorted[maxN]);
}
void Test_heap() {
	hp.sz = 0;
	f(i, 0, maxN) hp.push(&gt[i]);
	f(i, 0, maxN) {
		arr[i] = hp.heap[0];
		hp.pop();
	}
}

void Test_binarysearch(int & cnt) {
	f(i, 0, maxN) {
		bool bValid = true;
		rint idx[3];
		idx[0] = binsearchL(0, maxN - 1, arr, &sorted[i]);
		idx[1] = binsearch(0, maxN - 1, arr, &sorted[i]);
		idx[2] = binsearchH(0, maxN - 1, arr, &sorted[i]);

		f(j, 1, 3) {
			if (idx[j] < idx[j - 1]) {
				bValid = false;
				break;
			}
		}
		if (bValid == false) cnt++;
	}
}

void checkHeapsort(Data* arr[], int & cnt) {
	f(i, 0, maxN) {
		if (!(sorted[i] == *arr[i])) cnt++;
	}
}

int main() {
	clock_t ts;
	int ecnt[2] = { 0, };
	clock_t bench[2] = { 0, };
	f(i,0,10) {
		setup(i);
		ts = clock();
		Test_heap();
		bench[0] += (clock() - ts);
		checkHeapsort(arr, ecnt[0]);

		ts = clock();
		Test_binarysearch(ecnt[1]);
		bench[1] += (clock() - ts);
	}

	printf("Heap sort : %.3f, Binary search : %.3f\n",  
			double(bench[0]) / CLOCKS_PER_SEC, 
			double(bench[1]) / CLOCKS_PER_SEC);
	cout << ecnt[0] << ", " << ecnt[1] << endl;

	return 0;
}