#include <cstdio>
#include <cstdlib>

#include "Queap.h"

const int MAXN = 10000000;
const int MAXS = 60 * 1024 * 1024;
uint numbers[MAXN];
char buf[MAXS];

int n, T;
uint *diag, *trace, *pq;

void read() {
    size_t len = fread(buf, 1, MAXS, stdin);
    buf[len] = '\0';
    int i;
    numbers[i = 0] = 0;
    for (char *p = buf; *p && p - buf < len; ++p)
        if (*p == ' ' || *p == '\n')
            numbers[++i] = 0;
        else
            numbers[i] = numbers[i] * 10 + *p - '0';

    n = (int)numbers[0];
    diag = numbers + 1;
    trace = diag + n;
    T = (int)numbers[n + n + 1];
    pq = trace + n + 1;
}

uint binSearch(const uint *a, int sz, const uint &x);

int main() {
    read();
    Queap queap;
    uint d[n];

    for (int i = 0; i < n; ++i) {
        while (!queap.empty() && queap.size > trace[i])
            queap.dequeue();
        d[i] = queap.getMax();
        queap.enqueue(diag[i]);
    }

    qsort(d, n, sizeof(uint), [](const void *a, const void *b) -> int {
        return *(int *)a - *(int *)b;
    });

    for (int i = 0; i < T; ++i) {
        uint p_rank = binSearch(d, n, pq[2 * i]);
        while (p_rank > 0 && d[p_rank - 1] == pq[2 * i])
            --p_rank;
        uint pDay = p_rank + (d[p_rank] != pq[2 * i]);

        uint q_rank = binSearch(d, n, pq[2 * i + 1]);
        while (q_rank > 0 && d[q_rank - 1] == pq[2 * i + 1])
            --q_rank;
        uint qDay = q_rank - pDay + (d[q_rank] != pq[2 * i + 1]);

        printf("%u %u\n", pDay, qDay);
    }
    return 0;
}

uint binSearch(const uint *a, int sz, const uint &x) {
    uint lo = 0, hi = sz;
    while (lo < hi) {
        uint mi = lo + ((hi - lo) >> 1);
        x < a[mi] ? hi = mi : lo = mi + 1;
    }
    return lo - 1;
}
