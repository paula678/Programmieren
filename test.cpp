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
    Font meinFont; // Deklarieren eines Objektes vom Typ Font
    Font *meinFontZeiger; // Deklaration eines Pointers auf FOnt
    int s;
    int d;

    meinFontZeiger = &meinFont;
    meinFont.setSize(10);   // Nachricht
    s = meinFont.getSize();
    cout << s << "\n";

    Font font1;
    Font *fz1;
    fz1 = &font1;
    fz1 -> setSize(3);
    d = font1.getSize();
    cout << d << endl;

    Font font2;
    Font *fz2;
    fz1 = &font1;
    d = font2.getSize();
    cout << d << endl; 

    return 0;
}

