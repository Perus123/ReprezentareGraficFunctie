#include "header.hpp"


int main() {
    
    
    functie a;
    a.input="cos(x)^2+sin(x^3/4*5)*ln(e^x)";
    a.calculareOrdinePostfix();
    stackdump(a.sirPostfix);
    return 0;
}
