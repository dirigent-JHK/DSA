/*
Created by Jeonghan Kim

Top10 Partial insertion sort by single linked-list
*/

#define rt register
#define rint rt int

#define f(i,a,b) for(rint i=a;i<b;++i)
#define fe(i,a,b) for(rint i=a;i<=b;++i)

const int maxID = 100000 + 1;
struct Node {
	int v;
	int id;
	Node * n;

	void insert(Node* pv) {
		n = pv->n;
		pv->n = this;
	}
	void set(int vv, int i) {
		v = vv, id = i;
	}

	bool operator<(const Node& r) const {
		if (v != r.v) return v > r.v;
		return id < r.id;
	}

} buf[maxID];

Node head;
int num;
int bcnt;

void init() {
	bcnt = 0;
	head.n = 0;
	num = 0;
}

void addUser(int uID, int height) {
	Node& nd = buf[bcnt++];
	nd.set(height, uID);

	if (num > 0) {
		rt Node* p = &head;
		rint cnt = 0;
		for (; cnt < num && p->n && *p->n < nd; cnt++, p = p->n);

		if (num == 10) { 
			if (cnt < num) nd.insert(p); 
		} else nd.insert(p), ++num;
	}
	else { nd.insert(&head), num = 1; }
}

int getTop10(int result[10]) {
	rint ret = 0;

	rt Node* p = head.n;
	for (; p; p = p->n) {
		result[ret] = p->id;
		if (++ret == 10) break;
	}
	return ret;
}