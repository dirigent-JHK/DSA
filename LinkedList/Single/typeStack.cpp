/*
Created by Jeonghan Kim

Single Linked-list in the form of stack
Incremental insertion sort implemented
*/

#include <iostream>

using namespace std;

#include <algorithm>
#include <stack>
#include <cstdlib>
#include <ctime>

const int n = 1000;
const int nTbl = 1000;
const int nBuf = n*nTbl;

#define f(i,a,b)  for(register int i=a;i<b;++i)

struct Node {
	int v;
	Node* next;

	bool operator<(const Node & r) const {
		return v < r.v;
	}

	Node *alloc(int d, Node *n) {
		v = d;
		next = n;
		return  this;
	}
};

Node* heads[nTbl];
Node buf[nBuf];
int bcnt;

#define _nullchk(addr) if (0 == heads[(addr)]) return false
#define _new(p,v) (p) = buf[bcnt++].alloc(v, (p))
#define _next(p) for (; (p); (p) = (p)->next) 

void push(int addr, int v) {
	_new(heads[addr], v);
}

bool top(int & v, int addr) {
	_nullchk(addr);
	v = heads[addr]->v;
	return true;
}

bool pop(int addr) {
	_nullchk(addr);
	heads[addr] = heads[addr]->next;
	return true;
}

bool isEmpty(int addr) {
	return 0 == heads[addr];
}

void push_sorted(int addr, int v) {
	Node* p = heads[addr];
	Node* q = p;
	for (; p && p->v < v; q = p, p = p->next);

	if (p == heads[addr]) {
		_new(heads[addr], v);
	}
	else {
		_new(q->next, v);
	}
}

void print(int addr, int cnt) {
	cout << "===========================================================" << endl;
	Node* p = heads[addr];
	int i = 0;

	_next(p) {
		cout << p->v << " ";
		if (++i == cnt) break;
	}
	cout << endl;
	cout << "===========================================================" << endl;
}

void selectionSort(int addr) {

	Node* pi = heads[addr];
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
		heads[i] = 0;
	}
}

int gt[nTbl][n];
stack<int> stk[nTbl];

void pushpopTest() {
	init();

	f(i, 0, nTbl) {
		f(j, 0, n) {
			gt[i][j] = rand();
			stk[i].push(gt[i][j]);
			push(i, gt[i][j]);
		}

		//print(i, 5);
	}

	int badCnt = 0;
	f(i, 0, nTbl) {
		f(j, 0, n) {
			int v;
			top(v, i);

			if (stk[i].top() != v) {
				badCnt++;
			}

			stk[i].pop();
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
			push_sorted(i, gt[i][j]);
		}
	}

	clock_t te = clock() - ts;
	cout << "Elapsed time : " << double(te - ts) / CLOCKS_PER_SEC << "sec" << endl;

	int badCnt = 0;

	f(i, 0, nTbl) {
		sort(gt[i], gt[i] + n);
		Node* p = heads[i];
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
			push(i, gt[i][j]);
		}
		selectionSort(i);
	}
	clock_t te = clock() - ts;
	cout << "Elapsed time : " << double(te - ts) / CLOCKS_PER_SEC << "sec" << endl;

	int badCnt = 0;

	f(i, 0, nTbl) {
		sort(gt[i], gt[i] + n);
		Node* p = heads[i];
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