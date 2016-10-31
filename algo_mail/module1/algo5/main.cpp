/*
 * 5_1. Скобочная последовательность.
Дан фрагмент последовательности скобок, состоящей из символов (){}[].
Требуется определить, возможно ли продолжить фрагмент в обе стороны, получив корректную последовательность.
Длина исходной последовательности ≤ 200000.
Формат входных данных. Строка, содержащая символы (){}[] и, возможно, перевод строки.
Формат выходных данных. Если возможно ­ вывести минимальную корректную последовательность, иначе ­
напечатать "IMPOSSIBLE​".
 */
#include <iostream>
#include <deque>
#define START_SIZE 1000
using namespace std;
template <typename T>
class Stack {
private:
    T* arr;
    int top;
    int max_size;
    void increase();
public:
    Stack();
    Stack(const Stack &);
    ~Stack();
    T pop();
    void push(T x);
    bool empty_stack();
    T* get_arr();
    void show();
};
template <typename T>
Stack<T>::Stack()
{
    max_size = START_SIZE;
    top = 0;
    arr = new T[max_size]; //(int*) malloc(max_size * sizeof(int));
}
template <typename T>
Stack<T>::Stack(const Stack &obj)
{
    max_size = obj.max_size;
    top = obj.top;
    if(arr) delete[] arr;
    arr = new T[obj.max_size];  //Выделяю память для элемента объекта
    for (int i = 0; i < max_size; i++) {
        arr[i] = obj.arr[i]; //Поэлементно копирую каждый элемент из принимаемого объекта в текущий
    }
    max_size=obj.max_size;
}
template <typename T>
Stack<T>::~Stack()
{
    delete[] arr;
    //cout << "~ " << endl;
}
template <typename T>
void Stack<T>::increase()
{
    T* arr_copy = new T[max_size * 2];
    for (int i = 0; i < max_size; i++) {
        arr_copy[i] = arr[i];
    }
    delete[] arr;
    max_size *= 2;
}
template <typename T>
void Stack<T>::push(T x)
{
    arr[top] = x;
    top++;
    if(top >= max_size) { increase(); }
}
template <typename T>
T Stack<T>::pop()
{
    T x;
    if(top == 0) { return 0; }
    x = arr[top - 1];
    top--;
    return x;
}
template <typename T>
bool Stack<T>::empty_stack()
{
    return !top;
}
template <typename T>
T* Stack<T>::get_arr()
{
    return arr;
}
template <typename T>
void Stack<T>::show()
{
    int i;
    if(top == 0) { cout << "Stack is empty!"; }
    for(i = 0; i < top; i++) { cout << arr[i] << " "; }
    cout << endl;
}
char pair_skobka(char ch) {
    switch(ch) {
        case '(': return ')';
        case '[': return ']';
        case '{': return '}';
        case ')': return '(';
        case ']': return '[';
        case '}': return '{';
    }
}

int main()
{
    int i, j, N, M;
    int x, y, flag;
    char ch;
    Stack <char> A;
    deque <char> Res;
    string str;
    cin >> str;
    flag = 0;
    for(i = 0; i < str.size(); i++) {
        ch = str[i];
        switch (ch) {
            case '(':
            case '{':
            case '[':
                A.push(ch);
                Res.push_front(ch);
                break;
            case ')':
            case ']':
            case '}':
                if (A.empty_stack()) {
                    Res.push_back(pair_skobka(ch));
                    Res.push_front(ch);
                    break;
                }
                if (A.pop() != pair_skobka(ch)) {
                    flag = 1;
                }
                Res.push_front(ch);
                break;
        }
        if (flag == 1) { break; }
    }
    while((!A.empty_stack()) && (flag == 0)) {
        ch = A.pop();
        switch(ch) {
            case '{':
            case '[':
            case '(':
                Res.push_front(pair_skobka(ch));
                break;
            default: flag = 1; break;
        }
    }
    if(flag) { cout << "IMPOSSIBLE" << endl; }
    else {
        while(!Res.empty()) {
            cout << Res.back();
            Res.pop_back();
        }
        cout << endl;
    }
    return 0;
}
