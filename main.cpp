#include "header.hpp"

using namespace std;
int main()
{

    function a;
    vector<function> f;
    a.input = "x*(sin(1/x))";
    a.postfixOrderCalculation();
    grafic principal(1200,800);
    cout << principal.screenHeight << " " << principal.screenWidth;
    f.push_back(a);
    principal.initialiseGraphic(f);

    return 0;
}