#include <cmath>
#include <cstdio>
#include <cstring>

char str1[501005], str2[501005];

template <typename T>
void swap(T& a, T& b) {
    T t = a;
    a = b;
    b = t;
}

int max(int a, int b) {
    return a > b ? a : b;
}

int min(int a, int b) {
    return a < b ? a : b;
}

unsigned int lcs(char* A, int n, char* B, int m, int k) {
    if (n < m) {
        swap(A, B);
        swap(n, m);
    }  // make sure m <= n
    unsigned int* lcs1 = new unsigned int[m + 1];  // the current two rows are
    unsigned int* lcs2 = new unsigned int[m + 1];  // buffered alternatively
    memset(lcs1, 0x00, sizeof(unsigned int) * (m + 1));
    lcs2[0] = 0;  // sentinels
    for (int i = 0; i < n; swap(lcs1, lcs2), i++) {
        int lo = max(0, i - k), hi = min(m, i + k + 1);
        for (int j = lo; j < hi; j++)
            lcs2[j + 1] = (A[i] == B[j]) ? 1 + lcs1[j] : max(lcs2[j], lcs1[j + 1]);
    }
    unsigned int solu = lcs1[m];
    delete[] lcs1;
    delete[] lcs2;
    return solu;
}

int main() {
    int n, m, k;
    scanf("%d %d %d", &n, &m, &k);
    scanf("%s %s", str1, str2);

    if (abs(n - m) > k) {
        printf("-1\n");
        return 0;
    }

    int ans = m + n - 2 * lcs(str1, n, str2, m, k);
    if (ans > k) ans = -1;
    printf("%d\n", ans);
}