/*
Created by Jeonghan Kim

Performance comparison of Hash when string is encoded into 64bit key and original string key
Twice faster for 64bit integer case

*/


#include <iostream>
#include <cstdlib>
#include <ctime>
using namespace std;

#define rt register
#define rint rt int

#define f(i,a,b) for(rint i = a;i<b;++i)
#define fe(i,a,b) for(rint i = a;i<=b;++i)

typedef unsigned long long ull;
typedef unsigned int ui;

const int maxN = (int)1e5;

//#define STR

int mstrcmp(const char* s1, const char* s2) {
	rint i = 0;
	for (i = 0; s1[i]; ++i) {
		if (s1[i] != s2[i]) return s1[i] - s2[i];
	}

	return s1[i] - s2[i];
}

void mstrcpy(char* dst, const char* src) {
	rint i = 0;
	for (; src[i]; ++i) dst[i] = src[i];
	dst[i] = 0;
}

struct HashN {
	int id;
	int val;
	HashN* n;

#ifdef STR
	char key[12];
	void alloc(int i, int v, char* s, HashN* hd) {
		id = i, val = v, n = hd->n, hd->n = this;
		mstrcpy(key, s);
	}
#else
	ull key;
	void alloc(int i, int v, ull k, HashN* hd) {
		id = i, val = v, n = hd->n, hd->n = this;
		key = k;
}
#endif
};

const int word = 27;
// first 3 characters used for addressing (collision avoidance)
const int maxHash = word * word * word + 1;

#ifdef STR
ui makeKey(const char* s, char *dst) {
	ui addr = (s[0]-96);
	dst[0] = s[0];
	f(i, 1, 3) {
		addr *= word, addr += (s[i] - 96);
		dst[i] = s[i];
	}
	rint i = 3;
	for (; s[i]; ++i) dst[i] = s[i];
	dst[i] = 0;
	return addr;
}
#else
ui makeKey(const char* s, ull& key) {
	ui addr = (s[0]-96);
	f(i, 1, 3) addr *= word, addr += (s[i] - 96);
	key = addr;
	for (rint i = 3; s[i]; ++i) {
		key *= word, key += (s[i] - 96);
	}
	return addr;
}
#endif

#define next(p) for(;(p);(p)=(p)->n)

struct Hash {
	HashN buf[maxN + 1];
	HashN tbl[maxHash];
	int cnt;

#ifdef STR
	char key[12];
#else
	ull key;
#endif
	ui addr;

	void init() {
		cnt = 0;
		f(i, 0, maxHash) tbl[i].n = 0;
	}

#ifdef STR
	int find(char* s) {
		addr = makeKey(s, key);
		rt HashN* p = tbl[addr].n;
		next(p) { if (mstrcmp(p->key,s) == 0)  return p->id; }
		return -1;
	}
#else
	int find(char* s) {
		addr = makeKey(s, key);
		rt HashN* p = tbl[addr].n;
		next(p) { if (p->key == key) 	return p->id;}
		return -1;
	}

#endif
	void alloc(char* s, int v, int id) {
		buf[cnt++].alloc(id, v, key, &tbl[addr]);
	}
} hw;

struct Data {
	char name[12];
	int val;
};

Data gt[maxN];
Data* gt_unique[maxN];

void makeName(char* s) {
	f(j, 0, 3) {
		s[j] = rand() % 26 + 'a';
	}

	int n = 3 + rand() % 8;
	f(j, 3, n) {
		s[j] = rand() % 26 + 'a';
	}
	s[n] = 0;
}

void genData() {
	f(i, 0, maxN) {
		Data& g = gt[i];
		makeName(g.name);
		g.val = rand();
		g.val <<= 16;
		g.val += rand();
	}
}

const int maxTC = 100;
clock_t profile[maxTC];

int main() {
	
	f(tc, 0, maxTC) {
		hw.init();
		int ecnt = 0;
		srand(tc);
		genData();

		profile[tc] = clock();
		int cnt = 0;
		f(i, 0, maxN) {
			Data& g = gt[i];
			int id = hw.find(g.name);
			if (id < 0) {
				hw.alloc(g.name, g.val, cnt);
				gt_unique[cnt] = &g;
				cnt++;
			}
		}

		f(i, 0, cnt) {
			Data & g = *gt_unique[i];
			int id = hw.find(g.name);
			if (id < 0) ecnt++;
			else if (g.val != gt_unique[id]->val) ecnt++;
		}
		
		profile[tc] = (clock() - profile[tc]);

		printf("%d, %.3f\n", ecnt, double(profile[tc]) / CLOCKS_PER_SEC);
	}

	clock_t min = (clock_t)1e9;
	clock_t max = 0;
	clock_t sum = 0;
	f(t, 0, maxTC) {
		sum += profile[t];
		if (profile[t] > max) max = profile[t];
		else if (profile[t] < min) min = profile[t];
	}

	printf("max : %.3f, min : %.3f, avg: %.3f\n", double(max) / CLOCKS_PER_SEC, double(min) / CLOCKS_PER_SEC, double(sum) / CLOCKS_PER_SEC/ maxTC);
	return 0;
}