/*
Created by Jeonghan Kim

Square-root decomposition by linked list for large range of value (up to 1e9)
*/

const int maxNum = (int)1e9;
const int maxN = 1<<18;
const int maxAddr = 1<<9;
const int maxBucket = maxN/maxAddr;
const int halfBucket = maxBucket / 2;

#define rt register
#define rint rt int
#define f(i,a,b) for(rint i = a;i<b; ++i)
#define fe(i,a,b) for(rint i = a;i<=b; ++i)

struct Node {
	int v;
	Node* p, * n;

	void insert(Node* pv, Node **tail) {
		p = pv, n = pv->n;
		p->n = this;
		if (n) n->p = this;
		else *tail = this;
	}

	void alloc(int d, Node* pv, Node **tail) {
		v = d, insert(pv, tail);
	}

	void erase(Node **tail) {
		p->n = n;
		if (n) n->p = p;
		else *tail = p;
	}
};
int bn;

struct List {
	int bn;
	int cnt[maxAddr];
	Node buf[maxN];
	Node head[maxAddr], * tail[maxAddr];

	void init() {
		bn = 0;
		f(i, 0, maxAddr) {
			cnt[i] = 0;
			head[i].n = 0;
			tail[i] = &head[i];
		}
	}

	void insertF(int addr, Node* q) {
		q->insert(&head[addr], &tail[addr]), ++cnt[addr];
	}

	void insertB(int addr, Node* q) {
		q->insert(tail[addr], &tail[addr]), ++cnt[addr];
	}

	void pushsortedF(int addr, int v) {
		rt Node* p = &head[addr];
		for (; p->n && p->n->v < v; p = p->n);
		buf[bn++].alloc(v, p, &tail[addr]), ++cnt[addr];
	}

	void pushsortedR(int addr, int v) {
		rt Node* p = tail[addr];
		for (; p->p && v < p->v; p = p->p);
		buf[bn++].alloc(v, p, &tail[addr]), ++cnt[addr];
	}

	void erase(int addr, Node* p) {
		p->erase(&tail[addr]);
		--cnt[addr];
	}

	void insert(int data) {
		f(a, 0, maxAddr) {
			if (cnt[a] < maxBucket) {
				pushsortedR(a, data);
				break;
			}

			if (data < tail[a]->v) {
				pushsortedR(a, data);
				f(i, a, maxAddr - 1) {
					Node* q = tail[i];
					erase(i, tail[i]);

					insertF(i + 1, q);
					if (cnt[i + 1] <= maxBucket) break;
				}
				break;
			}
		}
	}

	Node* search(int idx, int& addr) {
		addr = idx / maxBucket;
		rint rem = idx % maxBucket;
		rt Node* p = 0;

		if (cnt[addr] < halfBucket || rem < halfBucket) {
			for (p = head[addr].n; p; p = p->n) {
				if (rem-- == 0) return p;
			}
		}
		else {
			rint n = cnt[addr];
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
			int last = maxAddr - 1;
			f(i, addr, last) {
				int ni = i + 1;

				if (list.cnt[ni] == 0) break;
				Node* q = head[ni].n;
				erase(ni, head[ni].n);
				insertB(i, q);
			}

			return p->v;
		}
		return 0;
	}
} list;

void init() {
	list.init();
}

#if 0
void pushfront(int addr, int v) {
	buf[bn++].alloc(v, &head[addr], &tail[addr]), ++nObj[addr];
}
void pushback(int addr, int v) {
	buf[bn].alloc(v, tail[addr], &tail[addr]), ++nObj[addr];
	tail[addr] = &buf[bn++];
}
#endif

int remove(int index) {
	return list.remove(index);
}

void insert(int data) {
	list.insert(data);
}

int searchByIndex(int index) {
	int addr;
	Node* p = list.search(index, addr);
	if (p) return p->v;

	return 0;
}