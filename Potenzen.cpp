// eberechnet für eingegebene Zahl Potenzen -> Array

#include <iostream>
using namespace std;

int i, n;

// Def 10-elem Vektor vom Typ Integer
int v[10];

// Def Funk b_hoch_n

int zahl_hoch_n(int b, int n){
    int ergebnis = 1;
    while (n > 0){
        ergebnis = ergebnis * b;
        n = n-1;
    }
    return ergebnis;
}

main () {
    i = 0;
    cout <<"Geben sie eine Zahl ein: \n";
    cin >> n;
    while (i < 10){
        v[i] = zahl_hoch_n(i+2, n);
    i = i+1;
    }
    cout << "\nDie Ergebnisse sind:" << endl;
    i  = 0;
    while (i<10){
        cout << (i+2) << " hoch " << n << " = " << v[i] << endl;
        i = i+1;
    }
}