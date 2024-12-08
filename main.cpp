#include "header.hpp"


int main() {
    
    
    functie a;
    a.input="2*3+(8-3)/2";
    a.calculareOrdinePostfix();
    stackdump(a.sirPostfix);
    return 0;
}
