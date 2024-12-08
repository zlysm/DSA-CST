#include <cstdio>
#include <cstdlib>

#define LC(x) ((x) << 1)
#define RC(x) ((x) << 1 | 1)

using Rank = unsigned int;
using ll = long long;

const int MAX_M = 200005;
Rank Interval[MAX_M << 1], Low[MAX_M], High[MAX_M];
bool Flip[MAX_M];

struct SegNode {
    ll count;
    Rank l, r;  // R(v) = [l, r)
    SegNode() : count(0), l(0), r(0) {}
};

class SegmentTree {
    Rank n;
    SegNode* nodes;
    int* lazy;

    void buildTree(Rank root, Rank left_idx, Rank right_idx);
    void updateChild(Rank root);

   public:
    SegmentTree(Rank size) {
        n = size;
        nodes = new SegNode[n << 2]();
        lazy = new int[n << 2]();
        buildTree(1, 0, n);
    }

    void flip(Rank root, Rank lo, Rank hi);
    ll query(Rank root, Rank lo, Rank hi);
};

int main() {
    Rank n, tmp[MAX_M << 1];
    int m;
    scanf("%u %d", &n, &m);

    for (int i = 0; i < m; ++i) {
        char op;
        Rank lo, hi;
        scanf(" %c %u %u", &op, &lo, &hi);

        Low[i] = lo;
        High[i] = hi + 1;
        Flip[i] = 'H' == op;
        tmp[i << 1] = lo;
        tmp[i << 1 | 1] = hi + 1;
    }
    tmp[m << 1] = 1;
    tmp[m << 1 | 1] = n + 1;

    // sort and unique
    auto cmp = [](const void* a, const void* b) -> int {
        return *(Rank*)a - *(Rank*)b;
    };
    std::qsort(tmp, (m << 1) + 2, sizeof(Rank), cmp);
    Rank points_num = 0;
    for (int i = 0; i < (m << 1) + 2; ++i)
        if (!i || tmp[i] != tmp[i - 1])
            Interval[points_num++] = tmp[i];

    SegmentTree st(points_num - 1);
    for (int i = 0; i < m; ++i)
        if (Flip[i])
            st.flip(1, Low[i], High[i]);
        else
            printf("%lld\n", st.query(1, Low[i], High[i]));

    return 0;
}

void SegmentTree::buildTree(Rank root, Rank left_idx, Rank right_idx) {
    nodes[root].l = Interval[left_idx];
    nodes[root].r = Interval[right_idx];
    if (right_idx - left_idx == 1) return;

    Rank mid_idx = left_idx + ((right_idx - left_idx) >> 1);
    buildTree(LC(root), left_idx, mid_idx);
    buildTree(RC(root), mid_idx, right_idx);
}

void SegmentTree::updateChild(Rank root) {
    if (!lazy[root]) return;
    nodes[LC(root)].count += (nodes[LC(root)].r - nodes[LC(root)].l) * lazy[root];
    nodes[RC(root)].count += (nodes[RC(root)].r - nodes[RC(root)].l) * lazy[root];
    lazy[LC(root)] += lazy[root];
    lazy[RC(root)] += lazy[root];
    lazy[root] = 0;
}

void SegmentTree::flip(Rank root, Rank lo, Rank hi) {
    if (hi <= nodes[root].l || lo >= nodes[root].r) return;
    if (lo <= nodes[root].l && hi >= nodes[root].r) {
        nodes[root].count += nodes[root].r - nodes[root].l;
        ++lazy[root];
        return;
    }

    updateChild(root);
    flip(LC(root), lo, hi);
    flip(RC(root), lo, hi);
    nodes[root].count = nodes[LC(root)].count + nodes[RC(root)].count;
}

ll SegmentTree::query(Rank root, Rank lo, Rank hi) {
    if (hi <= nodes[root].l || lo >= nodes[root].r) return 0;
    if (lo <= nodes[root].l && hi >= nodes[root].r) return nodes[root].count;

    updateChild(root);
    return query(LC(root), lo, hi) + query(RC(root), lo, hi);
}
