#include "header.hpp"


int main() {
    
    
    functie a;
    a.input="2*(x+1)";
    a.calculareOrdinePostfix();
    stackdump(a.sirPostfix);
    a.calcularePuncte(-2,5);
    for(int i=0;i<100;i++)
        cout<<a.valori[i].x<<" "<<a.valori[i].y<<'\n';

    return 0;
}
