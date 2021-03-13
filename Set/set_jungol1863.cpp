/*
Created by Jeonghan Kim

Set construction by path compressing
O(log*(n)) ~= O(1)
Jungol 1863
*/
#include <iostream>
using namespace std;

#define rt register
#define rint rt int

#define f(i,a,b) for(rint i=a;i<b;++i)
#define fe(i,a,b) for(rint i=a;i<=b;++i)

#define PATH_COMPRESSION
#define STACK

const int maxStudents = 50000+1;

int N, M, grp[maxStudents];

int maxReligions;
#ifdef PATH_COMPRESSION
#ifdef STACK
int stack[maxStudents];
int sp;
int find(int k) {
	sp = 0;
	while (k != grp[k]) stack[sp++] = k, k = grp[k];
	while (sp) grp[stack[--sp]] = k;
	return k;
}
#else
int find(int k) {
	if (grp[k] == k) return k;
	return grp[k] = find(grp[k]);
}
#endif
#endif

int main() {
	freopen("input1863.txt", "r", stdin);
	scanf("%d %d", &N, &M);
	maxReligions = N;

	fe(i, 1, N) grp[i] = i;

	int u, v;
	f(i, 0, M) {
		scanf("%d %d", &u, &v);
#ifdef PATH_COMPRESSION
		u = find(u), v = find(v);
		if (u == v) continue;
		grp[v] = u;
#else
		u = grp[u];
		v = grp[v];

		if (u == v) continue;
		fe(j, 1, N) {if (grp[j] == v) grp[j] = u;}
#endif
		maxReligions--;
		
	}
	printf("%d\n", maxReligions);
	return 0;
}