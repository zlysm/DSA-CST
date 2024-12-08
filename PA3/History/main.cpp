#include <cstdio>

const int MAX_ID = 63000000;
int hash[MAX_ID];

struct Result {
    int val;
    Result() : val(0) {}
};

class F {
    Result res;

   public:
    int getID(int x) {
        return x ^ res.val;
    }
    void print() {
        printf("%d\n", res.val);
    }
    void calcRes(int id, int idx) {
        res.val = hash[id] ? hash[id] : idx;
    }
};

int main() {
    int n, x;
    F func;
    scanf("%d", &n);
    for (int i = 1; i <= n; ++i) {
        scanf("%d", &x);
        int id = func.getID(x);
        func.calcRes(id, i);
        hash[id] = i;
        func.print();
    }
    return 0;
}
