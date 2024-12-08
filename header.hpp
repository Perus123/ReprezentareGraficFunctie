#ifndef HEADER_H
#define HEADER_H

#include <iostream> 
#include <string>
#include <stack>
#include <vector>
using namespace std;
vector<float> calcularePuncte(stack<string> stiva, float delta, float start, float end);
class functie
{
   string input;
   stack<string> ordineaOperatiilor;
   ///inputul trebuie prelucrat in REVERSE POLISH 
   float delta;
   vector<float> valori;
};

#endif 