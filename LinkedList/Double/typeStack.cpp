/*
Created by Jeonghan Kim

Double Linked-list in the form of stack
Incremental insertion sort implemented
*/

#include <iostream>
#include <list>
#include <algorithm>
#include <cstdlib>
#include <ctime>

#define rt register
#define rint rt int
#define f(i,a,b) for(rint i=a;i<b;++i)

#define _fwd(i) for(rt Node *p = head[i].n;p;p=p->n)

using namespace std;

const int M = 100;	// number of rows
const int N = 1000;	// number of columns
const int MAXBUF = N * M;

struct Pt {
	int r;
	int c;
	Pt() {}
	Pt(int x, int y) : r(x), c(y) {}
	int key() const {
		return r * N + c;
	}

	bool operator==(const Pt & r) const {return key() == r.key();}
	bool operator>(const Pt& r) const {return key() > r.key();}
	bool operator<=(const Pt& r) const {return key() <= r.key();}
	bool operator<(const Pt& r) const {return key() < r.key();}
};

struct Node {
	Pt *pd;
	Node* p;
	Node* n;
	
	void insert(Pt* _pd, Node* it) {
		pd = _pd;
		p = it;
		n = it->n;
		p->n = this;
		if (n) n->p = this;
	}

	void erase() {
		pd = 0;
		p->n = n;
		if (n) n->p = p;
	}

	void init() {
		n = 0;
		p = 0;
	}
};

Node buf[MAXBUF];
int cnt;
Node head[M];

void push(int k, Pt* pd) {
	buf[cnt++].insert(pd, &head[k]);
}

void push_sorted(int k, Pt* pd) {
	rt Node* p = &head[k];
	for (; p->n  && *(p->n->pd) < *pd; p = p->n);
	buf[cnt++].insert(pd, p);
}

Pt vec[M][N];
list<Pt> lst[M];
Pt out[M][N];

void init() {
	cnt = 0;
	f(i, 0, M) head[i].n = 0;
}

void test_push() {
	init();
	f(i, 0, M) {
		f(j, 0, N) {
			vec[i][j] = Pt(rand()%M, rand()%N);
			lst[i].push_front(vec[i][j]);
			push(i, &vec[i][j]);
		}
	}

	int ecount = 0;
	f(i, 0, M) {
		_fwd(i) {
			Pt pt = lst[i].front();
			if (!(pt == *(p->pd))) ecount++;
			lst[i].pop_front();
		}
	}

	cout << ecount << endl;
	ecount = 0;
	f(i, 0, M) {
		_fwd(i)	p->erase();
		_fwd(i) ecount++;
	}

	cout << ecount << endl;
}

void test_pushsorted() {
	init();
	f(i, 0, M) {
		f(j, 0, N) {
			vec[i][j] = Pt(rand() % M, rand() % N);
			lst[i].push_front(vec[i][j]);
			push_sorted(i, &vec[i][j]);
		}

		lst[i].sort();
	}

	int ecount = 0;
	f(i, 0, M) {
		_fwd(i) {
			Pt pt = lst[i].front();
			if (!(pt == *(p->pd))) ecount++;
			lst[i].pop_front();
		}
	}

	cout << ecount << endl;
	ecount = 0;
	f(i, 0, M) {
		_fwd(i)	p->erase();
		_fwd(i)	ecount++;
	}

	cout << ecount << endl;
}

int main() {
	srand(time(0));

	test_push();
	test_pushsorted();
 	return 0;
}