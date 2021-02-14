/*
Created by Jeonghan Kim

Binary search with array-type quick sort
Quick sort load : O(nlogn)
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

	bool operator>(const Data& r) const {
		if (ts != r.ts) return ts > r.ts;
		return id < r.id;
	}

	bool operator<=(const Data& r) const {
		if (ts != r.ts) return ts <= r.ts;
		return id >= r.id;
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

struct Idx {
	int first;
	int last;
	Idx() {}
	Idx(int f, int l) : first(f), last(l) {}
};

struct Stack {
	int sp;
	Idx buf[maxN<<1];

	void push(int first, int last) {
		Idx& idx = buf[sp++];
		idx.first = first, idx.last = last;
	}

	void pop(int& first, int& last) {
		Idx& idx = buf[--sp];
		first = idx.first, last = idx.last;
	}
};
Stack stack;

void setup(int tc) {
	srand(tc);

	f(i, 0, maxN) {
		rint k = i / maxN;
		gt[i] = { rand(),rand()%100 };
		sorted[i] = gt[i];
		arr[i] = &gt[i];
	}

	sort(&sorted[0], &sorted[maxN]);
}

template <typename T>
void qsort(int first, int last, T** input) {
	rint i, j, pivot;

	stack.sp = 0;
	stack.push(first, last);

	while (stack.sp > 0) {
		stack.pop(first, last);

		if (first < last) {
			pivot = first;
			i = first;
			j = last;

			rint mid = (i + j) >> 1;
			if (mid != pivot) {
				T* temp = input[mid];
				input[mid] = input[pivot];
				input[pivot] = temp;
			}

			while (i < j) {
				while (*input[i] <= *input[pivot] && i < last) i++;
				while (*input[j] > *input[pivot])	j--;
				if (i < j) {
					T* temp = input[i];
					input[i] = input[j];
					input[j] = temp;
				}
			}

			if (j != pivot) {
				T* temp = input[j];
				input[j] = input[pivot];
				input[pivot] = temp;
			}
			stack.push(first, j - 1);
			stack.push(j + 1, last);
		}
	}
}

void Test_qsort() {
	qsort(0, maxN-1, arr);
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

void checkQsort(Data* arr[], int & cnt) {
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
		Test_qsort();
		bench[0] += (clock() - ts);
		checkQsort(arr, ecnt[0]);

		ts = clock();
		Test_binarysearch(ecnt[1]);
		bench[1] += (clock() - ts);
	}

	printf("Quick sort : %.3f, Binary search : %.3f\n",  
			double(bench[0]) / CLOCKS_PER_SEC, 
			double(bench[1]) / CLOCKS_PER_SEC);
	cout << ecnt[0] << ", " << ecnt[1] << endl;

	return 0;
}