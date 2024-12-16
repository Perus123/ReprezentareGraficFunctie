#include "header.hpp"

using namespace std;
int main() {
    
    
    functie a;
    a.input="x^2";
    a.calculareOrdinePostfix();
    stackdump(a.sirPostfix);
    a.calcularePuncte(2,3);
    for(int i=0;i<=100;i++)
        cout<<a.valori[i].x<<" "<<a.valori[i].y<<"\n";
    grafic principal(800, 600);
    principal.initializareGrafic();

    return 0;
}