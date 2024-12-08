#include <cstdio>
#include <cstdlib>

using Rank = unsigned int;
using ll = long long;

const int MAX_M = 200005;
Rank Interval[MAX_M << 1], Low[MAX_M], High[MAX_M];
bool Flip[MAX_M];

class F {
   public:
    static Rank leftChild(Rank i) { return i << 1; }
    static Rank rightChild(Rank i) { return i << 1 | 1; }
};

struct SegNode {
    ll count;
    Rank l, r;  // R(v) = [l, r)
    SegNode() : count(0), l(0), r(0) {}
};

struct LazyTag {
    int value;
    LazyTag() : value(0) {}
};


class SegmentTree {
    Rank n;
    SegNode* nodes;
    LazyTag* lazy;

    void buildTree(Rank root, Rank left_idx, Rank right_idx);
    void updateChild(Rank root);

   public:
    SegmentTree(Rank size) {
        n = size;
        nodes = new SegNode[n << 2]();
        lazy = new LazyTag[n << 2]();
        buildTree(1, 0, n);
    }

    ~SegmentTree() {
        delete[] nodes;
        delete[] lazy;
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
    buildTree(F::leftChild(root), left_idx, mid_idx);
    buildTree(F::rightChild(root), mid_idx, right_idx);
}

void SegmentTree::updateChild(Rank root) {
    if (!lazy[root].value) return;
    nodes[F::leftChild(root)].count += (nodes[F::leftChild(root)].r - nodes[F::leftChild(root)].l) * lazy[root].value;
    nodes[F::rightChild(root)].count += (nodes[F::rightChild(root)].r - nodes[F::rightChild(root)].l) * lazy[root].value;
    lazy[F::leftChild(root)].value += lazy[root].value;
    lazy[F::rightChild(root)].value += lazy[root].value;
    lazy[root].value = 0;
}

void SegmentTree::flip(Rank root, Rank lo, Rank hi) {
    if (hi <= nodes[root].l || lo >= nodes[root].r) return;
    if (lo <= nodes[root].l && hi >= nodes[root].r) {
        nodes[root].count += nodes[root].r - nodes[root].l;
        ++lazy[root].value;
        return;
    }

    updateChild(root);
    flip(F::leftChild(root), lo, hi);
    flip(F::rightChild(root), lo, hi);
    nodes[root].count = nodes[F::leftChild(root)].count + nodes[F::rightChild(root)].count;
}

ll SegmentTree::query(Rank root, Rank lo, Rank hi) {
    if (hi <= nodes[root].l || lo >= nodes[root].r) return 0;
    if (lo <= nodes[root].l && hi >= nodes[root].r) return nodes[root].count;

    updateChild(root);
    return query(F::leftChild(root), lo, hi) + query(F::rightChild(root), lo, hi);
}
