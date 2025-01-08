#include <cstdio>
#include <cstring>
#include <ctime>

#include "AVL.hpp"
#include "Splay.hpp"

int main(int argc, char *argv[]) {
    BST *tree;
#ifndef _OJ_
    clock_t start = clock();
    if (argc != 2) {
        printf("Usage: %s [AVL|Splay]\n", argv[0]);
        return 1;
    }

    if (strcmp(argv[1], "AVL") == 0) {
        tree = new AVL();
    } else if (strcmp(argv[1], "Splay") == 0) {
        tree = new Splay();
    } else {
        printf("Error: unknown BST type %s\n", argv[1]);
        return 1;
    }
#else
    tree = new AVL();
#endif

    int n;
    scanf("%d", &n);
    while (n--) {
        char op;
        int key;
        scanf(" %c %d", &op, &key);
        switch (op) {
            case 'A':
                tree->insert(key);
                break;
            case 'B':
                tree->remove(key);
                break;
            case 'C':
                printf("%d\n", tree->searchMax(key));
                break;
        }
    }

#ifndef _OJ_
    clock_t end = clock();
    printf("Time: %.3fms\n", (double)(end - start) / CLOCKS_PER_SEC * 1000);
#endif

    return 0;
}
