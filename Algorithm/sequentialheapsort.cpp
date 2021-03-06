/*
Created by Jeonghan Kim

Efficient Heap management when sequential update
You can find that heap-sort based implementation is super faster than linked-list based insertion sort
Hashing is added
In this case, forward list update is most fast
*/

#include <iostream>
#include <cstdlib>
#include <ctime>
#include <algorithm>
using namespace std;

#define rt register
#define rint rt int
#define f(i,a,b) for(rint i=a;i<b;++i)

typedef unsigned long long ull;

const int maxQ = 70000;
const int maxAccountCall = 30001;
const int maxAccountID = 100001;
const int maxName = 10;
const int maxGroupName = 4;
const int maxBuf = maxAccountID;
const int maxGroup = 10;

const int maxMembers = maxAccountCall;

//#define HEAP

void mstrcpy(char dst[], const char src[]) {
	rint i = 0;
	for(;src[i];++i) dst[i] = src[i];
	dst[i] = 0;
}

enum {
	open,
	close,
	send,
	receive,
	max_op
};

const char groupPool[maxGroup][maxGroupName+1] = {
	"sw",
	"hw",
	"it",
	"qa",
	"aff",
	"secu",
	"fin",
	"plan",
	"mkt",
	"hr"
};

char namePool[maxMembers][maxName + 1];

void genNamePool(int n) {
	f(i, 0, n) {
		int len = 1+ rand() % maxName;
		f(j, 0, len) {
			namePool[i][j] = rand() % 26 + 'a';
		}
	}
}

struct Account {
	int time;
	int state;
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
} account[maxGroup][maxAccountID], gtaccount[maxGroup][maxAccountID];

const int maxGrpHash = maxGroup + 3;
const int maxNameHash = maxMembers + 3;

template <typename T>
struct HashN {
	T key;
	int id;		///data id
	int hid;
	HashN* n;

	void alloc(T k, int i, int hi, HashN* hd) {
		key = k, id = i, hid = hi, n = hd->n, hd->n = this;
	}
};

HashN<ull> hnamebuf[maxAccountCall + 3], hnametbl[maxNameHash];
HashN<int> hgrpbuf[maxAccountCall + 3], hgrptbl[maxGrpHash];

// when the length of char is less than 5
void key32(int& k, const char s[]) {
	k = 0;
	for (rint i = 0; s[i]; ++i) {
		k *= 26;
		k |= (s[i] - 96);
	}
}

//when the length of char is less than 12
void key64(ull& k, const char s[]) {
	k = 0;
	for (rint i = 0; s[i]; ++i) {
		k <<= 5;
		k |= (s[i] - 96);
	}
}
template <typename T>
struct HashW {
	int cnt;
	int maxKey;
	HashN<T>* buf;
	HashN<T>* tbl;

	void init(int maxK, HashN<T>* b, HashN<T>* t) {
		cnt = 0, maxKey = maxK;
		buf = b, tbl = t; 
	}

	int find(int & addr, int key) {
		addr = key % maxKey;
		rt HashN<T>* p = tbl[addr].n;
		for (; p; p = p->n) if (p->key == key) return p->hid;
		return -1;
	}

	int find(int & addr, ull & key) {
		addr = key % maxKey;
		rt HashN<T>* p = tbl[addr].n;
		for (; p; p = p->n) if (p->key == key) return p->hid;
		return -1;
	}

	int getid(int key, int did) {
		int addr;
		int id = find(addr, key);

		if (id < 0) {
			buf[cnt].alloc(key, did, cnt, &tbl[addr]);
			id = cnt++;
		}

		return id;
	}

	int getid(ull & key, int did) {
		int addr;
		int id = find(addr, key);

		if (id < 0) {
			buf[cnt].alloc(key, did, cnt, &tbl[addr]);
			id = cnt++;
		}

		return id;
	}
};


HashW<int> hgrp;
HashW<ull> hname;

#ifdef HEAP

template <typename T>
struct Heap {
	T* heap[maxAccountCall+2];
	int sz;

	void up(rint current) {
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
	void down(rint current) {
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
		if (current == 1 || *heap[current>>1] < *heap[current]) down(current);
		else up(current);
	}

	void push(T* value) {
		heap[sz] = value;
		up(sz++);
	}

	void pop() {
		sz--;
		heap[1] = heap[sz];
		down(1);
	}

	void pushonly(T* value) {
		heap[sz] = value;
		rint current = sz++;
		while (current > 1 && *heap[current] < *heap[current >> 1]) {
			rint pa = current >> 1;

			T* temp = heap[pa];
			heap[pa] = heap[current];
			heap[current] = temp;
			current = pa;
		}
	}

	void poponly() {
		sz--;
		heap[1] = heap[sz];

		rint current = 1;
		rint lc = 2;
		while (lc < sz) {
			rint ch;
			rint rc = lc + 1;
			if (rc == sz) 	ch = lc;
			else ch = *heap[lc] < *heap[rc] ? lc : rc;

			if (*heap[current] < *heap[ch]) break;

			T* temp = heap[current];
			heap[current] = heap[ch];
			heap[ch] = temp;
			current = ch;
			lc = (ch << 1);
		}
	}
};
Heap<Account> hp[maxGroup];
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
} buf[maxBuf], head[maxGroup];
int bcnt;
#endif // HEAP


