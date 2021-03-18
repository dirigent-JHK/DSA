/*
Created by Jeonghan Kim

Square-root decomposition by linked list for large range of value (up to 1e9)
*/

const int maxNum = (int)1e9;
const int maxN = 1<<18;
const int maxBucket = 1<<9;
const int BucketSize = maxN/maxBucket;
const int halfBucket = BucketSize / 2;

#define rt register
#define rint rt int
#define f(i,a,b) for(rint i = a;i<b; ++i)
#define fe(i,a,b) for(rint i = a;i<=b; ++i)
int nObj[maxBucket];

struct Node {
	int v;
	Node* p, * n;

	void insert(Node* pv) {
		p = pv, n = pv->n;
		p->n = this;
		if (n) n->p = this;
	}

	void alloc(int d, Node* pv) {
		v = d, insert(pv);
	}

	void erase() {
		p->n = n;
		if (n) n->p = p;
	}
} buf[maxN], head[maxBucket], *tail[maxBucket];
int bn;

void init() {
	bn = 0;
	f(i, 0, maxBucket) {
		nObj[i] = 0;
		head[i].n = 0;
		tail[i] = &head[i];
	}
}

#define tailUpdate(a,p) if ((p) == tail[a]) tail[a] = (p)->n

#if 0
void pushfront(int addr, int v) {
	buf[bn++].alloc(v, &head[addr]), ++nObj[addr];
	tailUpdate(addr, &head[addr]);
}
void pushback(int addr, int v) {
	buf[bn].alloc(v, tail[addr]), ++nObj[addr];
	tail[addr] = &buf[bn++];
}
#endif

void insertfront(int addr, Node* q) {
	q->insert(&head[addr]), ++nObj[addr];
	tailUpdate(addr, &head[addr]);
}

void insertback(int addr, Node *q) {
	q->insert(tail[addr]), ++nObj[addr];
	tail[addr] = q;
}

void pushsortedF(int addr, int v) {
	rt Node* p = &head[addr];
	for (; p->n && p->n->v < v; p = p->n);
	buf[bn++].alloc(v, p), ++nObj[addr];

	tailUpdate(addr, p);
}

void pushsortedR(int addr, int v) {
	rt Node* p = tail[addr];
	for (; p->p && v < p->v; p = p->p);
	buf[bn++].alloc(v, p), ++nObj[addr];

	tailUpdate(addr, p);
}

void erase(int addr, Node* p) {
	p->erase();
	if (p == tail[addr]) tail[addr] = p->p;
	--nObj[addr];
}

void insert(int data) {
	f(a, 0, maxBucket) {
		if (nObj[a] < BucketSize) {
			pushsortedR(a, data);
			break;
		}

		if (data < tail[a]->v) {
			pushsortedR(a, data);
			f(i, a, maxBucket - 1) {
				Node* q = tail[i];
				erase(i, tail[i]);

				insertfront(i + 1, q);
				if (nObj[i + 1] <= BucketSize) break;
			}
			break;
		}
	}
}

Node* search(int idx, int& addr) {
	addr = idx / BucketSize;
	rint rem = idx % BucketSize;
	rt Node* p = 0;

	if (nObj[addr] < halfBucket || rem < halfBucket) {
		for (p = head[addr].n; p; p = p->n) {
			if (rem-- == 0) return p;
		}
	} else {
		rint n = nObj[addr];
		for (p = tail[addr]; p != &head[addr]; p = p->p) {
			if (--n == rem) return p;
		}
	}
	return p;
}

int remove(int index) {
	int addr;
	Node* p = search(index, addr);
	if (p) {
		erase(addr, p);
		int last = maxBucket - 1;
		f(i, addr, last) {
			int ni = i + 1;

			if (nObj[ni] == 0) break;
			Node* q = head[ni].n;
			erase(ni, head[ni].n);
			insertback(i, q);
		}

		return p->v;
	}
	return 0;
}

int searchByIndex(int index) {
	int addr;
	Node* p = search(index, addr);
	if (p) return p->v;

	return 0;
}