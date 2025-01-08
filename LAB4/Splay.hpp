#ifndef SPLAY_HPP
#define SPLAY_HPP

#include "BST.hpp"

class Splay : public virtual BST {
   private:
    Node *splay(Node *v);

   public:
    Node *&search(const int &e) override;
    Node *insert(const int &e) override;
    bool remove(const int &e) override;
};

Node *Splay::splay(Node *v) {
    Node *p, *g;
    while ((p = v->parent) && (g = p->parent)) {  // double rotation
        Node *gg = g->parent;
        switch (((p == g->rc) << 1) | (v == p->rc)) {
            case 0b00:  // zig-zig
                g->attachLc(p->rc);
                p->attachLc(v->rc);
                p->attachRc(g);
                v->attachRc(p);
                break;
            case 0b01:  // zig-zag
                p->attachRc(v->lc);
                g->attachLc(v->rc);
                v->attachRc(g);
                v->attachLc(p);
                break;
            case 0b10:  // zag-zig
                p->attachLc(v->rc);
                g->attachRc(v->lc);
                v->attachLc(g);
                v->attachRc(p);
                break;
            default:  // zag-zag
                g->attachRc(p->lc);
                p->attachRc(v->lc);
                p->attachLc(g);
                v->attachLc(p);
                break;
        }
        if (!gg)
            v->parent = nullptr;
        else
            (g == gg->lc) ? gg->attachLc(v) : gg->attachRc(v);
    }

    p = v->parent;
    if (p) {  // p is root, single rotation
        if (v == p->lc) {
            p->attachLc(v->rc);
            v->attachRc(p);
        } else {
            p->attachRc(v->lc);
            v->attachLc(p);
        }
    }
    v->parent = nullptr;
    return v;
}

Node *&Splay::search(const int &e) {
    Node *p = BST::search(e);
    _root = p ? splay(p) : _hot ? splay(_hot)
                                : nullptr;
    return _root;
}

Node *Splay::insert(const int &e) {
    if (!_root) return _root = new Node(e);
    Node *t = search(e);
    if (t->key == e) return t;
    if (t->key < e) {
        _root = t->parent = new Node(e, nullptr, t, t->rc);
        t->rc = nullptr;
    } else {
        t->parent = _root = new Node(e, nullptr, t->lc, t);
        t->lc = nullptr;
    }
    return _root;
}

bool Splay::remove(const int &e) {
    if (!_root || search(e)->key != e) return false;
    Node *L = _root->lc, *R = _root->rc;
    delete _root;
    if (!R) {
        if (L) L->parent = nullptr;
        _root = L;
    } else {
        _root = R;
        R->parent = nullptr;
        search(e);  // splay the minimum node in the right subtree
        _root->attachLc(L);
    }
    return true;
}

#endif  // SPLAY_HPP
