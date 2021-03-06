/*
Created by Jeonghan Kim

Pattern matching by Hash used in Rabin-Karp matching
*/

const int maxN = 1000;
const int maxM = 20;
const int SIZE = 1 << 16;
int N, M, A[maxN+1][maxN+1], B[maxM+1][maxM+1], bcnt;

#define rt register
#define rint rt int
#define f(i,a,b) for(rint i=a;i<b;++i)
#define fe(i,a,b) for(rint i=a;i<=b;++i)

struct Node {
    int r, c;
    Node* n;
    void alloc(int _r, int _c, Node* hd) {
        r = _r, c = _c, n = hd->n; hd->n = this;
    }
}buf[(maxN+1) * (maxN + 1)], hash[SIZE];

int Key(int r, int c) {
    int key = 0;
    f(i,0,4) key = (key<< 4) + (A[r + i][c + 3] & 15);

    return key;
}

int checkmod;
void init(int n, int ap[][1000], int m) {
    N = n, M = m, bcnt = 0;
    checkmod = (1 << m) - 1;

    f(i,0, SIZE) hash[i].n = 0;
    f(i, 0, N){
        rint k = 0;
        f(j,0,N) A[i][j] = k = (k << 1) + ap[i][j];
    }

    fe(i,0,N-M) fe(j,0,N-M) {
        int key = Key(i, j);
        buf[bcnt++].alloc(i, j, &hash[key]);
    }
}

#if 1
void rotate(int b[][20]) {
    int t[20][20];
    f(i,0,M) f(j,0,M) t[i][j] = b[j][M-1-i];
    f(i, 0, M) f(j,0,M) b[i][j] = t[i][j];
}
#else
void rotate(int b[][20]) {
    int maxLayer = M >> 1;
    int M1 = M - 1;

    f(layer, 0, maxLayer) {
        int first = layer, last = M1 - layer;
        f(i, first, last) {
            int offset = i - first;
            int top = b[first][i];
            b[first][i] = b[last - offset][first];
            b[last - offset][first] = b[last][last - offset];
            b[last][last - offset] = b[i][last];
            b[i][last] = top;
        }
    }
}
#endif

int check(int r, int c) {
    f(i,0,M) {
        if ((A[r + i][c + M - 1] & checkmod) != B[i][M - 1]) return 0;
    }

    return 1;
}

int query(int bp[][20], int userAns[]) {
    int r, c, key, b[20][20];

    f(i,0, M) f(j,0,M) b[i][j] = bp[i][j];

    f(t, 0, 4) {
        f(i, 0, M) {
            rint k = 0;
            f(j,0,M) {
                if (b[i][j] == 9) r = i, c = j, b[i][j] = 1;
                B[i][j] = k = (k << 1) + b[i][j];
            }
        }
        key = 0;
        f(i, 0, 4) key = (key << 4) + B[i][3];
        for (rt Node* p = hash[key].n; p; p = p->n) {
            if (check(p->r, p->c)) {
                userAns[0] = p->r, userAns[1] = p->c;
                userAns[2] = p->r + r, userAns[3] = p->c + c;

                return t;
            }
        }
        b[r][c] = 9;
        rotate(b);
    }

    return 0;
}