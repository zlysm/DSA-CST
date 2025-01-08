#include <fstream>
#include <iostream>
#include <random>
#include <vector>

const int MAX = 8500000;

int main() {
    std::random_device rd;
    std::mt19937 gen(rd());

    // 1.in
    int n1 = 1e6;
    int insert1 = 5e5, remove1 = 3e5, search1 = 2e5;
    std::ofstream fout1("data/1.in");
    fout1 << n1 << std::endl;
    std::vector<int> v1;
    for (int i = 0; i < 3e5; ++i) {
        int x = gen() % MAX;
        v1.push_back(x);
        fout1 << "A " << x << std::endl;
    }
    insert1 -= 3e5;
    for (int i = 0; i < 2e5; ++i) {
        int idx = gen() % v1.size();
        fout1 << "B " << v1[idx] << std::endl;
        v1.erase(v1.begin() + idx);
    }
    remove1 -= 2e5;
    for (int i = 0; i < 1e5; ++i) {
        int x = gen() % MAX;
        fout1 << "C " << x << std::endl;
    }
    search1 -= 1e5;
    for (int i = 0; i < insert1; ++i) {
        int x = gen() % MAX;
        fout1 << "A " << x << std::endl;
        v1.push_back(x);
    }
    for (int i = 0; i < remove1; ++i) {
        int idx = gen() % v1.size();
        fout1 << "B " << v1[idx] << std::endl;
        v1.erase(v1.begin() + idx);
    }
    for (int i = 0; i < search1; ++i) {
        int x = gen() % MAX;
        fout1 << "C " << x << std::endl;
    }
    fout1.close();

    // 2.in
    int n2 = 1e6;
    int insert2 = 5e5, remove2 = 3e5, search2 = 2e5;
    std::ofstream fout2("data/2.in");
    fout2 << n2 << std::endl;
    std::vector<int> v2;
    for (int i = 0; i < 3e5; ++i) {
        v2.push_back(i);
        fout2 << "A " << i << std::endl;
    }
    insert2 -= 3e5;
    for (int i = 0; i < 2e5; ++i) {
        int idx = gen() % v2.size();
        fout2 << "B " << v2[idx] << std::endl;
        v2.erase(v2.begin() + idx);
    }
    remove2 -= 2e5;
    for (int i = 0; i < 1e5; ++i) {
        int x = gen() % MAX;
        fout2 << "C " << x << std::endl;
    }
    search2 -= 1e5;
    for (int i = 0; i < insert2; ++i) {
        int x = 3e5 + i;
        v2.push_back(x);
        fout2 << "A " << x << std::endl;
    }
    for (int i = 0; i < remove2; ++i) {
        int idx = gen() % v2.size();
        fout2 << "B " << v2[idx] << std::endl;
        v2.erase(v2.begin() + idx);
    }
    for (int i = 0; i < search2; ++i) {
        int x = gen() % MAX;
        fout2 << "C " << x << std::endl;
    }
    fout2.close();

    // 3.in
    int n3 = 1e6;
    int insert3 = 5e5, remove3 = 2e5, search3 = 3e5;
    std::ofstream fout3("data/3.in");
    fout3 << n3 << std::endl;
    std::vector<int> v3;
    for (int i = 0; i < 3e5; ++i) {
        v3.push_back(i);
        fout3 << "A " << i << std::endl;
    }
    insert3 -= 3e5;
    for (int i = 0; i < 1e5; ++i) {
        int idx = gen() % v3.size();
        fout3 << "B " << v3[idx] << std::endl;
        v3.erase(v3.begin() + idx);
    }
    remove3 -= 1e5;
    for (int i = 0; i < 2e5; ++i) {
        int x = gen() % 10000;
        fout3 << "C " << x << std::endl;
    }
    search3 -= 2e5;
    for (int i = 0; i < insert3; ++i) {
        int x = 3e5 + i;
        v3.push_back(x);
        fout3 << "A " << x << std::endl;
    }
    for (int i = 0; i < remove3; ++i) {
        int idx = gen() % v3.size();
        fout3 << "B " << v3[idx] << std::endl;
        v3.erase(v3.begin() + idx);
    }
    for (int i = 0; i < search3; ++i) {
        int x = gen() % 10000;
        fout3 << "C " << x << std::endl;
    }
    fout3.close();

    return 0;
}
