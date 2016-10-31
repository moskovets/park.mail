/*
​Задача 3_4.
​​Дан отсортированный массив различных целых чисел A[0..n­1] и массив целых чисел B[0..m­1].
Для каждого элемента массива B[i] найдите минимальный индекс элемента массива A[k],
ближайшего по значению к B[i]. Время работы поиска для каждого элемента B[i]: O(log(k)).
n ≤ 110000, m ≤ 1000.

*/

#include <iostream>
#include <assert.h>
#include <cmath>
using namespace std;
int  find_index(int* a, int l, int r, int x) {
    int m = l + (r - l) / 2;
    if(m == l) {
        return m;
    }
    if(a[m] > x) {
        return find_index(a, l, m, x);
    } else {
        return find_index(a, m, r, x);
    }
}

int find_index_logk(int*a, int n, int x) {
    int i = 1;
    int j = 0;
    for(i = 1; i < n; i *= 2) {
        if(a[i] >= x && (a[j] <= x || !j)) {
            break;
        }
        j = i;
    }
    if(i >= n) {
        return find_index(a, j, n, x);
    } else {
        return find_index(a, j, i + 1, x);
    }
}

int main()
{
    int n = 0, m = 0;
    cin >> n;
    int* A = new int[n];
    if(!A) {
        return 0;
    }
    for(int i = 0; i < n; ++i) {
        cin >> A[i];
    }
    cin >> m;
    int* B = new int[m];
    if(!B) {
        delete[] A;
        return 0;
    }
    for(int i = 0; i < m; ++i) {
        cin >> B[i];
    }
    for(int i = 0; i < m; ++i) {

        int j = find_index_logk(A, n, B[i]);

        if(j < n-1 && abs(A[j + 1] - B[i]) < abs(A[j] - B[i])) {
            j++;
        }
        cout << j << " ";
    }
    cout << endl;
    delete[] A;
    delete[] B;
    return 0;
}
