#include "header.hpp"

using namespace std;
int main() {
    
    
    functie a;
    vector<functie>f;
    a.input="-4";
    a.calculareOrdinePostfix();
    stackdump(a.sirPostfix);
    a.calcularePuncte(-10,10);
    grafic principal(1200, 800);
    cout<<principal.inaltimeEcran<<" "<<principal.latimeEcran;
    f.push_back(a);
    principal.initializareGrafic(f);

    return 0;
}