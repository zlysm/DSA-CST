#include <cstdio>
#include <utility>

using Parent = int[25];
using Node = std::pair<int, int>;  // {next, depth}

const int MAX_N = 1500005;
const int MAX_Q = 1e6 + 5;

int seq[MAX_N], query[MAX_Q];
Node nodes[MAX_N];
Parent parent[MAX_N];

class KMP {
    int mem[MAX_N];
    int _size;

   public:
    KMP() {
        _size = 0;
        init();
    }
    void init() {
        mem[0] = -1;
        int child_idx;
        for (int i = 1; i < MAX_N; ++i) {
            child_idx = i / 2;
            mem[i] = mem[child_idx] + 1;
        }
    }
    void build(int start) {
        for (int i = 1 + start, j = parent[start][0]; i <= _size; ++i) {
            while (j > 0 && seq[j + 1] != seq[i])
                j = parent[j][0];
            if (seq[j + 1] == seq[i]) ++j;
            parent[i][0] = j;
            nodes[i].second = nodes[j].second + 1;
            for (int k = 0; k < mem[nodes[i].second]; ++k)
                parent[i][k + 1] = parent[parent[i][k]][k];
        }
    }
    void getNext(int start) {
        for (int i = 1 + start, j = nodes[start].first; i <= _size; ++i) {
            while (j > 0 && seq[j + 1] != seq[i])
                j = parent[j][0];
            if (seq[j + 1] == seq[i]) ++j;
            while ((j << 1) > i) j = parent[j][0];
            nodes[i].first = j;
        }
    }
    void addSize(int size) { _size += size; }
    void getLCA(int &res, int len) {
        for (int i = 0; i < len - 1; ++i) {
            int j = res, k = nodes[query[i + 2]].first;
            if (nodes[j].second < nodes[k].second) {
                int tmp = j;
                j = k;
                k = tmp;
            }
            int h = nodes[j].second - nodes[k].second;

            for (int m = 0; m <= mem[nodes[j].second]; ++m)
                if (h & (1 << m)) j = parent[j][m];
            if (j == k) {
                res = j;
                continue;
            }
            for (int m = mem[nodes[k].second]; m >= 0; --m)
                if (parent[j][m] != parent[k][m]) {
                    j = parent[j][m];
                    k = parent[k][m];
                }
            res = parent[j][0];
        }
    }
};

int main() {
    int n, m;
    scanf("%d %d", &n, &m);
    for (int i = 1; i <= n; ++i)
        scanf("%d", &seq[i]);

    nodes[1] = {0, 1};
    KMP kmp;
    kmp.addSize(n);
    kmp.build(1);
    kmp.getNext(1);

    while (m--) {
        int op, q;
        scanf("%d %d", &op, &q);
        if (1 == op) {
            kmp.addSize(q);
            int nn = n;
            while (q--) scanf("%d", &seq[++n]);
            kmp.build(nn);
            kmp.getNext(nn);
        } else {
            for (int i = 1; i <= q; ++i)
                scanf("%d", &query[i]);
            int res = nodes[query[1]].first;
            kmp.getLCA(res, q);
            printf("%d\n", res);
        }
    }

    return 0;
}
