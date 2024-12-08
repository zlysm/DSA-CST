#include <cstdio>
#include <iostream>

using namespace std;

// 定义KD树结构
struct KDTree {
    // 定义KD树节点结构
    struct KDTreeNode {
        int median_index, left, right, bounds[5][2];
        KDTreeNode() = default;
        KDTreeNode(int m, int l, int r) : median_index(m), left(l), right(r) {}
    };

    KDTreeNode* kd_tree;  // 指向KD树节点的指针

    // 二维数组，存储所有点的坐标
    struct Point {
        int coords[5];
    };

    Point* points;  // 指向所有点的指针

    int dimensions;  // 点的维度

    // KDTree构造函数
    KDTree(int num_points, int dimensions) : dimensions(dimensions) {
        kd_tree = new KDTreeNode[num_points << 1];
        points = new Point[num_points];
    };

    // 构造KD树的递归函数
    int construct_kd_tree(int left, int right, int axis, int& current_node_count);
    // 搜索KD树的递归函数
    void search_kd_tree(int* query_point, int root, int axis, long long& min_distance);
    // 更新中位数的递归函数
    void update_median(int mid, int axis, int left, int right);
};

// 更新中位数函数
void KDTree::update_median(int mid, int axis, int left, int right) {
    if (left + 1 >= right) return;  // 递归终止条件
    int pivot = left - 1;
    for (int i = left; i <= right - 2; i++) {
        if (points[i].coords[axis] <= points[right - 1].coords[axis]) {
            Point temp = points[++pivot];
            points[pivot] = points[i];
            points[i] = temp;
        }
    }
    // 交换中位数和最后一个元素
    Point temp = points[++pivot];
    points[pivot] = points[right - 1];
    points[right - 1] = temp;
    if (pivot < mid) update_median(mid, axis, pivot + 1, right);
    if (pivot > mid) update_median(mid, axis, left, pivot);
}

// 构造KD树函数
int KDTree::construct_kd_tree(int left, int right, int axis, int& current_node_count) {
    if (left >= right) return 0;  // 递归终止条件
    if (left + 1 == right) {
        kd_tree[++current_node_count] = KDTreeNode(left, 0, 0);  // 创建叶子节点
        for (int i = 0; i < dimensions; i++)
            kd_tree[current_node_count].bounds[i][0] = kd_tree[current_node_count].bounds[i][1] = points[left].coords[i];
        return current_node_count;
    }
    int mid = (left + right) >> 1;                          // 计算中间点
    update_median(mid, axis, left, right);                  // 更新中位数
    kd_tree[++current_node_count] = KDTreeNode(mid, 0, 0);  // 创建节点
    int node_index = current_node_count;
    axis = (axis + 1) % dimensions;                                                           // 计算下一个轴
    kd_tree[node_index].right = construct_kd_tree(mid + 1, right, axis, current_node_count);  // 构造右子树
    kd_tree[node_index].left = construct_kd_tree(left, mid, axis, current_node_count);        // 构造左子树
    // 更新节点的边界
    for (int i = 0; i < dimensions; i++) {
        kd_tree[node_index].bounds[i][0] = kd_tree[node_index].bounds[i][1] = points[mid].coords[i];
        if (kd_tree[node_index].right) {
            kd_tree[node_index].bounds[i][0] = min(kd_tree[kd_tree[node_index].right].bounds[i][0], kd_tree[node_index].bounds[i][0]);
            kd_tree[node_index].bounds[i][1] = max(kd_tree[kd_tree[node_index].right].bounds[i][1], kd_tree[node_index].bounds[i][1]);
        }
        if (kd_tree[node_index].left) {
            kd_tree[node_index].bounds[i][0] = min(kd_tree[kd_tree[node_index].left].bounds[i][0], kd_tree[node_index].bounds[i][0]);
            kd_tree[node_index].bounds[i][1] = max(kd_tree[kd_tree[node_index].left].bounds[i][1], kd_tree[node_index].bounds[i][1]);
        }
    }
    return node_index;
}

// 搜索KD树函数
void KDTree::search_kd_tree(int* query_point, int root, int axis, long long& min_distance) {
    if (!root) return;                        // 如果节点为空，返回
    int next_axis = (axis + 1) % dimensions;  // 计算下一个轴
    long long distance = 0;
    // 计算当前节点到查询点的距离
    for (int i = 0; i < dimensions; i++) {
        int diff = points[kd_tree[root].median_index].coords[i] - query_point[i];
        distance += static_cast<long long>(diff) * diff;
    }
    min_distance = min(distance, min_distance);  // 更新最小距离
    // 根据查询点和当前节点在当前轴上的位置关系，选择左右子树进行递归搜索
    if (query_point[axis] < points[kd_tree[root].median_index].coords[axis]) {
        search_kd_tree(query_point, kd_tree[root].left, next_axis, min_distance);
        int diff = kd_tree[kd_tree[root].right].bounds[axis][0] - query_point[axis];
        if (static_cast<long long>(diff) * diff < min_distance)
            search_kd_tree(query_point, kd_tree[root].right, next_axis, min_distance);
    } else {
        search_kd_tree(query_point, kd_tree[root].right, next_axis, min_distance);
        int diff = kd_tree[kd_tree[root].left].bounds[axis][1] - query_point[axis];
        if (static_cast<long long>(diff) * diff < min_distance)
            search_kd_tree(query_point, kd_tree[root].left, next_axis, min_distance);
    }
}

int main() {
    int num_points, dimensions, num_queries, query_point[5];
    // 读取维度和点的数量
    scanf("%d %d", &dimensions, &num_points);

    KDTree kd_tree(num_points, dimensions);  // 创建KDTree对象

    // 读取所有点的坐标
    for (int i = 0; i < num_points; i++)
        for (int j = 0; j < dimensions; j++)
            scanf("%d", &kd_tree.points[i].coords[j]);

    int root = 0;
    kd_tree.construct_kd_tree(0, num_points, 0, root);  // 构造KD树

    // 读取查询次数并处理每次查询
    scanf("%d", &num_queries);
    while (num_queries--) {
        for (int j = 0; j < dimensions; j++)
            scanf("%d", &query_point[j]);

        long long min_distance = 20000000000000000LL;             // 初始化最小距离
        kd_tree.search_kd_tree(query_point, 1, 0, min_distance);  // 搜索最近点
        printf("%lld\n", min_distance);                           // 输出最近距离
    }
    return 0;
}
