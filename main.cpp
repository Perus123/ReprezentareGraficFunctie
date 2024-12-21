#include "header.hpp"

using namespace std;
int main()
{

    functie a;
    vector<functie> f;
    a.input = "x*(sin(1/x))";
    a.calculareOrdinePostfix();
    grafic principal(1200, 800);
    cout << principal.inaltimeEcran << " " << principal.latimeEcran;
    f.push_back(a);
    principal.initializareGrafic(f);

    return 0;
}