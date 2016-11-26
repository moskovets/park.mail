/*
 * 4_4.​ Реализуйте стратегию выбора опорного элемента “случайный элемент”.
 * Функцию Partition реализуйте методом прохода двумя итераторами от конца массива к началу.
 */
#include <iostream>
using namespace std;

template <class T>
int Partition(T *a, int n, int pivot, bool (* isLess)(const T&, const T&)) {
    swap(a[0], a[pivot]);
    int j = n - 2;
    int i = n - 1;
    while(j > 0 && isLess(a[0], a[i])) { i--; j--; }
    while(j > 0) {
        if(isLess(a[j], a[0])) {
            j--;
        } else {
            swap(a[i], a[j]);
            i--;
            j--;
        }
    }
    if(isLess(a[i], a[0])) {
        swap(a[i], a[0]);
    } else {
        i = 0;
    }
    return i;
}
template <class T>
T K_statictik(T* a, int l, int r, int k, bool (* isLess)(const T&, const T&)) {
    int n = r - l;
    int tmp = -1;
    while(tmp != k) {
        int pivot = l + rand() % (r - l);
        tmp = Partition(a, n, pivot, isLess);
        if(tmp > k) {
            r = tmp;
        } else {
            l = tmp;
        }
    }
    return a[k];
}
bool compare_int(const int &a, const int &b) {
    return a < b;
}

int main()
{
    srand(time(0));
    int n, k;
    cin >> n >> k;
    int *a = new int[n];
    for(int i = 0; i < n; i++) {
        cin >> a[i];
    }
    cout << K_statictik<int>(a, 0, n, k, compare_int);
    delete[] a;
    return 0;
}
