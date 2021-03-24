/*
Created by Jeonghan Kim

Top10 Partial insertion sort by double linked-list
*/

#define rt register
#define rint rt int

#define f(i,a,b) for(rint i=a;i<b;++i)
#define fe(i,a,b) for(rint i=a;i<=b;++i)

const int maxID = 100000 + 1;
struct Node {
	int v;
	int id;
	Node* p, * n;

	void insert(Node* pv, Node **tail) {
		p = pv, n = pv->n;
		p->n = this;
		if (n) n->p = this;
		else *tail = this;
	}

	void erase(Node **tail) {
		p->n = n;
		if (n) n->p = p;
		else *tail = p;
	}

	void set(int vv, int i) {
		v = vv, id = i;
	}

	bool operator<(const Node& r) const {
		if (v != r.v) return v > r.v;
		return id < r.id;
	}
} buf[maxID];

Node head, * tail;
int num;
int bcnt;

void init() {
	bcnt = 0;
	head.n = 0;
	tail = head.n;
	num = 0;
}


void addUser(int uID, int height) {
	Node& nd = buf[bcnt++];
	nd.set(height, uID);
	if (num > 0) {
		if (num == 10) {
			if (nd < *tail) {
#if 0
				rt Node* p = &head;
				for (; p->n && *p->n < nd; p = p->n);
#else
				rt Node* p = tail;
				for (; p != &head && nd < *p; p = p->p);
#endif
				nd.insert(p, &tail);
				tail->erase(&tail);
			}
		}
		else {
			++num;
			rt Node* p = tail;
			for (; p != &head && nd < *p; p = p->p);
			nd.insert(p, &tail);
		}
	}
	else {
		nd.insert(&head, &tail);
		num = 1;
	}
}

int getTop10(int result[10]) {
	rint ret = 0;
	rt Node* p = head.n;
	for (; p; p = p->n) {
		result[ret++] = p->id;
	}
	return ret;
}