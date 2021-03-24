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

#define bwd(i) for(rt Node *p = tail[i]; p != &head[i]; p = p->p)
#define fwd(i) for(rt Node *p = head[i].n; p; p = p->n)

using namespace std;

const int M = 100;	// number of rows
const int N = 1000;	// number of columns
const int MAXBUF = N * M + M;

struct Pt {
	int r;
	int c;
	int key;
	Pt() {}
	void set(int x, int y){
		r = x, c = y;
		key = r * N + c;
	}
	bool operator==(const Pt & r) const {return key == r.key;}
	bool operator>(const Pt& r) const {return key > r.key;}
	bool operator<=(const Pt& r) const {return key <= r.key;}
	bool operator<(const Pt& r) const {return key < r.key;}
};

struct Node {
	Pt *pd;
	Node* p;
	Node* n;

	void insert(Node* pv, Node **tail) {
		p = pv, n = pv->n;
		p->n = this;
		if (n) n->p = this;
		else *tail = this;
	}
	
	void add(Pt* d, Node* pv, Node **tail) {
		pd = d, insert(pv, tail);
	}

	void erase(Node **tail) {
		pd = 0;
		p->n = n;
		if (n) n->p = p;
		else *tail = p;
	}

	void init() {
		n = 0, p = 0;
	}
};

Node buf[MAXBUF];
int cnt;

Node head[M];
Node* tail[M];

void push_back(int k, Pt* pd) {
	rt Node* p = &buf[cnt++];
	p->add(pd, tail[k], &tail[k]);
}

void push_front(int k, Pt* pd) {
	buf[cnt++].add(pd, &head[k], &tail[k]);
}

void push_sorted(int k, Pt* pd) {
	rt Node* p = &head[k];
	for (; p->n  && *(p->n->pd) < *pd; p = p->n);
	buf[cnt++].add(pd, p, &tail[k]);
}

void push_rsorted(int k, Pt* pd) {
	rt Node* p = tail[k];
	for (; p->p && (*pd < *(p->pd)); p = p->p);
	buf[cnt++].add(pd, p, &tail[k]);
}

void erase(int k, Node* q) {
	q->erase(&tail[k]);
}

/*
* append source list to destination list
*/
void append(int dst, int src) {
	tail[dst]->n = head[src].n;
	if (head[src].n) head[src].n->p = tail[dst];

	tail[dst]->p = tail[src]->p;
	tail[dst]->n = 0;
	tail[dst] = tail[src];

	head[src].n = 0;
	tail[src] = &head[src];
}

Pt vec[M][N];
list<Pt> lst[M];
Pt out[M][N];

void init() {
	f(i, 0, M) {
		head[i].n = 0;
		tail[i] = &head[i];
	}
	cnt = M;
}

void test_pushback() {
	init();
	f(i, 0, M) {
		f(j, 0, N) {
			vec[i][j].set(rand()%M, rand()%N);
			lst[i].push_back(vec[i][j]);
			push_back(i, &vec[i][j]);
		}
	}

	int ecount = 0;
	f(i, 0, M) {
		bwd(i) {
			Pt pt = lst[i].back();
			if (!(pt == *(p->pd))) ecount++;
			lst[i].pop_back();
		}
	}

	cout << ecount << endl;
	ecount = 0;
	f(i, 0, M) {
		fwd(i)	erase(i, p);
		fwd(i) ecount++;
	}

	cout << ecount << endl;
}

void test_pushsorted() {
	init();
	f(i, 0, M) {
		f(j, 0, N) {
			vec[i][j].set(rand() % M, rand() % N);
			lst[i].push_back(vec[i][j]);
			push_sorted(i, &vec[i][j]);
		}
		lst[i].sort();
	}

	int ecount = 0;
	f(i, 0, M) {
		bwd(i) {
			Pt pt = lst[i].back();
			if (!(pt == *(p->pd))) ecount++;
			lst[i].pop_back();
		}
	}

	cout << ecount << endl;
	ecount = 0;
	f(i, 0, M) {
		fwd(i)	erase(i, p);
		fwd(i)	ecount++;
	}

	cout << ecount << endl;
}

void test_pushrsorted() {
	init();
	f(i, 0, M) {
		f(j, 0, N) {
			vec[i][j].set(rand() % M, rand() % N);
			lst[i].push_back(vec[i][j]);
			push_rsorted(i, &vec[i][j]);
		}

		lst[i].sort();
	}

	int ecount = 0;
	f(i, 0, M) {
		bwd(i){
			Pt pt = lst[i].back();
			if (!(pt == *(p->pd)))ecount++;
			lst[i].pop_back();
		}
	}

	cout << ecount << endl;
	ecount = 0;
	f(i, 0, M) {
		fwd(i)	erase(i,p);
		fwd(i)	ecount++;
	}

	cout << ecount << endl;
}

void test_pushfront() {
	init();

	f(i, 0, M) {
		f(j, 0, N) {
			vec[i][j].set(rand() % M, rand() % N);
			lst[i].push_front(vec[i][j]);
			push_front(i, &vec[i][j]);
		}
	}

	int ecount = 0;
	f(i, 0, M) {
		fwd(i){
			Pt pt = lst[i].front();
			if (!(pt == *(p->pd))) ecount++;
			lst[i].pop_front();
		}
	}

	cout << ecount << endl;

	ecount = 0;
	f(i, 0, M) {
		bwd(i) erase(i,p);
		bwd(i) ecount++;
	}

	cout << ecount << endl;
}


void test_merge() {
	init();

	f(i, 0, 2) {
		f(j, 0, N) {
			vec[i][j].set(rand() % M, rand() % N);
			push_back(i, &vec[i][j]);
			push_back(2, &vec[i][j]);
		}
	}

	f(i, 3, 5) {
		f(j, 0, N) {
			vec[i][j].set(rand() % M, rand() % N);
			push_front(i, &vec[i][j]);
			push_front(5, &vec[i][j]);
		}
	}
	append(0, 1);
	append(4, 3);

	int ecount = 0;
	int i = 0;

	rt Node* p = head[i].n;
	rt Node* q = head[2].n;

	for (; p; p = p->n) {
		if (!((*q->pd) == *(p->pd))) {
			ecount++;
		}
		q = q->n;
	}

	cout << ecount << endl;

	i = 4;
	p = head[i].n;
	q = head[5].n;

	for (; p; p = p->n) {
		if (!((*q->pd) == *(p->pd))) {
			ecount++;
		}
		q = q->n;
	}

	cout << ecount << endl;
}
int main() {
	srand((unsigned int)time(0));

	test_pushback();
	test_pushfront();
	test_pushsorted();
	test_pushrsorted();
	test_merge();

 	return 0;
}