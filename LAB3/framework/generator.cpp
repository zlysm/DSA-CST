#include <cstdlib>
#include <cstring>
#include <fstream>
#include <iostream>
#include <random>
#include <vector>

const int MAX_SIZE = 500000;  // poj.txt
char buffer[MAX_SIZE + 5][100];

int main(int argc, char* argv[]) {
    if (argc != 4) {
        std::cerr << "Usage: " << argv[0] << " <output_file> <insert_num> <query_num>" << std::endl;
        return 1;
    }

    std::ofstream fout(argv[1]);
    std::ifstream fin("../dataset/poj.txt");
    if (!fin.is_open()) {
        std::cerr << "Error: cannot open file '../dataset/poj.txt'" << std::endl;
        return 1;
    }
    int data_num = 0;
    while (fin.getline(buffer[data_num++], 100));

    int insert_num = std::atoi(argv[2]);
    int query_num = std::atoi(argv[3]);
    int max_num = std::min(2 * std::max(insert_num, query_num), data_num);
    if (insert_num > MAX_SIZE || query_num > MAX_SIZE) {
        std::cerr << "Error: insert_num or query_num is too large" << std::endl;
        return 1;
    }

    std::vector<int> index(max_num);
    for (int i = 0; i < max_num; ++i)
        index[i] = i;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::shuffle(index.begin(), index.end(), gen);

    for (int i = 0; i < insert_num; ++i) {
        char name[100], rank[100];
        sscanf(buffer[index[i]], "%*s %99s %99s", name, rank);
        fout << "0 " << name << " " << rank << std::endl;
    }

    std::shuffle(index.begin(), index.end(), gen);
    for (int i = 0; i < query_num; ++i) {
        char name[100];
        sscanf(buffer[index[i]], "%*s %99s", name);
        fout << "1 " << name << std::endl;
    }

    fout << "2" << std::endl;

    return 0;
}
