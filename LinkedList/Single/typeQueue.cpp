/*
Created by Jeonghan Kim

Single Linked-list in the form of queue
Incremental insertion sort implemented
*/

#include <iostream>

using namespace std;

#include <algorithm>
#include <queue>
#include <cstdlib>
#include <ctime>

const int n = 1000;
const int nTbl = 1000;
const int nBuf = n * nTbl + nTbl;

#define f(i,a,b)  for(register int i=a;i<b;++i)

struct Node {
	int v;
	Node* next;

	bool operator<(const Node& r) const {
		return v < r.v;
	}

	Node* alloc(int d, Node* n) {
		v = d;
		next = n;
		return  this;
	}
};

Node* heads[nTbl];
Node* tails[nTbl];
Node buf[nBuf];
int bcnt;

#define _nullchk(addr) if (tails[(addr)] == heads[(addr)]) return false
#define _new(v,p) (p) = buf[bcnt++].alloc(v, (p))
#define _next(p) for (; (p); (p) = (p)->next) 

void push(int v, int addr) {
	_new(v, tails[addr]->next);
	tails[addr] = tails[addr]->next;
}

bool front(int& v, int addr) {
	_nullchk(addr);
	v = heads[addr]->next->v;
	return true;
}

bool pop(int addr) {
	_nullchk(addr);
	heads[addr] = heads[addr]->next;
	return true;
}

bool isEmpty(int addr) {
	return tails[(addr)] == heads[addr];
}

void push_sorted(int v, int addr) {
	Node* q = heads[addr];
	Node* p = q->next;
	
	for (; p && p->v < v; q = p, p = p->next);
	_new(v, q->next);

	if (q == tails[addr]) {
		tails[addr] = q->next;
	}
}

void print(int addr, int cnt) {
	cout << "===========================================================" << endl;
	Node* p = heads[addr]->next;
	int i = 0;

	_next(p) {
		cout << p->v << " ";
		if (++i == cnt) break;
	}
	cout << endl;
	cout << "===========================================================" << endl;
}

void selectionSort(int addr) {
	Node* pi = heads[addr]->next;
	_next(pi) {
		Node* mp = pi;
		Node* pj = pi->next;

		_next(pj) {
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
		heads[i] = tails[i] = buf[bcnt++].alloc(0,0);
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
		if (isEmpty(i) == false) {
			badCnt++;
		}
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
		Node* p = heads[i]->next;
		int j = 0;
		_next(p) {
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
		Node* p = heads[i]->next;
		int j = 0;
		_next(p) {
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