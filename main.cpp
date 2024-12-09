#include "header.hpp"


int main() {
    
    
    functie a;
    a.input="cos(x)+3*x";
    a.calculareOrdinePostfix();
    stackdump(a.sirPostfix);
    executareFunctie(1, a);
    return 0;
}
