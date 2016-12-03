#include <iostream>
#include <algorithm>
#include <stack>
#include <queue>
#include <vector>
#include <cstring>
using namespace std;
struct AVL_tree
{
    AVL_tree* l, *r;
    int key;
    int h;
    int cnt;
    AVL_tree() {l = r = NULL; h = cnt = 0; }
    AVL_tree(int x) { l = r = NULL; key = x; h = cnt = 1; }
};
int height(AVL_tree *t) {
    return t == NULL ? 0 : t->h;
}
int difference_of_h(AVL_tree *t) {
    return height(t->r) - height(t->l);
}
int cnt_value(AVL_tree * t) {
    return t == NULL ? 0 : t->cnt;
}
int key_value(AVL_tree * t) {
    return t == NULL ? (-2) : t->key;
}
void right(AVL_tree * &t) {
    AVL_tree* tmp = t->l;
    int a, b, c;
    a = cnt_value(t->r);
    b = cnt_value(tmp->l);
    c = cnt_value(tmp->r);
    t->l = tmp->r;
    tmp->r = t;
    t->h = max(height(t->l), height(t->r)) + 1;
    t->cnt = a + c + 1;
    tmp->h = max(height(tmp->l), height(tmp->r)) + 1;
    //tmp->cnt = cnt_value(tmp->l) + cnt_value(tmp->r) + 1;
    tmp->cnt = a + b + c + 2;
    t = tmp;
}
/*AVL_tree* right(AVL_tree* p) // правый поворот вокруг p
{
	AVL_tree* q = p->l;
	p->l = q->r;
	q->r = p;
    p->h = max(height(p->l), height(p->r)) + 1;
    q->h = max(height(q->l), height(q->r)) + 1;
	return q;
}
AVL_tree* left(AVL_tree* q) // левый поворот вокруг q
{
	AVL_tree* p = q->r;
	q->r = p->l;
	p->l = q;
    q->h = max(height(q->l), height(q->r)) + 1;
    p->h = max(height(p->l), height(p->r)) + 1;
	return p;
}
*/
void left(AVL_tree * &t)
{
    AVL_tree* tmp = t->r;
    int a, b, c;
    a = cnt_value(t->l);
    b = cnt_value(tmp->l);
    c = cnt_value(tmp->r);
    t->r = tmp->l;
    tmp->l = t;
    t->h = max(height(t->l), height(t->r)) + 1;
    t->cnt = a + b + 1;
    tmp->h = max(height(tmp->l), height(tmp->r)) + 1;
    //tmp->cnt = cnt_value(tmp->l) + cnt_value(tmp->r) + 1;
    tmp->cnt = a + b + c + 2;

    t = tmp;
}

void balance(AVL_tree* &t) // балансировка узла p
{
    t->h = max(height(t->l), height(t->r)) + 1;
    t->cnt = cnt_value(t->l) + cnt_value(t->r) + 1;
    if(difference_of_h(t) == 2)
    {
        if(difference_of_h(t->r) < 0)
            right(t->r);
        return left(t);
    }
    if(difference_of_h(t) == (-2))
    {
        if(difference_of_h(t->l) > 0)
            left(t->l);
        return right(t);
    }
}
void add_to_avl(AVL_tree* &t, AVL_tree* tmp)
{
    if(t == NULL) {
        t = tmp;
        return;
    }
    if(t->key <= tmp->key) {
        add_to_avl(t->r, tmp);
    }
    else {
        add_to_avl(t->l, tmp);
    }
    balance(t);
    t->cnt++;
    //cout << t->key << " " << t->cnt << " " << cnt_value(t->l) << endl;
    //AVL_tree* tl = t->l;
    //AVL_tree* tr = t->r;
    //if(tl != NULL) { t->cnt += tl->cnt; }
    //if(tr != NULL) { t->cnt += tr->cnt; }
    //t->cnt = tl->cnt + tr->cnt;
}
AVL_tree* findmin(AVL_tree* t) {
    return t->l == NULL ? t : findmin(t->l);
}
AVL_tree* removemin(AVL_tree* t) {
    if( t->l == NULL)
        return t->r;
    t->l = removemin(t->l);
    balance(t);
    return t;
}
void delete_from_avl(AVL_tree* &t, int x) {
    if(t == NULL) { return; }
    if(x < t->key) {
        delete_from_avl(t->l, x);
    } else if(x > t->key) {
        delete_from_avl(t->r, x);
    } else {
        AVL_tree* i = t->l;
        AVL_tree* j = t->r;
        delete t;
        t = NULL;
        if(j == NULL) { t = i; return; }
        AVL_tree* m = findmin(j);
        m->r = removemin(j);
        m->l = i;
        balance(m);
        t = m;
    }
    t->cnt--;
    balance(t);
}
int index(AVL_tree* t, int x) {
    if(t == NULL) { return 0; }
    //cout << t->key << " " << t->cnt << "  " << key_value(t->l)
    //<< " " << key_value(t->r) << endl;
    if(x < t->key) { return cnt_value(t->r) + 1 + index(t->l, x); }
    if(x > t->key) { return index(t->r, x); }
    //if(t->l == NULL) { return 0; }
    //AVL_tree* tmp = t->l;
    return cnt_value(t->r) + 1;
}
int value_by_index(AVL_tree* t, int ind)
{
    //AVL_tree* tl = t->l;
    //AVL_tree* tr = t->r;
    if(t == NULL) { return (-1); }
    int tmp = cnt_value(t->l) + 1;
    if(tmp < ind) {
        return value_by_index(t->l, ind - tmp);
    }
    if(tmp > ind) {
        return value_by_index(t->r, ind);
    }
    return t->key;
}
int main()
{
    int i, N, x, k;
    cin >> N;
    x = -5;
    while((x < 0) && (N >= 0)) { cin >> x >> k; N--; }
    AVL_tree *root_avl = new AVL_tree(x);
    if(k == 0) {
        cout << x << endl;
    }
    for(i = 0; i < N; i++)
    {
        cin >> x >> k;
        if(x > 0)
        {
            AVL_tree * tmp = new AVL_tree(x);
            add_to_avl(root_avl, tmp);
            cout << value_by_index(root_avl, k+1) << endl;
        }
        else
        {
            //cout << y << " ";
            delete_from_avl(root_avl, -x);
            cout << value_by_index(root_avl, k+1) << endl;
        }
    }
//cout << root_avl->h << endl;
//inp.close();
//out.close();
    return 0;
}
