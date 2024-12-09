#include "header.hpp"

using namespace std;
int main() {
    
    
    functie a;
    a.input="cos(x)";
    cout<< curatareInput(a.input);
    a.calculareOrdinePostfix();
    stackdump(a.sirPostfix);
    a.calcularePuncte(-2,5);
    for(int i=0;i<100;i++)
        cout<<a.valori[i].x<<" ";

    return 0;
}
