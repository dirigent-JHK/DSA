/*
Created by Jeonghan Kim

Double Linked-list in the form of queue
Incremental insertion sort implemented
List merge could be implemented when in the form of queue
*/

#include <iostream>
#include <list>
#include <algorithm>
#include <cstdlib>
#include <ctime>

#define rt register
#define rint rt int
#define f(i,a,b) for(rint i=a;i<b;++i)

#define _bwd(i) for(rt Node *p = tail[i];p!=head[i];p=p->p)
#define _fwd(i) for(rt Node *p = head[i]->n;p;p=p->n)

using namespace std;

const int M = 100;	// number of rows
const int N = 1000;	// number of columns
const int MAXBUF = N * M + M;

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

Node* head[M];
Node* tail[M];

void push_back(int k, Pt* pd) {
	rt Node* p = &buf[cnt++];
	p->insert(pd, tail[k]);
	tail[k] = p;
}

void push_front(int k, Pt* pd) {
	buf[cnt++].insert(pd, head[k]);
}

void push_sorted(int k, Pt* pd) {
	rt Node* p = head[k];
	for (; p->n  && *(p->n->pd) < *pd; p = p->n);
	if (p != tail[k]) buf[cnt++].insert(pd, p);
	else push_back(k, pd);
}

void push_rsorted(int k, Pt* pd) {
	rt Node* p = tail[k];
	for (; p != head[k] && (*pd < *(p->pd)); p = p->p);
	if (p != tail[k]) buf[cnt++].insert(pd, p);
	else push_back(k, pd);
}

void append(int dst, int src) {

	tail[dst]->n = head[src]->n;
	if (head[src]->n) head[src]->n->p = tail[dst];

	tail[dst]->p = tail[src]->p;
	tail[dst]->n = 0;
	tail[dst] = tail[src];

	tail[src] = head[src];
	head[src]->n = 0;
}

Pt vec[M][N];
list<Pt> lst[M];
Pt out[M][N];

int qc;
void init() {
	if (qc == 0) {
		f(i, 0, M) head[i] = tail[i] = &buf[i];
	}
	else {
		f(i, 0, M) {
			head[i]->n = 0;
			tail[i] = &buf[i];
			tail[i]->n = tail[i]->p = 0;
		}
	}

	cnt = M;
	qc++;
}

void test_pushback() {
	init();
	f(i, 0, M) {
		f(j, 0, N) {
			vec[i][j] = Pt(rand()%M, rand()%N);
			lst[i].push_back(vec[i][j]);
			push_back(i, &vec[i][j]);
		}
	}

	int ecount = 0;
	f(i, 0, M) {
		_bwd(i) {
			Pt pt = lst[i].back();
			if (!(pt == *(p->pd))) ecount++;
			lst[i].pop_back();
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
			lst[i].push_back(vec[i][j]);
			push_sorted(i, &vec[i][j]);
		}

		lst[i].sort();
	}

	int ecount = 0;
	f(i, 0, M) {
		_bwd(i) {
			Pt pt = lst[i].back();
			if (!(pt == *(p->pd))) ecount++;
			lst[i].pop_back();
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

void test_pushrsorted() {
	init();
	f(i, 0, M) {
		f(j, 0, N) {
			vec[i][j] = Pt(rand() % M, rand() % N);
			lst[i].push_back(vec[i][j]);
			push_rsorted(i, &vec[i][j]);
		}

		lst[i].sort();
	}

	int ecount = 0;
	f(i, 0, M) {
		_bwd(i){
			Pt pt = lst[i].back();
			if (!(pt == *(p->pd)))ecount++;
			lst[i].pop_back();
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

void test_pushfront() {
	init();

	f(i, 0, M) {
		f(j, 0, N) {
			vec[i][j] = Pt(rand() % M, rand() % N);
			lst[i].push_front(vec[i][j]);
			push_front(i, &vec[i][j]);
		}
	}

	int ecount = 0;
	f(i, 0, M) {
		_fwd(i){
			Pt pt = lst[i].front();
			if (!(pt == *(p->pd))) ecount++;
			lst[i].pop_front();
		}
	}

	cout << ecount << endl;

	ecount = 0;
	f(i, 0, M) {
		_bwd(i) p->erase();
		_bwd(i) ecount++;
	}

	cout << ecount << endl;
}


void test_merge() {
	init();

	f(i, 0, 2) {
		f(j, 0, N) {
			vec[i][j] = Pt(rand() % M, rand() % N);
			push_back(i, &vec[i][j]);
			push_back(2, &vec[i][j]);
		}
	}

	f(i, 3, 5) {
		f(j, 0, N) {
			vec[i][j] = Pt(rand() % M, rand() % N);
			push_front(i, &vec[i][j]);
			push_front(5, &vec[i][j]);
		}
	}
	append(0, 1);
	append(4, 3);

	int ecount = 0;
	int i = 0;

	rt Node* p = head[i]->n;
	rt Node* q = head[2]->n;

	for (; p; p = p->n) {
		if (!((*q->pd) == *(p->pd))) {
			ecount++;
		}
		q = q->n;
	}

	cout << ecount << endl;

	i = 4;
	p = head[i]->n;
	q = head[5]->n;

	for (; p; p = p->n) {
		if (!((*q->pd) == *(p->pd))) {
			ecount++;
		}
		q = q->n;
	}

	cout << ecount << endl;
}
int main() {
	srand(time(0));

	test_pushback();
	test_pushfront();
	test_pushsorted();
	test_pushrsorted();
	test_merge();

 	return 0;
}