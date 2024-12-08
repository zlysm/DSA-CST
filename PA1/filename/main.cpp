#include <cmath>
#include <cstdio>

char str1[501005], str2[501005];
short dp[501005][501005];

int main() {
    int n, m, k;
    scanf("%d %d %d", &n, &m, &k);
    scanf("%s %s", str1, str2);

    if (abs(n - m) > k) {
        printf("-1\n");
        return 0;
    }

    for (int i = 0; i < n; ++i) {
        int lo = 0 > i - k ? 0 : i - k;
        int hi = m - 1 < i + k ? m - 1 : i + k;
        for (int j = lo; j <= hi; ++j) {
            if (str1[i] == str2[j])
                dp[i + 1][j + 1] = dp[i][j] + 1;
            else
                dp[i + 1][j + 1] = dp[i][j + 1] > dp[i + 1][j] ? dp[i][j + 1] : dp[i + 1][j];
        }
    }

    printf("%d\n", m + n - 2 * dp[n][m]);
}
