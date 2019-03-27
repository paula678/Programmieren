// Deklaration

class Font {
    public:
        Font (int s = 12) {Size = s;}
        void setSize(int size); // Methode, nur Deklaration
        int getSize(void){
            return Size;    // Methode
        };
    
    private:
        int Size; //Attribut
};

//Implementierung der Funktion setSize
void Font::setSize(int size) {
    Size = size;
};

//Zugriff
#include <iostream>
using namespace std;

int main() {
    int d;

    Font font2;
    //Font *fz2;
    //fz2 = &font2;
    d = font2.getSize();
    cout << d << endl; 

    return 0;
}

