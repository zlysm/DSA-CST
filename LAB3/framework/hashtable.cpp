#include "hashtable.h"
int naive_hashing::operator()(char* str, int N){
    if(str == NULL) return 0;
    else return (str[0]+N)%N;
}
int uneven_hashing::operator()(char* str, int N){
    if(str == NULL) return 0;
    int len = strlen(str);
    int sum = 0;
    for(int i = 0; i < len; ++i)
        sum += str[i];
    return sum % N;
}
int even_hashing::operator()(char* str, int N){
    if(str == NULL) return 0;
    int len = strlen(str);
    int p = 31;
    int sum = 0;
    for(int i = 0; i < len; ++i)
        sum = (sum * p + str[i]) % N;
    return sum;
}
int linear_probe::operator()(hash_entry* Table, int table_size, int last_choice){
    return (last_choice + 1) % table_size;
}
void linear_probe::init(){
    return;// do nothing
}

void double_square::init(){
    direction = 1;
    i = 1;
    inited = false;
}
int double_square::operator()(hash_entry* Table, int table_size, int last_choice) {
    if (!inited) {
        begin = last_choice;
        inited = true;
    }
    int factor = (int)ceil((double)i / 2);
    int next_choice = (begin + direction * factor * factor) % table_size;
    direction = -direction;
    ++i;
    return next_choice < 0 ? next_choice + table_size : next_choice;
}
void overflow_area::init(){
    overflow_count = 0;
}
int overflow_area::operator()(hash_entry* Table, int table_size, int last_choice){
    return int(0.8 * table_size) + overflow_count++;
}
