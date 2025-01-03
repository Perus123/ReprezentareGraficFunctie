#ifndef HEADER_H
#define HEADER_H
#pragma once
#include <iostream>
#include <string>
#include <stack>
#include <vector>
#include <cmath>
#include <SFML/Graphics.hpp>
#include <unordered_map>
using namespace std;
double extern zoomLevel;
bool isoperator(char c);
int prioritateOperator(char c);
int prioritateOperator(const string &c);
void stackdump(vector<string> st);
struct punct
{
   double x = 0, y = 0;
};
class functie
{
public:
   string input;
   vector<string> sirPostfix;
   double delta;
   vector<punct> valori;
   functie();
   friend double executareFunctie(double valoare, const functie &a);
   void calcularePuncte(double start, double end, double delta);
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
   const int numarPuncte = 2000;
   double delta, diviziune;
   double latimeEcran, inaltimeEcran;
   double capatSus, capatJos;
   double displacementX, displacementY;
   grafic(double screenWidth, double screenHeight);
   void calculareDeltasiDivizune();
   void initializareGrafic(vector<functie> &functii);
   void deseneazaNumere(sf::RenderWindow &window);
   void deseneazaLiniaFunctiei(sf::RenderWindow &window, const functie &functiaCurenta);
   void miscareEcran(const unordered_map<sf::Keyboard::Key, bool> keyStates,bool& recalculPuncte);
   void setareLinii(sf::VertexArray& lines);
   void schimbareZoom(const double Constanta);
};

bool estefunctie(const string &s);
bool isoperand(char c);
void curatareInput(string &s);
void setText(sf::Text &text, sf::Font &font, double value, double abscisa, double ordonata);
#endif