#include <cstdio>

#define isdigit(c) ((c) >= '0' && (c) <= '9')
#define MAX_SIZE 1000010

struct Node {
    int parent, fChild, lSib, rSib;
    int height, size;

    Node() : parent(0), fChild(0), lSib(0), rSib(0), height(0), size(0) {}
};

struct Pair {
    int size, height;

    Pair(int s, int h) : size(s), height(h) {}
};

typedef enum {
    SUB = 0,
    ADD
} Op;

Node nodes[MAX_SIZE];

int read() {
    char ch = getchar();
    int ans = 0;
    for (; !isdigit(ch); ch = getchar());
    for (; isdigit(ch); ch = getchar())
        ans = ans * 10 + ch - '0';
    return ans;
}

void init(int parent, int len, const int *child);

Pair updateAll(int id);

void updateLevel(int *levelHeight, int id);

void moveTree(int tar_len, int *tar, int des_len, int *des, int *levelHeight, int rank);

int locate(int len, const int *id);

int main() {
    int n = read();
    int m = read();
    int *levelHeight = new int[n + 1];
    nodes[0].height = levelHeight[0] = -1;
    for (int i = 1; i <= n; ++i) {
        int len = read();
        int *child = new int[len];
        for (int j = 0; j < len; ++j)
            child[j] = read();
        init(i, len, child);
        nodes[i].height = -1;
        levelHeight[i] = -1;
        delete[] child;
    }
    updateAll(1);
    for (int i = 1; i <= n; ++i) updateLevel(levelHeight, i);

    while (m--) {
        int op = read();
        switch (op) {
            case 0: {
                int tar_len = read();
                int *tar = new int[tar_len];
                for (int i = 0; i < tar_len; ++i) tar[i] = read();

                int des_len = read();
                int *des = new int[des_len];
                for (int i = 0; i < des_len; ++i) des[i] = read();

                int rank = read();
                moveTree(tar_len, tar, des_len, des, levelHeight, rank);
                delete[] tar;
                delete[] des;
                break;
            }
            case 1: {
                int len = read();
                int *id = new int[len];
                for (int i = 0; i < len; ++i) id[i] = read();
                printf("%d\n", nodes[locate(len, id)].height);
                delete[] id;
                break;
            }
            case 2: {
                int len = read();
                int *id = new int[len];
                for (int i = 0; i < len; ++i) id[i] = read();
                printf("%d\n", nodes[locate(len, id)].size);
                delete[] id;
                break;
            }
            default:
                break;
        }
    }
    delete[] levelHeight;
    return 0;
}

void init(int parent, int len, const int *child) {
    if (len)  // has child
        nodes[parent].fChild = child[0];
    for (int i = 0; i < len; ++i) {  // init siblings
        nodes[child[i]].parent = parent;
        if (i)
            nodes[child[i]].lSib = child[i - 1];
        else
            nodes[child[i]].lSib = 0;
        if (i < len - 1)
            nodes[child[i]].rSib = child[i + 1];
        else
            nodes[child[i]].rSib = 0;
    }
}

Pair updateAll(int id) {
    if (nodes[id].size != 0 || nodes[id].height != -1)  // already updated
        return {nodes[id].size, nodes[id].height};
    if (!nodes[id].fChild) {  // leaf
        nodes[id].size = 1;
        nodes[id].height = 0;
        return {1, 0};
    }
    int child = nodes[id].fChild;
    while (child) {  // traverse all children
        Pair pair = updateAll(child);
        nodes[id].size += pair.size;
        nodes[id].height = (nodes[id].height > pair.height + 1) ? nodes[id].height : pair.height + 1;
        child = nodes[child].rSib;
    }
    nodes[id].size += 1;
    return {nodes[id].size, nodes[id].height};
}

void updateLevel(int *levelHeight, int id) {
    if (levelHeight[id] != -1) return;
    levelHeight[id] = nodes[id].height > levelHeight[nodes[id].rSib] ? nodes[id].height : levelHeight[nodes[id].rSib];  // compare with right sibling
}

int locate(int len, const int *id) {
    int ans = 1, idx = 0;  // default is root
    while (idx < len && nodes[ans].fChild) {
        int now = id[idx++];
        int child = nodes[ans].fChild;
        while (now-- && child)
            child = nodes[child].rSib;
        if (!child) break;
        ans = child;
    }
    return ans;
}

void heightAbove(int tar, int *levelHeight, Op op) {
    if (ADD == op)
        levelHeight[tar] = levelHeight[nodes[tar].rSib] > nodes[tar].height ? levelHeight[nodes[tar].rSib] : nodes[tar].height;
    int left = nodes[tar].lSib, parent = nodes[tar].parent;
    while (parent) {
        while (left) {
            levelHeight[left] = levelHeight[nodes[left].rSib] > nodes[left].height ? levelHeight[nodes[left].rSib] : nodes[left].height;
            left = nodes[left].lSib;
        }
        nodes[parent].height = levelHeight[nodes[parent].fChild] + 1;
        levelHeight[parent] = nodes[parent].height > levelHeight[nodes[parent].rSib] ? nodes[parent].height : levelHeight[nodes[parent].rSib];
        left = nodes[parent].lSib;
        parent = nodes[parent].parent;
    }
}

void sizeAbove(int tar, Op op) {
    int parent = nodes[tar].parent;
    while (parent) {
        if (op == ADD)
            nodes[parent].size += nodes[tar].size;
        else
            nodes[parent].size -= nodes[tar].size;
        parent = nodes[parent].parent;
    }
}

void moveTree(int tar_len, int *tar, int des_len, int *des, int *levelHeight, int rank) {
    int tar_id = locate(tar_len, tar);
    if (tar_id == nodes[nodes[tar_id].parent].fChild)
        nodes[nodes[tar_id].parent].fChild = nodes[tar_id].rSib;
    if (nodes[tar_id].lSib)
        nodes[nodes[tar_id].lSib].rSib = nodes[tar_id].rSib;
    if (nodes[tar_id].rSib)
        nodes[nodes[tar_id].rSib].lSib = nodes[tar_id].lSib;

    sizeAbove(tar_id, SUB);
    heightAbove(tar_id, levelHeight, SUB);

    int des_id = locate(des_len, des);
    int child = nodes[des_id].fChild;
    int left = 0;
    for (int i = 0; i < rank; ++i) {  // find the rank-th child
        left = child;
        child = nodes[child].rSib;
    }
    if (left) nodes[left].rSib = tar_id;  // update siblings
    if (child) nodes[child].lSib = tar_id;
    nodes[tar_id].parent = des_id;
    nodes[tar_id].lSib = left;
    nodes[tar_id].rSib = child;
    if (!rank) nodes[des_id].fChild = tar_id;

    sizeAbove(tar_id, ADD);  // size and height will change after moving
    heightAbove(tar_id, levelHeight, ADD);
}
