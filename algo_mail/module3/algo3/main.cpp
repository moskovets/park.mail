/*
 * Задача 3_2. ​Вычислить количество узлов в самом широком слое декартового дерева и
количество узлов в самом широком слое наивного дерева поиска. Вывести их разницу.
Разница может быть отрицательна.
 */
#include <iostream>
#include <algorithm>
#include <cstring>
#include <vector>
using namespace std;
template <typename T>
struct TreapNode {
    T key;
    int prior;
    TreapNode * l, * r;
    TreapNode() {
        l = NULL;
        r = NULL;
    }
    TreapNode (T key, int prior) : key(key), prior(prior), l(NULL), r(NULL) { }
};
template <typename T>
class CTreap {
private:
    TreapNode<T> *head = NULL;
    void free_tree(TreapNode<T> *tmp);
    //int treap_deep_of_search(TreapNode<T> *tmp);
    void split(TreapNode<T> *t, int key, TreapNode<T>* &l, TreapNode<T>* &r);
    void insert(TreapNode<T>* &t, TreapNode<T>* it);
    void merge(TreapNode<T>* &t, TreapNode<T>* l, TreapNode<T>* r);

public:
    CTreap();
    ~CTreap();
    void print()
    {
        TreapNode<T> *tmp = head;
        while(tmp) {
            cout << tmp->key;
            tmp = tmp->r;
        }
    }
    void Insert(T &k, int p);
    void Erase(TreapNode<T> *t, T &key);
    int Max_width_of_treap();
};
template <typename T>
void CTreap<T>::Insert(T &k, int p) {
    TreapNode<T> * tmp = new TreapNode<T>(k, p);
    insert(head, tmp);
}

template <typename T>
CTreap<T>::~CTreap() {
    free_tree(head);
}
template <typename T>
CTreap<T>::CTreap() {
    head = NULL;
}
template <typename T>
void CTreap<T>::free_tree(TreapNode<T> *tmp) {
    if(tmp) {
        free_tree(tmp->l);
        free_tree(tmp->r);
        delete tmp;
    }
}
template <typename T>
int CTreap<T>::Max_width_of_treap() {
    if(head == NULL) { return 0; }
    vector<TreapNode<T>*> arr;
    arr.push_back(head);
    int l = 0, r = 1;
    int res = 1;
    while(l < r) {
        if(res < (r - l)) {
            res = r - l;
        }
        for(int i = l; i < r; i++) {
            if(arr[i]->l) {
                arr.push_back(arr[i]->l);
            }
            if(arr[i]->r) {
                arr.push_back(arr[i]->r);
            }
        }
        l = r;
        r = arr.size();
    }
    return res;
}
template <typename T>
void CTreap<T>::split(TreapNode<T> *t, int key, TreapNode<T> * &l, TreapNode<T>* &r) {
    if (!t)
        l = r = NULL;
    else if (key < t->key)
        split (t->l, key, l, t->l),  r = t;
    else
        split (t->r, key, t->r, r),  l = t;
}
template <typename T>
void CTreap<T>::insert(TreapNode<T> * &t, TreapNode<T> *it) {
    if (!t)
        t = it;
    else if (it->prior > t->prior)
        split (t, it->key, it->l, it->r),  t = it;
    else
        insert (it->key < t->key ? t->l : t->r, it);
}
template <typename T>
void CTreap<T>::merge(TreapNode<T> * &t, TreapNode<T> *l, TreapNode<T> *r) {
    if (!l || !r)
        t = l ? l : r;
    else if (l->prior > r->prior)
        merge (l->r, l->r, r),  t = l;
    else
        merge (r->l, l, r->l),  t = r;
}
template <typename T>
void CTreap<T>::Erase(TreapNode<T> *t, T &key) {
    if (t->key == key)
        merge (t, t->l, t->r);
    else
        Erase(key < t->key ? t->l : t->r, key);
}

/*TreapNode unite (TreapNode l, TreapNode r) {
    if (!l || !r)  return l ? l : r;
    if (l->prior < r->prior)  swap (l, r);
    TreapNode lt, rt;
    split (r, l->key, lt, rt);
    l->l = unite (l->l, lt);
    l->r = unite (l->r, rt);
    return l;
}*/
template <typename T>
struct element {
    T value;
    element *l = NULL;
    element *r = NULL;
    element(T x) {
        value = x;
        l = NULL;
        r = NULL;
    }
};
//Binary search tree
template <typename T>
class CBST {
private:
    element<T> *head = NULL;
    void delete_tree(element<T>* tmp);
    void delete_remove(element<T>* prev, element<T> *tmp);

public:
    CBST();
    void Insert(T x);
    bool Remove(T x);
    int Max_width_of_tree();
    ~CBST();
};
void log(const char *str)
{
    ;  //cout << endl << "l: " << str << endl;
}

