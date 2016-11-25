/*
 * 1_4. Строки.
Напишите программу, печатающую набор строк в лексикографическом порядке.
Строки разделяются символом перевода строки '\n'. Если последний символ в потоке ввода '\n',
считать, что после него нет пустой строки. Максимальная длина строки 255 символов. Написать свою
функцию сравнения строк.
 Сортировка вставками
 */
#include <iostream>
#define SIZE_STR 270
using namespace std;
void my_strcpy(char *a, const char *b) {
    int i;
    for(i = 0; b[i]; i++) {
        a[i] = b[i];
    }
    a[i] = b[i];
}

struct str {
    char val[SIZE_STR];
    str& operator= (str &obj) {
        my_strcpy(val, obj.val);
        return *this;
    }
};
//1 if a < b
bool my_strcmp(const str &a, const str &b) {
    int i;
    for(i = 0; i < (SIZE_STR - 1) && a.val[i] && b.val[i] && a.val[i] == b.val[i]; i++) {
       ;
    }
    return a.val[i] < b.val[i];
}
template <class T>
void InsertionSort(T* arr, int l, int r, bool (* isLess)(const T&, const T&)) {
    int j = l;
    T tmp;
    for(int i = l+1; i < r; i++) {
        tmp = arr[i];
        //cout << i << " ";
        for(j = i - 1; j >= l; j--) {
            //cout << j << " ";
            if(!isLess(tmp, arr[j])) {
                break;
            }
            arr[j + 1] = arr[j];
        }
        //cout << "j" << j + 1 << endl;
        arr[j + 1] = tmp;
    }
}
/*
void InsertionSort(str* arr, int N)
{
    int j = 0;
    char tmp[SIZE_STR];
    for(int i = 1; i < N; i++) {
        my_strcpy(tmp, arr[i].val);
        //cout << i << " ";
        for(j = i - 1; j >= 0; j--) {
            //cout << j << " ";
            if(my_strcmp(tmp, arr[j].val)) {
                break;
            }
            my_strcpy(arr[j + 1].val, arr[j].val);
        }
        //cout << "j" << j + 1 << endl;
        my_strcpy(arr[j + 1].val, tmp);

    }
}

*/
int main() {
    int N;
    cin >> N;
    str *arr = new str[N];
    for(int i = 0; i < N; i++) {
        cin >> arr[i].val;
    }
    //my_strcpy(arr[0].val, arr[1].val);
    //cout << my_strcmp(arr[0].val, arr[1].val) << endl;
    //cout << my_strcmp(arr[0].val, arr[2].val) << endl;
    //cout << my_strcmp(arr[2].val, arr[3].val) << endl;
    InsertionSort<str>(arr, 0, N, &my_strcmp);
    for(int i = 0; i < N; i++) {
        cout << arr[i].val << endl;
    }
    delete[] arr;
    return 0;
}