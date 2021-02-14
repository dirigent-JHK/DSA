/*
Created by Jeonghan Kim

Binary search with array-type insertion sort
Useful when number of data is small because of O(n^2) load
*/

#include <iostream>
#include <algorithm>
#include <cstdlib>
#include <ctime>
using namespace std;

#define rt register
#define rint rt int
#define f(i,a,b) for(rint i=a;i<b;++i)
const int maxN = 10000;

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
int len;

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

//insertion sort starting from back
void insort_back(Data *v) {
	rint i = len;
	for (; i > 0 && *v < *arr[i-1] ; --i);
	for (rint j = len; j > i; --j) arr[j] = arr[j - 1];
	arr[i] = v, len++;
}

//insertion sort starting from front
void insort_front(Data* v) {
	rint i = 0;
	for (; i < len && *arr[i] < *v; ++i);
	for (rint j = len; j > i; --j) arr[j] = arr[j - 1];
	arr[i] = v, len++;
}

void setup(int tc) {
	len = 0;
	srand(tc);

	f(i, 0, maxN) {
		rint k = i / maxN;
		gt[i] = { rand(),rand()%100 };
		sorted[i] = gt[i];
	}

	sort(&sorted[0], &sorted[maxN]);
}
void Test_insortback() {
	len = 0;
	f(i, 0, maxN) 	insort_back(&gt[i]);
}
void Test_insortfront() {
	len = 0;
	f(i, 0, maxN) 	insort_front(&gt[i]);
}

void Test_binarysearch(int & cnt) {
	f(i, 0, maxN) {
		bool bValid = false;
		rint idx[3];
		idx[0] = binsearchL(0, maxN - 1, arr, &sorted[i]);
		idx[1] = binsearch(0, maxN - 1, arr, &sorted[i]);
		idx[2] = binsearchH(0, maxN - 1, arr, &sorted[i]);

		f(j, 0, 3) {
			if (idx[j] == i) {
				bValid = true;
				break;
			}
		}
		f(j, 1, 3) {
			if (idx[j] < idx[j - 1]) {
				bValid = false;
				break;
			}
		}
		if (bValid == false) cnt++;
	}
}

void checkInsort(Data* arr[], int & cnt) {
	f(i, 0, maxN) {
		if (!(sorted[i] == *arr[i])) cnt++;
	}
}

int main() {

	clock_t ts;

	int ecnt[10] = { 0, };
	clock_t bench[10] = { 0, };
	f(i,0,10) {
		setup(i);
		ts = clock();
		Test_insortback();
		bench[0] += (clock() - ts);
		checkInsort(arr, ecnt[0]);

		ts = clock();
		Test_insortfront();
		bench[1] += (clock() - ts);
		checkInsort(arr, ecnt[1]);
		
		ts = clock();
		Test_binarysearch(ecnt[2]);
		bench[2] += (clock() - ts);
	}

	printf("Insertion sort from Back : %.3f, Insertion sort from Front : %.3f, Binary search : %.3f\n",  
			double(bench[0]) / CLOCKS_PER_SEC, 
			double(bench[1]) / CLOCKS_PER_SEC, 
			double(bench[2]) / CLOCKS_PER_SEC);
	cout << ecnt[0] << ", " << ecnt[1] << ", " << ecnt[2] << endl;

	return 0;
}