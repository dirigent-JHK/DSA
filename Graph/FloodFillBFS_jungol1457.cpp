/*
Created by Jeonghan Kim

Problem solving of jungol 1457
Flood-fill based upon BFS using queue
*/

#include <iostream>
using namespace std;

int nRows, nCols, K;
int maxRow, maxCol;
#define f(i,a,b) for(register int i=a;i<b;++i)

const int MAX_N = 100;

int dir[4][2] = {
	1,0,
	-1,0,
	0,1,
	0, -1
};


struct Pt {
	int r;
	int c;

	Pt() {}
	Pt(int r_, int c_) : r(r_), c(c_) { }
};


Pt queue[MAX_N * MAX_N];
int head, tail;

void pop(int& r, int& c) {
	Pt& pt = queue[head++];
	r = pt.r, c = pt.c;
}

int input[MAX_N * MAX_N];
int nAns;


void fill(int x[], int y[], int map[][MAX_N]) {
	f(i, y[0], y[1]) {
		f(j, x[0], x[1]) {
			map[maxRow - i][j] = 1;
		}
	}
}

bool boundaryChk(int r, int c) {
	if (r < 0 || r > maxRow || c < 0 || c > maxCol) return false;
	return true;
}


struct Idx {
	int first;
	int last;
	Idx() {}
	Idx(int f, int l) : first(f), last(l) {}
};


Idx stack_idx[MAX_N * MAX_N];
int sp_idx;

void push_idx(int f, int l) {
	Idx& idx = stack_idx[sp_idx++];
	idx.first = f, idx.last = l;
}

bool pop_idx(int& f, int& l) {

	if (sp_idx == 0) return false;
	Idx& idx = stack_idx[--sp_idx];
	f = idx.first, l = idx.last;

	return true;
}

void quickSort(int first, int last)
{
	int pivot;
	int i;
	int j;
	int temp;
	int m;

	push_idx(first, last);

	while (sp_idx > 0) {
		pop_idx(first, last);
		if (first < last)
		{
			pivot = first;
			i = first;
			j = last;
			m = (i + j) >> 1;
			if (m != pivot) {
				temp = input[m];
				input[m] = input[pivot];
				input[pivot] = temp;
			}

			while (i < j)
			{
				while (input[i] <= input[pivot] && i < last)
				{
					i++;
				}
				while (input[j] > input[pivot])
				{
					j--;
				}
				if (i < j)
				{
					temp = input[i];
					input[i] = input[j];
					input[j] = temp;
				}
			}

			temp = input[pivot];
			input[pivot] = input[j];
			input[j] = temp;

			push_idx(first, j - 1);
			push_idx(j+ 1, last);
		}
	}


}


void quickArgSort(int first, int last, int idx[])
{
	int pivot;
	int i;
	int j;
	int temp;
	int m;

	push_idx(first, last);

	while (sp_idx > 0) {
		pop_idx(first, last);
		if (first < last)
		{
			pivot = first;
			i = first;
			j = last;
			m = (i + j) >> 1;
			if (m != pivot) {
				temp = idx[m];
				idx[m] = idx[pivot];
				idx[pivot] = temp;
			}

			while (i < j)
			{
				while (input[idx[i]] <= input[idx[pivot]] && i < last)
				{
					i++;
				}
				while (input[idx[j]] > input[idx[pivot]])
				{
					j--;
				}
				if (i < j)
				{
					temp = idx[i];
					idx[i] = idx[j];
					idx[j] = temp;
				}
			}

			if (pivot != j) {
				temp = idx[pivot];
				idx[pivot] = idx[j];
				idx[j] = temp;
			}

			push_idx(first, j - 1);
			push_idx(j + 1, last);
		}
	}


}

void visit(int r, int c, int map[][MAX_N], int & area) {
	map[r][c] = 2;
	area++;
	Pt& pt = queue[tail++];
	pt.r = r, pt.c = c;
}

int bfs(int r, int c, int map[][MAX_N]) {
	int area = 0;
	
	visit(r, c, map, area);
	while (head < tail) {
		pop(r, c);
		f(i, 0, 4) {
			int rr = r + dir[i][0];
			int cc = c + dir[i][1];

			if (boundaryChk(rr, cc) == false) continue;
			if (map[rr][cc]) continue;
			
			visit(rr, cc, map, area);
		}
	}

	return area;
}


void init() {
	nAns = 0;
	head = tail = 0;
	sp_idx = 0;
}

int main() {

	freopen("input_floodfill.txt", "r", stdin);

	scanf("%d %d %d", &nRows, &nCols, &K);
	init();

	int map[MAX_N][MAX_N] = { 0, };
	int idx[MAX_N];


	maxRow = nRows - 1;
	maxCol = nCols - 1;

	f(i, 0, K) {
		int x[2], y[2];
		
		scanf("%d %d %d %d", x, y, x + 1, y + 1);
		fill(x, y, map);
	}

	f(x, 0, nRows) {
		f(c, 0, nCols) {
			int r = maxRow - x;
			if (map[r][c] == 0) {
				int area = bfs(r, c, map);

				if (area > 0) {
					input[nAns++] = area;
				}
			}
		}
	}

	printf("%d\n", nAns);

	if (nAns > 0) {
		
#if 0
		quickSort(0, nAns - 1);
		f(i, 0, nAns) {
			printf("%d ", input[i]);
		}
#else
		f(i, 0, nAns) idx[i] = i;
		quickArgSort(0, nAns - 1, idx);

		f(i, 0, nAns) {
			printf("%d ", input[idx[i]]);
		}
#endif
		printf("\n");
	}

	
	return 0;
}