#include "header.hpp"


int main() {
    
    
    functie a;
    a.input="cos 2^x+ sin 3x + ln  x 7 ln x";
    cout<<curatareInput(a.input)<<' ';
    a.calculareOrdinePostfix();
    stackdump(a.sirPostfix);
    a.calcularePuncte(0,5);
    for(int i=0;i<100;i++)
        cout<<a.valori[i]<<" ";

  
    return 0;
}