template <typename T>
void CBST<T>::Insert(T x) {
    element<T> *new_elem = new element<T>(x);
    element<T> *tmp = head;
    if(!head) {
        head = new_elem;
        return;
    }
    while(1) {
        if(!tmp) {
            tmp = new_elem;
            break;
        }
        if(tmp->value < x) {
            if(tmp->r) {
                tmp = tmp->r;
            }
            else {
                tmp->r = new_elem;
                break;
            }
        }
        else {
            if(tmp->l) {
                tmp = tmp->l;
            }
            else {
                tmp->l = new_elem;
                break;
            }
        }
    }
}
template <typename T>
void CBST<T>::delete_remove(element<T>* prev, element<T> *tmp) {
    element<T> *b = NULL;
    int flag = 0;
    if((!tmp->l) && (!tmp->r)) {
        b = NULL;
        flag = 1;
    }
    if(tmp->l && !tmp->r) {
        b = tmp->l;
        flag = 1;
    }
    if(!tmp->l && tmp->r) {
        b = tmp->r;
        flag = 1;
    }
    if(flag) {
        if(prev == NULL || prev == tmp) {
            head = b;
        }
        if(prev->l == tmp) {
            prev->l = b;
        }
        else if(prev->r == tmp){
            prev->r = b;
        }
        delete tmp;
        return;
    }
    element<T>* a = tmp->r;
    element<T>* new_elem = tmp;
    while(a) {
        if(a->l) {
            new_elem = a;
            a = a->l;
        }
        else {
            break;
        }
    }
    tmp->value = a->value;
    if(new_elem->l == a) {
        new_elem->l = a->r;
    }
    else {
        new_elem->r = a->r;
    }
    delete a;
}
template <typename T>
bool CBST<T>::Remove(T x) {
    element<T> *tmp = head;
    element<T> *prev = NULL;
    int cmp = 0;
    while(1) {
        if(!tmp) {
            return false;
        }
        if(x == tmp->value) {
            delete_remove(prev, tmp);
            return true;
        }
        prev = tmp;
        if(tmp->value < x) {
            if(tmp->r) {
                tmp = tmp->r;
            }
            else {
                return false;
            }
        }
        else {
            if(tmp->l) {
                tmp = tmp->l;
            }
            else {
                return false;
            }
        }
    }

}
template <typename T>
CBST<T>::~CBST() {
    log("~");
    delete_tree(head);
    head = NULL;
}
template <typename T>
void CBST<T>::delete_tree(element<T> *tmp) {
    if(tmp) {
        delete_tree(tmp->l);
        delete_tree(tmp->r);
        delete tmp;
        tmp = NULL;
    }
}
template <typename T>
CBST<T>::CBST() {
    head = NULL;
}

template <typename T>
int CBST<T>::Max_width_of_tree() {
    if(head == NULL) { return 0; }
    vector<element<T>*> arr;
    arr.push_back(head);
    int l = 0, r = 1;
    int res = 1;
    while(l < r) {
        if(res < (r - l)) {
            res = r - l;
        }
        for(int i = l; i < r; i++) {
            if(arr[i]->l) {
                arr.push_back(arr[i]->l);
            }
            if(arr[i]->r) {
                arr.push_back(arr[i]->r);
            }
        }
        l = r;
        r = arr.size();
    }
    return res;
}

/*struct Tree
{
    int data;
    Tree* left;
    Tree* right;
    Tree* parent;
    Tree() {
        left = NULL;
        right = NULL;
        parent = NULL;
    }
};
void add_to_tree(Tree* tmp, int x)
{
    if(tmp->data <= x) {
        if(tmp->right == NULL) {
            Tree* n = new Tree;
            n->parent = tmp;
            n->data = x;
            n->left = n->right = NULL;
            tmp->right = n;
        }
        else {
            add_to_tree(tmp->right, x);
        }
    }
    else {
        if(tmp->left == NULL) {
            Tree* n = new Tree;
            n->parent = tmp;
            n->data = x;
            n->left = n->right = NULL;
            tmp->left = n;
        }
        else {
            add_to_tree(tmp->left, x);
        }
    }
}
int tree_deep_of_search(Tree* tmp) {
    if(tmp == NULL) { return 0; }
    return max(tree_deep_of_search(tmp->left), tree_deep_of_search(tmp->right)) + 1;
}
void free_tree(Tree* tmp) {
    if(tmp) {
        free_tree(tmp->left);
        free_tree(tmp->right);
        delete tmp;
    }
}

int max_width_of_treap(Tree* tmp) {
    if(tmp == NULL) { return 0; }
    vector<Tree*> arr;
    arr.push_back(tmp);
    int l = 0, r = 1;
    int res = 1;
    while(l < r) {
        if(res < (r - l)) {
            res = r - l;
        }
        for(int i = l; i < r; i++) {
            if(arr[i]->left) {
                arr.push_back(arr[i]->left);
            }
            if(arr[i]->right) {
                arr.push_back(arr[i]->right);
            }
        }
        l = r;
        r = arr.size();
    }
    return res;
}*/
int main()
{
    int i, N, x, y;
    CBST<int> tree;
    cin >> N;
    CTreap<int> treap;
    for(i = 0; i < N-1; i++) {
        cin >> x >> y;
        tree.Insert(x);
        treap.Insert(x, y);
    }
    cout << treap.Max_width_of_treap() - tree.Max_width_of_tree()  << endl;
    //free_tree(root);
    //treap.print();
    return 0;
}
