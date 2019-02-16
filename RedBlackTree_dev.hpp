#ifndef __ALICE_RED_BLACK_TREE_HPP__
#define __ALICE_RED_BLACK_TREE_HPP__

#include <cstdio>
#include <cassert>

namespace Alice{

template<typename T>
struct Node
{
    enum Color : bool { Red, Black };
    T key;
    Color color;
    Node *p, *l, *r;
};



template<typename T, typename Node = Node<T> >
class RedBlackTree
{

public:

    RedBlackTree();
    ~RedBlackTree();

    Node* Find(const T& key);
    void Insert(const T& key);
    void Delete(Node* x);
    void Delete(const T& key);

    void Show() const;

    void Travel(void Fn(Node*));
    void Travel(Node* node, void Fn(Node*));

    void Travel(void Fn(const Node*)) const;
    void Travel(Node* node, void Fn(const Node*)) const;

    Node* Minimum(Node* node) const;
    const Node* Nil() const;

private:

    using Color = typename Node::Color;

    Node* const nil;
    Node* root;

    Node* NewNode(const T& key);
    void DeleteNode(Node* node);

    void LeftRotate(Node* x);
    void RightRotate(Node* x);
    void Transplant(Node* x, Node* y);

    void InsertFixUp(Node* x);
    void DeleteFixUp(Node* x);

};


template<typename T, typename Node>
RedBlackTree<T, Node>::RedBlackTree()
    : nil((Node*) operator new(sizeof(Node)))
{
    nil->color = Color::Black;
    nil->p = nil->l = nil->r = nil;
    root = nil;
}

template<typename T, typename Node>
RedBlackTree<T, Node>::~RedBlackTree()
{
    auto del = [] (Node* node){
        operator delete(node);
    };
    Travel(del);
    operator delete(nil);
}

template<typename T, typename Node>
const Node* RedBlackTree<T, Node>::Nil() const
{
    return nil;
}

template<typename T, typename Node>
void RedBlackTree<T, Node>::Travel(void Fn(Node*))
{
    Travel(root, Fn);
}

template<typename T, typename Node>
void RedBlackTree<T, Node>::Travel(Node* node, void Fn(Node*))
{
    if(node == nil) return;
    Travel(node->l, Fn);
    Fn(node);
    Travel(node->r, Fn);
}

template<typename T, typename Node>
void RedBlackTree<T, Node>::Travel(void Fn(const Node*)) const
{
    Travel(root, Fn);
}

template<typename T, typename Node>
void RedBlackTree<T, Node>::Travel(Node* node, void Fn(const Node*)) const
{
    if(node == nil) return;
    Travel(node->l, Fn);
    Fn(node);
    Travel(node->r, Fn);
}

template<typename T, typename Node>
void RedBlackTree<T, Node>::Show() const
{
    auto print = [](const Node* node){
        std::cout << node->key << " ";
    };
    Travel(print);
    std::cout << std::endl;
}

template<typename T, typename Node>
Node* RedBlackTree<T, Node>::Find(const T& key)
{
    Node *x = root;
    while(x != nil && key != x->key)
    {
        if(key < x->key) x = x->l;
        else x = x->r;
    }
    return x;
}

template<typename T, typename Node>
Node* RedBlackTree<T, Node>::NewNode(const T& key)
{
    Node* node = (Node*) operator new(sizeof(Node));
    node->key = key;
    node->color = Color::Red;
    node->p = node->l = node->r = nil;
    return node;
}

template<typename T, typename Node>
void RedBlackTree<T, Node>::DeleteNode(Node* x)
{
    operator delete(x);
}

template<typename T, typename Node>
void RedBlackTree<T, Node>::LeftRotate(Node* x)
{
    Node* y = x->r;
    if(x->p == nil) root = y;
    else if(x == x->p->l) x->p->l = y;
    else x->p->r = y;
    y->p = x->p;
    x->r = y->l;
    if(y->l != nil) y->l->p = x;
    y->l = x;
    x->p = y;
}

template<typename T, typename Node>
void RedBlackTree<T, Node>::RightRotate(Node* x)
{
    Node* y = x->l;
    if(x->p == nil) root = y;
    else if(x == x->p->l) x->p->l = y;
    else x->p->r = y;
    y->p = x->p;
    x->l = y->r;
    if(y->r != nil) y->r->p = x;
    y->r = x;
    x->p = y;
}

template<typename T, typename Node>
Node* RedBlackTree<T, Node>::Minimum(Node* x) const
{
    while(x->l != nil) x = x->l;
    return x;
}

template<typename T, typename Node>
void RedBlackTree<T, Node>::Insert(const T& key)
{
    Node *x = root, *y = nil, *z = NewNode(key);
    while(x != nil)
    {
        y = x;
        if(key < x->key) x = x->l;
        else x = x->r;
    }
    z->p = y;
    if(y == nil) root = z;
    else if(key < y->key) y->l = z;
    else y->r = z;
    InsertFixUp(z);
}

template<typename T, typename Node>
void RedBlackTree<T, Node>::InsertFixUp(Node* x)
{
    Node* y;
    while(x->p->color == Color::Red)
    {
        if(x->p == x->p->p->l)
        {
            y = x->p->p->r;
            if(y->color == Color::Red)
            {
                x->p->color = Color::Black;
                y->color = Color::Black;
                x->p->p->color = Color::Red;
                x = x->p->p;
            }
            else
            {
                if(x == x->p->r)
                {
                    x = x->p;
                    LeftRotate(x);
                }
                x->p->color = Color::Black;
                x->p->p->color = Color::Red;
                RightRotate(x->p->p);
            }
        }
        else
        {
            y = x->p->p->l;
            if(y->color == Color::Red)
            {
                x->p->color = Color::Black;
                y->color = Color::Black;
                x->p->p->color = Color::Red;
                x = x->p->p;
            }
            else
            {
                if(x == x->p->l)
                {
                    x = x->p;
                    RightRotate(x);
                }
                x->p->color = Color::Black;
                x->p->p->color = Color::Red;
                LeftRotate(x->p->p);
            }
        }
    }
    root->color = Color::Black;
}


template<typename T, typename Node>
void RedBlackTree<T, Node>::Transplant(Node* x, Node* y)
{
    if(x->p == nil) root = y;
    else if(x == x->p->l) x->p->l = y;
    else x->p->r = y;
    y->p = x->p;
}

template<typename T, typename Node>
void RedBlackTree<T, Node>::Delete(const T& key)
{
    Delete(Find(key));
}

template<typename T, typename Node>
void RedBlackTree<T, Node>::Delete(Node* x)
{
    if(x == nil) return;
    Node *y = x, *z;
    Color color = y->color;   //y's original color
    if(x->l == nil){
        z = x->r;
        Transplant(x, x->r);
    }
    else if(x->r == nil){
        z = x->l;
        Transplant(x, x->l);
    }
    else
    {
        y = Minimum(x->r);
        color = y->color;
        z = y->r;
        if(y->p != x){
            Transplant(y, z);
            y->r = x->r;
            x->r->p = y;
        }
        y->l = x->l;
        x->l->p = y;
        y->color = x->color;
        Transplant(x, y);
    }
    if(color == Color::Black) DeleteFixUp(z);
    DeleteNode(x);
}

template<typename T, typename Node>
void RedBlackTree<T, Node>::DeleteFixUp(Node* x)
{
    while(x != root && x->color == Color::Black)
    {
        Node* y;
        if(x == x->p->l)
        {
            y = x->p->r;
            if(y->color == Color::Red)
            {
                y->color = Color::Black;
                y->p->color = Color::Red;
                LeftRotate(y->p);
                y = x->p->r;
            }
            if(y->l->color == Color::Black && y->r->color == Color::Black)
            {
                y->color = Color::Red;
                x = x->p;
            }
            else{
                if(y->r->color == Color::Black)
                {
                    y->color = Color::Red;
                    y->l->color = Color::Black;
                    RightRotate(y);
                    y = x->p->r;
                }
                y->r->color = Color::Black;
                y->color = y->p->color;
                y->p->color = Color::Black;
                LeftRotate(y->p);
                x = root;
            }
        }
        else{
            y = x->p->l;
            if(y->color == Color::Red)
            {
                y->color = Color::Black;
                y->p->color = Color::Red;
                RightRotate(y->p);
                y = x->p->l;
            }
            if(y->l->color == Color::Black && y->r->color == Color::Black)
            {
                y->color = Color::Red;
                x = x->p;
            }
            else
            {
                if(y->l->color == Color::Black)
                {
                    y->color = Color::Red;
                    y->r->color = Color::Black;
                    LeftRotate(y);
                    y = x->p->l;
                }
                y->l->color = Color::Black;
                y->color = y->p->color;
                y->p->color = Color::Black;
                RightRotate(y->p);
                x = root;
            }
        }
    }
    x->color = Color::Black;
}





}        //namespace Alice

using Alice::RedBlackTree;


#endif //    #ifndef __ALICE_RED_BLACK_TREE_HPP__

