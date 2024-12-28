#include <cstdio>
#include <iostream>
#include <cstring>

// Structure to store metadata for each node
// Each node contains an array of ancestors, a jump value, and a depth value
struct MetaData {
    int ancestor[23]; // Ancestors for binary lifting
    int jump;         // Jump value for the longest prefix-suffix
    int depth;        // Depth of the node in the tree
} nodes[1500003];

int array[1500003];       // Array to store the sequence of notes
int log2Cache[1500003];   // Cache to store precomputed log2 values

// Function to precompute log2 values for fast access
inline void precomputeLog2Cache() {
    log2Cache[0] = -1;
    for (int i = 1; i < 1500003; i++) {
        log2Cache[i] = log2Cache[i >> 1] + 1;
    }
}

// Fast input function to read integers
inline int fastRead() {
    int x = 0;
    char c = getchar_unlocked();
    while (c < '0' || c > '9') c = getchar_unlocked();
    while (c >= '0' && c <= '9') {
        x = x * 10 + (c - '0');
        c = getchar_unlocked();
    }
    return x;
}

// Fast output function to write integers
inline void fastWrite(int x) {
    if (x == 0) {
        putchar_unlocked('0');
        putchar_unlocked('\n');
        return;
    }
    char buf[11];
    int len = 0;
    while (x) {
        buf[len++] = x % 10 + '0';
        x /= 10;
    }
    while (len--) {
        putchar_unlocked(buf[len]);
    }
    putchar_unlocked('\n');
}

int main() {
    // Read the number of initial notes and number of operations
    int nodeCount = fastRead();
    int queryCount = fastRead();

    // Read the initial sequence of notes
    for (int i = 1; i <= nodeCount; i++) {
        array[i] = fastRead();
    }

    // Precompute the log2 values
    precomputeLog2Cache();

    // Initialize the depth of the first node
    nodes[1].depth = 1;

    // Calculate the ancestor and depth for each node using KMP algorithm-like preprocessing
    for (int x = 2, k = nodes[1].ancestor[0]; x <= nodeCount; ++x) {
        while (k > 0 && array[k + 1] != array[x]) {
            k = nodes[k].ancestor[0];
        }
        if (array[k + 1] == array[x]) {
            ++k;
        }
        nodes[x].ancestor[0] = k;
        nodes[x].depth = nodes[k].depth + 1;
        for (int y = 0; y < log2Cache[nodes[x].depth]; ++y) {
            nodes[x].ancestor[y + 1] = nodes[nodes[x].ancestor[y]].ancestor[y];
        }
    }

    // Calculate the jump values for each node
    for (int x = 2, k = nodes[1].jump; x <= nodeCount; ++x) {
        while (k > 0 && array[k + 1] != array[x]) {
            k = nodes[k].ancestor[0];
        }
        if (array[k + 1] == array[x]) {
            ++k;
        }
        while ((k << 1) > x) {
            k = nodes[k].ancestor[0];
        }
        nodes[x].jump = k;
    }

    int optType, length;
    int operations[1000003];

    // Process each operation
    while (queryCount--) {
        optType = fastRead();
        length = fastRead();

        // Handle the "create" operation
        if (optType == 1) {
            for (int j = 1; j <= length; j++) {
                operations[j] = fastRead();
            }
            // Append the new notes to the array
            memcpy(array + nodeCount + 1, operations + 1, length * sizeof(int));

            // Recalculate ancestors and depth for the new nodes
            for (int x = nodeCount + 1, k = nodes[nodeCount].ancestor[0]; x <= nodeCount + length; ++x) {
                while (k > 0 && array[k + 1] != array[x]) {
                    k = nodes[k].ancestor[0];
                }
                if (array[k + 1] == array[x]) {
                    ++k;
                }
                nodes[x].ancestor[0] = k;
                nodes[x].depth = nodes[k].depth + 1;
                for (int y = 0; y < log2Cache[nodes[x].depth]; ++y) {
                    nodes[x].ancestor[y + 1] = nodes[nodes[x].ancestor[y]].ancestor[y];
                }
            }

            // Recalculate the jump values for the new nodes
            for (int x = nodeCount + 1, k = nodes[nodeCount].jump; x <= nodeCount + length; ++x) {
                while (k > 0 && array[k + 1] != array[x]) {
                    k = nodes[k].ancestor[0];
                }
                if (array[k + 1] == array[x]) {
                    ++k;
                }
                while ((k << 1) > x) {
                    k = nodes[k].ancestor[0];
                }
                nodes[x].jump = k;
            }

            // Update the total node count
            nodeCount += length;

        // Handle the "test" operation
        } else {
            for (int j = 1; j <= length; j++) {
                operations[j] = fastRead();
            }
            int result = nodes[operations[1]].jump;

            // Find the longest common prefix-suffix among the prefixes
            for (int j = 0; j < length - 1; j++) {
                int u = result, v = nodes[operations[j + 2]].jump;
                if (nodes[u].depth < nodes[v].depth) {
                    std::swap(u, v);
                }
                int depthDiff = nodes[u].depth - nodes[v].depth;

                // Lift u to the same depth as v
                for (int i = 0; i <= log2Cache[nodes[u].depth]; ++i) {
                    if (depthDiff & (1 << i)) {
                        u = nodes[u].ancestor[i];
                    }
                }

                // If they are the same, this is the LCA
                if (u == v) {
                    result = u;
                    continue;
                }

                // Binary lift both u and v until their ancestors match
                for (int i = log2Cache[nodes[v].depth]; i >= 0; --i) {
                    if (nodes[u].ancestor[i] != nodes[v].ancestor[i]) {
                        u = nodes[u].ancestor[i];
                        v = nodes[v].ancestor[i];
                    }
                }

                // The ancestor of u (or v) is the LCA
                result = nodes[u].ancestor[0];
            }

            // Output the result for the "test" operation
            fastWrite(result);
        }
    }
    return 0;
}
