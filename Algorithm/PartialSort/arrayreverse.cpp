/*
Created by Jeonghan Kim

Top10 Partial insertion sort by array(reverse order)
*/


#define rt register
#define rint rt int

#define f(i,a,b) for(rint i=a;i<b;++i)

const int maxID = 100000 + 1;
struct Data {
	int v;
	int id;
	bool operator<(const Data& r) const {
		if (v != r.v) return v > r.v;
		return id < r.id;
	}

	void set(int vv, int i) { v = vv, id = i; }
} buf[maxID];

int num;
int bcnt;
Data* top10[11];

void init() {
	num = 0;
	bcnt = 0;
}

void addUser(int uID, int height) {
	Data & d = buf[bcnt++];
	d.set(height, uID);

	if (num > 0) {
		rint pos = num;
		for (; pos > 0 && d < *top10[pos-1] ; --pos);
		for (rint i = num; i > pos; --i) top10[i] = top10[i - 1];
		
		top10[pos] = &d;
		if (num < 10) ++num;
	}
	else  {
		top10[0] = &d;
		num = 1;
	}
}

int getTop10(int result[10]) {
	f(i,0,num) result[i] = top10[num-1-i]->id;
	return num;
}