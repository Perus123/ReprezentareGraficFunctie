#ifndef HEADER_H
#define HEADER_H
#pragma once
#include <iostream>
#include <string>
#include <stack>
#include <vector>
#include <cmath>
#include <SFML/Graphics.hpp>
using namespace std;
bool isoperator(char c);
int prioritateOperator(char c);
int prioritateOperator(const string &c);
void stackdump(vector<string> st);
struct punct
{
   double x=0,y=0;
};
class functie
{
public:
   string input;
   vector<string> sirPostfix;
   /// inputul trebuie prelucrat in REVERSE POLISH
   double delta;
   vector<punct> valori;
   functie();
   friend double executareFunctie(double valoare, const functie& a);
   void calcularePuncte(double start, double end);
   void calculareOrdinePostfix();
   void prelucrareInput();
   /// cos(x)^2+x^3/5*2+sin(4*x/5)
   /// 2*3 + (8-3)/2
};
class grafic
{
   public:
   punct centru;
   double capatStanga, capatDreapta;
   const int numarPuncte=2000;
   double delta,diviziune;
   double latimeEcran, inaltimeEcran;
   double capatSus, capatJos;
   grafic(double screenWidth, double screenHeight);
   void calculareDeltasiDivizune();
   void initializareGrafic();

};

bool estefunctie(const string& s);
bool isoperand(char c);
void curatareInput(string& s);
#endif