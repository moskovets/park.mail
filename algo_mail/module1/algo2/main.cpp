/*
​Задача 2_1.
​Даны два массива целых чисел одинаковой длины A[0..n­1] и B[0..n­1].
Необходимо найти первую пару индексов i0 и j0, i0 ≤ j0, такую что
A[i0] + B[j0] = max {A[i] + B[j], где 0 <= i < n, 0 <= j < n, i <= j}.
Время работы ­O(n). n ≤ 100000.
*/



#include <iostream>
#include <assert.h>
#include <stdio.h>
using namespace std;

struct CPair_index {
    int i;
    int j;
};
CPair_index find_index(int n, int* A, int* B) {
    assert(A && B);
    CPair_index res;
    res.i = n - 1;
    res.j = n - 1;

    int dj = n - 1;
    for(int i = n - 1; i >= 0; --i) {
        if(A[i] >= A[res.i]) {
            res.i = i;
        }
        if(B[i] >= B[dj]) {
            if(res.i == i) {
                res.j = i;
                dj = i;
            } else {
                dj = i;
            }
        }
        if(B[dj] - B[res.j] >= A[res.i] - A[i]) {
            res.i = i;
            res.j = dj;
        }
    }
    return res;
}

int main()
{
    int n = 0;
    cin >> n;
    int* A = new int[n];
    if(!A) {
        return 0;
    }
    int* B = new int[n];
    if(!B) {
        delete[] A;
        return 0;
    }
    for(int i = 0; i < n; ++i) {
        cin >> A[i];
    }
    for(int i = 0; i < n; ++i) {
        cin >> B[i];
    }
    CPair_index res = find_index(n, A, B);
    cout << res.i << " " << res.j;

    delete[] A;
    delete[] B;
    return 0;
}
