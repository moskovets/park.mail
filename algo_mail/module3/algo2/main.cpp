/*Задача 3_2
 * Дано число N < 106 и последовательность целых чисел из [-231..231] длиной N.
Требуется построить бинарное дерево, заданное наивным порядком вставки.
Т.е., при добавлении очередного числа K в дерево с корнем root, если root→Key ≤ K, то
узел K добавляется в правое поддерево root; иначе в левое поддерево root.
Рекурсия запрещена.
 */
#include <stdlib.h>
#include <iostream>
using namespace std;
template <typename T>
struct element {
    T value;
    element *left = NULL;
    element *right = NULL;
    element(T x) {
        value = x;
        left = NULL;
        right = NULL;
    }
};
//Binary search tree
template <typename T>
class CBST {
private:
    element<T> *head = NULL;
    void delete_tree(element<T>* tmp);
    void operator_copy(element<T> **head, element<T> *tmp);
    void LRRoot(element<T> *tmp);
    void delete_remove(element<T>* prev, element<T> *tmp);
public:
    CBST(const CBST &obj);
    CBST();
    CBST<T>& operator=(const CBST& obj);
    void Insert(T x);
    bool Remove(T x);
    ~CBST();
    void LeftRightRoot();

};
void log(const char *str)
{
    ;  //cout << endl << "l: " << str << endl;
}

template <typename T>
CBST<T>& CBST<T>::operator=(const CBST<T> &obj) {
    log("=");
    operator_copy(&head, obj.head);
}
template <typename T>
void CBST<T>::operator_copy(element<T> **head, element<T> *tmp) {
    element<T> *a = *head;
    if(tmp) {
        a = new element<T>(tmp->value);
        operator_copy(&a->left, tmp->left);
        operator_copy(&a->right, tmp->right);
    }
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
            if(tmp->right) {
                tmp = tmp->right;
            }
            else {
                tmp->right = new_elem;
                break;
            }
        }
        else {
            if(tmp->left) {
                tmp = tmp->left;
            }
            else {
                tmp->left = new_elem;
                break;
            }
        }
    }
}
template <typename T>
void CBST<T>::delete_remove(element<T>* prev, element<T> *tmp) {
    element<T> *b = NULL;
    int flag = 0;
    if((!tmp->left) && (!tmp->right)) {
        b = NULL;
        flag = 1;
    }
    if(tmp->left && !tmp->right) {
        b = tmp->left;
        flag = 1;
    }
    if(!tmp->left && tmp->right) {
        b = tmp->right;
        flag = 1;
    }
    if(flag) {
        if(prev == NULL || prev == tmp) {
            head = b;
        }
        if(prev->left == tmp) {
            prev->left = b;
        }
        else if(prev->right == tmp){
            prev->right = b;
        }
        delete tmp;
        return;
    }
    element<T>* a = tmp->right;
    element<T>* new_elem = tmp;
    while(a) {
        if(a->left) {
            new_elem = a;
            a = a->left;
        }
        else {
            break;
        }
    }
    tmp->value = a->value;
    if(new_elem->left == a) {
        new_elem->left = a->right;
    }
    else {
        new_elem->right = a->right;
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
            if(tmp->right) {
                tmp = tmp->right;
            }
            else {
                return false;
            }
        }
        else {
            if(tmp->left) {
                tmp = tmp->left;
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
        delete_tree(tmp->left);
        delete_tree(tmp->right);
        delete tmp;
        tmp = NULL;
    }
}
template <typename T>
CBST<T>::CBST(const CBST &obj) {
    log("copy");
    if(&obj != this) {
        *this = obj;
    }
}
template <typename T>
CBST<T>::CBST() {
    head = NULL;
}
template <typename T>
void CBST<T>::LRRoot(element<T> *tmp) {
    if (tmp) {
        LRRoot(tmp->left);
        LRRoot(tmp->right);
        cout << tmp->value << " ";
    }
}
template <typename T>
void CBST<T>::LeftRightRoot() {
    LRRoot(head);
    cout << endl;
}

int main() {
    CBST <int> tree;
    int N, x;
    cin >> N;
    for(int i = 0; i < N; i++) {
        cin >> x;
        tree.Insert(x);
    }
    tree.LeftRightRoot();
}
