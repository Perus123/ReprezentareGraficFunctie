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
int operatorsPriority(char c);
int operatorsPriority(const string &c);
void stackdump(vector<string> st);
struct point
{
   double x = 0, y = 0;
};
class function
{
public:
   string input;
   vector<string> postfixRow;
   double delta;
   vector<point> values;
   function();
   friend double executeFunction(double value, const function &a);
   void calculatePoints(double start, double end, double delta);
   void postfixOrderCalculation();
   void prelucrareInput();

   /// cos(x)^2+x^3/5*2+sin(4*x/5)
   /// 2*3 + (8-3)/2
};
class grafic
{
public:
   point  center;
   double RightEnd, LeftEnd;
   const int pointsNumber = 2000;
   double delta, division;
   double latimeEcran, inaltimeEcran;
   double upperBound, lowerBound;
   
   grafic(double screenWidth, double screenHeight);
   void calculateDeltaDivision();
   void initialiseGraphic(vector<function> &functions);
   void drawNumbers(sf::RenderWindow &window);
   void drawFunctionLines(sf::RenderWindow &window, const function &currentFunction);
   void screenMovement(const unordered_map<sf::Keyboard::Key, bool> keyStates,bool& pointsRecalculation);
   void settingLines(sf::VertexArray& lines);
   void zoomChange(const double constant);
};

bool isFunction(const string &s);
bool isoperand(char c);
void curatareInput(string &s);
void setText(sf::Text &text, sf::Font &font, double value, double abscissa, double ordinate);
#endif