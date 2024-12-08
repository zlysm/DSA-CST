#include <cstdio>
#include <cstdlib>
int main(){
    system("g++ rand_input.cpp -o rand_input");
    system("g++ check_input.cpp -o check_input");
    system("g++ solution_1.cpp -o solution_1");
    system("g++ solution_3.cpp -o solution_3");
    while(1){
        system("./rand_input > rand.in");
        if(system("./check_input < rand.in")!=0){
            printf("invalid input!\n");
            break;
        }
        system("./solution_1 < rand.in > 1.out");
        system("./solution_3 < rand.in > 3.out");
        if(system("diff 1.out 3.out")!=0){
            printf("different output!\n");
            break;
        }
    }
    return 0;
}