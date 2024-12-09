#ifndef HEADER_H
#define HEADER_H
#include <iostream>
#include <string>
#include <queue>
#include <stack>
#include <vector>
using namespace std;
bool isoperator(char c)
{
   return (c == '+') || (c == '-') || (c == '*') || (c == '/') || (c == '^');
}
int prioritateOperator(char c)
{
   if (c == '+' || c == '-')
      return 1;
   if (c == '*' || c == '/')
      return 2;
   if (c == '^')
      return 3;
   return 0;
}
int prioritateOperator(const string &c)
{
   if (c[0] == '+' || c[0] == '-')
      return 1;
   if (c[0] == '*' || c[0] == '/')
      return 2;
   if (c[0] == '^')
      return 3;
   if(c[0]>='a'&&c[0]<='z')
      return 4;
   return 0;
}
void stackdump(queue<string> st)
{

   while (st.empty() == 0)
   {
      cout << st.front();
      st.pop();
   }
}
class functie
{
public:
   string input;
   queue<string> sirPostfix;
   /// inputul trebuie prelucrat in REVERSE POLISH
   float delta;
   vector<float> valori;
   functie()
   {
      input = "";
      delta = 0.001;
   }
   void calcularePuncte(float start, float end);
   void calculareOrdinePostfix()
   {
      int i = 0, lungime = input.size();
      string numar = "";   ///string auxiliar pentru adaugat elemente in stiva
      stack<string> operatori;   ///stiva de operatori
      // 2*3 + (8-3)/2
      for (i; i < lungime; i++)
      {
         char c = input[i];  ///patru cazuri 1. Functii sin, cos, ln, tan / 2. numere sau variabila x, 3. operatori / 4. paranteze
                             ///functia de prelucrare input se asigura ca nu vor exista probleme
         if (c >= 'a' && c <= 'z' && c != 'x')
         {
            while (c >= 'a' && c <= 'z')
            {
               numar += c;
               i++;
               c = input[i];
            }
            i--;
            operatori.push(numar);
            numar = "";///creeaza functie si o pune in stack
         }
         else if (isdigit(c) || c == 'x')
         {
            while (isdigit(c) || c == 'x')
            {
               numar += c;
               i++;
               c = input[i];
            }
            i--;
            sirPostfix.push(numar);
            numar = ""; ///creeaza numar si il pune in sir
         }
         else if (isoperator(c))
         {

            while (operatori.empty() == 0 &&    
                  (prioritateOperator(operatori.top()) > prioritateOperator(c) || 
                  (prioritateOperator(operatori.top()) == prioritateOperator(c) && c != '^')))
            {
               sirPostfix.push(operatori.top());
               operatori.pop();
            }
            numar += c;
            operatori.push(numar);
            numar = ""; ///creeaza operator si il pune in sir 

         }
         else if (c == '(')
            operatori.push("(");
         else if (c == ')')
         {
            while (operatori.empty() == 0 && operatori.top() != "(")
            {

               sirPostfix.push(operatori.top());
               operatori.pop();
            }
            if (operatori.empty() == 0)
               operatori.pop();
            if (operatori.empty()==0 && operatori.top()[0] >= 'a' && operatori.top()[0] <= 'z')
            {
               sirPostfix.push(operatori.top());
               operatori.pop();
            } ///rezolva parantezele
         }
      }
      while (operatori.empty() == 0)
      {
         sirPostfix.push(operatori.top());
         operatori.pop();
      }
   }

   void prelucrareInput();
   /// cos(x)^2+x^3/5*2+sin(4*x/5)
   /// 2*3 + (8-3)/2
};

#endif