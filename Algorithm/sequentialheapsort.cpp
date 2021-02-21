/*
Created by Jeonghan Kim

Efficient Heap management when sequential update
You can find that heap-sort based implementation is super faster than linked-list based insertion sort
*/

#include <iostream>
#include <cstdlib>
#include <ctime>
#include <algorithm>
using namespace std;

#define rt register
#define rint rt int
#define f(i,a,b) for(rint i=a;i<b;++i)

const int maxQ = 70000;
const int maxAccountCall = 30001;
const int maxAccountID = 100001;
const int minName = 2;
const int maxName = 4;
const int maxBuf = maxAccountID;
const int maxHeap = 10;

#define HEAP

enum {
	open,
	close,
	send,
	receive,
	max_op
};

struct Account {
	int time;
	int state;
	int name;
	int group;
	int asset;
	int number;
	int nid;
	int gid;
	int bid;		// list buf, or heap index

	bool operator<(const Account& r) const {
		if (time != r.time) return time > r.time;
		return state > r.state;
	}

	bool operator!=(const Account& r) const {
		return number != r.number;
	}
} account[maxAccountID], gtaccount[maxAccountID];

#ifdef HEAP

template <typename T>
struct Heap {
	T* heap[maxAccountCall+2];
	int sz;

	void updateUp(rint current) {
		while (current > 1 && *heap[current] < *heap[current>>1]) {
			rint pa = current >> 1;

			T* temp = heap[pa];
			heap[pa] = heap[current];
			heap[current] = temp;
			heap[pa]->bid = pa;
			heap[current]->bid = current;
			current = pa;
		}
	}
	void updateDown(rint current) {
		rint lc = current << 1;
		while (lc < sz) {
			rint ch;
			rint rc = lc + 1;
			if (rc == sz) 	ch = lc;
			else ch = *heap[lc] < *heap[rc] ? lc : rc;

			if (*heap[current] < *heap[ch]) break;

			T* temp = heap[current];
			heap[current] = heap[ch];
			heap[ch] = temp;
			heap[ch]->bid = ch;
			heap[current]->bid = current;
			current = ch;
			lc = (ch << 1);
		}
	}

	void update(rint current) {
		if (current > 1 && *heap[current] < *heap[current >> 1]) 
			updateUp(current);
		else 
			updateDown(current);
	}

	void push(T* value) {
		heap[sz] = value;
		updateUp(sz);
		sz++;
	}

	void pop() {
		sz--;
		heap[1] = heap[sz];
		updateDown(1);
	}
};
Heap<Account> hp[maxHeap];
#else
struct Node {
	Account* pa;
	Node* n, * p;

	void insert(Node* pv) {
		p = pv, n = pv->n;
		p->n = this;
		if (n) n->p = this;
	}
	void alloc(Account* pd, Node* hd) {
		pa = pd;
		insert(hd);
	}

	void erase() {
		p->n = n;
		if (n) n->p = p;
	}
} buf[maxBuf], head[maxBuf];
int bcnt;
#endif // HEAP


int hashing(const char c[]) {
	rint ret = c[0] - 96;
	for (rint i = 1; c[i]; ++i) {
		ret *= 26;
		ret += (c[i] - 96);
	}

	return ret;
}

int tc;
void init() {
#ifdef HEAP
	f(i, 0, maxHeap) {
		hp[i].sz = 1;
		//hp[i].sz = 0;
	}
#else
	bcnt = 0;
	f(i, 0, maxAccountCall) {
		head[i].n = 0;
	}
#endif

	f(i, 1, maxAccountID) {
		account[i].state = close;
		gtaccount[i].state = close;
	}
}


#ifdef HEAP
void update(Account* pa) {
	hp[pa->nid].update(pa->bid);
}

#else
void update(Account *pa) {
	rt Node* p = &buf[pa->bid];
	rt Node* q = p->p;
	for (; q != &head[pa->nid] && *p->pa < *q->pa; q = q->p);

	p->erase();
	p->insert(q);
}
#endif

const int maxTime = maxAccountCall;
const int maxTC = 2;

clock_t prof;
void run(int Q) {
	int nameID = 0;
	int groupID = 0;
	int ecnt = 0;

	clock_t ts = clock();
	f(i, 0, Q) {
		srand(i);

		account[i].time = i + 1;
		account[i].asset = rand() % 1000;
		account[i].state = open;
		account[i].number = i + 1;
		account[i].nid = nameID;
		account[i].name = hashing("kim");
		account[i].group = hashing("dev");
		account[i].gid = groupID;

		gtaccount[i] = account[i];

#ifdef HEAP
		hp[nameID].push(&account[i]);
#else
		buf[bcnt].alloc(&account[i], &head[nameID]);
		account[i].bid = bcnt++;
#endif
	}

#if 1
	f(i, 0, Q) {
		rint aid = 1 + rand() % Q;
		rint state = send + rand() % 2;
		rint dasset = rand() % 100;
		rint time = Q + i + 1;
		account[aid].time = time;
		account[aid].state = state;
		account[aid].asset += dasset;

		gtaccount[aid].time = time;
		gtaccount[aid].state = state;
		gtaccount[aid].asset += dasset;

		update(&account[aid]);
	}
#endif
	prof += (clock() - ts);
	sort(gtaccount, gtaccount + Q);

#ifdef HEAP
	f(i, 0, Q) {
		if (*hp[nameID].heap[1] != gtaccount[i]) 
			ecnt++;
		hp[nameID].pop();
	}
#else
	rt Node* p = head[nameID].n;
	rint a = 0;

	for (; p; p = p->n) {
		if (gtaccount[a] != *p->pa) {
			ecnt++;
		}
		a++;
	}
#endif

	cout << ecnt << endl;
}
int main() {

	f(t, 0, maxTC) {
		srand(t);
		init();
		run(maxTime);
	}

	printf("%.3f\n", double(prof) / CLOCKS_PER_SEC);
}