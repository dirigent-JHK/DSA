/*
Created by Jeonghan Kim

Top10 by heap
*/


#define rt register
#define rint rt int

#define f(i,a,b) for(rint i=a;i<b;++i)
#define fe(i,a,b) for(rint i=a;i<=b;++i)

const int maxID = 100000 + 1;
struct Node {
	int v;
	int id;

	void set(int vv, int i) {
		v = vv, id = i;
	}

	bool operator<(const Node& r) const {
		if (v != r.v) return v > r.v;
		return id < r.id;
	}

} buf[maxID];


struct Heap {
	int sz;
	Node* heap[maxID + 1];

	void init() {
		sz = 1;
	}

	void push(Node* v) {
		heap[sz] = v;

		rint curr = sz;

		while (curr > 1 && *heap[curr] < *heap[curr >> 1]) {
			rint pa = curr >> 1;
			Node* t = heap[curr];
			heap[curr] = heap[pa];
			heap[pa] = t;

			curr = pa;
		}
		sz++;
	}

	void erase() {
		--sz;
		heap[1] = heap[sz];

		rint curr = 1;
		rint lc = 2;

		while (lc < sz) {
			rint rc = lc + 1;
			rint ch;
			if (rc == sz) ch = lc;
			else ch = (*heap[lc] < *heap[rc]) ? lc : rc;

			if (*heap[curr] < *heap[ch]) break;

			Node* t = heap[curr];
			heap[curr] = heap[ch];
			heap[ch] = t;

			curr = ch;
			lc = ch << 1;
		}
	}
} hp;
int bcnt;

void init() {
	bcnt = 0;
	hp.init();
}

void addUser(int uID, int height) {
	Node& nd = buf[bcnt++];
	nd.set(height, uID);
	hp.push(&nd);
}

int getTop10(int result[10]) {
	rint ret = 0;
	Node* poped[10];
	while (hp.sz > 1) {
		result[ret] = hp.heap[1]->id;
		poped[ret] = hp.heap[1];
		hp.erase();

		if (++ret == 10) break;
	}

	for (rint i = ret - 1; i >= 0; --i) {
		hp.push(poped[i]);
	}
	return ret;
}