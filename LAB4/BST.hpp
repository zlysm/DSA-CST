#ifndef BST_HPP
#define BST_HPP

#define stature(p) ((int)((p) ? (p)->height : -1))
#define FromParentTo(x) ((x)->parent ? (((x) == (x)->parent->lc) ? (x)->parent->lc : (x)->parent->rc) : _root)

struct Node {
    Node *parent, *lc, *rc;
    int key, height;
    Node() : parent(nullptr), lc(nullptr), rc(nullptr), height(0) {}
    Node(int e, Node *p = nullptr, Node *lc = nullptr, Node *rc = nullptr, int h = 0)
        : key(e), parent(p), lc(lc), rc(rc), height(h) {
        if (lc) lc->parent = this;
        if (rc) rc->parent = this;
    }
    void updateHeight() {
        this->height = stature(lc) > stature(rc) ? stature(lc) + 1 : stature(rc) + 1;
    }
    void attachLc(Node *x) {
        lc = x;
        if (x) x->parent = this;
    }
    void attachRc(Node *x) {
        rc = x;
        if (x) x->parent = this;
    }
};

class BST {
   protected:
    Node *_root, *_hot;
    Node *connect34(Node *a, Node *b, Node *c, Node *T0, Node *T1, Node *T2, Node *T3);
    Node *rotateAt(Node *v);

   public:
    BST() : _root(nullptr), _hot(nullptr) {}
    virtual Node *&search(const int &e);
    virtual Node *insert(const int &e) = 0;
    virtual bool remove(const int &e) = 0;
    int searchMax(const int &e);
};

Node *BST::connect34(Node *a, Node *b, Node *c, Node *T0, Node *T1, Node *T2, Node *T3) {
    a->lc = T0;
    if (T0) T0->parent = a;
    a->rc = T1;
    if (T1) T1->parent = a;
    a->updateHeight();

    c->lc = T2;
    if (T2) T2->parent = c;
    c->rc = T3;
    if (T3) T3->parent = c;
    c->updateHeight();

    b->lc = a;
    a->parent = b;
    b->rc = c;
    c->parent = b;
    b->updateHeight();

    return b;
}

Node *BST::rotateAt(Node *v) {
    Node *p = v->parent;
    int turnV = (v == p->rc);
    Node *g = p->parent;
    int turnP = (p == g->rc);
    Node *r = (turnV == turnP) ? p : v;
    FromParentTo(g) = r;
    r->parent = g->parent;

    switch ((turnP << 1) | turnV) {
        case 0b00:  // zig-zig
            return connect34(v, p, g, v->lc, v->rc, p->rc, g->rc);
        case 0b01:  // zig-zag
            return connect34(p, v, g, p->lc, v->lc, v->rc, g->rc);
        case 0b10:  // zag-zig
            return connect34(g, v, p, g->lc, v->lc, v->rc, p->rc);
        default:  // zag-zag
            return connect34(g, p, v, g->lc, p->lc, v->lc, v->rc);
    }
}

Node *&BST::search(const int &e) {
    if (!_root || _root->key == e) {
        _hot = nullptr;
        return _root;
    }
    _hot = _root;
    while (true) {
        Node *&v = (e < _hot->key) ? _hot->lc : _hot->rc;
        if (!v || v->key == e) return v;
        _hot = v;
    }
}

int BST::searchMax(const int &e) {
    Node *cur = _root;
    int res = -1;
    while (cur)
        if (cur->key <= e) {
            res = cur->key;
            cur = cur->rc;
        } else {
            cur = cur->lc;
        }
    return res;
}

#endif  // BST_HPP
