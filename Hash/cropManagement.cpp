/*
Created by Jeonghan Kim

Problem for Hash and Set
*/

#define rt register
#define rint rt int
#define f(i,a,b) for(rint i=a;i<b;++i)
#define fe(i,a,b) for(rint i=a;i<=b;++i)

typedef long long LL;
const int maxHash = (1 << 15);
const int MOD = maxHash - 1;
const int maxN = 150+1;

void mstrcpy(char* dst, char* src) {
	rint i = 0;
	for (; src[i]; ++i) dst[i] = src[i];
	dst[i] = 0;
}

struct Cell {
	int root;
	int mr, mc;	//parent
	int id, area;
}cell[maxN][maxN];

struct Crop {
	LL key;
	char name[12];
	int cell_cnt;
	int area;
} cropdata[maxHash];
int cropid;

struct Hash {
	int id;
	Hash* n;

	void alloc(int i, LL _k, Hash* hd) {
		id = i;
		cropdata[i].key = _k, n = hd->n, hd->n = this;
	}
} hbuf[maxHash], htbl[maxHash];

struct HashW {
	int cnt;
	Hash* buf;
	Hash* tbl;

	void setbuf(Hash* b, Hash* t) {
		buf = b, tbl = t;
	}

	int find(int addr, LL key) {
		rt Hash* p = tbl[addr].n;
		for (; p; p = p->n) {
			if (cropdata[p->id].key == key) return p->id;
		}
		return -1;
	}

	int getid(char str[]) {
		LL key = 0L;
		for (rint i = 0; str[i]; ++i) {
			key <<= 5;
			key |= (str[i] - 96);
		}

		int addr = key % maxHash;
		int id = find(addr, key);
		
		if (id < 0) {
			id = ++cropid;
			buf[cnt++].alloc(id, key, &tbl[addr]);
			mstrcpy(cropdata[id].name, str);
		}		
		return id;
	}
};
HashW hw;

void init(int Row, int Col) {
	cropid = 0;
	hw.cnt = 0;
	hw.setbuf(hbuf, htbl);

	f(i, 0, maxHash) {
		hw.tbl[i].n = 0;
		cropdata[i].key = 0;
		cropdata[i].name[0] = 0;
		cropdata[i].area = cropdata[i].cell_cnt = 0;
	}

	fe(i, 1, Row) {
		fe(j, 1, Col) {
			cell[i][j].mr = i, cell[i][j].mc = j;
			cell[i][j].id = 0;		//À¯ÈÞÁö
			cell[i][j].root = cell[i][j].area = 1;
		}
	}
}

// Finding uppermost parent for each cell
void findroot(int& row, int& col) {
	int r, c;

	while (!cell[row][col].root) {
		r = cell[row][col].mr, c = cell[row][col].mc;
		row = r, col = c;
	}
}

void setFarm(int row, int col, char crop[]) {
	findroot(row, col);
	
	//removing old
	int id = cell[row][col].id;
	cropdata[id].cell_cnt--;
	cropdata[id].area -= cell[row][col].area;

	id = hw.getid(crop);
	cell[row][col].id = id;
	cropdata[id].cell_cnt++;
	cropdata[id].area += cell[row][col].area;
}

void getCrop(int row, int col, char crop[]) {
	findroot(row, col);
	mstrcpy(crop, cropdata[cell[row][col].id].name);
}

void mswap(int& a, int& b) {
	int t = a;
	a = b;
	b = t;
}

void mergeCell(int row1, int col1, int row2, int col2, int sw) {
	if (sw == 2) mswap(col1, col2), mswap(row1, row2);

	findroot(row1, col1);
	findroot(row2, col2);

	Cell* a = &cell[row1][col1], * b = &cell[row2][col2];

	a->area += b->area;
	b->root = 0, b->mr = row1, b->mc = col1;

	cropdata[b->id].cell_cnt--;
	cropdata[b->id].area -= b->area;
	cropdata[a->id].area += b->area;
}

int cntCell(char crop[]) {
	return cropdata[hw.getid(crop)].cell_cnt;
}

int cntArea(char crop[]) {
	return cropdata[hw.getid(crop)].area;
}