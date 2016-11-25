/*
 * 6_2. LSD для long long.
Дан массив неотрицательных целых 64-битных чисел. Количество чисел не больше 10^6.
Отсортировать массив методом поразрядной сортировки LSD по байтам.
 */

#include <iostream>
#include <cstring>
using namespace std;

template <class T>
T GetByte(T x, int byte) {
    return (x >> (8 * byte)) & 255;
}

template <class T>
void CountingSort(T* arr, int l, int r, int byte) {
    int k = 256;
    int* count = new int[k];
    for(int i = 0; i < k; ++i) {
        count[i] = 0;
    }
    for(int i = l; i < r; ++i) {
        ++count[GetByte<T>(arr[i], byte)];
    }
    for(int i = 1; i < k; i++) {
        count[i] += count[i-1];
    }
    T* new_arr = new T[r - l];
    for(int i = r - 1; i >= l; --i) {
        new_arr[--count[GetByte<T>(arr[i], byte)] - l] = arr[i];
    }
    delete[] count;
    memcpy(arr + l, new_arr, (r - l) * sizeof(T));
    delete[] new_arr;
}

template <class T>
void LSD(T* arr, int l, int r) {
    for(int i = 0; i < sizeof(T); ++i) {
        CountingSort<T>(arr, l, r, i);
    }
}

int main() {
    int n;
    cin >> n;

    long long *a = new long long [n];

    for(int i = 0; i < n; i++) {
        cin >> a[i];
    }

    LSD<long long>(a, 0, n);

    for(int i = 0; i < n; i++) {
        cout << a[i] << " ";
    }
    delete[] a;

    return 0;
}

