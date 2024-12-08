#include <cstdio>

int matrix[2001][2001];
int rowsum[2001][2001];

int main() {
    int n, m, q;
    scanf("%d%d", &n, &m);
    for (int i = 1; i <= n; ++i) {
        for (int j = 1; j <= m; ++j) {
            scanf("%d", &matrix[i][j]);
        }
    }
    for (int i = 1; i <= n; ++i) {
        rowsum[i][0] = 0;
        for (int j = 1; j <= m; ++j) {
            rowsum[i][j] = rowsum[i][j - 1] + matrix[i][j];
        }
    }
    scanf("%d", &q);
    for (int i = 1; i <= q; ++i) {
        int x, y, a, b;
        int sum = 0;
        scanf("%d %d %d %d", &x, &y, &a, &b);
        for (int j = 0; j < a; ++j) {
            sum += rowsum[x + j][y + b - 1] - rowsum[x + j][y - 1];
        }
        printf("%d\n", sum);
    }
    return 0;
}