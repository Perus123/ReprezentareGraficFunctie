#ifndef HEADER_H
#define HEADER_H
#pragma once
#include <iostream>
#include <string>
#include <stack>
#include <vector>
#include <cmath>
#include <SFML/Graphics.hpp>
#include <SFML/Window/Mouse.hpp>
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
   int extremePoints;
   double sum;
   function();
   friend double executeFunction(double value, const function &a, bool& validFunction);
   bool calculatePoints(double start, double end, double delta);
   void postfixOrderCalculation();
   void prelucrareInput();

   /// cos(x)^2+x^3/5*2+sin(4*x/5)
   /// 2*3 + (8-3)/2
};
class TextofBox
{
public:
    sf::RectangleShape box, deleteButton; // box reprezinta fundalul
    sf::Text textBox, textButon;          // textBox este textul functiei afisate iar textButton textul butonului
   TextofBox(sf::Font&f);
    
};

struct Theme {
    sf::Color backgroundColor;
    sf::Color textColor;
    sf::Color axisColor;
    sf::Color functionColor;
    sf::Color inputBoxFillColor;
    sf::Color inputBoxOutlineColor;
    sf::Color buttonFillColor;
    sf::Color buttonHoverColor;
    sf::Color minimumPointColor;
    sf::Color maximumPointColor;
};
class grafic
{
public:
   point center;
   double rightEnd, leftEnd;
   const int pointsNumber = 3000;
   double delta, division;
   std::vector<sf::Color> colors1;
   std::vector<sf::Color> colors2;
   double screenWidth, screenHeight;
   double upperBound, lowerBound;
  
   sf::Font font;
   grafic(double screenWidthParameter, double screenHeightParameter);//constructorul clasei, initializeaza obiectul grafic cu dimensiunile graficului
   void calculateDeltaDivision();
   void initialiseGraphic(vector<function> &functions);
   void drawNumbers(sf::RenderWindow &window);
   void drawFunctionLines(sf::RenderWindow &window, function &currentFunction, sf::Vector2i mouseCoordinates, int index);
   void screenMovement(const unordered_map<sf::Keyboard::Key, bool> keyStates, bool& pointsRecalculation);
   void settingLines(sf::VertexArray& lines);
   void zoomChange(const double constant);
   void showCalculationWindow(const vector<function>& functions, vector<TextofBox> textBoxes);

private:
    bool isDarkTheme;
    Theme lightTheme;
    Theme darkTheme;
    sf::RectangleShape themeButton;
    sf::Text themeButtonText;
    
    void initializeThemes();
    void setupThemeButton(sf::Font& font);
    void toggleTheme();
    Theme& getCurrentTheme() { return isDarkTheme ? darkTheme : lightTheme; }
};

bool isFunction(const string &s);
bool isoperand(char c);
void curatareInput(string &s);
void setText(sf::Text &text, sf::Font &font, double value, double abscissa, double ordinate);
#endif