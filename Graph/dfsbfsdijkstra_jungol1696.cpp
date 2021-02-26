/*
Created by Jeonghan Kim

DFS, BFS, Dijkstra algorithm 
for problem 1696 in jungol

Dijkstra is most slow in this case
*/

#include <iostream>

#define rt register
#define rint rt int
using namespace std;

const int LM = 51;
const int INF = 99;	// max. number of changes from black square to white square (Horizaontal : 49, Vertial : 49+1)

int N, map[LM][LM], levTbl[LM][LM];

int dr[] = { -1,1, 0, 0 };
int dc[] = {0,0, -1, 1 };

#define f(i,a,b) for(rint i=a;i<b;++i)
#define fe(i,a,b) for(rint i=a;i<=b;++i)

struct Vertex {
	int r, c, lev;
	const bool operator<(const Vertex& r) const { return lev > r.lev; }
};

#define DIJKSTRA
//#define BFS
//#define DFS

#ifdef BFS
int fr, re;
Vertex que[LM*LM*INF];

void bfs() {
	Vertex v = { 1,1, 0 };
	que[re++] = v;

	while (fr < re) {
		v = que[fr++];
		f(i, 0, 4) {
			Vertex w;
			w.r = v.r + dr[i];
			w.c = v.c + dc[i];
			w.lev = v.lev + map[w.r][w.c];
			if (w.lev < levTbl[w.r][w.c] ) {
				levTbl[w.r][w.c] = w.lev;
				que[re++] = w;
			}
		}
		//push(tmp.r + "0211"[i] - '1', tmp.c+"1102"[i] - '1', tmp.lev);
	}
}

#endif

void input() {
	scanf("%d", &N);
	fe(i, 1, N) {
		fe(j, 1, N) {
			//get input when no space between number
			scanf("%1d", &map[i][j]);		
			// inversion of value for convenience
			// cost increased when changing color from black to white
			//mat[i][j] = 1 - mat[i][j];
			map[i][j] = !map[i][j];
			levTbl[i][j] = INF;
		}
	}
}

#ifdef DFS
void dfs(int r, int c, int lev) {
	
	if (levTbl[r][c] <= lev + map[r][c]) return;
	levTbl[r][c] = lev + map[r][c];
	f(i, 0, 4) dfs(r + dr[i], c + dc[i], lev + map[r + dr[i]][c + dc[i]]);
}

Vertex stack[LM * LM * INF];
int sp;

void dfs_stack() {
	Vertex v = { 1,1,0 };
	stack[sp++] = v;

	while (sp > 0) {
		v = stack[--sp];
		f(i, 0, 4) {
			Vertex w;
			w.r = v.r + dr[i];
			w.c = v.c + dc[i];
			w.lev = v.lev + map[w.r][w.c];
			if (w.lev < levTbl[w.r][w.c]) {
				levTbl[w.r][w.c] = w.lev;
				stack[sp++] = w;
			}
		}
	}
}

#endif


#ifdef DIJKSTRA
template <typename T>
struct Heap {
	T* heap[LM * LM * INF*4+1];
	int sz;

	void up(rint current) {
		while (current > 1 && *heap[current] < *heap[current >> 1]) {
			rint pa = current >> 1;

			T* temp = heap[pa];
			heap[pa] = heap[current];
			heap[current] = temp;
			//heap[pa]->bid = pa;
			//heap[current]->bid = current;
			current = pa;
		}
	}
	void down(rint current) {
		rint lc = current << 1;
		while (lc < sz) {
			rint ch;
			rint rc = lc + 1;
			if (rc == sz) 	ch = lc;
			else ch = *heap[lc] < *heap[rc] ? lc : rc;

			if (*heap[current] < *heap[ch]) break;

			T* temp = heap[current];
			heap[current] = heap[ch];
			heap[ch] = temp;
			//heap[ch]->bid = ch;
			//heap[current]->bid = current;
			current = ch;
			lc = (ch << 1);
		}
	}

	void update(rint current) {
		if (current > 1 && *heap[current] < *heap[current >> 1])
			up(current);
		else
			down(current);
	}

	void push(T* value) {
		heap[sz] = value;
		rint current = sz;
		while (current > 1 && *heap[current] < *heap[current >> 1]) {
			rint pa = current >> 1;

			T* temp = heap[pa];
			heap[pa] = heap[current];
			heap[current] = temp;
			//heap[pa]->bid = pa;
			//heap[current]->bid = current;
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
			rint ch;
			rint rc = lc + 1;
			if (rc == sz) 	ch = lc;
			else ch = *heap[lc] < *heap[rc] ? lc : rc;

			if (*heap[current] < *heap[ch]) break;

			T* temp = heap[current];
			heap[current] = heap[ch];
			heap[ch] = temp;
			//heap[ch]->bid = ch;
			//heap[current]->bid = current;
			current = ch;
			lc = (ch << 1);
		}
	}
};

Heap<Vertex> hp;
Vertex hbuf[LM * LM * INF*4];
int hcnt;

void dijkstra() {
	hcnt = 0;
	hp.sz = 1;		//dummy
	Vertex* v = &hbuf[hcnt];
	v->r = 1, v->c = 1, v->lev = 0;
	hp.push(v);
	hcnt++;

	while (hp.sz > 1) {
		v = hp.heap[1];
		hp.pop();

		if (levTbl[v->r][v->c] <= v->lev) continue;
		levTbl[v->r][v->c] = v->lev;

		f(i, 0, 4) {
			Vertex* w = &hbuf[hcnt++];
			w->r = v->r + dr[i];
			w->c = v->c + dc[i];
			w->lev = v->lev + map[w->r][w->c];
			if (w->lev < levTbl[w->r][w->c]) hp.push(w);
		}
	}
}

#endif

int main() {
	//freopen("input_makemaze.txt", "r", stdin);

	input();
#ifdef DFS
	//dfs(1, 1, 0);
	dfs_stack();
#endif
#ifdef BFS
	bfs();
#endif
#ifdef DIJKSTRA	
	dijkstra();
#endif
	printf("%d\n", levTbl[N][N]);
	return 0;
}