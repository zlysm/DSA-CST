#include <cstdio>
#include <cstring>

#include "Stack.h"

#define isdigit(c) ((c) >= '0' && (c) <= '9')

const int MOD = 1000000007;
char buf[1000005], str[5000000];
int str_idx = 0, str_len = 0;

struct Num {
    int coef[65];
    int high;
    Num() : high(0) { memset(coef, 0, sizeof(coef)); }
};

void readNumber(Stack<Num> &opnd) {
    Num res;
    if (str[str_idx] == 'x') {
        res.coef[1] = 1;
        res.high = 1;
        str_idx++;
    } else {
        int num = 0;
        while (isdigit(str[str_idx]))
            num = num * 10 + str[str_idx++] - '0';
        res.coef[0] = num;
        res.high = 0;
    }

    opnd.push(res);
}

Num calc(Num rs1, char op, Num rs2) {
    Num res;
    switch (op) {
        case '+':
            res.high = rs1.high > rs2.high ? rs1.high : rs2.high;
            for (int i = 0; i <= res.high; ++i) {
                res.coef[i] = rs1.coef[i] + rs2.coef[i];
                res.coef[i] %= MOD;
            }
            while (res.coef[res.high] == 0 && res.high > 0)
                res.high--;
            break;
        case '-':
            res.high = rs1.high > rs2.high ? rs1.high : rs2.high;
            for (int i = 0; i <= res.high; ++i) {
                res.coef[i] = rs1.coef[i] - rs2.coef[i];
                res.coef[i] %= MOD;
            }
            while (res.coef[res.high] == 0 && res.high > 0)
                res.high--;
            break;
        case '*':
            res.high = rs1.high + rs2.high;
            for (int i = 0; i <= rs1.high; ++i)
                for (int j = 0; j <= rs2.high; ++j) {
                    long long tmp = (long long)rs1.coef[i] * rs2.coef[j];
                    res.coef[i + j] = (res.coef[i + j] + tmp) % MOD;
                }
            break;
        case '^': {
            Num tmp = rs1;
            for (int i = 1; i < rs2.coef[0]; ++i)
                tmp = calc(tmp, '*', rs1);
            return tmp;
        }
    }

    return res;
}

int main() {
    int buf_len = fread(buf, 1, sizeof(buf), stdin);
    buf[buf_len] = '\0';

    for (int i = 0; i < buf_len; ++i) {  // add *
        if (buf[i + 1] == '(' && (buf[i] == 'x' || isdigit(buf[i]) || buf[i] == ')')) {
            str[str_len++] = buf[i];
            str[str_len++] = '*';
        } else if (buf[i + 1] == 'x' && (isdigit(buf[i]) || buf[i] == ')')) {
            str[str_len++] = buf[i];
            str[str_len++] = '*';
        } else {
            str[str_len++] = buf[i];
        }
    }
    str[str_len] = '\0';

    Stack<Num> opnd(900000);
    Stack<char> optr(1000000);
    optr.push('\0');

    while (!optr.empty()) {
        if (isdigit(str[str_idx]) || str[str_idx] == 'x')
            readNumber(opnd);
        else
            switch (getPri(optr.top(), str[str_idx])) {
                case '<':
                    optr.push(str[str_idx++]);
                    break;
                case '>': {
                    char op = optr.pop();
                    Num rs2 = opnd.pop(), rs1 = opnd.pop();
                    opnd.push(calc(rs1, op, rs2));
                    break;
                }
                case '~':
                    optr.pop();
                    ++str_idx;
                    break;
            }
    }

    Num ans = opnd.pop();
    int high = ans.high;
    while (ans.coef[high] == 0 && high > 0)
        high--;
    for (int i = high; i >= 0; --i)
        printf("%d ", (ans.coef[i] + MOD) % MOD);

    return 0;
}
