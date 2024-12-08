#include <iostream>

#define LC(x) (x << 1)
#define RC(x) (x << 1 | 1)

using ll = long long;
const int MAX_DIM = 5;
const ll MAX_DIST = 1e18;
int cmp_axis;

struct Point {
    int coord[MAX_DIM];
};

struct KDNode {
    int bound[MAX_DIM][2];  // 0: min, 1: max
    Point mid;
};

class KDTree {
    KDNode *nodes;
    int dim, size;

    ll getDistance(const Point &a, const Point &b);
    bool isLeaf(int root) {
        for (int i = 0; i < dim; ++i)
            if (nodes[root].bound[i][0] != nodes[root].bound[i][1])
                return false;
        return true;
    }

   public:
    Point *points;

    KDTree(int d, int n) : dim(d), size(n) {
        nodes = new KDNode[n << 2];
        points = new Point[n];
    }
    ~KDTree() {
        delete[] nodes;
        delete[] points;
    }
    void build(int root, int lo, int hi, int depth);
    ll query(int root, const Point &p, int depth, ll min_dist);
};

int main() {
    int d, n;
    scanf("%d %d", &d, &n);

    KDTree tree(d, n);
    for (int i = 0; i < n; ++i)
        for (int j = 0; j < d; ++j)
            scanf("%d", &tree.points[i].coord[j]);

    tree.build(1, 0, n, 0);
    int q;
    Point p;
    scanf("%d", &q);
    while (q--) {
        for (int i = 0; i < d; ++i)
            scanf("%d", &p.coord[i]);
        printf("%lld\n", tree.query(1, p, 0, MAX_DIST));
    }

    return 0;
}

void KDTree::build(int root, int lo, int hi, int depth) {
    if (lo + 1 == hi) {  // leaf, [lo, hi)
        for (int i = 0; i < dim; i++)
            nodes[root].bound[i][0] = nodes[root].bound[i][1] = points[lo].coord[i];
        nodes[root].mid = points[lo];
        return;
    }

    cmp_axis = depth % dim;
    auto cmp = [](const void *a, const void *b) -> int {
        return ((Point *)a)->coord[cmp_axis] - ((Point *)b)->coord[cmp_axis];
    };
    std::qsort(points + lo, hi - lo, sizeof(Point), cmp);

    int mid = (lo + hi) >> 1;
    nodes[root].mid = points[mid];
    build(LC(root), lo, mid, depth + 1);
    build(RC(root), mid, hi, depth + 1);

    for (int i = 0; i < dim; i++) {
        nodes[root].bound[i][0] = std::min(nodes[LC(root)].bound[i][0], nodes[RC(root)].bound[i][0]);
        nodes[root].bound[i][1] = std::max(nodes[LC(root)].bound[i][1], nodes[RC(root)].bound[i][1]);
    }
}

ll KDTree::getDistance(const Point &a, const Point &b) {
    ll ret = 0;
    for (int i = 0; i < dim; i++) {
        ll diff = a.coord[i] - b.coord[i];
        ret += diff * diff;
    }
    return ret;
}

ll KDTree::query(int root, const Point &p, int depth, ll min_dist) {
    if (isLeaf(root))
        return std::min(min_dist, getDistance(p, nodes[root].mid));

    int axis = depth % dim;
    ll cur_dist = getDistance(p, nodes[root].mid);
    min_dist = std::min(min_dist, cur_dist);

    bool go_left = p.coord[axis] < nodes[root].mid.coord[axis];
    int first = go_left ? LC(root) : RC(root);
    int second = go_left ? RC(root) : LC(root);

    min_dist = query(first, p, depth + 1, min_dist);
    int diff = go_left ? nodes[second].bound[axis][0] - p.coord[axis]
                       : p.coord[axis] - nodes[second].bound[axis][1];
    if (static_cast<ll>(diff) * diff < min_dist)
        min_dist = query(second, p, depth + 1, min_dist);

    return min_dist;
}
