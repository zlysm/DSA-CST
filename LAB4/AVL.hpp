#ifndef AVL_HPP
#define AVL_HPP

#include "BST.hpp"

#define BalFac(x) (stature((x)->lc) - stature((x)->rc))
#define AvlBalanced(x) ((-2 < BalFac(x)) && (BalFac(x) < 2))

class AVL : public virtual BST {
   private:
    Node *&tallerChild(Node *x);

   public:
    Node *insert(const int &e) override;
    bool remove(const int &e) override;
};

Node *&AVL::tallerChild(Node *x) {
    if (stature(x->lc) > stature(x->rc))
        return x->lc;
    else if (stature(x->lc) < stature(x->rc))
        return x->rc;
    else if ((x->parent) && (x == x->parent->lc))
        return x->lc;
    else
        return x->rc;
}

Node *AVL::insert(const int &e) {
    Node *&x = search(e);
    if (x) return x;

    Node *xx = x = new Node(e, _hot);
    for (Node *g = _hot; g; g->updateHeight(), g = g->parent)
        if (!AvlBalanced(g)) {
            rotateAt(tallerChild(tallerChild(g)));
            break;
        }
    return xx;
}

bool AVL::remove(const int &e) {
    Node *&x = search(e);
    if (!x) return false;

    Node *succ = nullptr, *w = x;
    if (!x->lc)
        succ = x = x->rc;
    else if (!x->rc)
        succ = x = x->lc;
    else {  // x has two children
        w = x->rc;
        while (w->lc) w = w->lc;
        x->key = w->key;

        Node *u = w->parent;
        (u == x ? u->rc : u->lc) = succ = w->rc;
    }

    _hot = w->parent;
    if (succ) succ->parent = _hot;
    delete w;

    for (Node *g = _hot; g; g->updateHeight(), g = g->parent)
        if (!AvlBalanced(g))
            rotateAt(tallerChild(tallerChild(g)));
    return true;
}

#endif  // AVL_HPP
