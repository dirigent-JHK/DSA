/*
Created by Jeonghan Kim

Keep heap structure with values changed incrementally
Backjoon online problem 14427
*/

#include <iostream>

using namespace std;

#define rt register
#define rint rt int
#define f(i,a,b) for(rint i=a;i<b;++i)
#define fe(i,a,b) for(rint i=a;i<=b;++i)

const int maxN = 100000 + 1;

struct Data {
	int v;
	int idx;
	int hid;
	long long sv;

	bool operator<(const Data& r) const {
#if 0
		if (v != r.v) return v < r.v;
		return idx < r.idx;
#else
		return sv < r.sv;
#endif
	}
} sd[maxN];


// Heap buffer could be managed partition by partition externally 
// In this example, only one part is used, so Data *hbuf[maxN] would be sufficient.
Data *hbuf[2][maxN];

template <typename T>
struct Heap {
	T** heap;
	int sz;

	void init(T** buf) { sz = 1, heap = buf; }

	void update(T* v) {
		v->sv = ((long long)(v->v) << 17) + v->idx;
		if (up(v->hid) == v->hid) down(v->hid);
	}

	int up(int curr) {
		heap[curr]->hid = curr;
		while (curr > 1 && *heap[curr] < *heap[curr >> 1]) {
			rint pa = (curr >> 1);
			T* temp = heap[curr];
			heap[curr] = heap[pa];
			heap[pa] = temp;

			heap[curr]->hid = curr;
			heap[pa]->hid = pa;
			curr = pa;
		}

		return curr;
	}

	void down(int curr) {
		heap[curr]->hid = curr;
		rint lc = (curr << 1);

		while (lc < sz) {
			rint rc = lc + 1; 
			rint ch;
			if (rc == sz) ch = lc;
			else ch = (*heap[lc] < *heap[rc]) ? lc : rc;

			if (*heap[curr] < *heap[ch]) break;

			T* temp = heap[curr];
			heap[curr] = heap[ch];
			heap[ch] = temp;

			heap[curr]->hid = curr;
			heap[ch]->hid = ch;

			curr = ch;
			lc = (ch << 1);
		}
	}

	void push(T* v) {
		heap[sz] = v;
		up(sz);
		sz++;
	}

	void pop() {
		--sz;
		heap[1] = heap[sz];
		down(1);
	}
};


int mN;

Heap<Data> hp;
 
void input() {
	hp.init(hbuf[0]);
	fe(i, 1, mN) {
		sd[i].idx = i;
		scanf("%d", &sd[i].v);
		sd[i].sv = ((long long)(sd[i].v) << 17) + sd[i].idx;
		hp.push(&sd[i]);
	}
}

typedef void (*cmd_fp)(void);

void func1(void) {
	int idx;
	scanf("%d", &idx);
	scanf("%d", &sd[idx].v);
	hp.update(&sd[idx]);
}

void func2(void) {
	printf("%d\n", hp.heap[1]->idx);
}

cmd_fp ftbl[3] = {
	0,
	func1,
	func2
};

int main() {
	freopen("input14427.txt", "r", stdin);

	scanf("%d", &mN);

	input();
	int nQ;
	scanf("%d", &nQ);

	f(q, 0, nQ) {
		int cmd;
		scanf("%d", &cmd);
		ftbl[cmd]();
	}
	return 0;
} 