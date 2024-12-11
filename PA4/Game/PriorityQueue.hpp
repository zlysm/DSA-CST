#define Parent(i) ((i - 1) >> 1)
#define LChild(i) ((i << 1) | 1)
#define RChild(i) ((i + 1) << 1)
#define InHeap(n, i) (((-1) != (i)) && ((i) < (n)))
#define LChildValid(n, i) InHeap(n, LChild(i))
#define RChildValid(n, i) InHeap(n, RChild(i))
#define Bigger(PQ, i, j) ((PQ[i] < PQ[j]) ? j : i)
#define ProperParent(PQ, n, i) (RChildValid(n, i) ? Bigger(PQ, Bigger(PQ, i, LChild(i)), RChild(i)) : (LChildValid(n, i) ? Bigger(PQ, i, LChild(i)) : i))

const int MAX_SIZE = 100000;

struct HeapNode {
    int key, value;
    HeapNode(int k = 0, int v = 0) : key(k), value(v) {}
    bool operator<(const HeapNode &a) const {
        return value >= a.value;
    }
};

class PriorityQueue {
    int _size;
    HeapNode _heap[MAX_SIZE + 5];
    void swap(HeapNode &a, HeapNode &b) {
        HeapNode t = a;
        a = b;
        b = t;
    }
    int percolateUp(int i) {
        while (0 < i) {
            int j = Parent(i);
            if (!(_heap[j] < _heap[i])) break;
            swap(_heap[i], _heap[j]);
            i = j;
        }
        return i;
    }
    int percolateDown(int i) {
        int j;
        while (i != (j = ProperParent(_heap, _size, i))) {
            swap(_heap[i], _heap[j]);
            i = j;
        }
        return i;
    }

   public:
    PriorityQueue() : _size(0) {}

    void push(int id, int weight) {
        _heap[_size++] = HeapNode(id, weight);
        percolateUp(_size - 1);
    }

    int pop() {
        swap(_heap[0], _heap[--_size]);
        percolateDown(0);
        return _heap[_size].key;
    }

    bool empty() {
        return 0 == _size;
    }
};
