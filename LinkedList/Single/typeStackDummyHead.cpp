/*
Created by Jeonghan Kim

Single Linked-list in the form of stack with dummy head
Incremental insertion sort implemented
*/

#include <iostream>

using namespace std;

#include <algorithm>
#include <stack>
#include <cstdlib>
#include <ctime>

#define rt register

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

	void alloc(int d, Node *head) {
		v = d;
		next = head->next;
		head->next = this;
	}
};

Node heads[nTbl];
Node buf[nBuf];
int bcnt;

#define _nullchk(addr) if (0 == heads[(addr)].next) return false
#define _new(v, p) buf[bcnt++].alloc(v, (p))
#define _next(p) for (; (p); (p) = (p)->next) 

void push(int v, int addr) {
	_new(v, &heads[addr]);
}

bool top(int & v, int addr) {
	_nullchk(addr);
	v = heads[addr].next->v;
	return true;
}

bool pop(int addr) {
	_nullchk(addr);
	heads[addr].next = heads[addr].next->next;
	return true;
}

bool isEmpty(int addr) {
	return 0 == heads[addr].next;
}

// More compact compared with the case without dummy head
void push_sorted(int v, int addr) {
	rt Node* p = &heads[addr];
	for (; p->next && p->next->v < v; p = p->next);

	_new(v, p);
}

void print(int addr, int cnt) {
	cout << "===========================================================" << endl;
	Node* p = heads[addr].next;
	int i = 0;

	_next(p) {
		cout << p->v << " ";
		if (++i == cnt) break;
	}
	cout << endl;
	cout << "===========================================================" << endl;
}

void selectionSort(int addr) {

	Node* pi = heads[addr].next;
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
		heads[i].next = 0;
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
			push(gt[i][j], i);
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
			push_sorted(gt[i][j], i);
		}
	}

	clock_t te = clock() - ts;
	cout << "Elapsed time : " << double(te) / CLOCKS_PER_SEC << "sec" << endl;

	int badCnt = 0;

	f(i, 0, nTbl) {
		sort(gt[i], gt[i] + n);
		Node* p = heads[i].next;
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
		Node* p = heads[i].next;
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