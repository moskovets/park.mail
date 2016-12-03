#include <iostream>
#include <algorithm>
#include <cstring>
#include <vector>
using namespace std;
struct treap {
    int key, prior;
    treap * l, * r;
    treap() {
        l = NULL;
        r = NULL;
    }
    treap (int key, int prior) : key(key), prior(prior), l(NULL), r(NULL) { }
};
typedef treap * TreapNode;
int treap_deep_of_search(TreapNode tmp) {
    if(tmp == NULL) {
        return 0;
    }
    return max(treap_deep_of_search(tmp->l), treap_deep_of_search(tmp->r)) + 1;
}
void free_tree(TreapNode tmp) {
    if(tmp) {
        free_tree(tmp->l);
        free_tree(tmp->r);
        delete tmp;
    }
}
int max_width_of_treap(TreapNode tmp) {
    if(tmp == NULL) { return 0; }
    vector<TreapNode> arr;
    arr.push_back(tmp);
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
void split (TreapNode t, int key, TreapNode & l, TreapNode & r) {
    if (!t)
        l = r = NULL;
    else if (key < t->key)
        split (t->l, key, l, t->l),  r = t;
    else
        split (t->r, key, t->r, r),  l = t;
}
void insert (TreapNode & t, TreapNode it) {
    if (!t)
        t = it;
    else if (it->prior > t->prior)
        split (t, it->key, it->l, it->r),  t = it;
    else
        insert (it->key < t->key ? t->l : t->r, it);
}
void merge (TreapNode & t, TreapNode l, TreapNode r) {
    if (!l || !r)
        t = l ? l : r;
    else if (l->prior > r->prior)
        merge (l->r, l->r, r),  t = l;
    else
        merge (r->l, l, r->l),  t = r;
}

void erase (TreapNode & t, int key) {
    if (t->key == key)
        merge (t, t->l, t->r);
    else
        erase (key < t->key ? t->l : t->r, key);
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
struct Tree
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
}
int main()
{
    int i, N, x, y;
    Tree* root = new Tree;
    cin >> N;
    cin >> x >> y;
    root->data = x;
    TreapNode root_treap = new treap(x, y);

    for(i = 0; i < N-1; i++) {
        cin >> x >> y;
        TreapNode tmp = new treap(x, y);
        add_to_tree(root, x);
        insert(root_treap, tmp);
    }
    cout << max_width_of_treap(root_treap) - max_width_of_treap(root)  << endl;
    free_tree(root_treap);
    free_tree(root);
    return 0;
}
