/*
Created by Jeonghan Kim

Single Linked-list in the form of queue
Incremental insertion sort implemented
*/

#include <iostream>

using namespace std;

#define rt register

#include <algorithm>
#include <queue>
#include <cstdlib>
#include <ctime>

const int n = 1000;
const int nTbl = 1000;
const int nBuf = n * nTbl + nTbl;

#define rt register
#define rint rt int
#define f(i,a,b)  for(rint i=a;i<b;++i)

struct Node {
	int v;
	Node* n;

	bool operator<(const Node& r) const {
		return v < r.v;
	}

	void alloc(int d, Node* pv) {
		v = d;
		n = pv->n;
		pv->n = this;
	}
};

Node heads[nTbl];
Node* tails[nTbl];
Node buf[nBuf];
int bcnt;

#define nullchk(addr) if (tails[(addr)] == &heads[(addr)]) return false
#define alloc(v,p) buf[bcnt++].alloc(v, (p))
#define next(p) for (; (p); (p) = (p)->n) 

void push(int v, int addr) {
	alloc(v, tails[addr]);
	tails[addr] = tails[addr]->n;
}

bool front(int& v, int addr) {
	nullchk(addr);
	v = heads[addr].n->v;
	return true;
}

bool pop(int addr) {
	nullchk(addr);
	heads[addr].n = heads[addr].n->n;
	if (heads[addr].n == 0) tails[addr] = &heads[addr];
	return true;
}

void push_sorted(int v, int addr) {
	rt Node* p = &heads[addr];
	
	for (; p->n && p->n->v < v; p = p->n);
	alloc(v, p);

	if (p == tails[addr]) {
		tails[addr] = p->n;
	}
}

void print(int addr, int cnt) {
	cout << "===========================================================" << endl;
	Node* p = heads[addr].n;
	int i = 0;

	next(p) {
		cout << p->v << " ";
		if (++i == cnt) break;
	}
	cout << endl;
	cout << "===========================================================" << endl;
}

void selectionSort(int addr) {
	Node* pi = heads[addr].n;
	next(pi) {
		Node* mp = pi;
		Node* pj = pi->n;

		next(pj) {
			if (pj->v < mp->v) mp = pj;
		}

		if (mp != pi) {
			int t(mp->v);
			mp->v = pi->v;
			pi->v = t;
		}
	}
}

void init() {
	bcnt = 0;

	f(i, 0, nTbl) {
		heads[i].n = 0;
		tails[i] = &heads[i];
	}
}

int gt[nTbl][n];
queue<int> que[nTbl];

void pushpopTest() {
	init();

	f(i, 0, nTbl) {
		f(j, 0, n) {
			gt[i][j] = rand();
			que[i].push(gt[i][j]);
			push(gt[i][j], i);
		}
	}

	int badCnt = 0;
	f(i, 0, nTbl) {
		f(j, 0, n) {
			int v;
			front(v, i);

			if (que[i].front() != v) {
				badCnt++;
			}

			que[i].pop();
			pop(i);
		}
	}

	f(i, 0, nTbl) {
		if (tails[i] != &heads[i]) 
			badCnt++;
	}

	cout << badCnt << endl;
}

void insertionSortTest() {
	clock_t ts = clock();

	init();
	f(i, 0, nTbl) {
		f(j, 0, n) {
			gt[i][j] = rand();
			push_sorted(gt[i][j], i);
		}
	}

	clock_t te = clock() - ts;
	cout << "Elapsed time : " << double(te) / CLOCKS_PER_SEC << "sec" << endl;

	int badCnt = 0;

	f(i, 0, nTbl) {
		sort(gt[i], gt[i] + n);
		Node* p = heads[i].n;
		int j = 0;
		next(p) {
			if (gt[i][j++] != p->v) {
				badCnt++;
			}
		}
	}
	cout << badCnt << endl;

}

void selectionSortTest() {
	clock_t ts = clock();

	init();

	f(i, 0, nTbl) {
		f(j, 0, n) {
			gt[i][j] = rand();
			push(gt[i][j], i);
		}
		selectionSort(i);
	}
	clock_t te = clock() - ts;
	cout << "Elapsed time : " << double(te) / CLOCKS_PER_SEC << "sec" << endl;

	int badCnt = 0;

	f(i, 0, nTbl) {
		sort(gt[i], gt[i] + n);
		Node* p = heads[i].n;
		int j = 0;
		next(p) {
			if (gt[i][j++] != p->v) {
				badCnt++;
			}
		}
	}
	cout << badCnt << endl;
}

int main() {

	srand(0);

	pushpopTest();
	insertionSortTest();
	selectionSortTest();

	return 0;
}