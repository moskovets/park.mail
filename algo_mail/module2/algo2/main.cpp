/* Задача 2_2. Быстрое сложение
 * Для сложения чисел используется старый компьютер. Время, затрачиваемое на нахождение суммы двух чисел равно их сумме.
Таким образом для нахождения суммы чисел 1,2,3 может потребоваться разное время, в зависимости от порядка вычислений.
((1+2)+3) -> 1+2 + 3+3 = 9
((1+3)+2) -> 1+3 + 4+2 = 10
((2+3)+1) -> 2+3 + 5+1 = 11
Требуется написать программу, которая определяет минимальное время, достаточное для вычисления суммы заданного набора чисел.

    Формат входных данных. Вначале вводится n - количество чисел. Затем вводится n строк - значения
чисел (значение каждого числа не превосходит 10^9, сумма всех чисел не превосходит 2*10^9).
    Формат выходных данных. Натуральное число - минимальное время.
 */
#include <iostream>
//#include <algorithm>
using namespace std;
class CHeap {
private:
    int* arr;
    int n;
    void makeHeap();
public:
    CHeap();
    CHeap(int* a, int len);
    void Add(int elem);
    int Extract();
    bool IsEmpty();
    ~CHeap();
};
bool CHeap::IsEmpty() {
    return n == 0;
}
CHeap::~CHeap() {
    delete[] arr;
}
CHeap::CHeap() {
    arr = NULL;
    n = 0;
}
CHeap::CHeap(int *a, int len) {
    arr = new int[len + 1];
    if(arr) {
        for (int i = 0; i < len; i++) {
            arr[i + 1] = a[i];
        }
        n = len;
        makeHeap();
    }
}
void CHeap::makeHeap() {
    for (int i = n / 2; i >= 1; --i) {
        for (int j = i; j <= n / 2; ) {
            int k = j * 2;
            if (k + 1 <= n and arr[k] > arr[k+1]) {
                k++;
            }
            if (arr[j] > arr[k]) {
                swap(arr[k], arr[j]);
                j = k;
            } else {
                break;
            }
        }
    }
}
void CHeap::Add(int elem) {
    n++;
    arr[n] = elem;
    for (int i = n; i > 0; ) {
        if (arr[i] < arr[i / 2]) {
            swap(arr[i], arr[i/2]);
            i = i / 2;
        } else {
            break;
        }
    }
}
int CHeap::Extract() {
    int tmp = arr[1];
    swap(arr[n], arr[1]);
    n--;
    for (int i = 1; 2 * i <= n;) {
        i *= 2;
        if (i + 1 <= n && arr[i] > arr[i+1]) {
            i++;
        }
        if (arr[i/2] > arr[i]) {
            swap(arr[i/2], arr[i]);
        }
    }
    return tmp;
}
int quick_addition(CHeap &heap) {
    int res = 0;
    int x = 0, y = 0;
    while(!heap.IsEmpty()) {
        x = heap.Extract();
        if(heap.IsEmpty()) {
            break;
        }
        y = heap.Extract();
        res += x + y;
        if(!heap.IsEmpty()) {
            heap.Add(x+y);
        }
    }
    return res;
}
int main() {
    int n;
    cin >> n;
    int* a = new int[n];
    for(int i = 0; i < n; i++) {
        cin >> a[i];
    }
    CHeap heap(a, n);
    cout << quick_addition(heap);
    delete[]a;
    return 0;
}