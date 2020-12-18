#include <iostream>

using namespace std;

//вариант 7

template<typename T>
int schetchik(const T *mas, int n, T x){
    int counter = 0;
    for(int i = 0; i < n; i++){
        if(mas[i] == x)
            counter++;
    }
    return counter;
}

template<>
int schetchik(const char *mas, int n, char x){
    int counter = 0;
    for(int i = 0; i < n; i++){
        if(mas[i] == x)
            counter++;
    }
    return counter;
}

int main() {
    char mas1[] = {'a', 'b', 'a', 'd', 'a'};
    cout << schetchik(mas1, 5, 'a') << endl;

    int mas2[] = {5, 8, 6, 6, 6};
    cout << schetchik(mas2, 5, 6) << endl;
    return 0;
    }
