/*
 * 5_2. Сортировка почти упорядоченной последовательности.
Дана последовательность целых чисел a1...an и натуральное число k, такое что для любых i, j: если
j >= i + k, то a[i] <= a[j]. Требуется отсортировать последовательность. Последовательность может
быть очень длинной. Время работы O(n * log(k)). Доп. память O(k). Использовать слияние.
 */
#include <iostream>
#include <cstring>

using namespace std;

template <class T>
void Merge(T* a, int Na, T* b, int Nb, T* buf, bool (* isLess)(const T&, const T&)) {
    int i = 0, j = 0;
    while((i < Na) && (j < Nb)) {
        if(isLess(a[i],b[j])) {
            buf[i + j] = a[i];
            i++;
        } else {
            buf[i + j] = b[j];
            j++;
        }
    }
    while(j < Nb) {
        buf[i + j] = b[j];
        j++;
    }
    while(i < Na) {
        buf[i + j] = a[i];
        i++;
    }
}

bool compare_int(const int &a, const int &b) {
    return a < b;
}

template <class T>
void MergeSort(T* arr, int l, int r, T* buf, bool (* isLess)(const T&, const T&)) {
    int mid;
    if((r - l) <= 1) { return; }
    mid = (r - l) / 2 + l;
    MergeSort<T>(arr, l, mid, buf, isLess);
    MergeSort<T>(arr, mid, r, buf, isLess);

    Merge<T>(arr + l, mid - l, arr+mid, r - mid, buf, isLess);

    memmove(arr+l, buf, sizeof(T) * (r - l));
}

int main() {
    int n, k;
    cin >> n >> k;
    int *a = new int[3 * k];
    int end = min(n, k);
    for(int i = 0; i < min(n, k); i++) {
        cin >> a[i+k];
    }
    MergeSort<int>(a+k, 0, end, a, compare_int);

    for(int i = k; i < n; i += k) {
        end = min(n, i + k);
        for(int j = i; j < end; j++) {
            cin >> a[j - i + 2 * k];
        }
        MergeSort<int>(a+2*k, 0, end - i, a, compare_int);
        Merge<int>(a+k, k, a+2*k, end - i, a, compare_int);
        for(int j = 0; j < k; j++) {
            cout << a[j] << " ";
        }
    }
    end = n % k;
    if(end == 0) {
        end += k;
    }
    for(int j = 0; j < end; j++) {
        cout << a[j+k] << " ";
    }

    delete[] a;
    return 0;
}
