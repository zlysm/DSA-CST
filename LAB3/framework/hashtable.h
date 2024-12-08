#include <cstring>
#include <cstdio>
#include <cmath>
const int TABLE_SIZE = 499883; // = 124970 * 4 + 3
struct hash_entry{
    char* my_string;
    int my_data; 
    hash_entry(){
        my_string = NULL;my_data = 0;
    }
    hash_entry(char* str, int data):my_data(data){
        my_string = new char[strlen(str)+1];
        strcpy(my_string,str);
    }
};
struct hashing_strategy{
    virtual int operator()(char* str, int N)=0;
};
struct naive_hashing: public hashing_strategy{
    int operator()(char* str, int N) override;
};

// 用到字符串中所有的信息，但将字符串不均匀地映射到哈希表中
struct uneven_hashing: public hashing_strategy{
    int operator()(char* str, int N) override;
};

// 用到字符串中所有的信息，将字符串均匀地映射到哈希表中
struct even_hashing: public hashing_strategy{
    int operator()(char* str, int N) override;
};

struct collision_strategy{
    virtual void init()=0;// pure virtual function
    virtual int operator()(hash_entry* Table, int table_size, int last_choice)=0;
};
struct linear_probe: public collision_strategy{
    void init() override;
    int operator()(hash_entry* Table, int table_size, int last_choice) override;
};

// 双向平方试探策略
struct double_square: public collision_strategy{
    int direction, i, begin;
    bool inited;
    void init() override;
    int operator()(hash_entry* Table, int table_size, int last_choice) override;
};

// 公共溢出区
struct overflow_area: public collision_strategy{
    int overflow_count;
    void init() override;
    int operator()(hash_entry* Table, int table_size, int last_choice) override;
};

struct hashtable{
    hash_entry* Table;
    int table_size;
    bool modify_table_size;
    hashing_strategy* my_hashing; // 如果改为hashing_strategy my_hashing, 即不是用指针作为hashtable的成员, 而是让hashing_strategy结构体直接作为hashtable的成员. 会发生什么bug?
    collision_strategy* my_collision;
    hashtable(int size, hashing_strategy* hashing, collision_strategy* collision)
        :table_size(size),my_hashing(hashing),my_collision(collision)
    {
        Table = new hash_entry[table_size];
        modify_table_size = false;
    }
    bool insert(hash_entry entry){ 
        if (!modify_table_size && dynamic_cast<overflow_area*>(my_collision)){
            table_size = int(0.8 * table_size);
            modify_table_size = true;
        }
        int last_choice = (*my_hashing)(entry.my_string,table_size);
        my_collision->init();
        while(Table[last_choice].my_string!=NULL){ // loop infinitely? return false when no more space?
            last_choice = (*my_collision)(Table, table_size, last_choice);
        }
        Table[last_choice] = entry;
        return true;
    }
    int query(char* query_string){
        if (!modify_table_size && dynamic_cast<overflow_area*>(my_collision)){
            table_size = int(0.8 * table_size);
            modify_table_size = true;
        }
        int last_choice = (*my_hashing)(query_string,table_size);
        my_collision->init();
        while(Table[last_choice].my_string!=NULL && 
            strcmp(Table[last_choice].my_string, query_string)!=0){ // 未处理的情况: 哈希表已满?
            last_choice = (*my_collision)(Table, table_size, last_choice);
        }
        if(Table[last_choice].my_string == NULL){
            return -1;
        }
        else {
            return Table[last_choice].my_data;
        }
    }
};
