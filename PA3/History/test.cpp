#include <cstdio>

const int MAX_ID = 63000000;
int hash[MAX_ID];

int main() {
    int n, x, res = 0;
    scanf("%d", &n);
    for (int i = 1; i <= n; ++i) {
        scanf("%d", &x);
        int id = x ^ res;
        res = hash[id] ? hash[id] : i;
        hash[id] = i;
        printf("%d\n", res);
    }
    return 0;
}
