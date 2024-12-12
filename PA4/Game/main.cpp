#include <cstdio>
#include <cstring>

#include "PriorityQueue.hpp"

const int MOD = 1000000007;

struct Edge {
    int next, weight;
    Edge *succ;
    Edge(int n = 0, int w = 0, Edge *s = nullptr) : next(n), weight(w), succ(s) {}
};

class Graph {
    Edge *_edges;

   public:
    Graph(int v) { _edges = new Edge[v]; }
    void insert(int u, int v, int w) {
        _edges[u].succ = new Edge(v, w, _edges[u].succ);
    }
    Edge *getEdge(int u) { return _edges[u].succ; }
};

int main() {
    int n, m;
    scanf("%d %d", &n, &m);

    int time[MAX_SIZE + 5];
    for (int i = 0; i < n; ++i)
        scanf("%d", &time[i]);

    Graph g(n);
    while (m--) {
        int u, v;
        scanf("%d %d", &u, &v);
        --u, --v;
        g.insert(u, v, time[v]);
        g.insert(v, u, time[u]);
    }

    PriorityQueue pq;
    int dist[MAX_SIZE + 5], cnt[MAX_SIZE + 5] = {0};
    memset(dist, 0x3f, sizeof(dist));
    cnt[0] = 1;

    pq.push(0, dist[0] = time[0]);
    while (!pq.empty()) {
        int u = pq.pop();
        for (Edge *e = g.getEdge(u); e; e = e->succ) {
            int v = e->next;
            if (dist[u] + e->weight < dist[v]) {
                dist[v] = dist[u] + e->weight;
                cnt[v] = cnt[u];
                pq.push(v, dist[v]);
            } else if (dist[u] + e->weight == dist[v]) {
                cnt[v] = (cnt[v] + cnt[u]) % MOD;
            }
        }
    }

    printf("%d\n%d\n", dist[n - 1], cnt[n - 1]);
    return 0;
}
