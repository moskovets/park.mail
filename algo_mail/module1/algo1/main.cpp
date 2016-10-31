/*
Задача 1_2.
​Вывести разложение натурального числа n на простые множители. Простые множители должны быть
упорядочены по возрастанию и разделены пробелами.
2 ≤ n ≤ 10^6
*/


#include <iostream>
#include <assert.h>
#include <stdio.h>
#define DEL_COUNT 100
using namespace std;


int* decomposition(int n) {
    assert(n > 1);
    int* a;
    int i = 0;
    int del = 2;

    a = (int*)malloc(DEL_COUNT * sizeof(int));
    if(a == NULL)
        return a;

    while(del <= n) {
        if(n % del == 0) {
            a[i++] = del;
            n /= del;
        }
        else
            del++;
    }
    a[i] = 0;
    return a;
}

int main()
{
    int n;
    cin >> n;
    int* arr = decomposition(n);
    if(arr == NULL) {
        return 0;
    }
    int i = 0;
    while(arr[i] != 0)
        cout << arr[i++] << " ";
    free(arr);
    return 0;
}