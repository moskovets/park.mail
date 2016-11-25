/*
 * 3_4. Закраска прямой 2.
На числовой прямой окрасили N отрезков. Известны координаты левого и правого концов каждого
отрезка (Li и Ri).
Найти сумму длин частей числовой прямой, окрашенных ровно в один слой.
 */
#include <iostream>
using namespace std;
struct Segment
{
    int x;
    bool flag_end; //true if end
};
template <class T>
void Up(T* arr, int i, int n, bool (* isLess)(const T&, const T&))
{
    int p;
    while(i > 0)
    {
        p = (i - 1) / 2;
        if(isLess(arr[i], arr[p])) { return; }
        swap(arr[i], arr[p]);
        i = p;
    }
}
template <class T>
void Down(T* arr, int i, int n, bool (* isLess)(const T&, const T&))
{
    int l = i * 2 + 1;
    int r = i * 2 + 2;
    int m = i;
    if((l < n) && (!isLess(arr[l], arr[m]))) { m = l; }
    if((r < n) && (!isLess(arr[r], arr[m]))) { m = r; }
    if(m != i)
    {
        swap(arr[i], arr[m]);
        Down(arr, m, n, isLess);
    }
}
bool compare_segments(const Segment &a, const Segment &b) {
    return (a.x < b.x);
}
template <class T>
void HeapSort(T* arr, int l, int r, bool (* isLess)(const T&, const T&)) {
    for(int i = l; i <= r; i++) {
        Up(arr, i, i + 1, isLess);
    }
    int j = r;
    while(j > 0)
    {
        swap(arr[0], arr[j-1]);
        j--;
        Down(arr, 0, j, isLess);
    }
}
int segments_union_measure (Segment* arr, int n)
{
    int i;
    int result = 0, level = 0;
    for(i = 0; i < n; i++)
    {
        if(abs(level) == 1) {
            result += (arr[i].x - arr[i - 1].x);
        }
        if(arr[i].flag_end) {
            --level;
        } else {
            ++level;
        }
    }
    return result;
}

int main()
{
//ios_base::sync_with_stdio(false);
    int n;
    cin >> n;
    n *= 2;
    Segment *a = new Segment[n];
    for(int i = 0; i < n; i++) {
        cin >> a[i].x;
        a[i].flag_end = (i % 2 == 1);
    }
    HeapSort<Segment>(a, 0, n, compare_segments);

    /*for(int i = 0; i < n; i++) {
        cout << a[i].x << " " << a[i].flag_end << endl;
    }*/
    cout << segments_union_measure(a, n);
    delete[] a;
    return 0;
}