/*Задача 1_1.
 * Реализуйте структуру данных типа “множество строк” на основе динамической
хеш-таблицы с открытой адресацией. Хранимые строки непустые и состоят из строчных латинских букв.
Хеш-функция строки должна быть реализована с помощью вычисления значения многочлена методом Горнера.
Начальный размер таблицы должен быть равным 8-ми. Перехеширование выполняйте
при добавлении элементов в случае, когда коэффициент заполнения таблицы достигает 3/4.
Структура данных должна поддерживать операции добавления строки в множество,
удаления строки из множества и проверки принадлежности данной строки множеству.

1_2.​ Для разрешения коллизий используйте двойное хеширование.

 */
#include <iostream>
#include <queue>
#include <assert.h>

#define H1 13
#define H2 7
using namespace std;

unsigned int Hash(string x, int p) {
    unsigned int h = 1;
    for(int i = 0; i < x.size(); ++i) {
        h *= p;
        h += (unsigned int) (x[i] - '\0');
    }
    return h;
}

template <class T>
class HashTable
{
private:
    T* table;
    short int* status; // 0 - pusto, 1 - is, 2 - del
    int tableSize;
    int count_of_elem;
public:
    void print() {
        for(int i = 0; i < tableSize; i++) {
            cout << status[i] << " " << table[i] << endl;
        }
    }
    HashTable(int Start_size) {
        tableSize = Start_size;
        table = new T [tableSize];
        status = new short int [tableSize]; // 0 - pusto, 1 - is, 2 - del
        for(int i = 0; i < tableSize; ++i) {
            status[i] = 0;
        }
        count_of_elem = 0;
    }
    ~HashTable() {
        delete[] table;
        delete[] status;
    }
    void New_Table(int new_size) {
        assert(new_size > tableSize);
        queue <T> A;
        for(int i = 0; i < tableSize; ++i) {
            if(status[i] == 1) {
                A.push(table[i]);
            }
        }
        delete[] table;
        delete[] status;
        table = new T [new_size];
        status = new short int [new_size]; // 0 - pusto, 1 - is, 2 - del
        tableSize = new_size;
        for(int i = 0; i < new_size; ++i) {
            status[i] = 0;
        }
        count_of_elem = 0;
        while(!A.empty()) {
            Insert(A.front());
            A.pop();
        }
    }
    bool Insert(T k) {
        unsigned int h1 = Hash(k, H1);
        unsigned int h2 = Hash(k, H2) * 2 + 1;

        int insert_place = -1;

        for(unsigned int i = 0; i < tableSize; ++i) {
            unsigned int j = (h1 + i * h2) % tableSize;

            if(status[j] == 1 && table[j] == k) {
                return false;
            }
            if(status[j] == 2 && insert_place == (-1)) {
                insert_place = j;
            }
            if(status[j] == 0) {
                if(insert_place == -1) {
                    insert_place = j;
                }
                break;
            }
        }
        if(insert_place >= 0) {
            table[insert_place] = k;
            count_of_elem++;
            status[insert_place] = 1;
            if(4 * count_of_elem > 3 * tableSize) {
                New_Table(tableSize * 2);
            }
            return true;
        }
        return false;
    }
    bool Search_element(T x) {
        unsigned int h1 = Hash(x, H1);
        unsigned int h2 = Hash(x, H2) * 2 + 1;

        for(int i = 0; i < tableSize; ++i) {
            unsigned int j = (h1 + i * h2) % tableSize;
            if(status[j] == 0) {
                return false;
            }
            if(status[j] == 1 && table[j] == x) {
                return true;
            }
        }
        return false;
    }
    bool Delete_element(T x) {
        unsigned int h1 = Hash(x, H1);
        unsigned int h2 = Hash(x, H2) * 2 + 1;

        for(int i = 0; i < tableSize; ++i) {
            unsigned int j = (h1 + i * h2) % tableSize;
            if((status[j] == 1) && (table[j] == x)) {
                status[j] = 2;
                count_of_elem--;
                return true;
            }
            if(status[j] == 0) {
                return false;
            }
        }
        return false;
    }
};

int main() {
    char ch;
    string s;
    HashTable <string> Table(8);
    while(cin >> ch) {
        cin >> s;
        switch(ch) {
            case '+':
                cout << (Table.Insert(s) ? "OK" : "FAIL") << endl;
                break;
            case '-':
                cout << (Table.Delete_element(s) ? "OK" : "FAIL") << endl;
                break;
            case '?':
                cout << (Table.Search_element(s) ? "OK" : "FAIL") << endl;
                break;
        }
    }
    return 0;
}
