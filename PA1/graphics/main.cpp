#include <cstdio>
#include <cstdlib>

const int MAXN = 10000000;
const int MAXS = 60 * 1024 * 1024;

int numbers[MAXN];
char buf[MAXS];

void read() {
    int len = fread(buf, 1, MAXS, stdin);
    buf[len] = '\0';
    int i;
    numbers[i = 0] = 0;
    for (char *p = buf; *p && p - buf < len; ++p)
        if (*p == ' ' || *p == '\n')
            numbers[++i] = 0;
        else
            numbers[i] = numbers[i] * 10 + *p - '0';
}

inline bool cross(int x, int y, int a, int b) {  // 判断端点是否在直线上方
    long long x1 = x, y1 = y, x2 = a, y2 = b;    // 1代表轴上点，2代表直线端点
    return x2 * y1 + (y2 - y1) * x1 >= 0;        // 直线与端点叉乘
}

int count(int *arr, int x, int y, int n);

int main() {
    read();
    int n = numbers[0];

    auto cmp = [](const void *a, const void *b) { return *(const int *)a - *(const int *)b; };
    qsort(numbers + 1, n, sizeof(int), cmp);  // 排序
    qsort(numbers + n + 1, n, sizeof(int), cmp);

    int m = numbers[n + n + 1];
    int idx = n + n + 2;
    while (m--) {
        int x = numbers[idx++];
        int y = numbers[idx++];
        int ans = count(numbers + 1, x, y, n);
        printf("%d\n", ans);
    }
    return 0;
}

int count(int *arr, int x, int y, int n) {  // 二分查找最后一个在直线上的点
    int ans = 0, lo = 0, hi = n - 1;
    while (lo <= hi) {
        int mid = lo + ((hi - lo) >> 1);
        if (cross(arr[mid], arr[mid + n], x, y)) {
            ans = mid + 1;
            lo = mid + 1;
        } else
            hi = mid - 1;
    }
    return ans;
}
