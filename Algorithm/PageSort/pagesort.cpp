/*
Created by Jeonghan Kim

Exercise for insertion sort based upon array and linked list
Efficient sorting when only partial elements are updated based upon paging
*/

#include <iostream>
#include <ctime>
#include <cstdlib>
#include <algorithm>
using namespace std;

#define rt register
#define rint rt int

#define f(i,a,b) for(rint i=a;i<b;++i)
#define fe(i,a,b) for(rint i=a;i<=b;++i)

const int maxID = 100000;
const int pageSize = 100;
const int maxPage = maxID / pageSize;
const int maxUpdate = 100000;
#define LIST

#ifdef LIST 
struct Data {
	int id;
	int score;
	int page;

	bool operator<(const Data& r) const {
		if (score != r.score) return score > r.score;
		return id < r.id;
	}

	bool operator<=(const Data& r) const {
		if (score != r.score) return score >= r.score;
		return id <= r.id;
	}

	bool operator>(const Data& r) const {
		if (score != r.score) return score < r.score;
		return id > r.id;
	}

	bool operator!=(const Data& r) const {
		return (score != r.score || id != r.id);
	}
	void print() {
		cout << id << "," << score << endl;
	}
};

struct Node {
	Data* pd;
	Node *n, *p;
	void erase(Node **tail) {
		p->n = n;
		if (n) n->p = p;
		else *tail = p;
	}

	void insert(Node* pv, Node **tail) {
		p = pv, n = pv->n;
		p->n = this;
		if (n) n->p = this;
		else *tail = this;
	}

	void push_back(Data* d, Node **tail) {
		pd = d, insert(*tail, tail);
	}
} buf[maxID + maxPage + 1];

int bcnt;

Node head[maxPage];
Node* tail[maxPage];

Node* nodetbl[maxID + 1];

void remove(int page, rt Node *p) {
	p->erase(&tail[page]);
}

Node* push_back(int page, Data* pd) {
	rt Node* p = &buf[bcnt++];
	p->push_back(pd, &tail[page]);
	tail[page] = p;
	return p;
}

void pushsortedR(int pg, Node* q) {
	rt Node* p = tail[pg];
	for (; p != &head[pg] && *q->pd < *p->pd; p = p->p);

	q->insert(p, &tail[pg]);
}

void pushsortedF(int pg, Node* q) {
	rt Node* p = &head[pg];
	for (; p->n && *p->n->pd < *q->pd; p = p->n);

	q->insert(p, &tail[pg]);
}

void pushfront(int pg, Node* q) {
	q->insert(&head[pg], &tail[pg]);
}
#else
struct Data {
	int id;
	int score;
	int pos;

	int page() {
		return pos / pageSize;
	}

	int slot() {
		return pos % pageSize;
	}
	bool operator<(const Data& r) const {
		if (score != r.score) return score > r.score;
		return id < r.id;
	}

	bool operator<=(const Data& r) const {
		if (score != r.score) return score >= r.score;
		return id <= r.id;
	}

	bool operator>(const Data& r) const {
		if (score != r.score) return score < r.score;
		return id > r.id;
	}

	bool operator!=(const Data& r) const {
		return (score != r.score || id != r.id);
	}
	void print() {
		cout << id << "," << score << endl;
	}

};

#endif

Data* sorted[maxID + 1];
Data user[maxID + 1];
Data user2[maxID + 1];

struct Idx {
	int first;
	int last;
};

int sp;
Idx stack[maxUpdate];

void push(int f, int l) {
	stack[sp++] = { f,l };
}

void pop(int& f, int& l) {
	sp--;
	f = stack[sp].first, l = stack[sp].last;
}

void quickSort(int first, int last) {
	int pivot;
	int i;
	int j;
	rt Data* temp;

	if (first < last) {
		pivot = first;
		i = first;
		j = last;

		rint m = (i + j) >> 1;

		if (pivot != m) {
			temp = sorted[pivot];
			sorted[pivot] = sorted[m];
			sorted[m] = temp;
		}
		while (i < j) { 
			while (*sorted[i] <= *sorted[pivot] && i < last) i++;
			while (*sorted[j] > *sorted[pivot])	j--;
			if (i < j) {
				temp = sorted[i];
				sorted[i] = sorted[j];
				sorted[j] = temp;
			}
		}

		if (pivot != j) {
			temp = sorted[pivot];
			sorted[pivot] = sorted[j];
			sorted[j] = temp;
		}
		quickSort(first, j - 1);
		quickSort(j + 1, last);
	}
}

clock_t profile[2];
template <typename T>
void quickSortStack(int first, int last, T** input) {
	rint pivot, i,j;
	rt T* temp;
	sp = 0;

	push(first, last);

	while (sp > 0) {
		pop(first, last);

		if (first < last) {
			pivot = first;
			i = first;
			j = last;

			rint m = (i + j) >> 1;

			if (pivot != m) {
				temp = input[pivot];
				input[pivot] = input[m];
				input[m] = temp;
			}
			while (i < j) {
				while (*input[i] <= *input[pivot] && i < last) i++;
				while (*input[j] > *input[pivot]) j--;

				if (i < j){
					temp = input[i];
					input[i] = input[j];
					input[j] = temp;
				}
			}

			if (pivot != j) {
				temp = input[pivot];
				input[pivot] = input[j];
				input[j] = temp;
			}

			push(first, j-1);
			push(j+1, last);
		}
	}
}

