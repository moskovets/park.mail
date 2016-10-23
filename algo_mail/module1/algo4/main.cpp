/* Задача 4_2
 * ​Реализовать дек с динамическим зацикленным буфером.
 */
#include <iostream>
#include <queue>
#define START_SIZE 10
#define DATA 50
#define Len_of_word 50
using namespace std;
template <typename T>
class CDeque
{
private:
    T* arr = NULL;
    int back;
    int front;
    int max_size;
    void increase();
public:
    CDeque();
    CDeque(const CDeque &);
    ~CDeque();
    T PopBack();
    T PopFront();
    void PushBack(T x);
    void PushFront(T x);
    bool Empty_CDeque();
    CDeque& operator= (const CDeque &obj);
    T* get_arr();
    void show();
};
template <typename T>
CDeque<T>::CDeque()
{
    //cout << "const" << endl;
    max_size = START_SIZE;
    back = front = 0;
    arr = new T[max_size];
}
template <typename T>
CDeque<T>::CDeque(const CDeque &obj)
{
    //cout << "copy" << endl;
    max_size = obj.max_size;
    front = obj.front;
    back = obj.back;
    if(arr) {
        delete[] arr;
        arr = NULL;
    }
    arr = new T[obj.max_size];  //Выделяю память для элемента объекта
    for (int i = 0; i < max_size; i++) {
        arr[i] = obj.arr[i]; //Поэлементно копирую каждый элемент из принимаемого объекта в текущий
    }
    max_size=obj.max_size;
}
template <typename T>
CDeque<T>& CDeque<T>::operator=(const CDeque &obj)
{
    //cout << "=";
    if(this != &obj) {
        max_size = obj.max_size;
        front = obj.front;
        back = obj.back;
        if(arr) delete[] arr;
        arr = new T[obj.max_size];  //Выделяю память для элемента объекта
        for (int i = 0; i < max_size; i++) {
            arr[i] = obj.arr[i]; //Поэлементно копирую каждый элемент из принимаемого объекта в текущий
        }
        max_size=obj.max_size;
    }
    return *this;
}
template <typename T>
CDeque<T>::~CDeque() {
    //cout << "~ " << endl;
    if(arr) {
        //cout << "del";
        delete[] arr;
    }
}
template <typename T>
void CDeque<T>::increase() { //TODO
    //cout << "in" << endl;
    T* arr_copy = new T[max_size * 2];
    int j = 0;
    int i = back;
    while(i != front) {
        arr_copy[j] = arr[i];
        j++;
        i++;
        i %= max_size;
    }
    back = 0;
    front = j;
    delete[] arr;
    arr = arr_copy;
    arr_copy = NULL;
    max_size *= 2;
}
template <typename T>
void CDeque<T>::PushFront(T x) {
    if((front+1) % max_size == back) {
        increase();
    }
    arr[front] = x;
    front++;
    front %= max_size;
}
template <typename T>
void CDeque<T>::PushBack(T x) {
    if(front == (back - 1 + max_size) % max_size) {
        increase();
    }
    back--;
    back += max_size;
    back %= max_size;
    arr[back] = x;
}
template <typename T>
T CDeque<T>::PopFront() {
    T x;
    if(back == front) { return 0; }
    front--;
    front += max_size;
    front %= max_size;
    x = arr[front];
    return x;
}
template <typename T>
T CDeque<T>::PopBack() {
    T x;
    if(back == front) { return 0; }
    x = arr[back];
    back++;
    back %= max_size;
    return x;
}

template <typename T>
bool CDeque<T>::Empty_CDeque()
{
    return back == front;
}
template <typename T>
T* CDeque<T>::get_arr()
{
    return arr;
}

int main()
{
    int i, N;
    int x, y, flag;
    cin >> N;
    flag = 0;
    CDeque <int> A;
    for(i = 0; i < N; i++)
    {
        cin >> x >> y;
        switch(x) {
            case 1:
                A.PushFront(y);
                break;
            case 2:
                if(A.Empty_CDeque()) {
                    if(y != (-1)) { flag = 1; }
                    break;
                }
                if(y != A.PopFront()) { flag = 1; }
                break;
            case 3:
                A.PushBack(y);
                break;
            case 4:
                if(A.Empty_CDeque()) {
                    if(y != (-1)) { flag = 1; }
                    break;
                }
                if(y != A.PopBack()) { flag = 1; }
                break;
        }
    }
    if(flag == 0) {
        cout << "YES";
    }
    else {
        cout << "NO";
    }
    return 0;
}
