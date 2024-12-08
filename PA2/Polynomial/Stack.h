#include <cstdio>

template <typename T>
class Stack {
    T *data;
    int size;
    int top_idx;

   public:
    Stack(int size) : size(size), top_idx(-1) { data = new T[size]; }
    bool empty() { return top_idx == -1; }
    void push(const T &e) { data[++top_idx] = e; }
    T pop() { return data[top_idx--]; }
    T &top() { return data[top_idx]; }
};

const char pri[7][7] = {
    // 运算符优先级[栈顶][当前]
    '>', '>', '<', '<', '<', '>', '>',  // +
    '>', '>', '<', '<', '<', '>', '>',  // -
    '>', '>', '>', '<', '<', '>', '>',  // *
    '>', '>', '>', '>', '<', '>', '>',  // ^
    '<', '<', '<', '<', '<', '~', ' ',  // (
    ' ', ' ', ' ', ' ', ' ', ' ', ' ',  // )
    '<', '<', '<', '<', '<', ' ', '~'   // \0
};

int op2rank(char op) {
    switch (op) {
        case '+':
            return 0;
        case '-':
            return 1;
        case '*':
            return 2;
        case '^':
            return 3;
        case '(':
            return 4;
        case ')':
            return 5;
        default:
            return 6;
    }
}

char getPri(char a, char b) { return pri[op2rank(a)][op2rank(b)]; }
