/*
Created by Jeonghan Kim

Square-root decomposition for O(sqrt(N))
Another main program needed. Only APIs are implemented
*/

#define rt register
#define rint rt int
#define f(i,a,b) for(rint i=a;i<b;++i)
#define fe(i,a,b) for(rint i=a;i<=b;++i)

// max number is 1000000
const int maxPart = 1001;
const int maxLen = 1000;
const int maxN = maxPart * maxLen;

int num[maxPart], cnt[maxPart][maxLen];

void init() {
    f(i, 0, maxPart) {
        if (num[i] == 0) continue;
        num[i] = 0;
        f(j,0, maxLen) cnt[i][j] = 0;
    }
}

//insert by ascending order
void insert(int v) {
    rint addr = v/ maxLen;
    num[addr]++;
    cnt[addr][v % maxLen]++;
}

//get indices corresponding to index'th order
bool search(int index, int & gid, int & id) {
    index++;
    f(i,0,maxPart) {
        if (index >  num[i]) index -= num[i];
        else {
            gid = i;
            f(j,0, maxLen) {
                if (index > cnt[i][j]) index -= cnt[i][j];
                else {
                    id = j;
                    return true;
                }
            }
        }
    }

    return false;
}

// remove and get corresponding to index'th order
int remove(int index) {
    rint gid, id;
    if (search(index, gid, id)) {
        cnt[gid][id]--; 
        num[gid]--;
        return (gid* maxLen) + id;
    }

    return -1;
}

// searth the entity and get the entity corresponding to index'th order
int searchByIndex(int index) {
    rint gid, id;
    if (search(index, gid, id)) {
        return (gid * maxLen) + id;
    }
    
    return -1;
}