/*
6_1.​ Высокая пирамида.
Дано N кубиков. Требуется определить каким количеством способов можно выстроить из этих кубиков пирамиду.
Каждый вышележащий слой пирамиды должен быть не больше нижележащего.
N ≤ 200.
*/

#include <iostream>
using namespace std;

long long piramida(int n) {
    if(n == 0) {
        return 0;
    }
    long long **D = new long long* [n+1];
    if(!D) {
        return -1;
    }
    for(int i = 0; i <= n; i++) {
        D[i] = new long long [n+1];
    }

    for(int i = 0; i <= n; ++i) {
        for(int j = 0; j <= n; ++j) {
            D[i][j] = 0;
        }
        D[i][1] = D[i][i] = 1;
    }
    for(int i = 0; i <= n; ++i) {
        for(int j = 2; j <= n; ++j) {
            D[i][j] = i >= j ? D[i][j - 1] + D[i - j][j] : D[i][i];
        }
    }
    long long res = D[n][n];

    for(int i = 0; i <= n; i++) {
        delete []D[i];
    }
    delete []D;
    return res;
}

int main() {
    int N;
    cin >> N;
    cout << piramida(N) << endl;
    return 0;
}