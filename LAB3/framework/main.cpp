#include <cstdio>
#include <cstring>
#include "hashtable.h"

int main(int argc, char* argv[]){
    int type;
    char buffer[1000];int data;

#ifndef _OJ_
    if (argc != 3) {
        printf("Usage: %s <hashing_strategy> <collision_strategy>\n", argv[0]);
        return 1;
    }

    hashing_strategy *hasing;
    collision_strategy *collision;

    if (strcmp(argv[1], "uneven") == 0) {
        hasing = new uneven_hashing();
    } else if (strcmp(argv[1], "even") == 0) {
        hasing = new even_hashing();
    } else {
        printf("Error: unknown hashing strategy\n");
        return 1;
    }

    if (strcmp(argv[2], "linear") == 0) {
        collision = new linear_probe();
    } else if (strcmp(argv[2], "double") == 0) {
        collision = new double_square();
    } else if (strcmp(argv[2], "overflow") == 0) {
        collision = new overflow_area();
    } else {
        printf("Error: unknown collision strategy\n");
        return 1;
    }

    hashtable table(TABLE_SIZE, hasing, collision);
#else
    hashtable table(10000,new naive_hashing(),new linear_probe());
#endif

    while(true){
        scanf("%d", &type);
        if(type == 0){
            scanf("%s", buffer);scanf("%d",&data);
            table.insert(hash_entry(buffer, data));
        }else if(type == 1){
            scanf("%s",buffer);
            printf("%d\n", table.query(buffer));
        }else break;
    }
    return 0;
}