#include <string>
#include <iostream>
using namespace std;

enum FontType {normal, fett, kursiv};

class Font {
    public:
        Font (int s = 3) {Size = s;}
        void setSize(int size){
            Size = size;
        };
        int getSize() {return Size;}

    private:
        int Size;
};

class SpecialFont : public Font {
    public:
        FontType t;
        string getType(void){
            if (t = normal) {
                return "normal";
            } else if (t = fett) {
                "fett";
            } else if (t = kursiv){
                return "kursiv";
            } else{ return "fehler";}
        };
};

int main()
{
    SpecialFont f;
    f.t = normal;
    string str = f.getType();
    cout << str;

    return 0;
}