template <typename T>
struct Heap {
	T* heap[maxID + 2];
	int sz;

	void push(T* value) {
		heap[sz] = value;

		rint current = sz;
		while (current > 1 && *heap[current] < *heap[current >> 1]) {
			rint pa = current >> 1;
			Data* temp = heap[pa];
			heap[pa] = heap[current];
			heap[current] = temp;
			current = pa;
		}

		sz++;
	}

	void pop() {
		sz--;

		heap[1] = heap[sz];

		rint current = 1;
		rint lc = 2;
		while (lc < sz) {
			rint rc = lc + 1;
			rint ch;
			if (rc == sz) 	ch = lc;
			else ch = *heap[lc] < *heap[rc] ? lc : rc;

			if (*heap[current] < *heap[ch]) break;

			Data* temp = heap[current];
			heap[current] = heap[ch];
			heap[ch] = temp;

			current = ch;

			lc = (ch << 1);
		}
	}
};

Heap<Data> hp;

int tc;
void init() {
	if (tc == 0) {
		fe(i, 1, maxID) {
			user[i].id = i;
		}
	}
#ifdef LIST
	f(i, 0, maxPage) {
		head[i].n = 0;
		tail[i] = &head[i];
	}
	bcnt = maxPage;
#endif
	tc++;
}

#ifdef LIST
void update(int id, int scoredelta) {
	rint pg = nodetbl[id]->pd->page;
	rint endpg = pg;
	nodetbl[id]->pd->score += scoredelta;
	remove(endpg, nodetbl[id]);

	for (; pg > 0 && *nodetbl[id]->pd < *tail[pg - 1]->pd; pg--);
	pushsortedR(pg, nodetbl[id]);
	//pushsortedF(pg, nodetbl[id]);
	nodetbl[id]->pd->page = pg;
	
	while (pg < endpg) {
		rint id2 = tail[pg]->pd->id;
		remove(pg, nodetbl[id2]);
		rint nextpg = pg + 1;
		pushfront(nextpg, nodetbl[id2]);
		nodetbl[id2]->pd->page = nextpg;
		pg = nextpg;
	}
}
#else
void updateR(int id, int scoredelta) {
	rt Data* pu = &user[id];
	pu->score += scoredelta;
	rint end = pu->pos;
	rint pos = end;
	for (; pos > 0 && *pu < *sorted[pos-1]; pos--);

	for (rint i = end; i > pos; --i) {
		sorted[i] = sorted[i - 1];
		sorted[i]->pos = i;
	}

	sorted[pos]=pu;
	sorted[pos]->pos = pos;
}

void updateF(int id, int scoredelta) {
	rt Data* pu = &user[id];
	pu->score += scoredelta;
	rint end = pu->pos;
	rint pos = 0;
	
	for (; pos < end && *sorted[pos] < *pu; pos++);

	rt Data* pv = sorted[pos];

	for (rint i = end; i > pos; --i) {
		sorted[i] = sorted[i - 1];
		sorted[i]->pos = i;
	}

	sorted[pos] = pu;
	sorted[pos]->pos = pos;
}
#endif

int main() {

	clock_t ts[2];
	f(t, 0, 10) {
		srand(t);
		init();

		fe(i, 1, maxID) {
			user[i].score = rand()%10000;
			//user[i].score = i;
			sorted[i-1] = &user[i];
			user2[i-1] = user[i];
		}
		ts[0] = clock();
#if 0
		quickSortStack(0, maxID-1, sorted);
		//quickSort(0, maxID-1);
		f(i, 0, maxID) {
			sorted[i]->pos = i;
		}
#else
		hp.sz = 1;
		f(i, 0, maxID) 	hp.push(sorted[i]);

		f(i, 0, maxID) {
			sorted[i] = hp.heap[1];
#ifdef LIST
#else
			sorted[i]->pos = i;
#endif
			hp.pop();
		}
#endif
		profile[0] += (clock()-ts[0]);

#ifdef LIST
		f(i, 0, maxID) {
			rint page = i / pageSize;
			sorted[i]->page = page;
			nodetbl[sorted[i]->id] = push_back(page, sorted[i]);
		}

		ts[1] = clock();
		fe(i, 1, maxUpdate) {
			rint dscore = rand() % 10;
			rint id = 1 + (rand() % maxID);
			update(id, dscore);
			user2[id - 1].score = user[id].score;
		}
		profile[1] += (clock() - ts[1]);
#else
		ts[0] = clock();
		fe(i, 1, maxUpdate) {
			rint dscore = rand() % 10;
			rint id = 1 + (rand() % maxID);
			updateR(id, dscore);
			user2[id - 1].score = user[id].score;
		}
		profile[1] += (clock() - ts[0]);
#endif
		sort(user2, user2 + maxID);
		int ecnt = 0;
#ifdef LIST
		int idx = 0;
		f(i,0,maxPage) {
			rt Node* p = head[i].n;
			for (; p; p = p->n) {
				if (user2[idx++] != *p->pd) {
					ecnt++;
				}
			}
		}
#else
		f(i, 0, maxID) {
			if (user2[i] !=  *sorted[i]) {
				ecnt++;
			}
		}
#endif
		cout << ecnt << endl;
	}

	printf("%.3f, %.3f\n", double(profile[0]) / CLOCKS_PER_SEC, double(profile[1]) / CLOCKS_PER_SEC);

	return 0;
}