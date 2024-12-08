using uint = unsigned int;

struct Node {
    uint data, count;
    Node *pred, *succ;

    explicit Node(uint data = 0, Node *pred = nullptr, Node *succ = nullptr) : data(data), count(1), pred(pred), succ(succ) {}

    Node *insertAsPred(const uint &e) {
        Node *x = new Node(e, pred, this);
        pred->succ = x;
        pred = x;
        return x;
    }
};

class Queue {
   public:
    Node *header, *trailer;
    uint _size;

    Queue() : _size(0) {
        header = new Node;
        trailer = new Node;
        header->succ = trailer;
        trailer->pred = header;
    }

    ~Queue() {
        while (header->succ != trailer) {
            Node *x = header->succ;
            header->succ = x->succ;
            delete x;
        }
        delete header;
        delete trailer;
    }

    bool empty() const { return _size <= 0; }

    uint &front() const { return header->succ->data; }

    void enqueue(const uint &e);

    uint dequeue();

    uint remove(Node *x);  // return count
};

void Queue::enqueue(const uint &e) {
    trailer->insertAsPred(e);
    ++_size;
}

uint Queue::dequeue() {
    if (empty()) return 0;
    Node *x = header->succ;
    uint e = x->data;
    header->succ = x->succ;
    x->succ->pred = header;
    delete x;
    --_size;
    return e;
}

uint Queue::remove(Node *x) {
    uint e = x->count;
    x->pred->succ = x->succ;
    x->succ->pred = x->pred;
    delete x;
    --_size;
    return e;
}

class Queap {
   public:
    Queue P, Q;  // P for priority queue
    uint size;

    Queap() : size(0) {}

    bool empty() const { return size <= 0; }

    void enqueue(const uint &e);

    uint dequeue();

    uint getMax() const { return P.front(); }
};

void Queap::enqueue(const uint &e) {
    Q.enqueue(e);
    P.enqueue(e);
    ++size;

    Node *x = P.trailer->pred;
    for (Node *p = x->pred; p != P.header && (p->data <= e);) {
        p = p->pred;
        x->count += P.remove(p->succ);
    }
}

uint Queap::dequeue() {
    if (empty()) return 0;
    uint e = Q.dequeue();
    if (P.header->succ->count > 1)
        --P.header->succ->count;
    else
        P.dequeue();
    --size;
    return e;
}
