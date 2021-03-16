/*
Created by Jeonghan Kim

Permutation and Combination
*/

#include <iostream>
#define rt register
#define rint rt int

using namespace std;

//6+1
const int maxDice = 7;
int chk[maxDice];
int idx[maxDice];

// around 7^6
const int maxN = 1 << 17;

int res[maxN][maxDice];
int ridx;

#define f(i,a,b) for (rint i = a; i<b;++i)
#define fe(i,a,b) for (rint i = a; i<=b;++i)

#define getres(k) {fe(i, 1, k) {res[ridx][i] = idx[i];} \
				ridx++; }

void printres(int k) {
	f(r, 0, ridx) {
		printf("[%d] : ", r + 1);
		fe(i, 1, k) cout << res[r][i] << " ";
		cout << endl;
	}
}

void init() {
	f(i, 0, maxDice) {
		chk[i] = 0;
		idx[i] = 0;
	}

	idx[0] = 1;
	ridx = 0;
}

void perm_rep(int s, int n, int k) {
	if (s > k) {
		getres(k);
		return;
	}
	
	f(i, 1, n + 1) {
		idx[s] = i;
		perm_rep(s+1, n, k);
	}
}

void perm(int s, int n, int k) {
	if (s > k) {
		getres(k);
		return;
	}

	fe(i, 1, n) {
		if (chk[i]) continue;
		chk[i] = 1;
		idx[s] = i;
		perm(s + 1, n, k);
		chk[i] = 0;
	}
}

void comb_rep(int s, int n, int k) {
	if (s > k) {
		getres(k);
		return;
	}

	f(i, idx[s - 1], n + 1) {
		idx[s] = i;
		comb_rep(s + 1, n, k);
	}
}

void comb(int s, int n, int k) {
	if (s > k) {
		getres(k);
		return;
	}

	f(i, idx[s - 1], n + 1) {
		if (chk[i]) continue;
		chk[i] = 1;
		idx[s] = i;

		comb(s + 1, n, k);
		chk[i] = 0;
	}
}

typedef void (*fpTest)(int, int);

void PermutationRepeated(int n, int k) {
	init();
	perm_rep(1, n, k);
}

void Permutation(int n, int k) {
	init();
	perm(1, n, k);
}

void CombinationRepeated(int n, int k) {
	init();
	comb_rep(1, n, k);
}

void Combination(int n, int k) {
	init();
	comb(1, n, k);
}

enum {
	OP_PERMREP,
	OP_PERM,
	OP_COMBREP,
	OP_COMB,
	MAX_OP
};


fpTest ftbl[MAX_OP] = {
	PermutationRepeated,
	Permutation,
	CombinationRepeated,
	Combination
};

int main() {
	int n = 6;
	int k = 3;
	int op;
	//op = OP_PERMREP;
	//op = OP_PERM;
	//op = OP_COMBREP;
	op = OP_COMB;
	ftbl[op](n, k);
	printres(k);
}