int tc;
int groupCnt[maxGroup];
void init() {
#ifdef HEAP
	f(i, 0, maxGroup) {
		hp[i].sz = 1;
		//hp[i].sz = 0;
	}
#else
	bcnt = 0;
	f(i, 0, maxGroup) {
		head[i].n = 0;
	}
#endif

	f(i, 0, maxGrpHash) hgrptbl[i].n = 0;
	f(i, 0, maxNameHash) hnametbl[i].n = 0;

	f(g, 0, maxGroup) {
		groupCnt[g] = 0;
		f(n, 1, maxAccountID) {
			account[g][n].state = close;
			gtaccount[g][n].state = close;
		}
	}

	hgrp.init(maxGrpHash, hgrpbuf, hgrptbl);
	hname.init(maxNameHash, hnamebuf, hnametbl);
}


#ifdef HEAP
void update(Account* pa) {
	hp[pa->gid].update(pa->bid);
}

#else
void update(Account *pa) {
	rt Node* p = &buf[pa->bid];
	p->erase();
#if 0	//Rearward
	rt Node* q = p->p;
	for (; q != &head[pa->gid] && *p->pa < *q->pa; q = q->p);
#else	//Forward
	rt Node* q = &head[pa->gid];
	for (; q->n && *q->n->pa  < *p->pa; q = q->n);
#endif
	p->insert(q);
}

void updateF(Account* pa) {
	rt Node* p = &buf[pa->bid];
	rt Node* q = p->p;
	for (; q != &head[pa->gid] && *p->pa < *q->pa; q = q->p);

	p->erase();
	p->insert(q);
}
#endif

const int maxTime = maxAccountCall;
const int maxTC = 3;

clock_t prof;
const int topN = 10;

void run(int Q) {
	int ecnt = 0;

	clock_t ts = clock();
	f(i, 0, Q) {
		int npid = rand() % maxMembers;
		int gpid = rand() % maxGroup;
		int grpK;
		ull nameK;
		key32(grpK, groupPool[gpid]);
		key64(nameK, namePool[npid]);

		int gid = hgrp.getid(grpK, i);
		int& gc = groupCnt[gid];

		account[gid][gc].gid = gid;
		account[gid][gc].nid = hname.getid(nameK, i);

		account[gid][gc].time = i + 1;
		account[gid][gc].asset = rand() % 1000;
		account[gid][gc].state = open;
		account[gid][gc].number = i + 1;

		gtaccount[gid][gc] = account[gid][gc];

#ifdef HEAP
		hp[gid].push(&account[gid][gc]);
#else
		buf[bcnt].alloc(&account[gid][gc], &head[gid]);
		account[gid][gc].bid = bcnt++;
#endif
		gc++;
	}

#if 1
	f(i, 0, Q) {
		int gid = rand() % maxGroup;
		int id = rand() % groupCnt[gid];
		int state = send + rand() % 2;
		int dasset = rand() % 100;
		int time = Q + i + 1;
		
		account[gid][id].time = time;
		account[gid][id].state = state;
		account[gid][id].asset += dasset;

		gtaccount[gid][id].time = time;
		gtaccount[gid][id].state = state;
		gtaccount[gid][id].asset += dasset;

		update(&account[gid][id]);
#ifdef HEAP
		//top N
		Account* poped[maxGroup][topN];
		int nPoped[maxGroup] = { 0, };

		f(g, 0, maxGroup) {
			while (hp[g].sz > 1) {
				poped[g][nPoped[g]] = hp[g].heap[1];
				hp[g].pop();
				if (++nPoped[g] == topN) break;
			}
		}
		
		f(g, 0, maxGroup) {
			//check
			f(i, 1, nPoped[g]) if (*poped[g][i] < *poped[g][i - 1]) ecnt++;

			//push back afer peeking top N
			f(i, 0, nPoped[g]) 	hp[g].push(poped[g][i]);
		}
#endif
	}
#endif
	prof += (clock() - ts);
	f(g, 0, maxGroup) {
		sort(&gtaccount[g][0], &gtaccount[g][groupCnt[g]]);
	}

#ifdef HEAP
	f(g, 0, maxGroup) {
		f(i, 0, groupCnt[g]) {
			//if (*hp[g].heap[1] != gtaccount[g][i]) 	ecnt++;
			hp[g].pop();
		}
	}
#else
	f(g, 0, maxGroup) {
		rt Node* p = head[g].n;
		rint a = 0;

		for (; p; p = p->n) {
			if (gtaccount[g][a] != *p->pa) {
				ecnt++;
			}
			a++;
		}
	}
#endif

	cout << ecnt << endl;
}
int main() {

	genNamePool(maxMembers);

	f(t, 0, maxTC) {
		srand(t);
		init();
		run(maxTime);
	}

	printf("%.3f\n", double(prof) / CLOCKS_PER_SEC);
}